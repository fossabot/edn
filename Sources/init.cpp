/**
 *******************************************************************************
 * @file init.cpp
 * @brief Editeur De N'ours : main fonction
 * @author Edouard DUPIN
 * @date 26/01/2011
 * @par Project
 * Edn 
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include "tools_debug.h"
#include "tools_globals.h"
#include "Display.h"
#include "BufferManager.h"
#include "ColorizeManager.h"
#include "HighlightManager.h"
#include "ClipBoard.h"
#include <string>
#include "WindowsManager.h"
#include "Search.h"
#include <unistd.h>
#include "readtags.h"
#include "CTagsManager.h"


#include "Edn.h"


/**
 * @brief Main start function of the system
 * 
 * @param[in] argc    number of argument when called
 * @param[in] argv    sus nomer arguments
 *
 * @return EXIT_SUCCESS, all time
 *
 */
int main (int argc, char *argv[])
{
	EDN_INFO("Start Edn");

	//Edn::TestUntaire_String();
	//return 0;


	// Use and remove GTK arguments from the application argument list.
	gtk_init (&argc, &argv);

	// init internal global value
	globals::init();
	ClipBoard::Init();
	Display::Init();
	
	
	// init ALL Singleton :
	(void)MsgBroadcastCore::getInstance();
	(void)AccelKey::getInstance();
	(void)WindowsManager::getInstance();
	(void)CTagsManager::getInstance();
	BufferManager *myBufferManager = BufferManager::getInstance();
	
	// set color and other trucs...
	ColorizeManager *myColorManager = NULL;
	myColorManager = ColorizeManager::getInstance();
	Edn::String homedir;
	//homedir = getenv("HOME");
#ifdef NDEBUG
	homedir = "/usr/share/edn/";
#else
	homedir = "./data/";
#endif
	homedir += "color_black.xml";
	myColorManager->LoadFile( homedir.c_str() );
	myColorManager->DisplayListOfColor();
	
	HighlightManager *myHighlightManager = NULL;
	myHighlightManager = HighlightManager::getInstance();
	myHighlightManager->loadLanguages();

	// open display
	MsgBroadcastCore::getInstance()->SendMessage(NULL, EDN_MSG__GUI_SHOW_MAIN_WINDOWS);
	
	// get the curent program folder
	char cCurrentPath[FILENAME_MAX];
	if (!getcwd(cCurrentPath, FILENAME_MAX)) {
		return -1;
	}
	cCurrentPath[FILENAME_MAX - 1] = '\0';
	//EDN_INFO("The current working directory is " << cCurrentPath);

	
	// add files
	EDN_INFO("show list of files : ");
	for( int32_t i=1 ; i<argc; i++) {
		EDN_INFO("need load file : \"" << argv[i] << "\"" );
		Edn::File myfile = (char *)argv[i];

		if (false == myBufferManager->Exist(myfile) ) {
			int32_t idBuffOpened = myBufferManager->Open(myfile);
			if (1==i) {
				MsgBroadcastCore::getInstance()->SendMessage(NULL, EDN_MSG__CURRENT_CHANGE_BUFFER_ID, idBuffOpened);
			}
		}
	}

	EDN_INFO("Start gtk main");
	gtk_main();
	EDN_INFO("Stop gtk main");

	//Kill all singleton
	EDN_INFO("Stop BufferManager");
	BufferManager::kill();
	EDN_INFO("Stop ColorizeManager");
	ColorizeManager::kill();
	EDN_INFO("Stop Search");
	Search::kill();
	EDN_INFO("Stop Accel key");
	AccelKey::kill();
	EDN_INFO("Stop Display");
	Display::UnInit();
	


	EDN_INFO("Stop Edn");
	return EXIT_SUCCESS;
} 

