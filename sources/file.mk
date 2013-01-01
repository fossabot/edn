

###############################################################################
### Files Listes                                                            ###
###############################################################################

# ExuberantCtags reading file tools (extern OPEN Sources) :
FILE_LIST+=		appl/ctags/readtags.cpp \
				appl/ctags/CTagsManager.cpp

# Globals debug tool:
FILE_LIST+=		appl/Debug.cpp \
				appl/global.cpp \
				appl/globalMsg.cpp

# Gui:
FILE_LIST+=		appl/Gui/BufferView.cpp \
				appl/Gui/CodeView.cpp \
				appl/Gui/MainWindows.cpp \
				appl/Gui/Search.cpp \
				appl/Gui/SearchData.cpp \
				appl/Gui/TagFileSelection.cpp \
				appl/Gui/TagFileList.cpp

# All needed for the buffer management :
FILE_LIST+=		appl/Buffer/EdnBuf/EdnBuf.cpp \
				appl/Buffer/EdnBuf/EdnBuf_HighLight.cpp \
				appl/Buffer/EdnBuf/EdnBuf_History.cpp \
				appl/Buffer/EdnBuf/EdnBuf_Selection.cpp \
				appl/Buffer/EdnBuf/EdnBufHistory.cpp \
				appl/Buffer/BufferText.cpp \
				appl/Buffer/BufferManager.cpp

# Generic color management for the text editor : 
FILE_LIST+=		appl/Colorize/Colorize.cpp \
				appl/Colorize/ColorizeManager.cpp

# syntax coloration for the text editor
FILE_LIST+=		appl/Highlight/HighlightPattern.cpp \
				appl/Highlight/Highlight.cpp \
				appl/Highlight/HighlightManager.cpp

# Main entry file : 
FILE_LIST+=		appl/init.cpp


LOCAL_COPY_FOLDERS := ../data/icon.*: \
                      ../data/color/*.xml:color/ \
                      ../data/languages/asm/*.xml:languages/asm/ \
                      ../data/languages/bash/*.xml:languages/bash/ \
                      ../data/languages/boo/*.xml:languages/boo/ \
                      ../data/languages/c/*.xml:languages/c/ \
                      ../data/languages/glsl/*.xml:languages/glsl/ \
                      ../data/languages/in/*.xml:languages/in/ \
                      ../data/languages/java/*.xml:languages/java/ \
                      ../data/languages/lua/*.xml:languages/lua/ \
                      ../data/languages/makefile/*.xml:languages/makefile/ \
                      ../data/languages/matlab/*.xml:languages/matlab/ \
                      ../data/languages/php/*.xml:languages/php/ \
                      ../data/languages/xml/*.xml:languages/xml/ \
                      ../data/theme/default/*.svg:theme/default/


ifneq ($(__EWOL_INTEGRATED_FONT__),$(empty))

LOCAL_COPY_FILES := ../data/Font/freefont/FreeSerif.ttf:fonts/FreeSerif.ttf
LOCAL_COPY_FOLDERS += ../data/Font/freefont/FreeMon*.ttf:fonts
endif
