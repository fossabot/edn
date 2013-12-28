#!/usr/bin/python
import monkModule as module
import monkTools as tools
import monkDebug as debug
import datetime


def get_desc():
	return "EDN application N'ours editor (Editeur De N'ours)"

def create():
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'edn', 'APPLICATION')
	# enable doculentation :
	myModule.set_website("http://heeroyui.github.io/edn/")
	myModule.set_website_sources("http://github.com/heeroyui/edn/")
	myModule.set_path(tools.get_current_path(__file__) + "/appl/")
	
	# add the currrent module at the 
	return myModule


