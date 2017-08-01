#define LOG_TAG "BasicBt"
#include <stdint.h>  
#include <sys/types.h>  
#include <utils/Log.h>  
#include <binder/Parcel.h>  

#include "IBasicBt.h"  

namespace android {  

	enum {  
		TEST_CMD = IBinder::FIRST_CALL_TRANSACTION,   
	};  

	class BpBasicBt : public BpInterface<IBasicBt>  
	{  
		public:  
			BpBasicBt(const sp<IBinder>& impl) :  
				BpInterface<IBasicBt>(impl) {  
				}  

			int32_t testCmd(uint32_t cmdId, uint32_t paramLen) {  
				Parcel data, reply;  
				data.writeInterfaceToken(descriptor);  
				data.writeInt32(cmdId);  
				data.writeInt32(paramLen);  
				remote()->transact(TEST_CMD, data, &reply);  
				reply.readExceptionCode();  
				return reply.readInt32();  
			}   
	};  

	IMPLEMENT_META_INTERFACE(BasicBt, "android.IBasicBt");  


	status_t BnBasicBt::onTransact(uint32_t code, const Parcel& data, Parcel* reply,  
			uint32_t flags) {  
		switch(code) {  
			case TEST_CMD: {  
					       CHECK_INTERFACE(IBasicBt, data, reply);  
					       int32_t cmdId = data.readInt32();  
					       int32_t paramLen = data.readInt32();  
					       // const uint8_t* param = static_cast<const uint8_t *>(data.readInplace(paramLen));  
					       ALOGV("%s, %d, %d", __func__, cmdId,  paramLen);        
					       const int32_t ret = testCmd(cmdId, paramLen);   
					       reply->writeNoException();  
					       reply->writeInt32(ret);  
					       return NO_ERROR;  
				       }  

			default:  
				       return BBinder::onTransact(code, data, reply, flags);  
		}  
	}   


}; // namespace android  
