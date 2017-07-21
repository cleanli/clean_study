#include <stdio.h>
#include <cutils/memory.h>

#include <utils/Log.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include <gui/Surface.h>
#include <android/native_window.h>
#include <gui/SurfaceComposerClient.h>

using namespace android;
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
	SurfaceComposerClient::closeGlobalTransaction();

	ANativeWindow_Buffer outBuffer;
	surface->lock(&outBuffer, NULL);
	ssize_t bpr = outBuffer.stride * bytesPerPixel(outBuffer.format);
	android_memset16((uint16_t*)outBuffer.bits, 0xF800, bpr*outBuffer.height);
	surface->unlockAndPost();
	getchar();

	surface->lock(&outBuffer, NULL);
	android_memset16((uint16_t*)outBuffer.bits, 0x07E0, bpr*outBuffer.height);
	surface->unlockAndPost();
	getchar();

	SurfaceComposerClient::openGlobalTransaction();
	surfaceControl->setSize(320, 240);
	SurfaceComposerClient::closeGlobalTransaction();
	getchar();

	return 0;
}
