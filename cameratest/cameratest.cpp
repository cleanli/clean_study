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
int main()
{
	printf("hello world\n");
	{
		int cameraId=0;
		String16 clientName("hellocameraworld");
		sp<Camera> camera;
		// Default path: hal version is don't care, do normal camera connect.
		camera = Camera::connect(cameraId, clientName,
				Camera::USE_CALLING_UID, Camera::USE_CALLING_PID);
	}

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
	SurfaceComposerClient::closeGlobalTransaction();

	ANativeWindow_Buffer outBuffer;
	surface->lock(&outBuffer, NULL);
	ssize_t bpr = outBuffer.stride * bytesPerPixel(outBuffer.format);
	android_memset16((uint16_t*)outBuffer.bits, 0xF800, bpr*outBuffer.height);
	surface->unlockAndPost();
	getchar();

	return 0;
}
