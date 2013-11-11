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

#include <appl/debug.h>
#include <appl/global.h>
#include <etk/os/FSNode.h>
#include <etk/tool.h>
#include <unistd.h>
//#include <ewol/config.h>
#include <ewol/commandLine.h>
//#include <ewol/UserConfig.h>
#include <ewol/renderer/eContext.h>
#include <appl/TextPluginManager.h>
#include <appl/BufferManager.h>
#include <appl/HighlightManager.h>
#include <appl/Gui/MainWindows.h>
#include <appl/Gui/Search.h>
#include <appl/ctags/readtags.h>
#include <appl/ctags/CTagsManager.h>
#include <appl/globalMsg.h>
#include <vector>
#include <string>
#include <etk/unicode.h>

char32_t mychar32;


/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	// only one things to do:
	return ewol::run(_argc, _argv);
}
appl::BufferManager* bufferManager = NULL;

etk::CCout& operator <<(etk::CCout& _os, const std::u32string& _obj) {
	etk::Vector<etk::UChar> tmpp;
	for (size_t iii=0; iii<_obj.size(); ++iii) {
		tmpp.pushBack(_obj[iii]);
	}
	etk::Vector<char> output_UTF8;
	unicode::convertUnicodeToUtf8(tmpp, output_UTF8);
	output_UTF8.pushBack('\0');
	_os << &output_UTF8[0];
	return _os;
}

/**
 * @brief main application function initialisation
 */
bool APP_Init(ewol::eContext& _context) {
	APPL_INFO(" == > init APPL (START) [" << ewol::getBoardType() << "] (" << ewol::getCompilationMode() << ")");
	
	std::vector<int32_t> valueExample;
	valueExample.push_back(23);
	valueExample.push_back(23);
	valueExample.push_back(23);
	valueExample.push_back(23);
	APPL_INFO("test de vector : " << valueExample[0]);
	std::cout << "test de debug direct ..." << std::endl;
	std::cerr << "test de debug direct .2." << std::endl;
	std::u32string ploppppp;
	ploppppp = U"exemple de texte sans accent : ";
	APPL_INFO( "retert : " << ploppppp);
	APPL_CRITICAL("kjkjhkjh");
	
	
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
	
	etk::theme::setName("COLOR", "colorBlack/");
	etk::theme::setName("COLOR", "colorWhite/");
	
	// init internal global value
	globals::init();
	
	// init ALL Singleton :
	//(void)CTagsManager::getInstance();
	bufferManager = appl::BufferManager::keep();
	
	appl::highlightManager::init();
	cTagsManager::init();
	appl::textPluginManager::init();
	appl::textPluginManager::addDefaultPlugin();
	
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
			bufferManager->open(tmpppp);
		}
	}
	
	APPL_INFO(" == > init " PROJECT_NAME " (END)");
	return true;
}


/**
 * @brief main application function Un-Initialisation
 */
void APP_UnInit(ewol::eContext& _context) {
	APPL_INFO(" == > Un-Init " PROJECT_NAME " (START)");
	ewol::Windows* tmpWindows = _context.getWindows();
	
	_context.setWindows(NULL);
	
	if (NULL != tmpWindows) {
		delete(tmpWindows);
		tmpWindows = NULL;
	}
	appl::textPluginManager::unInit();
	cTagsManager::unInit();
	
	APPL_INFO("Stop Hightlight");
	appl::highlightManager::unInit();
	//Kill all singleton
	if (bufferManager != NULL) {
		APPL_INFO("Stop BufferManager");
		appl::BufferManager::release(bufferManager);
		bufferManager = NULL;
	}
	APPL_INFO(" == > Un-Init " PROJECT_NAME " (END)");
}

