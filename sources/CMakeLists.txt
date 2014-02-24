cmake_minimum_required(VERSION 2.8)

# Declare the project
project(edn)

##
## Include C++ X11 dependency ... (check correct flags)
##
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
endif()


# set output path:
set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(.)

#Create src file list
set(src_files
	appl/ctags/readtags.cpp
	appl/debug.cpp
	appl/global.cpp
	appl/globalMsg.cpp
	appl/init.cpp
	appl/Gui/BufferView.cpp
	appl/Gui/TextViewer.cpp
	appl/Gui/ViewerManager.cpp
	appl/Gui/MainWindows.cpp
	appl/Gui/Search.cpp
	appl/Gui/TagFileSelection.cpp
	appl/Gui/TagFileList.cpp
	appl/Gui/WorkerSaveFile.cpp
	appl/Gui/WorkerSaveAllFile.cpp
	appl/Gui/WorkerCloseFile.cpp
	appl/Gui/WorkerCloseAllFile.cpp
	appl/Buffer.cpp
	appl/BufferManager.cpp
	appl/TextPlugin.cpp
	appl/TextPluginCopy.cpp
	appl/TextPluginMultiLineTab.cpp
	appl/TextPluginAutoIndent.cpp
	appl/TextPluginHistory.cpp
	appl/TextPluginRmLine.cpp
	appl/TextPluginSelectAll.cpp
	appl/TextPluginCtags.cpp
	appl/TextPluginManager.cpp
	appl/GlyphDecoration.cpp
	appl/GlyphPainting.cpp
	appl/HighlightPattern.cpp
	appl/Highlight.cpp
	appl/HighlightManager.cpp
)

add_definitions( -DDEBUG_LEVEL=3 )
add_definitions( -DDEBUG=1 )
add_definitions( -DPROJECT_NAME="edn" )
# read version :
file (STRINGS "tag" BUILD_VERSION)
add_definitions( -DAPPL_VERSION="${BUILD_VERSION}" )

#Create a static Lib:
add_executable(edn ${src_files} )

include_directories(${zlib_SOURCE_DIR}/contrib/)
include_directories(${linearmath_SOURCE_DIR}/bullet/src/)
include_directories(${etk_SOURCE_DIR})
include_directories(${freetype_SOURCE_DIR})
include_directories(${exml_SOURCE_DIR})
include_directories(${ejson_SOURCE_DIR})
include_directories(${egami_SOURCE_DIR})
include_directories(${edtaa3_SOURCE_DIR})
include_directories(${date_SOURCE_DIR})
include_directories(${agg_SOURCE_DIR})
include_directories(${ewol_SOURCE_DIR})


#target_link_libraries(edn ewol)
target_link_libraries(edn ewol linearmath zlib etk freetype exml ejson egami edtaa3 date esvg png)
target_link_libraries(edn -lGL -lX11 -lpthread)

add_definitions( -D__TARGET_OS__Linux )

