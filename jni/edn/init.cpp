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
#include <ewol/WidgetMessageMultiCast.h>

MainWindows * basicWindows = NULL;


/**
 * @brief main application function Initialisation
 */
void APP_Init(void)
{
	EDN_INFO("==> Init Edn (START)");
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
	ewol::SetDefaultFont("freefont/FreeSerif.ttf", 12);
	//ewol::SetDefaultFont("freefont/FreeMonoBold.ttf", 12);
	//ewol::SetDefaultFont("ACharmingFont.ttf", 45);
	//ewol::SetDefaultFont("Monospace/Monospace", 40);
	//ewol::SetDefaultFont("unispace.ttf", 12);
	
	
	// init internal global value
	globals::init();
	ClipBoard::Init();
	
	
	// init ALL Singleton :
	(void)WindowsManager::getInstance();
	(void)CTagsManager::getInstance();
	BufferManager *myBufferManager = BufferManager::getInstance();
	
	// set color and other trucs...
	ColorizeManager *myColorManager = NULL;
	myColorManager = ColorizeManager::getInstance();
	etk::String homedir = "color_white.xml";
	myColorManager->LoadFile( homedir.c_str() );
	myColorManager->DisplayListOfColor();
	
	HighlightManager *myHighlightManager = NULL;
	myHighlightManager = HighlightManager::getInstance();
	myHighlightManager->loadLanguages();

	
	// get the curent program folder
	char cCurrentPath[FILENAME_MAX];
	if (!getcwd(cCurrentPath, FILENAME_MAX)) {
		return ;
	}
	cCurrentPath[FILENAME_MAX - 1] = '\0';
	//EDN_INFO("The current working directory is " << cCurrentPath);

	basicWindows = new MainWindows();
	
	if (NULL == basicWindows) {
		EDN_ERROR("Can not allocate the basic windows");
		ewol::Stop();
		return;
	}
	// create the specific windows
	ewol::DisplayWindows(basicWindows);
	
	// add files
	EDN_INFO("show list of files : ");
	
	for( int32_t iii=0 ; iii<ewol::CmdLineNb(); iii++) {
		EDN_INFO("need load file : \"" << ewol::CmdLineGet(iii) << "\"" );
		ewol::widgetMessageMultiCast::Send(-1, ednMsgOpenFile, ewol::CmdLineGet(iii).c_str());
	}
	EDN_INFO("==> Init Edn (END)");
}


/**
 * @brief main application function Un-Initialisation
 */
void APP_UnInit(void)
{
	EDN_INFO("==> Un-Init Edn (START)");
	// Remove windows :
	ewol::DisplayWindows(NULL);
	
	//Kill all singleton
	EDN_INFO("Stop BufferManager");
	BufferManager::Kill();
	EDN_INFO("Stop ColorizeManager");
	ColorizeManager::Kill();
	EDN_INFO("Stop Search");
	Search::Kill();
	//EDN_INFO("Stop Accel key");
	//AccelKey::Kill();
	
	if (NULL != basicWindows) {
		delete(basicWindows);
	}
	basicWindows = NULL;
	EDN_INFO("==> Un-Init Edn (END)");
}

