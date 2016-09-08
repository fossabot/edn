#!/usr/bin/python
import lutinModule as module
import lutinTools as tools

def get_desc():
	return "gameplay : video game engine (based on bullet lib)"

"""
 multline comment
"""
'''
 multline comment
'''

def create(target):
	myModule = module.Module(__file__, 'gameplay', 'LIBRARY')
	
	# add the file to compile:
	myModule.add_src_file([
		'ege/debug.cpp',
		'ege/AudioElement.cpp',
		'ege/AudioEngine.cpp',
		'ege/Camera.cpp'
		])
	
	# myModule.copy_folder('data/ParticuleMesh.*','')
	
	# name of the dependency
	myModule.add_depend(['etk', 'ewol', 'bullet'])
	
	myModule.add_flag_CC([
		'-Wno-write-strings',
		'-Wall'])
	
	myModule.add_export_path(tools.get_current_path(__file__))
	
	
	# add the currrent module at the 
	return myModule



"""
multline comment error ...
