#define LOG_TAG "BasicBt"  
#include <cutils/log.h>  
#include <utils/Log.h>  
#include <binder/IPCThreadState.h>  
#include <binder/IServiceManager.h>  
#include <binder/PermissionCache.h>  
#include <utils/String16.h>  
#include <hardware/hardware.h>  

#include "BasicBt.h"  


using namespace android;  

int main()  
{  
	ALOGI("TestService Starting " LOG_TAG);  
	sp<ProcessState> proc(ProcessState::self());    
	sp<IServiceManager> sm = defaultServiceManager();    
	BasicBt::instantiate();  
	ProcessState::self()->startThreadPool();    
	IPCThreadState::self()->joinThreadPool();    
	ALOGI("TestService end " LOG_TAG);  
	return 0;  
}  
