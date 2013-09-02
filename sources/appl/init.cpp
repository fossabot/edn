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
	return ewol::Run(_argc, _argv);
}


/**
 * @brief main application function Initialisation
 */
bool APP_Init(ewol::eContext& _context)
{
	APPL_INFO("==> Init APPL (START) [" << ewol::GetBoardType() << "] (" << ewol::GetCompilationMode() << ")");
	
	// TODO : Remove this : Move if in the windows properties
	_context.SetSize(vec2(800, 600));
	
	// select internal data for font ...
	_context.GetFontDefault().SetUseExternal(true);
	#ifdef __TARGET_OS__Android
		_context.GetFontDefault().Set("FreeSerif", 19);
	#else
		_context.GetFontDefault().Set("FreeSerif;DejaVuSansMono",14);
	#endif
	
	// set the application icon ...
	_context.SetIcon("DATA:icon.png");
	
	// init internal global value
	globals::Init();
	
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
	//ewol::userConfig::Load();
	
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
		_context.Stop();
		return false;
	}
	// create the specific windows
	_context.SetWindows(basicWindows);
	
	
	// add files
	APPL_INFO("show list of files : ");
	bool ctagDetected = false;
	for( int32_t iii=0 ; iii<_context.GetCmd().Size(); iii++) {
		etk::UString tmpppp = _context.GetCmd().Get(iii);
		if (tmpppp == "-t") {
			ctagDetected = true;
		} else if (true == ctagDetected) {
			APPL_INFO("Load ctag file : \"" << tmpppp << "\"" );
			ctagDetected = false;
			_context.GetEObjectManager().MultiCast().AnonymousSend(ednMsgCtagsLoadFile, tmpppp);
		} else {
			APPL_INFO("need load file : \"" << tmpppp << "\"" );
			_context.GetEObjectManager().MultiCast().AnonymousSend(ednMsgOpenFile, tmpppp);
		}
	}
	
	APPL_INFO("==> Init "PROJECT_NAME" (END)");
	return true;
}


/**
 * @brief main application function Un-Initialisation
 */
void APP_UnInit(ewol::eContext& _context)
{
	APPL_INFO("==> Un-Init "PROJECT_NAME" (START)");
	ewol::Windows* tmpWindows = _context.GetWindows();
	
	_context.SetWindows(NULL);
	
	if (NULL != tmpWindows) {
		delete(tmpWindows);
		tmpWindows = NULL;
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

