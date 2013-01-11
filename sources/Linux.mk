LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_CONFIG_FILES := Config.in

# name of the librairy
LOCAL_MODULE := edn
# get the tag of the current project : 
LOCAL_VERSION=$(shell cat $(LOCAL_PATH)/../tag)
$(info [TAG:$(LOCAL_MODULE)] $(LOCAL_VERSION))

# name of the dependency
LOCAL_STATIC_LIBRARIES := ewol

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(addprefix $(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_SRC_FILES := $(FILE_LIST)


LOCAL_LDLIBS    := 

LOCAL_CFLAGS    :=  -DPROJECT_NAME="\"$(LOCAL_MODULE)\"" \
                    -DAPPL_VERSION_TAG_NAME="\"$(LOCAL_VERSION)-$(BUILD_DIRECTORY_MODE)\""

LOCAL_COPY_FILES += ../data/icon.png:icon.png

include $(BUILD_EXECUTABLE)

