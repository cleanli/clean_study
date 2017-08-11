LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= cameratest.cpp
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \
	libui \
	libcamera_client \
	libcamera_metadata \
	libgui

LOCAL_C_INCLUDES += \
    $(TOP)/system/media/camera/include \
    
LOCAL_MODULE:= cameratest
include $(BUILD_EXECUTABLE)
