#!/usr/bin/python
import module
import buildTools

# prevent erro integration
#if 'myModule' in globals():
#	del myModule

# module name is 'edn' and type binary.
myModule = module.module(__file__, 'edn', 'BINARY')
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
	'appl/Gui/CodeView.cpp',
	'appl/Gui/MainWindows.cpp',
	'appl/Gui/Search.cpp',
	'appl/Gui/SearchData.cpp',
	'appl/Gui/TagFileSelection.cpp',
	'appl/Gui/TagFileList.cpp'])

# All needed for the buffer management :
myModule.AddSrcFile([
	'appl/Buffer/EdnBuf/EdnBuf.cpp',
	'appl/Buffer/EdnBuf/EdnBuf_HighLight.cpp',
	'appl/Buffer/EdnBuf/EdnBuf_History.cpp',
	'appl/Buffer/EdnBuf/EdnBuf_Selection.cpp',
	'appl/Buffer/EdnBuf/EdnBufHistory.cpp',
	'appl/Buffer/BufferText.cpp',
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
	'-DPROJECT_NAME="\\"'+myModule.name+'\\""',
	'-DAPPL_VERSION_TAG_NAME="\\"4.25.26.23.25.88\\""'])

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
myModule.CopyFolder('../data/languages/lua/*.xml','languages/lua/')
myModule.CopyFolder('../data/languages/makefile/*.xml','languages/makefile/')
myModule.CopyFolder('../data/languages/matlab/*.xml','languages/matlab/')
myModule.CopyFolder('../data/languages/php/*.xml','languages/php/')
myModule.CopyFolder('../data/languages/xml/*.xml','languages/xml/')
myModule.CopyFolder('../data/languages/python/*.xml','languages/python/')
myModule.CopyFolder('../data/theme/default/*.svg','theme/default/')

myModule.AddPath(buildTools.GetCurrentPath(__file__))
myModule.AddPath(buildTools.GetCurrentPath(__file__)+"/appl")
myModule.AddPath(buildTools.GetCurrentPath(__file__)+"/appl/Buffer")
myModule.AddPath(buildTools.GetCurrentPath(__file__)+"/appl/Buffer/EdnBuf")
myModule.AddPath(buildTools.GetCurrentPath(__file__)+"/appl/Colorize")
myModule.AddPath(buildTools.GetCurrentPath(__file__)+"/appl/ctags")
myModule.AddPath(buildTools.GetCurrentPath(__file__)+"/appl/Gui")
myModule.AddPath(buildTools.GetCurrentPath(__file__)+"/appl/Highlight")

# add the currrent module at the 
module.AddModule(myModule)
