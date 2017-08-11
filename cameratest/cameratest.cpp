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
class camtestListener: public CameraListener
{
public:
    virtual void notify(int32_t msgType, int32_t ext1, int32_t ext2);
    virtual void postData(int32_t msgType, const sp<IMemory>& dataPtr,
                          camera_frame_metadata_t *metadata);
    virtual void postDataTimestamp(nsecs_t timestamp, int32_t msgType, const sp<IMemory>& dataPtr);
    virtual void postRecordingFrameHandleTimestamp(nsecs_t timestamp, native_handle_t* handle);

private:
    void copyAndPost(const sp<IMemory>& dataPtr, int msgType);
    Mutex       mLock;
};

void camtestListener::notify(int32_t msgType, int32_t ext1, int32_t ext2)
{
    ALOGV("notify");
    messg("%s %d\n", __func__, __LINE__);

    // VM pointer will be NULL if object is released
    Mutex::Autolock _l(mLock);
    if (msgType == CAMERA_MSG_RAW_IMAGE_NOTIFY) {
        msgType = CAMERA_MSG_RAW_IMAGE;
    }

    messg("Got notify: msgType %d ext1 %d ext2 %d\n", msgType, ext1, ext2);
}

void camtestListener::copyAndPost(const sp<IMemory>& dataPtr, int msgType)
{
    // allocate Java byte array and copy data
    if (dataPtr != NULL) {
        ssize_t offset;
        size_t size;
        sp<IMemoryHeap> heap = dataPtr->getMemory(&offset, &size);
        ALOGV("copyAndPost: off=%zd, size=%zu", offset, size);
        uint8_t *heapBase = (uint8_t*)heap->base();

        if (heapBase != NULL) {

#if 1
            if (msgType == CAMERA_MSG_RAW_IMAGE) {
                //obj = getCallbackBuffer(env, &mRawImageCallbackBuffers, size);
            } else if (msgType == CAMERA_MSG_COMPRESSED_IMAGE) {
		    messg("got jpeg\n");
		    FILE * fp = fopen("/data/cameratest_dir/picture.jpg", "w");
		    if(fp != nullptr){
			    fwrite(heapBase, 1, size, fp);
			    fclose(fp);
		    }
		    else{
			    messg("open file failed\n");
		    }
            } else {
                ALOGV("Allocating callback buffer");
                //obj = env->NewByteArray(size);
            }
#endif
        } else {
            ALOGE("image heap is NULL");
        }
    }
#if 0
    // post image data to Java
    env->CallStaticVoidMethod(mCameraJClass, fields.post_event,
            mCameraJObjectWeak, msgType, 0, 0, obj);
    if (obj) {
        env->DeleteLocalRef(obj);
    }
#endif
}

void camtestListener::postData(int32_t msgType, const sp<IMemory>& dataPtr,
                                camera_frame_metadata_t *metadata)
{
    // VM pointer will be NULL if object is released
    Mutex::Autolock _l(mLock);
    messg("%s %d\n", __func__, __LINE__);

    int32_t dataMsgType = msgType & ~CAMERA_MSG_PREVIEW_METADATA;

    // return data based on callback type
    switch (dataMsgType) {
        case CAMERA_MSG_VIDEO_FRAME:
            // should never happen
            break;

        // For backward-compatibility purpose, if there is no callback
        // buffer for raw image, the callback returns null.
        case CAMERA_MSG_RAW_IMAGE:
            ALOGV("rawCallback");
#if 0
            if (mRawImageCallbackBuffers.isEmpty()) {
                env->CallStaticVoidMethod(mCameraJClass, fields.post_event,
                        mCameraJObjectWeak, dataMsgType, 0, 0, NULL);
            } else {
                copyAndPost(dataPtr, dataMsgType);
            }
#endif
            break;

        // There is no data.
        case 0:
            break;

        default:
            ALOGV("dataCallback(%d, %p)", dataMsgType, dataPtr.get());
            copyAndPost(dataPtr, dataMsgType);
            break;
    }

    // post frame metadata to Java
    if (metadata && (msgType & CAMERA_MSG_PREVIEW_METADATA)) {
        //postMetadata(env, CAMERA_MSG_PREVIEW_METADATA, metadata);
    }
}

void camtestListener::postDataTimestamp(nsecs_t, int32_t msgType, const sp<IMemory>& dataPtr)
{
    // TODO: plumb up to Java. For now, just drop the timestamp
    messg("%s %d\n", __func__, __LINE__);
    postData(msgType, dataPtr, NULL);
}

void camtestListener::postRecordingFrameHandleTimestamp(nsecs_t, native_handle_t* handle) {
    // Video buffers are not needed at app layer so just return the video buffers here.
    // This may be called when stagefright just releases camera but there are still outstanding
    // video buffers.
    messg("%s %d\n", __func__, __LINE__);
    if (camera != nullptr) {
        camera->releaseRecordingFrameHandle(handle);
    } else {
        native_handle_close(handle);
        native_handle_delete(handle);
    }
}

sp<camtestListener> lsn;
int main()
{
	printf("hello world\n");

#if 1//if remove this part of code, preview will be transparent
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

	ANativeWindow_Buffer outBuffer;
	surface->lock(&outBuffer, NULL);
	ssize_t bpr = outBuffer.stride * bytesPerPixel(outBuffer.format);
	android_memset16((uint16_t*)outBuffer.bits, 0x0000, bpr*outBuffer.height);
	surface->unlockAndPost();
	//getchar();
#endif

	{
		// create a client to surfaceflinger
		sp<SurfaceComposerClient> client1 = new SurfaceComposerClient();

		sp<SurfaceControl> surfaceControl1 = client1->createSurface(String8("camera"),
				160, 240, PIXEL_FORMAT_RGB_565, 0);

		sp<Surface> surface1 = surfaceControl1->getSurface();

		SurfaceComposerClient::openGlobalTransaction();
		surfaceControl1->setLayer(100000);
		surfaceControl1->setSize(320, 240);
		//surfaceControl->setAlpha(1.0);
		SurfaceComposerClient::closeGlobalTransaction();

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
		lsn = new camtestListener;
		camera->setListener(lsn);

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
		if (surface1 != NULL) {
			gbp = surface1->getIGraphicBufferProducer();
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
		camera->takePicture(CAMERA_MSG_SHUTTER|CAMERA_MSG_COMPRESSED_IMAGE);
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
