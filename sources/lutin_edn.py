#!/usr/bin/python
import lutinModule
import lutinTools
import datetime

def Create(target):
	# module name is 'edn' and type binary.
	myModule = lutinModule.module(__file__, 'edn', 'PACKAGE')
	# add the file to compile:
	myModule.AddSrcFile([
		'appl/ctags/readtags.cpp',
		'appl/ctags/CTagsManager.cpp'])
	
	myModule.AddSrcFile([
		'appl/Debug.cpp',
		'appl/global.cpp',
		'appl/globalMsg.cpp',
		'appl/init.cpp'])
	
	# Gui:
	myModule.AddSrcFile([
		'appl/Gui/BufferView.cpp',
		'appl/Gui/TextViewer.cpp',
		'appl/Gui/MainWindows.cpp',
		'appl/Gui/Search.cpp',
		'appl/Gui/SearchData.cpp',
		'appl/Gui/TagFileSelection.cpp',
		'appl/Gui/TagFileList.cpp'])
	
	# All needed for the buffer management :
	myModule.AddSrcFile([
		'appl/Buffer/Buffer.cpp',
		'appl/Buffer/TextPlugin.cpp',
		'appl/Buffer/TextPluginCopy.cpp',
		'appl/Buffer/TextPluginMultiLineTab.cpp',
		'appl/Buffer/TextPluginManager.cpp',
		'appl/Buffer/BufferManager.cpp'])
	
	# Generic color management for the text editor : 
	myModule.AddSrcFile([
		'appl/Colorize/Colorize.cpp',
		'appl/Colorize/ColorizeManager.cpp'])
	
	# syntax coloration for the text editor
	myModule.AddSrcFile([
		'appl/Highlight/HighlightPattern.cpp',
		'appl/Highlight/Highlight.cpp',
		'appl/Highlight/HighlightManager.cpp'])
	
	myModule.AddModuleDepend('ewol')
	
	myModule.CompileFlags_CC([
		"-DPROJECT_NAME=\"\\\""+myModule.name+"\\\"\""])
	
	myModule.CopyFile('../data/icon.png','icon.png')
	
	myModule.CopyFolder('../data/icon.*','')
	myModule.CopyFolder('../data/color/*.xml','color/')
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
	
	myModule.AddPath(lutinTools.GetCurrentPath(__file__))
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/appl")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/appl/Buffer")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/appl/Buffer/EdnBuf")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/appl/Colorize")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/appl/ctags")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/appl/Gui")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/appl/Highlight")
	
	
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




