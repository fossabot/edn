LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

# name of the librairy
LOCAL_MODULE := ednn

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_STATIC_LIBRARIES := ewol etk tinyxml libzip libpng libfreetype parsersvg agg

LOCAL_C_INCLUDES := -I$(LOCAL_PATH) $(addprefix -I$(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_SRC_FILES := $(FILE_LIST)


LOCAL_LDLIBS    := 

LOCAL_CFLAGS    :=  -D__PLATFORM__Linux \
                    -DEWOL_USE_FREE_TYPE \
                    -DEDN_DEBUG_LEVEL=3

include $(BUILD_EXECUTABLE)

NDK_MODULE_PATH := $(LOCAL_PATH)/../../


$(call import-module,ewol/Sources)
