#!/usr/bin/python
import lutinModule
import lutinTools
import datetime

def Create(target):
	# module name is 'edn' and type binary.
	myModule = lutinModule.module(__file__, 'edn', 'PACKAGE')
	# add the file to compile:
	myModule.AddSrcFile([
		'appl/ctags/readtags.cpp'])
	
	myModule.AddSrcFile([
		'appl/debug.cpp',
		'appl/global.cpp',
		'appl/globalMsg.cpp',
		'appl/init.cpp'])
	
	# Gui:
	myModule.AddSrcFile([
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
	myModule.AddSrcFile([
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
	myModule.AddSrcFile([
		'appl/GlyphDecoration.cpp',
		'appl/GlyphPainting.cpp'])
	
	# syntax coloration for the text editor
	myModule.AddSrcFile([
		'appl/HighlightPattern.cpp',
		'appl/Highlight.cpp',
		'appl/HighlightManager.cpp'])
	
	myModule.AddModuleDepend('ewol')
	
	myModule.CompileFlags_CC([
		"-DPROJECT_NAME=\"\\\""+myModule.name+"\\\"\""])
	
	myModule.CopyFile('../data/icon.png','icon.png')
	
	myModule.CopyFolder('../data/icon.*','')
	myModule.CopyFolder('../data/languages/asm/*.xml','languages/asm/')
	myModule.CopyFolder('../data/languages/bash/*.xml','languages/bash/')
	myModule.CopyFolder('../data/languages/boo/*.xml','languages/boo/')
	myModule.CopyFolder('../data/languages/c/*.xml','languages/c/')
	myModule.CopyFolder('../data/languages/glsl/*.xml','languages/glsl/')
	myModule.CopyFolder('../data/languages/in/*.xml','languages/in/')
	myModule.CopyFolder('../data/languages/java/*.xml','languages/java/')
	myModule.CopyFolder('../data/languages/json/*.xml','languages/json/')
	myModule.CopyFolder('../data/languages/lua/*.xml','languages/lua/')
	myModule.CopyFolder('../data/languages/makefile/*.xml','languages/makefile/')
	myModule.CopyFolder('../data/languages/matlab/*.xml','languages/matlab/')
	myModule.CopyFolder('../data/languages/php/*.xml','languages/php/')
	myModule.CopyFolder('../data/languages/xml/*.xml','languages/xml/')
	myModule.CopyFolder('../data/languages/python/*.xml','languages/python/')
	myModule.CopyFolder('../data/theme/default/*.svg','theme/default/')
	myModule.CopyFolder('../data/theme/colorWhite/*.json','theme/colorWhite/')
	myModule.CopyFolder('../data/theme/colorBlack/*.json','theme/colorBlack/')
	myModule.CopyFolder('../data/GUI-Search.xml','')
	
	myModule.AddPath(lutinTools.GetCurrentPath(__file__))
	
	
	myModule.CopyFile("../data/Font/freefont/FreeSerif.ttf","fonts/FreeSerif.ttf")
	myModule.CopyFolder("../data/Font/freefont/FreeMon*.ttf","fonts/")
	
	now = datetime.datetime.now()
	versionID=str(now.year-2012)+"."+str(now.month)+"."+str(now.day)
	
	# set the package properties :
	myModule.pkgSet("VERSION", versionID)
	myModule.pkgSet("COMPAGNY_TYPE", "org")
	myModule.pkgSet("COMPAGNY_NAME", "Edouard DUPIN")
	myModule.pkgSet("MAINTAINER", ["Mr DUPIN Edouard <yui.heero@gmail.com>"])
	myModule.pkgSet("ICON", lutinTools.GetCurrentPath(__file__) + "/../data/icon.png")
	myModule.pkgSet("SECTION", ["Development", "Editors"])
	myModule.pkgSet("PRIORITY", "optional")
	myModule.pkgSet("DESCRIPTION", "Text editor for sources code with ctags management")
	myModule.pkgSet("NAME", "Editeur de N'ours")
	
	myModule.pkgAdd("RIGHT", "WRITE_EXTERNAL_STORAGE")
	myModule.pkgAdd("RIGHT", "SET_ORIENTATION")
	
	# add the currrent module at the 
	return myModule




