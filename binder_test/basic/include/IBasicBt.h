#ifndef IBASICBT_H_  
#define IBASICBT_H_  
  
#include <inttypes.h>  
#include <utils/Errors.h>  
#include <binder/IInterface.h>  
#include <binder/Parcel.h>  
  
namespace android {  
  
class IBasicBt : public IInterface {  
    public:  
        DECLARE_META_INTERFACE(BasicBt);  
  
        virtual int32_t testCmd(uint32_t cmdId,  uint32_t paramLen) = 0;  
};  
  
class BnBasicBt: public BnInterface<IBasicBt> {  
    public:  
        virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,  
                uint32_t flags = 0);  
};  
  
}; // namespace android  
#endif 
