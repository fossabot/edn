LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_MODULE := ednn
LOCAL_STATIC_LIBRARIES := ewol etk tinyxml libzip libpng 

LOCAL_C_INCLUDES := -I$(LOCAL_PATH) $(addprefix -I$(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_SRC_FILES := $(FILE_LIST)
LOCAL_LDLIBS    := 

LOCAL_CFLAGS    :=	-DEWOL_USE_FREE_TYPE \
					-DEDN_DEBUG_LEVEL=3

include $(BUILD_EXECUTABLE)

NDK_MODULE_PATH := $(LOCAL_PATH)/../../


$(call import-module,ewol/Sources)
