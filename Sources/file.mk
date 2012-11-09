

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
FILE_LIST+=		appl/Buffer/EdnVectorBuf.cpp \
				appl/Buffer/EdnBuf/EdnBuf.cpp \
				appl/Buffer/EdnBuf/EdnBuf_HighLight.cpp \
				appl/Buffer/EdnBuf/EdnBuf_History.cpp \
				appl/Buffer/EdnBuf/EdnBuf_Selection.cpp \
				appl/Buffer/EdnBuf/EdnBufHistory.cpp \
				appl/Buffer/Buffer.cpp \
				appl/Buffer/BufferText.cpp \
				appl/Buffer/BufferEmpty.cpp \
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



LOCAL_COPY_FILES := \
                    ../share/Font/freefont/FreeSerif.ttf:Font/freefont/FreeSerif.ttf

LOCAL_COPY_FOLDERS := \
                      ../share/color/*.xml:color/ \
                      ../share/languages/asm/*.xml:languages/asm/ \
                      ../share/languages/bash/*.xml:languages/bash/ \
                      ../share/languages/boo/*.xml:languages/boo/ \
                      ../share/languages/c/*.xml:languages/c/ \
                      ../share/languages/glsl/*.xml:languages/glsl/ \
                      ../share/languages/in/*.xml:languages/in/ \
                      ../share/languages/java/*.xml:languages/java/ \
                      ../share/languages/lua/*.xml:languages/lua/ \
                      ../share/languages/makefile/*.xml:languages/makefile/ \
                      ../share/languages/matlab/*.xml:languages/matlab/ \
                      ../share/languages/php/*.xml:languages/php/ \
                      ../share/languages/xml/*.xml:languages/xml/ \
                      ../share/theme/default/*.svg:theme/default/ \
                      ../share/Font/freefont/FreeMon*.ttf:Font/freefont

