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
#include <ewol/renderer/EObject.h>
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
//#include <ewol/config.h>
#include <ewol/commandLine.h>
//#include <ewol/UserConfig.h>
#include <ewol/renderer/eContext.h>

/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[])
{
	// only one things to do:
	return ewol::run(_argc, _argv);
}


/**
 * @brief main application function initialisation
 */
bool APP_Init(ewol::eContext& _context)
{
	APPL_INFO(" == > init APPL (START) [" << ewol::getBoardType() << "] (" << ewol::getCompilationMode() << ")");
	
	// TODO : remove this : Move if in the windows properties
	_context.setSize(vec2(800, 600));
	
	// select internal data for font ...
	_context.getFontDefault().setUseExternal(true);
	#ifdef __TARGET_OS__Android
		_context.getFontDefault().set("FreeSerif", 19);
	#else
		_context.getFontDefault().set("FreeSerif;DejaVuSansMono",14);
	#endif
	
	// set the application icon ...
	_context.setIcon("DATA:icon.png");
	
	// init internal global value
	globals::init();
	
	// init ALL Singleton :
	//(void)CTagsManager::getInstance();
	BufferManager::init();
	
	// set color and other trucs...
	ColorizeManager::init();
	ColorizeManager::loadFile( "white" );
	ColorizeManager::displayListOfColor();
	
	HighlightManager::init();
	HighlightManager::loadLanguages();
	cTagsManager::init();
	
	// Request load of the user configuration ...
	//ewol::userConfig::load();
	
	char cCurrentPath[FILENAME_MAX];
	// get the curent program folder
	if (!getcwd(cCurrentPath, FILENAME_MAX)) {
		return false;
	}
	cCurrentPath[FILENAME_MAX - 1] = '\0';
	//APPL_INFO("The current working directory is " << cCurrentPath);
	
	MainWindows* basicWindows = new MainWindows();
	
	if (NULL == basicWindows) {
		APPL_ERROR("Can not allocate the basic windows");
		_context.stop();
		return false;
	}
	// create the specific windows
	_context.setWindows(basicWindows);
	
	
	// add files
	APPL_INFO("show list of files : ");
	bool ctagDetected = false;
	for( int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
		etk::UString tmpppp = _context.getCmd().get(iii);
		if (tmpppp == "-t") {
			ctagDetected = true;
		} else if (true == ctagDetected) {
			APPL_INFO("Load ctag file : \"" << tmpppp << "\"" );
			ctagDetected = false;
			_context.getEObjectManager().multiCast().anonymousSend(ednMsgCtagsLoadFile, tmpppp);
		} else {
			APPL_INFO("need load file : \"" << tmpppp << "\"" );
			_context.getEObjectManager().multiCast().anonymousSend(ednMsgOpenFile, tmpppp);
		}
	}
	
	APPL_INFO(" == > init "PROJECT_NAME" (END)");
	return true;
}


/**
 * @brief main application function Un-Initialisation
 */
void APP_UnInit(ewol::eContext& _context)
{
	APPL_INFO(" == > Un-Init "PROJECT_NAME" (START)");
	ewol::Windows* tmpWindows = _context.getWindows();
	
	_context.setWindows(NULL);
	
	if (NULL != tmpWindows) {
		delete(tmpWindows);
		tmpWindows = NULL;
	}
	
	cTagsManager::unInit();
	
	APPL_INFO("Stop Hightlight");
	HighlightManager::unInit();
	//Kill all singleton
	APPL_INFO("Stop BufferManager");
	BufferManager::unInit();
	APPL_INFO("Stop ColorizeManager");
	ColorizeManager::unInit();
	APPL_INFO(" == > Un-Init "PROJECT_NAME" (END)");
}

