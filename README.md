Edn
====

`Edn` (Editeur De N'ours) is a FREE software.

This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.

Instructions
============

download the software :

	mkdir yourDevFolder
	cd yourDevFolder
	git clone git://github.com/HeeroYui/ewol.git
	cd ewol
	git submodule init
	git submodule update
	cd ..
	git clone git://github.com/HeeroYui/edn.git
	cd edn


(debug) Compile software & Run debug version:

	../ewol/build/lutin.py --color --mode=debug
	or
	../ewol/build/lutin.py --color --compilator=clang --mode=debug
	./out/Linux/debug/staging/edn//usr/bin/edn -l6 yourFile.txt
	Note : -l6 corespond at the LOG level to display.


(release) Compile software & install & run:

	# generate binary, tree, package and install it ...
	../ewol/build/lutin.py -c edn-install
	edn exemple.txt


(Android) Compile software & install

	cd yourDevFolder
	mkdir andoid
	cd android
	download here in "sdk" and "ndk" the coresponding SDK and NDK of Android:
		http://developer.android.com/tools/sdk/ndk/index.html
		http://developer.android.com/sdk/index.html
		==> you need to download sub package of the NDK (refer to the NDK doccumentation) but only supported android version >4.0
	cd ..
	# Generate package and install on Android device (in debug mode a generic key is set)
	../ewol/build/lutin.py --color --target=Android --mode=debug edn-install
	# to show the log :
	../ewol/build/lutin.py --target=Android edn-log


(Windows) Compile software & install

	cd yourDevFolder
	../ewol/build/lutin.py --color --target=Windows --mode=debug edn
	
	#we have many problem for windows compilation now...
	


(MAC) All needed and some useful packages

	- Git access (client you want or : http://code.google.com/p/git-osx-installer/ ==>need a ctrl+click to remove the normal installation control)
	- Xcode ==> for all developement packages
		- in Xcode : XCode->Setting->Download and install component: "Command Line Tools"
	
	cd yourDevFolder
	../ewol/build/lutin.py --color --mode=debug edn


(IOs) All needed and some useful packages

	- Git access (client you want or : http://code.google.com/p/git-osx-installer/ ==>need a ctrl+click to remove the normal installation control)
	- Xcode ==> for all developement packages
		- in Xcode : XCode->Setting->Download and install component: "Command Line Tools"
	
	for porting on IOs you need a developper account
	
	cd yourDevFolder
	# compile and install on simulator (no developper account needed)
	../ewol/build/lutin.py --color --target=IOs --mode=debug --simulation edn-intall
	# launch IOs simulator in xcode, and your application will appear on the second page (32 bit mode only)
	
	# compile and install on board
	../ewol/build/lutin.py --color --target=IOs --mode=debug edn-intall
	# compile and install on board and debug mode :
	../ewol/build/lutin.py --color --target=IOs --mode=debug edn-log


On linux you can generate 3 board in one time
=============================================

	cd yourDevFolder/edn
	../ewol/build/lutin.py --color --target=Linux --mode=debug edn --target=Windows --mode=debug edn --target=Android --mode=debug edn

Dependency packages
===================
	sudo apt-get install g++ libgl1-mesa-dev zlib1g-dev libasound2-dev
	# if you want to compile with clang :
	sudo apt-get install clang
	# if you want to compile for windows :
	sudo apt-get install mingw32


License (GPLv3)
==================

	Edn (Editeur De N'ours) : Source code editor
	Copyright (C) Copyright 2010 Edouard DUPIN
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

Annexe
==================
	sudo cp data/edn.desktop /usr/share/applications/
