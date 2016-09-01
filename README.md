Edn
====

`Edn` (Editeur De N'ours) is a FREE software.

This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.


[![Build Status](https://travis-ci.org/HeeroYui/edn.svg?branch=dev)](https://travis-ci.org/HeeroYui/edn)

Instructions
============

need google repo:
-----------------

see: http://source.android.com/source/downloading.html#installing-repo

	mkdir ~/.bin
	PATH=~/.bin:$PATH
	curl https://storage.googleapis.com/git-repo-downloads/repo > ~/.bin/repo
	chmod a+x ~/.bin/repo

download Build system:
----------------------

	sudo pip install lutin
	sudo pip install pillow


download the Framework:
----------------------

	mkdir -p WORKING_DIRECTORY/framework
	cd WORKING_DIRECTORY/framework
	repo init -u git://github.com/atria-soft/manifest.git
	repo sync -j8
	cd ../..

download the software:
----------------------

	mkdir -p WORKING_DIRECTORY/application
	cd WORKING_DIRECTORY/application
	git clone https://github.com/HeeroYui/edn.git
	cd ../..

Compile software:
-----------------

	cd WORKING_DIRECTORY
	lutin -C -P edn

(debug) Compile software & Run debug version:

	lutin -C -P -m debug edn
	# or
	lutin -C -P -c clang -m debug edn
	# full compile and run:
	lutin -C -P -m debug edn?build?run:--elog-level=5


(release) Compile software & install:

	# generate binary, tree, package and install it ...
	lutin -C -P edn?install


(Android) Compile software & install

	mkdir andoid
	cd android
	download here in "sdk" and "ndk" the coresponding SDK and NDK of Android:
		http://developer.android.com/tools/sdk/ndk/index.html
		http://developer.android.com/sdk/index.html
		==> you need to download sub package of the NDK (refer to the NDK doccumentation) but only supported android version >4.0
	cd ..
	# Generate package and install on Android device (in debug mode a generic key is set)
	lutin -C -P -t Android -m debug edn?install
	# to show the log :
	lutin -C -P -t Android edn?log


(Windows) Compile software

	lutin -C -P -t Windows -m debug edn


(MAC) All needed and some useful packages

	- Git access (client you want or : http://code.google.com/p/git-osx-installer/ ==>need a ctrl+click to remove the normal installation control)
	- Xcode ==> for all developement packages
		- in Xcode : XCode->Setting->Download and install component: "Command Line Tools"
	
	lutin -C -P -m debug edn


(IOs) All needed and some useful packages

	- Git access (client you want or : http://code.google.com/p/git-osx-installer/ ==>need a ctrl+click to remove the normal installation control)
	- Xcode ==> for all developement packages
		- in Xcode : XCode->Setting->Download and install component: "Command Line Tools"
	
	for porting on IOs you need a developper account
	
	# compile and install on simulator (no developper account needed)
	lutin -C -P -t IOs -m debug --simulation edn?install
	# launch IOs simulator in xcode, and your application will appear on the second page (32 bit mode only)
	
	# compile and install on board
	lutin -C -P -t IOs -m debug edn?install
	# compile and install on board and debug mode :
	lutin -C -P -t IOs -m debug --simulation edn?log

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
