

###############################################################################
### Files Listes                                                            ###
###############################################################################

# ExuberantCtags reading file tools (extern OPEN Sources) :
FILE_LIST+=		edn/ctags/readtags.cpp \
				edn/ctags/CTagsManager.cpp


# Globals debug tool:
FILE_LIST+=		edn/tools/debug/tools_debug.cpp \
				edn/tools/globals/tools_globals.cpp \
				edn/tools/memory/toolsMemory.cpp

# Buffers internal:
FILE_LIST+=		edn/tools/EdnTemplate/EdnVectorBuf.cpp \
				edn/tools/EdnBuf/EdnBuf.cpp \
				edn/tools/EdnBuf/EdnBuf_HighLight.cpp \
				edn/tools/EdnBuf/EdnBuf_History.cpp \
				edn/tools/EdnBuf/EdnBuf_Selection.cpp \
				edn/tools/EdnBuf/EdnBufHistory.cpp
				

# Tools internal:
FILE_LIST+=		edn/tools/Display/Display.cpp \
				edn/tools/ClipBoard/ClipBoard.cpp \
				edn/tools/MsgBroadcast/MsgBroadcast.cpp \
				edn/tools/MsgBroadcast/AccelKey.cpp

# Gui:
FILE_LIST+=		edn/Gui/BufferView.cpp \
				edn/Gui/CodeView.cpp \
				edn/Gui/MainWindows.cpp \
				edn/Gui/Search.cpp \
				edn/Gui/SearchData.cpp

# Basic Interface :
FILE_LIST+=		edn/Buffer/Buffer.cpp \
				edn/Buffer/BufferText.cpp \
				edn/Buffer/BufferEmpty.cpp \
				edn/Buffer/BufferManager.cpp \
				edn/Colorize/Colorize.cpp \
				edn/Colorize/ColorizeManager.cpp \
				edn/Highlight/HighlightPattern.cpp \
				edn/Highlight/Highlight.cpp \
				edn/Highlight/HighlightManager.cpp

# Main entry file : 
FILE_LIST+=		edn/init.cpp

