LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

# name of the librairy
LOCAL_MODULE := edn

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_STATIC_LIBRARIES := etk ewol tinyxml libzip libpng libfreetype parsersvg agg

LOCAL_C_INCLUDES := -I$(LOCAL_PATH) $(addprefix -I$(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_SRC_FILES := $(FILE_LIST)


LOCAL_LDLIBS    := 

ifeq ($(DEBUG),1)
LOCAL_CFLAGS    :=  -D__PLATFORM__Linux \
                    -DETK_DEBUG_LEVEL=3 \
                    -DAPPL_DEBUG_LEVEL=3
else
LOCAL_CFLAGS    :=  -D__PLATFORM__Linux \
                    -DETK_DEBUG_LEVEL=3 \
                    -DAPPL_DEBUG_LEVEL=1
endif

include $(BUILD_EXECUTABLE)

NDK_MODULE_PATH := $(LOCAL_PATH)/../../


$(call import-module,ewol/Sources)
