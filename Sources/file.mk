

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
				appl/Gui/SearchData.cpp

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
