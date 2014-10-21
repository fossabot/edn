#!/usr/bin/python
import lutinModule as module
import lutinTools as tools
import lutinDebug as debug
import datetime


def get_desc():
	return "EDN application N'ours editor (Editeur De N'ours)"

def create(target):
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'edn', 'PACKAGE')
	
	myModule.add_extra_compile_flags()
	# add the file to compile:
	myModule.add_src_file([
		'appl/ctags/readtags.cpp'])
	
	myModule.add_src_file([
		'appl/debug.cpp',
		'appl/global.cpp',
		'appl/globalMsg.cpp',
		'appl/init.cpp'])
	
	# Gui:
	myModule.add_src_file([
		'appl/Gui/BufferView.cpp',
		'appl/Gui/TextViewer.cpp',
		'appl/Gui/ViewerManager.cpp',
		'appl/Gui/MainWindows.cpp',
		'appl/Gui/Search.cpp',
		'appl/Gui/TagFileSelection.cpp',
		'appl/Gui/TagFileList.cpp',
		'appl/Gui/WorkerSaveFile.cpp',
		'appl/Gui/WorkerSaveAllFile.cpp',
		'appl/Gui/WorkerCloseFile.cpp',
		'appl/Gui/WorkerCloseAllFile.cpp'])
	
	# All needed for the buffer management :
	myModule.add_src_file([
		'appl/Buffer.cpp',
		'appl/BufferManager.cpp',
		'appl/TextPlugin.cpp',
		'appl/TextPluginCopy.cpp',
		'appl/TextPluginMultiLineTab.cpp',
		'appl/TextPluginAutoIndent.cpp',
		'appl/TextPluginHistory.cpp',
		'appl/TextPluginRmLine.cpp',
		'appl/TextPluginSelectAll.cpp',
		'appl/TextPluginCtags.cpp',
		'appl/TextPluginManager.cpp'])
	
	# Generic color management for the text editor : 
	myModule.add_src_file([
		'appl/GlyphDecoration.cpp',
		'appl/GlyphPainting.cpp'])
	
	# syntax coloration for the text editor
	myModule.add_src_file([
		'appl/HighlightPattern.cpp',
		'appl/Highlight.cpp',
		'appl/HighlightManager.cpp'])
	
	myModule.add_module_depend('ewol')
	
	myModule.compile_flags_CC([
		"-DPROJECT_NAME=\"\\\""+myModule.name+"\\\"\""
		])
	
	#myModule.copy_file('../data/icon.png','icon.png')
	
	myModule.copy_folder('../data/icon.*','')
	myModule.copy_folder('../data/languages/asm/*.xml','languages/asm/')
	myModule.copy_folder('../data/languages/bash/*.xml','languages/bash/')
	myModule.copy_folder('../data/languages/boo/*.xml','languages/boo/')
	myModule.copy_folder('../data/languages/cpp/*.xml','languages/cpp/')
	myModule.copy_folder('../data/languages/c/*.xml','languages/c/')
	myModule.copy_folder('../data/languages/cmake/*.xml','languages/cmake/')
	myModule.copy_folder('../data/languages/glsl/*.xml','languages/glsl/')
	myModule.copy_folder('../data/languages/in/*.xml','languages/in/')
	myModule.copy_folder('../data/languages/java/*.xml','languages/java/')
	myModule.copy_folder('../data/languages/json/*.xml','languages/json/')
	myModule.copy_folder('../data/languages/lua/*.xml','languages/lua/')
	myModule.copy_folder('../data/languages/makefile/*.xml','languages/makefile/')
	myModule.copy_folder('../data/languages/matlab/*.xml','languages/matlab/')
	myModule.copy_folder('../data/languages/php/*.xml','languages/php/')
	myModule.copy_folder('../data/languages/xml/*.xml','languages/xml/')
	myModule.copy_folder('../data/languages/python/*.xml','languages/python/')
	myModule.copy_folder('../data/theme/default/*.svg','theme/shape/square/')
	myModule.copy_folder('../data/theme/default/*.edf','theme/shape/square/')
	myModule.copy_folder('../data/theme/colorWhite/*.json','theme/color/white/')
	myModule.copy_folder('../data/theme/colorBlack/*.json','theme/color/black/')
	myModule.copy_folder('../data/GUI-Search.xml','')
	
	myModule.add_path(tools.get_current_path(__file__))
	
	myModule.copy_file("../data/Font/freefont/FreeSerif.ttf","fonts/FreeSerif.ttf")
	myModule.copy_folder("../data/Font/freefont/FreeMon*.ttf","fonts/")
	
	tagFile = tools.get_current_path(__file__) + "/tag"
	versionID = tools.file_read_data(tagFile)
	myModule.compile_flags_CC([
		"-DAPPL_VERSION=\"\\\"" + versionID + "\\\"\""
		])
	tagFile = tools.get_current_path(__file__) + "/tagCode"
	versionIDCode = tools.file_read_data(tagFile)
	
	# set the package properties :
	myModule.pkg_set("VERSION", versionID)
	myModule.pkg_set("VERSION_CODE", versionIDCode)
	myModule.pkg_set("COMPAGNY_TYPE", "org")
	myModule.pkg_set("COMPAGNY_NAME", "Edouard DUPIN")
	myModule.pkg_set("MAINTAINER", ["Mr DUPIN Edouard <yui.heero@gmail.com>"])
	if target.name=="MacOs":
		myModule.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.icns")
	else:
		myModule.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.png")
	
	myModule.pkg_set("SECTION", ["Development", "Editors"])
	myModule.pkg_set("PRIORITY", "optional")
	myModule.pkg_set("DESCRIPTION", "Text editor for sources code with ctags management")
	myModule.pkg_set("NAME", "Editeur de N'ours")
	
	myModule.pkg_add("RIGHT", "WRITE_EXTERNAL_STORAGE")
	myModule.pkg_add("RIGHT", "SET_ORIENTATION")
	
	# add the currrent module at the 
	return myModule

