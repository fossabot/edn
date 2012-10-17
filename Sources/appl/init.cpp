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

#include <etk/UString.h>
#include <ewol/ewol.h>
#include <ewol/eObject/EObject.h>
#include <ewol/widget/WidgetManager.h>

#include <appl/Debug.h>
#include <appl/global.h>
#include <etk/File.h>
#include <etk/tool.h>
#include <Gui/MainWindows.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <HighlightManager.h>
#include <Gui/Search.h>
#include <unistd.h>
#include <readtags.h>
#include <CTagsManager.h>
#include <globalMsg.h>
#include <ewol/font/FontManager.h>

MainWindows * basicWindows = NULL;


/**
 * @brief main application function Initialisation
 */
void APP_Init(void)
{
	#ifdef __TARGET_OS__Linux
		#ifdef MODE_RELEASE
			APPL_INFO("==> Init "PROJECT_NAME" (START) (Linux) (Release)");
		#else
			APPL_INFO("==> Init "PROJECT_NAME" (START) (Linux) (Debug)");
		#endif
	#else
		#ifdef MODE_RELEASE
			APPL_INFO("==> Init "PROJECT_NAME" (START) (Android) (Release)");
		#else
			APPL_INFO("==> Init "PROJECT_NAME" (START) (Android) (Debug)");
		#endif
	#endif
	ewol::ChangeSize(Vector2D<int32_t>(800, 600));
	etk::InitDefaultFolder(PROJECT_NAME);

	ewol::font::SetDefaultFont("Font/freefont/FreeSerif.ttf");
	#ifdef __TARGET_OS__Android
		ewol::font::SetDefaultSize(19);
	#else
		ewol::font::SetDefaultSize(14);
	#endif
	// init internal global value
	globals::init();
	
	
	// init ALL Singleton :
	//(void)CTagsManager::getInstance();
	BufferManager::Init();
	
	// set color and other trucs...
	ColorizeManager::Init();
	etk::UString corlorFile = "color_white.xml";
	ColorizeManager::LoadFile( corlorFile.c_str() );
	ColorizeManager::DisplayListOfColor();
	
	HighlightManager::Init();
	HighlightManager::loadLanguages();
	cTagsManager::Init();

	char cCurrentPath[FILENAME_MAX];
	// get the curent program folder
	if (!getcwd(cCurrentPath, FILENAME_MAX)) {
		return ;
	}
	cCurrentPath[FILENAME_MAX - 1] = '\0';
	//APPL_INFO("The current working directory is " << cCurrentPath);

	basicWindows = new MainWindows();
	
	if (NULL == basicWindows) {
		APPL_ERROR("Can not allocate the basic windows");
		ewol::Stop();
		return;
	}
	// create the specific windows
	ewol::DisplayWindows(basicWindows);
	
	// add generic shortcut ...
	//                 (shift, control, alt,  meta,  uniChar_t unicodeValue, const char * generateEventId, etk::UString& data)
	ewol::shortCut::Add("ctrl+o",       ednMsgGuiOpen,  "");
	ewol::shortCut::Add("ctrl+n",       ednMsgGuiNew,   "");
	
	ewol::shortCut::Add("ctrl+s",       ednMsgGuiSave, "current");
	ewol::shortCut::Add("ctrl+shift+s", ednMsgGuiSave, "All");
	
	ewol::shortCut::Add("ctrl+q",       ednMsgGuiClose, "current");
	ewol::shortCut::Add("ctrl+shift+q", ednMsgGuiClose, "All");
	
	ewol::shortCut::Add("ctrl+z",       ednMsgGuiUndo, "");
	ewol::shortCut::Add("ctrl+shift+z", ednMsgGuiRedo, "");
	
	ewol::shortCut::Add("ctrl+w",       ednMsgGuiRm,     "Line");
	ewol::shortCut::Add("ctrl+shift+w", ednMsgGuiRm,     "Paragraph");
	ewol::shortCut::Add("ctrl+x",       ednMsgGuiCut,    "STD");
	ewol::shortCut::Add("ctrl+c",       ednMsgGuiCopy,   "STD");
	ewol::shortCut::Add("ctrl+v",       ednMsgGuiPaste,  "STD");
	ewol::shortCut::Add("ctrl+a",       ednMsgGuiSelect, "ALL");
	ewol::shortCut::Add("ctrl+shift+a", ednMsgGuiSelect, "NONE");
	
	ewol::shortCut::Add("ctrl+l",       ednMsgGuiGotoLine, "???");
	
	ewol::shortCut::Add("ctrl+f",       ednMsgGuiSearch, "");
	ewol::shortCut::Add("F12",          ednMsgGuiReloadShader, "");
	
	ewol::shortCut::Add("ctrl+d",       ednMsgGuiCtags, "Jump");
	
	
	// add files
	APPL_INFO("show list of files : ");
	bool ctagDetected = false;
	for( int32_t iii=0 ; iii<ewol::CmdLine::Nb(); iii++) {
		etk::UString tmpppp = ewol::CmdLine::Get(iii);
		if (tmpppp == "-t") {
			ctagDetected = true;
		} else if (true == ctagDetected) {
			APPL_INFO("Load ctag file : \"" << tmpppp << "\"" );
			ctagDetected = false;
			ewol::EObjectMessageMultiCast::AnonymousSend(ednMsgCtagsLoadFile, tmpppp);
		} else {
			APPL_INFO("need load file : \"" << tmpppp << "\"" );
			ewol::EObjectMessageMultiCast::AnonymousSend(ednMsgOpenFile, tmpppp);
		}
	}
	
	APPL_INFO("==> Init Edn (END)");
}


etk::File APP_Icon(void)
{
	etk::File bitmapFile("iconEdn.bmp", etk::FILE_TYPE_DATA);
	return bitmapFile;
}

/**
 * @brief main application function Un-Initialisation
 */
void APP_UnInit(void)
{
	APPL_INFO("==> Un-Init Edn (START)");
	// Remove windows :
	ewol::DisplayWindows(NULL);
	
	cTagsManager::UnInit();
	
	APPL_INFO("Stop Hightlight");
	HighlightManager::UnInit();
	//Kill all singleton
	APPL_INFO("Stop BufferManager");
	BufferManager::UnInit();
	APPL_INFO("Stop ColorizeManager");
	ColorizeManager::UnInit();
	APPL_INFO("Stop Search");
	//Search::Kill();
	//APPL_INFO("Stop Accel key");
	//AccelKey::Kill();
	
	if (NULL != basicWindows) {
		delete(basicWindows);
		basicWindows = NULL;
	}
	APPL_INFO("==> Un-Init Edn (END)");
}

