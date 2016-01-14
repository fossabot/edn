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
	tag_file = os.path.join(tools.get_current_path(__file__), "tag")
	version_id = tools.file_read_data(tag_file)
	tmp_value = version_id.split("-")
	if len(tmp_value) > 1:
		dirty_tag = tmp_value[1]
	else:
		dirty_tag = ""
	tmp_value = tmp_value[0]
	out = []
	for elem in tmp_value.split("."):
		out.append(int(elem))
	if dirty_tag != "":
		out.append(dirty_tag)
	return out

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_extra_compile_flags()
	my_module.add_src_file([
		'appl/ctags/readtags.cpp'
		])
	my_module.add_src_file([
		'appl/debug.cpp',
		'appl/global.cpp',
		'appl/globalMsg.cpp',
		'appl/init.cpp'
		])
	# Gui:
	my_module.add_src_file([
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
		'appl/Gui/WorkerCloseAllFile.cpp'
		])
	# All needed for the buffer management :
	my_module.add_src_file([
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
		'appl/TextPluginManager.cpp'
		])
	# Generic color management for the text editor : 
	my_module.add_src_file([
		'appl/GlyphDecoration.cpp',
		'appl/GlyphPainting.cpp'
		])
	# syntax coloration for the text editor
	my_module.add_src_file([
		'appl/HighlightPattern.cpp',
		'appl/Highlight.cpp',
		'appl/HighlightManager.cpp'
		])
	my_module.add_module_depend(['ewol', 'date'])
	my_module.compile_flags('c++', [
		"-DPROJECT_NAME=\"\\\""+my_module.name+"\\\"\""
		])
	my_module.copy_path('../data/icon.*','')
	"""
	my_module.copy_path('../data/languages/gcov/*.xml','languages/gcov/')
	my_module.copy_path('../data/languages/asm/*.xml','languages/asm/')
	my_module.copy_path('../data/languages/bash/*.xml','languages/bash/')
	my_module.copy_path('../data/languages/boo/*.xml','languages/boo/')
	"""
	my_module.copy_path('../data/languages/cpp/*.xml','languages/cpp/')
	"""
	my_module.copy_path('../data/languages/c/*.xml','languages/c/')
	my_module.copy_path('../data/languages/cmake/*.xml','languages/cmake/')
	my_module.copy_path('../data/languages/glsl/*.xml','languages/glsl/')
	my_module.copy_path('../data/languages/in/*.xml','languages/in/')
	my_module.copy_path('../data/languages/java/*.xml','languages/java/')
	my_module.copy_path('../data/languages/json/*.xml','languages/json/')
	my_module.copy_path('../data/languages/lua/*.xml','languages/lua/')
	my_module.copy_path('../data/languages/makefile/*.xml','languages/makefile/')
	my_module.copy_path('../data/languages/matlab/*.xml','languages/matlab/')
	my_module.copy_path('../data/languages/php/*.xml','languages/php/')
	my_module.copy_path('../data/languages/xml/*.xml','languages/xml/')
	my_module.copy_path('../data/languages/python/*.xml','languages/python/')
	"""
	my_module.copy_path('../data/theme/default/*.svg','theme/shape/square/')
	my_module.copy_path('../data/theme/default/*.edf','theme/shape/square/')
	my_module.copy_path('../data/theme/colorWhite/*.json','theme/color/white/')
	my_module.copy_path('../data/theme/colorBlack/*.json','theme/color/black/')
	my_module.copy_path('../data/GUI-Search.xml','')
	
	my_module.add_path(tools.get_current_path(__file__))
	
	my_module.copy_file("../data/Font/freefont/FreeSerif.ttf","fonts/FreeSerif.ttf")
	my_module.copy_path("../data/Font/freefont/FreeMon*.ttf","fonts/")
	
	my_module.compile_flags('c', [
		"-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	tagFile = os.path.join(tools.get_current_path(__file__), "tagCode")
	versionIDCode = tools.file_read_data(tagFile)
	
	# set the package properties:
	my_module.pkg_set("VERSION_CODE", versionIDCode)
	if target.name=="MacOs":
		my_module.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.icns")
	else:
		my_module.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.png")
	
	my_module.pkg_set("SECTION", ["Development", "Editors"])
	my_module.pkg_set("PRIORITY", "optional")
	my_module.pkg_add("RIGHT", "WRITE_EXTERNAL_STORAGE")
	my_module.pkg_add("RIGHT", "SET_ORIENTATION")
	
	return my_module

