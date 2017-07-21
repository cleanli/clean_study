#define LOG_TAG "BasicBt"  

#include <binder/IServiceManager.h>  
#include <hardware/hardware.h>  
//#include <hardware/hw_auth_token.h>  
#include <utils/Log.h>  

#include "BasicBt.h"  


namespace android {  

	BasicBt* BasicBt::sInstance = NULL;  


	BasicBt::BasicBt() {  
		ALOGD("BasicBt started (pid=%d)", getpid());  

		ALOGD("BasicBt end (pid=%d)", getpid());  
	}  

	BasicBt::~BasicBt() {  
		ALOGD("BasicBt started (pid=%d)", getpid());  
	}  

	void BasicBt::instantiate() {  
		ALOGD("Enter %s", __func__);  
		status_t st = defaultServiceManager()->addService(    
				String16("my.test.binder"), new BasicBt());    
		ALOGD("ServiceManager addService ret=%d", st);   
		ALOGD("Enter %s", __func__);  
	}  


	int32_t BasicBt::testCmd(uint32_t cmdId,  uint32_t paramLen) {  
		ALOGD("cmdif = %d,  paramLen = %d", cmdId,  paramLen);  

		return 0;  
	}    

	void BasicBt::binderDied(const wp<IBinder>& who) {  
		ALOGD("binder died");  

	}   
}  
