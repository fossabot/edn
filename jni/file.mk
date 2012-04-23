

###############################################################################
### Files Listes                                                            ###
###############################################################################

# ExuberantCtags reading file tools (extern OPEN Sources) :
FILE_LIST+=		appl/ctags/readtags.cpp \
				appl/ctags/CTagsManager.cpp


# Globals debug tool:
FILE_LIST+=		appl/Debug.cpp \
				appl/tools/globals/tools_globals.cpp \
				appl/tools/memory/toolsMemory.cpp

# Buffers internal:
FILE_LIST+=		appl/tools/EdnTemplate/EdnVectorBuf.cpp \
				appl/tools/EdnBuf/EdnBuf.cpp \
				appl/tools/EdnBuf/EdnBuf_HighLight.cpp \
				appl/tools/EdnBuf/EdnBuf_History.cpp \
				appl/tools/EdnBuf/EdnBuf_Selection.cpp \
				appl/tools/EdnBuf/EdnBufHistory.cpp
				

# Tools internal:
FILE_LIST+=		appl/tools/Display/Display.cpp \
				appl/tools/MsgBroadcast/MsgBroadcast.cpp

# Gui:
FILE_LIST+=		appl/Gui/BufferView.cpp \
				appl/Gui/CodeView.cpp \
				appl/Gui/MainWindows.cpp \
				appl/Gui/Search.cpp \
				appl/Gui/SearchData.cpp

# Basic Interface :
FILE_LIST+=		appl/Buffer/Buffer.cpp \
				appl/Buffer/BufferText.cpp \
				appl/Buffer/BufferEmpty.cpp \
				appl/Buffer/BufferManager.cpp \
				appl/Colorize/Colorize.cpp \
				appl/Colorize/ColorizeManager.cpp \
				appl/Highlight/HighlightPattern.cpp \
				appl/Highlight/Highlight.cpp \
				appl/Highlight/HighlightManager.cpp

# Main entry file : 
FILE_LIST+=		appl/init.cpp

