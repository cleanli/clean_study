LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libbasicbt
LOCAL_SRC_FILES := IBasicBt.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libbinder \
	libutils \
	libhardware
include $(BUILD_SHARED_LIBRARY)
