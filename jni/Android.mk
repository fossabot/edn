LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ednpackage
LOCAL_STATIC_LIBRARIES := ewol

LOCAL_C_INCLUDES := $(LOCAL_PATH)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := ewolAndroidAbstraction.cpp \
                   $(FILE_LIST)

LOCAL_LDLIBS    := -llog -landroid

include $(BUILD_SHARED_LIBRARY)

NDK_MODULE_PATH := $(LOCAL_PATH)/../../


$(call import-module,ewol/Sources)
