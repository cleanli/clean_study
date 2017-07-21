LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := basicbt_client
LOCAL_SRC_FILES := BasicBt_client.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libbinder \
	libutils \
	libbasicbt \
	libhardware
include $(BUILD_EXECUTABLE)
