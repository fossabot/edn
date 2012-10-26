LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_CONFIG_FILES := Config.in

# name of the librairy
ifeq ($(CONFIG___EWOL_APPL_NAME__),$(empty))
LOCAL_MODULE := edn
else
LOCAL_MODULE := $(call convert-special-char,$(CONFIG___EWOL_APPL_NAME__))
endif
# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_STATIC_LIBRARIES := ewol

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(addprefix $(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_SRC_FILES := $(FILE_LIST)


LOCAL_LDLIBS    := 

ifeq ($(DEBUG),1)
LOCAL_CFLAGS    :=  -DPROJECT_NAME="\"$(LOCAL_MODULE)\"" \
                    -DAPPL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-$(BUILD_DIRECTORY_MODE)\""
else
LOCAL_CFLAGS    :=  -DMODE_RELEASE \
                    -DPROJECT_NAME="\"$(LOCAL_MODULE)\"" \
                    -DAPPL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-$(BUILD_DIRECTORY_MODE)\""
endif

LOCAL_COPY_FILES := ../share/color_white.xml:usr/share/$(LOCAL_MODULE)/color_white.xml \
                    ../share/color_black.xml:usr/share/$(LOCAL_MODULE)/color_black.xml \
                    ../share/lang_asm.xml:usr/share/$(LOCAL_MODULE)/lang_asm.xml \
                    ../share/lang_bash.xml:usr/share/$(LOCAL_MODULE)/lang_bash.xml \
                    ../share/lang_boo.xml:usr/share/$(LOCAL_MODULE)/lang_boo.xml \
                    ../share/lang_c.xml:usr/share/$(LOCAL_MODULE)/lang_c.xml \
                    ../share/lang_glsl.xml:usr/share/$(LOCAL_MODULE)/lang_glsl.xml \
                    ../share/lang_in.xml:usr/share/$(LOCAL_MODULE)/lang_in.xml \
                    ../share/lang_java.xml:usr/share/$(LOCAL_MODULE)/lang_java.xml \
                    ../share/lang_lua.xml:usr/share/$(LOCAL_MODULE)/lang_lua.xml \
                    ../share/lang_Makefile.xml:usr/share/$(LOCAL_MODULE)/lang_Makefile.xml \
                    ../share/lang_matlab.xml:usr/share/$(LOCAL_MODULE)/lang_matlab.xml \
                    ../share/lang_php.xml:usr/share/$(LOCAL_MODULE)/lang_php.xml \
                    ../share/lang_xml.xml:usr/share/$(LOCAL_MODULE)/lang_xml.xml


include $(BUILD_EXECUTABLE)

NDK_MODULE_PATH := $(LOCAL_PATH)/../../



$(call import-module,ewol/Sources)
