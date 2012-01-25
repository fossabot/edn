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

#include <tools_debug.h>
#include <tools_globals.h>
#include <ewol/ewol.h>
#include <Display.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <HighlightManager.h>
#include <ClipBoard.h>
#include <etk/String.h>
#include <WindowsManager.h>
#include <Search.h>
#include <unistd.h>
#include <readtags.h>
#include <CTagsManager.h>

MainWindows * basicWindows = NULL;


/**
 * @brief main application function Initialisation
 */
void APP_Init(int argc, char *argv[])
{
	EDN_INFO("Start Edn");
	ewol::ChangeSize(800, 600);

	// set the default Path of the application : 
	#ifdef PLATFORM_Linux
		etk::String homedir;
		#ifdef NDEBUG
			homedir = "/usr/share/"PROJECT_NAME"/";
		#else
			char cCurrentPath[FILENAME_MAX];
			if (!getcwd(cCurrentPath, FILENAME_MAX)) {
				homedir = "./assets/";
			} else {
				cCurrentPath[FILENAME_MAX - 1] = '\0';
				homedir = cCurrentPath;
				homedir += "/assets/";
			}
		#endif
		SetBaseFolderData(homedir.c_str());
		SetBaseFolderDataUser("~/."PROJECT_NAME"/");
		SetBaseFolderCache("/tmp/"PROJECT_NAME"/");
	#endif
	
	ewol::SetFontFolder("Font");
	//ewol::SetDefaultFont("freefont/FreeMono.ttf", 12);
	//ewol::SetDefaultFont("freefont/FreeMonoBold.ttf", 12);
	//ewol::SetDefaultFont("ACharmingFont.ttf", 45);
	//ewol::SetDefaultFont("Monospace/Monospace", 40);
	//ewol::SetDefaultFont("unispace.ttf", 12);
	
	
	EDN_DEBUG("TEST1");
	// init internal global value
	globals::init();
	EDN_DEBUG("TEST2");
	ClipBoard::Init();
	
	EDN_DEBUG("TEST3");
	
	// init ALL Singleton :
	(void)MsgBroadcastCore::getInstance();
	EDN_DEBUG("TEST4");
	//(void)AccelKey::getInstance();
	(void)WindowsManager::getInstance();
	EDN_DEBUG("TEST5");
	(void)CTagsManager::getInstance();
	EDN_DEBUG("TEST6");
	BufferManager *myBufferManager = BufferManager::getInstance();
	EDN_DEBUG("TEST7");
	
	// set color and other trucs...
	ColorizeManager *myColorManager = NULL;
	myColorManager = ColorizeManager::getInstance();
	EDN_DEBUG("TEST8");
	etk::String homedir;
	//homedir = getenv("HOME");
#ifdef NDEBUG
	homedir = "/usr/share/edn/";
#else
	homedir = "./data/";
#endif
	//homedir += "color_black.xml";
	homedir = "color_white.xml";
	myColorManager->LoadFile( homedir.c_str() );
	EDN_DEBUG("TEST9");
	myColorManager->DisplayListOfColor();
	EDN_DEBUG("TEST10");
	
	HighlightManager *myHighlightManager = NULL;
	myHighlightManager = HighlightManager::getInstance();
	EDN_DEBUG("TEST11");
	myHighlightManager->loadLanguages();
	EDN_DEBUG("TEST12");

	// open display
	MsgBroadcastCore::getInstance()->SendMessage(NULL, EDN_MSG__GUI_SHOW_MAIN_WINDOWS);
	EDN_DEBUG("TEST13");
	
	// get the curent program folder
	char cCurrentPath[FILENAME_MAX];
	if (!getcwd(cCurrentPath, FILENAME_MAX)) {
		return ;
	}
	cCurrentPath[FILENAME_MAX - 1] = '\0';
	//EDN_INFO("The current working directory is " << cCurrentPath);

	EDN_DEBUG("TEST14");
	basicWindows = new MainWindows();
	
	EDN_DEBUG("TEST15");
	
	// add files
	EDN_INFO("show list of files : ");
	/*
	for( int32_t i=1 ; i<argc; i++) {
		EDN_INFO("need load file : \"" << argv[i] << "\"" );
		etk::File myfile((char *)argv[i], etk::FILE_TYPE_DIRECT);
		if (false == myBufferManager->Exist(myfile) ) {
			int32_t idBuffOpened = myBufferManager->Open(myfile);
			if (1==i) {
				MsgBroadcastCore::getInstance()->SendMessage(NULL, EDN_MSG__CURRENT_CHANGE_BUFFER_ID, idBuffOpened);
			}
		}
	}*/
	
	EDN_DEBUG("TEST16");
	/*
	{
		etk::File myfile((char *)"licence.txt", etk::FILE_TYPE_DIRECT);
		if (false == myBufferManager->Exist(myfile) ) {
			int32_t idBuffOpened = myBufferManager->Open(myfile);
			MsgBroadcastCore::getInstance()->SendMessage(NULL, EDN_MSG__CURRENT_CHANGE_BUFFER_ID, idBuffOpened);
		}
	}
	*/
	if (NULL == basicWindows) {
		EDN_ERROR("Can not allocate the basic windows");
		ewol::Stop();
	}
	EDN_DEBUG("TEST17");
	
	// create the specific windows
	ewol::DisplayWindows(basicWindows);
	EDN_DEBUG("TEST18");
	
}


/**
 * @brief main application function Un-Initialisation
 */
void APP_UnInit(void)
{

	//Kill all singleton
	EDN_INFO("Stop BufferManager");
	//BufferManager::kill();
	EDN_INFO("Stop ColorizeManager");
	//ColorizeManager::kill();
	EDN_INFO("Stop Search");
	//Search::kill();
	EDN_INFO("Stop Accel key");
	//AccelKey::kill();
	if (NULL != basicWindows) {
		delete(basicWindows);
	}


	EDN_INFO("Stop Edn");
	//return EXIT_SUCCESS;
} 

