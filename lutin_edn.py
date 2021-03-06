#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug
import os

def get_type():
	return "BINARY"

def get_name():
	return "Editeur de N'ours"

def get_desc():
	return "Text editor for sources code with ctags management"

def get_licence():
	return "GPL-3"

def get_compagny_type():
	return "org"

def get_compagny_name():
	return "Edouard DUPIN"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [1,3,0]

def get_version_id():
	return 6

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_extra_compile_flags()
	my_module.add_src_file([
		'sources/appl/ctags/readtags.cpp'
		])
	my_module.add_src_file([
		'sources/appl/debug.cpp',
		'sources/appl/global.cpp',
		'sources/appl/globalMsg.cpp',
		'sources/appl/init.cpp'
		])
	# Gui:
	my_module.add_src_file([
		'sources/appl/Gui/BufferView.cpp',
		'sources/appl/Gui/TextViewer.cpp',
		'sources/appl/Gui/ViewerManager.cpp',
		'sources/appl/Gui/MainWindows.cpp',
		'sources/appl/Gui/Search.cpp',
		'sources/appl/Gui/TagFileSelection.cpp',
		'sources/appl/Gui/TagFileList.cpp',
		'sources/appl/Gui/WorkerSaveFile.cpp',
		'sources/appl/Gui/WorkerSaveAllFile.cpp',
		'sources/appl/Gui/WorkerCloseFile.cpp',
		'sources/appl/Gui/WorkerCloseAllFile.cpp'
		])
	# All needed for the buffer management :
	my_module.add_src_file([
		'sources/appl/Buffer.cpp',
		'sources/appl/BufferManager.cpp',
		'sources/appl/TextPlugin.cpp',
		'sources/appl/TextPluginCopy.cpp',
		'sources/appl/TextPluginMultiLineTab.cpp',
		'sources/appl/TextPluginAutoIndent.cpp',
		'sources/appl/TextPluginHistory.cpp',
		'sources/appl/TextPluginRmLine.cpp',
		'sources/appl/TextPluginSelectAll.cpp',
		'sources/appl/TextPluginCtags.cpp',
		'sources/appl/TextPluginManager.cpp'
		])
	# Generic color management for the text editor : 
	my_module.add_src_file([
		'sources/appl/GlyphDecoration.cpp',
		'sources/appl/GlyphPainting.cpp'
		])
	# syntax coloration for the text editor
	my_module.add_src_file([
		'sources/appl/HighlightPattern.cpp',
		'sources/appl/Highlight.cpp',
		'sources/appl/HighlightManager.cpp'
		])
	my_module.add_module_depend(['ewol'])
	my_module.copy_path('data/icon.*','')
	my_module.copy_path('data/languages/gcov/*.xml','languages/gcov/')
	my_module.copy_path('data/languages/asm/*.xml','languages/asm/')
	my_module.copy_path('data/languages/bash/*.xml','languages/bash/')
	my_module.copy_path('data/languages/boo/*.xml','languages/boo/')
	my_module.copy_path('data/languages/cpp/*.xml','languages/cpp/')
	my_module.copy_path('data/languages/c/*.xml','languages/c/')
	my_module.copy_path('data/languages/cmake/*.xml','languages/cmake/')
	my_module.copy_path('data/languages/glsl/*.xml','languages/glsl/')
	my_module.copy_path('data/languages/in/*.xml','languages/in/')
	my_module.copy_path('data/languages/java/*.xml','languages/java/')
	my_module.copy_path('data/languages/json/*.xml','languages/json/')
	my_module.copy_path('data/languages/lua/*.xml','languages/lua/')
	my_module.copy_path('data/languages/makefile/*.xml','languages/makefile/')
	my_module.copy_path('data/languages/matlab/*.xml','languages/matlab/')
	my_module.copy_path('data/languages/php/*.xml','languages/php/')
	my_module.copy_path('data/languages/xml/*.xml','languages/xml/')
	my_module.copy_path('data/languages/python/*.xml','languages/python/')
	my_module.copy_path('data/theme/default/*.svg','theme/shape/square/')
	my_module.copy_path('data/theme/default/*.edf','theme/shape/square/')
	my_module.copy_path('data/theme/colorWhite/*.json','theme/color/white/')
	my_module.copy_path('data/theme/colorBlack/*.json','theme/color/black/')
	my_module.copy_path('data/GUI-Search.xml','')
	
	my_module.add_path(os.path.join(tools.get_current_path(__file__), "sources"))
	
	my_module.copy_file("data/Font/freefont/FreeSerif.ttf","fonts/FreeSerif.ttf")
	my_module.copy_path("data/Font/freefont/FreeMon*.ttf","fonts/")
	
	my_module.compile_flags('c', [
		"-DPROJECT_NAME=\"\\\""+my_module.name+"\\\"\"",
		"-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	versionIDCode = str(get_version_id())
	
	# set the package properties:
	my_module.pkg_set("VERSION_CODE", versionIDCode)
	if target.name=="MacOs":
		my_module.pkg_set("ICON", os.path.join(tools.get_current_path(__file__), "data/icon.icns"))
	else:
		my_module.pkg_set("ICON", os.path.join(tools.get_current_path(__file__), "data/icon.png"))
	
	my_module.pkg_set("SECTION", ["Development", "Editors"])
	my_module.pkg_set("PRIORITY", "optional")
	my_module.pkg_add("RIGHT", "WRITE_EXTERNAL_STORAGE")
	my_module.pkg_add("RIGHT", "SET_ORIENTATION")
	
	return my_module

