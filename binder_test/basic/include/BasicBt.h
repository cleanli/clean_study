#ifndef BASICBT_H_  
#define BASICBT_H_  
  
  
#include "IBasicBt.h"  
  
  
namespace android {  
  
    class BasicBt : public BnBasicBt{  
        public:  
            static BasicBt* getInstance() {  
                if (sInstance == NULL) {  
                    ALOGD("lanhai %s", __func__);  
                    sInstance = new BasicBt();  
                }  
                return sInstance;  
            }  
  
        static void instantiate();  
        virtual int32_t testCmd(uint32_t cmdId,  uint32_t paramLen);  
  
  
         private:     
            BasicBt();  
            virtual ~BasicBt();  
            void binderDied(const wp<IBinder>& who);  
            static BasicBt* sInstance;  
    };  
  
} // namespace android  
  
#endif 
