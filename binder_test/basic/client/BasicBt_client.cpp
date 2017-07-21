#define LOG_TAG "BasicBt_client"    

#include <utils/Log.h>    
#include <nativehelper/jni.h>    
#include <nativehelper/JNIHelp.h>    
#include <android_runtime/AndroidRuntime.h>    
#include <binder/IServiceManager.h>   


#include "IBasicBt.h"  


using namespace android;    

int main()    
{    
	int sum = 0;    
	const uint8_t* param;  
	ALOGE("main enter");  
	sp<IBasicBt> mTestBinserService;    
	if (mTestBinserService.get() == 0) {    
		sp<IServiceManager> sm = defaultServiceManager();    
		sp<IBinder> binder;    
		do {    
			ALOGE("binder enter");  
			binder = sm->getService(String16("my.test.binder"));  
			if (binder != 0) {    
				ALOGE("getService ok");    
				break;    
			}  
			ALOGE("getService fail");    
			usleep(500000); // 0.5 s    
		} while (true);    
		mTestBinserService = interface_cast<IBasicBt> (binder);   
		ALOGE("mTestBinserService");    
		ALOGE_IF(mTestBinserService == 0, "no ITestBinserService!?");    
	}    
	sum = mTestBinserService->testCmd(3, 4);    
	ALOGI("sum = %d", sum);    
	return 0;    

}    
