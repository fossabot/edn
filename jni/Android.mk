LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_MODULE := ednpackage
LOCAL_STATIC_LIBRARIES := ewol etk tinyxml libzip libpng

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(addprefix $(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_SRC_FILES := ewolAndroidAbstraction.cpp \
                   $(FILE_LIST)

LOCAL_LDLIBS    := -llog -landroid

LOCAL_CFLAGS    :=  -D__PLATFORM__Android \
                    -D__MODE__Touch \
                    -DEWOL_USE_FREE_TYPE \
                    -DETK_DEBUG_LEVEL=3 \
                    -DEDN_DEBUG_LEVEL=3 \
                    -DDATA_IN_APK

include $(BUILD_SHARED_LIBRARY)

NDK_MODULE_PATH := $(LOCAL_PATH)/../../


$(call import-module,ewol/Sources)
