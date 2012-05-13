LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

# name of the librairy
LOCAL_MODULE := ednpackage

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_STATIC_LIBRARIES := ewol etk tinyxml libzip libpng libfreetype libparsersvg libagg

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(addprefix $(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_SRC_FILES := ewolAndroidAbstraction.cpp \
                   $(FILE_LIST)

LOCAL_LDLIBS    := -llog -landroid

# -frtti
ifeq ($(DEBUG),1)
LOCAL_CFLAGS    :=  -D__PLATFORM__Android \
                    -D__MODE__Touch \
                    -DETK_DEBUG_LEVEL=3 \
                    -DAPPL_DEBUG_LEVEL=3 \
                    -DPROJECT_NAME="\"$(LOCAL_MODULE)\"" 
else
LOCAL_CFLAGS    :=  -D__PLATFORM__Android \
                    -D__MODE__Touch \
                    -DETK_DEBUG_LEVEL=1 \
                    -DAPPL_DEBUG_LEVEL=1 \
                    -DMODE_RELEASE \
                    -DPROJECT_NAME="\"$(LOCAL_MODULE)\"" 
endif


include $(BUILD_SHARED_LIBRARY)

NDK_MODULE_PATH := $(LOCAL_PATH)/../../


$(call import-module,ewol/Sources)
