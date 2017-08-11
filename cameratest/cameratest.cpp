#include <stdio.h>
#include <cutils/memory.h>

#include <utils/Log.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include <gui/Surface.h>
#include <android/native_window.h>
#include <gui/SurfaceComposerClient.h>
#include <camera/Camera.h>

using namespace android;
#define messg(format, args ... )  ALOGE(format, ## args);printf(format, ## args);
/*
void messg(const char*s, ...)
{
	printf("%s", s, ...);
	ALOGE("%s", s, ...);
}
*/
sp<Camera> camera;
int main()
{
	printf("hello world\n");

	/*copy from frameworks/native/services/surfaceflinger/tests/resize/resize.cpp Lollipop*/
	// set up the thread-pool
	sp<ProcessState> proc(ProcessState::self());
	ProcessState::self()->startThreadPool();

	// create a client to surfaceflinger
	sp<SurfaceComposerClient> client = new SurfaceComposerClient();

	sp<SurfaceControl> surfaceControl = client->createSurface(String8("resize"),
			160, 240, PIXEL_FORMAT_RGB_565, 0);

	sp<Surface> surface = surfaceControl->getSurface();

	SurfaceComposerClient::openGlobalTransaction();
	surfaceControl->setLayer(100000);
	surfaceControl->setSize(320, 240);
	//surfaceControl->setAlpha(1.0);
	SurfaceComposerClient::closeGlobalTransaction();

#if 0
	ANativeWindow_Buffer outBuffer;
	surface->lock(&outBuffer, NULL);
	ssize_t bpr = outBuffer.stride * bytesPerPixel(outBuffer.format);
	android_memset16((uint16_t*)outBuffer.bits, 0xF800, bpr*outBuffer.height);
	surface->unlockAndPost();
	getchar();
#endif

	{
		int cameraId=0, camera_num;
		String16 clientName("hellocameraworld");
		camera_num = Camera::getNumberOfCameras();
		messg("getNumberOfCameras %d\n", camera_num);
		// Default path: hal version is don't care, do normal camera connect.
		camera = Camera::connect(cameraId, clientName,
				Camera::USE_CALLING_UID, Camera::USE_CALLING_PID);

		if (camera == NULL) {
			messg("camera connect failed\n");
			return -EACCES;
		}
		else{
			messg("camera connect ok\n");
		}

		// make sure camera hardware is alive
		if (camera->getStatus() != NO_ERROR) {
			messg("camera getStatus failed\n");
			return NO_INIT;
		}
		else{
			messg("camera getStatus ok\n");
		}

		CameraInfo cameraInfo;
		status_t rc = Camera::getCameraInfo(cameraId, &cameraInfo);
		if (rc != NO_ERROR) {
			return rc;
		}
		messg("defaultOrientation %d\n",cameraInfo.orientation);
		int defaultOrientation = 0;
		switch (cameraInfo.orientation) {
			case 0:
				break;
			case 90:
				if (cameraInfo.facing == CAMERA_FACING_FRONT) {
					defaultOrientation = 180;
				}
				break;
			case 180:
				defaultOrientation = 180;
				break;
			case 270:
				if (cameraInfo.facing != CAMERA_FACING_FRONT) {
					defaultOrientation = 180;
				}
				break;
			default:
				ALOGE("Unexpected camera orientation %d!", cameraInfo.orientation);
				break;
		}
		if (defaultOrientation != 0) {
			ALOGV("Setting default display orientation to %d", defaultOrientation);
			rc = camera->sendCommand(CAMERA_CMD_SET_DISPLAY_ORIENTATION,
					defaultOrientation, 0);
			if (rc != NO_ERROR) {
				ALOGE("Unable to update default orientation: %s (%d)",
						strerror(-rc), rc);
				return rc;
			}
		}

		sp<IGraphicBufferProducer> gbp;
		if (surface != NULL) {
			gbp = surface->getIGraphicBufferProducer();
		}

		if (camera->setPreviewTarget(gbp) != NO_ERROR) {
			messg("setPreviewTarget failed\n");
			return NO_INIT;
		}
		else{
			messg("setPreviewTarget ok\n");
		}

		camera->startPreview();
		getchar();
		messg("camera destroy\n");
		if (camera != NULL) {
			camera->setPreviewCallbackFlags(CAMERA_FRAME_CALLBACK_FLAG_NOOP);
			camera->disconnect();
		}
	}
	return 0;
}
