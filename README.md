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

	make DEBUG=1
	or
	make CLANG=1 DEBUG=1
	./out/Linux/debug/staging/edn/usr/bin/edn -l6 yourFile.txt
	Note : -l6 corespond at the LOG level to display.
	Note : If you not compile in debug mode, you must install it to execure it.

(release) Compile software & install & run:

	# generate binary and tree
	make
	# generate .deb packages
	make final
	# install .deb packages
	make install
	edn exemple.txt

(Android) Compile software & install

	cd yourDevFolder
	mkdir andoid
	cd android
	download here in "sdk" and "ndk" the coresponding SDK and NDK of Android:
		http://developer.android.com/tools/sdk/ndk/index.html
		http://developer.android.com/sdk/index.html
		==> you need to download sub package of the NDK (refer to the NDK doccumentation) but only supported android version >4.0
	cd ../edn
	# generate .so
	make PLATFORM=Android
	# generate .apk
	make PLATFORM=Android final
	# To send it on the board :
	make PLATFORM=Android install
	# to show the log :
	make PLATFORM=Android log


(Windows) Compile software & install

	cd yourDevFolder/edn
	make PLATFORM=Windows
	# generate ...
	make PLATFORM=Windows final


Dependency packages
===================

	sudo apt-get install g++ libgl1-mesa-dev zlib1g-dev libasound2-dev
	# if you want to compile with clang :
	sudo apt-get install clang
	# if you want to compile for windows :
	sudo apt-get install mingw32


License (GPL)
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
