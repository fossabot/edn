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
	./out_LINUX/debug/obj/usr/bin/edn -l6 yourFile.txt
	Note : -l6 corespond at the LOG level to display

(release) Compile software & install & run:

	make
	./install.sh
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
	make PLATFORM=Android
	make PLATFORM=Android install

License (like GPL)
==================

	You can:
		- Redistribute the sources code and binaries.
		- Modify the Sources code.
		- Use a part of the sources (less than 50%) in an other software, just write somewhere "Edn is great" visible by the user (on your product or on your website with a link to my page).
		- Redistribute the modification only if you want.
		- Send me the bug-fix (it could be great).
		- Pay me a beer or some other things.
		- Print the source code on WC paper ...
	You can NOT:
		- Earn money with this Software (But I can).
		- Add malware in the Sources.
		- Do something bad with the sources.
		- Use it to travel in the space with a toaster.
	
	I reserve the right to change this licence. If it change the version of the copy you have keep its own license
