LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := basicbt_server
LOCAL_SRC_FILES := BasicBt_server.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libbinder \
	libutils \
	libbasicbt \
	libbasicbt_service \
	libhardware
include $(BUILD_EXECUTABLE)
