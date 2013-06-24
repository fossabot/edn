/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/ewol.h>
#include <ewol/eObject/EObject.h>
#include <ewol/widget/WidgetManager.h>

#include <appl/Debug.h>
#include <appl/global.h>
#include <etk/os/FSNode.h>
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
#include <ewol/config.h>
#include <ewol/commandLine.h>
#include <ewol/UserConfig.h>

MainWindows * basicWindows = NULL;


/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int argc, const char *argv[])
{
	// only one things to do : 
	return ewol::Run(argc, argv);
}


/**
 * @brief main application function Initialisation
 */
void APP_Init(void)
{
	#ifdef MODE_RELEASE
		const char * debugMode = "Release";
	#else
		const char * debugMode = "Debug";
	#endif
	#ifdef __TARGET_OS__Linux
		const char * osMode = "Linux";
	#elif defined(__TARGET_OS__Android)
		const char * osMode = "Android";
	#elif defined(__TARGET_OS__Windows)
		const char * osMode = "Windows";
	#elif defined(__TARGET_OS__IOs)
		const char * osMode = "IOs";
	#elif defined(__TARGET_OS__MacOs)
		const char * osMode = "MacOs";
	#else
		const char * osMode = "Unknown";
	#endif
	APPL_INFO("==> Init "PROJECT_NAME" (START) [" << osMode << "] (" << debugMode << ")");
	
	ewol::ChangeSize(ivec2(800, 600));
	#ifdef __TARGET_OS__Android
		ewol::config::FontSetDefault("FreeSerif", 19);
	#else
		ewol::config::FontSetDefault("FreeSerif", 14);
	#endif
	
	// set the application icon ...
	ewol::SetIcon("DATA:icon.png");
	
	// init internal global value
	globals::Init();
	// set the application icon ...
	ewol::SetIcon("DATA:icon.png");
	
	// init ALL Singleton :
	//(void)CTagsManager::getInstance();
	BufferManager::Init();
	
	// set color and other trucs...
	ColorizeManager::Init();
	ColorizeManager::LoadFile( "white" );
	ColorizeManager::DisplayListOfColor();
	
	HighlightManager::Init();
	HighlightManager::loadLanguages();
	cTagsManager::Init();
	
	// Request load of the user configuration ...
	ewol::userConfig::Load();
	
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
	ewol::WindowsSet(basicWindows);
	
	
	// add files
	APPL_INFO("show list of files : ");
	bool ctagDetected = false;
	for( int32_t iii=0 ; iii<ewol::commandLine::Size(); iii++) {
		etk::UString tmpppp = ewol::commandLine::Get(iii);
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
	
	APPL_INFO("==> Init "PROJECT_NAME" (END)");
}


/**
 * @brief main application function Un-Initialisation
 */
void APP_UnInit(void)
{
	APPL_INFO("==> Un-Init "PROJECT_NAME" (START)");
	
	if (NULL != basicWindows) {
		delete(basicWindows);
		basicWindows = NULL;
	}
	
	cTagsManager::UnInit();
	
	APPL_INFO("Stop Hightlight");
	HighlightManager::UnInit();
	//Kill all singleton
	APPL_INFO("Stop BufferManager");
	BufferManager::UnInit();
	APPL_INFO("Stop ColorizeManager");
	ColorizeManager::UnInit();
	APPL_INFO("==> Un-Init "PROJECT_NAME" (END)");
}

