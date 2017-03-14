/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/widget/Manager.hpp>

#include <appl/debug.hpp>
#include <appl/global.hpp>
#include <etk/os/FSNode.hpp>
#include <etk/tool.hpp>
#include <unistd.h>
//#include <ewol/config.hpp>
#include <gale/context/commandLine.hpp>
//#include <ewol/UserConfig.hpp>
#include <ewol/context/Context.hpp>
#include <appl/TextPluginManager.hpp>
#include <appl/BufferManager.hpp>
#include <appl/HighlightManager.hpp>
#include <appl/Gui/MainWindows.hpp>
#include <appl/Gui/Search.hpp>
#include <appl/ctags/readtags.hpp>
#include <appl/globalMsg.hpp>

class MainApplication : public ewol::context::Application {
	private:
		ememory::SharedPtr<appl::BufferManager> m_bufferManager;
		ememory::SharedPtr<appl::textPluginManager> m_pluginManager;
	public:
		virtual void onCreate(ewol::Context& _context) override {
			APPL_INFO(" == > CREATE ... " << PROJECT_NAME << "  v" << APPL_VERSION << " (START) [" << gale::getBoardType() << "] (" << gale::getCompilationMode() << ") (BEGIN)");
			for( int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
				std::string tmpppp = _context.getCmd().get(iii);
				if (    tmpppp == "-h"
				     || tmpppp == "--help") {
					APPL_INFO("  -t c-flags-file-name" );
					APPL_INFO("  -h/--help display this help" );
					exit(0);
				} 
			}
			//etk::theme::setName("COLOR", "color/black/");
			etk::theme::setName("COLOR", "color/white/");
			
			// TODO : remove this : Move if in the windows properties
			_context.setSize(vec2(800, 600));
			
			_context.setTitle("edn");
			
			// select internal data for font ...
			_context.getFontDefault().setUseExternal(true);
			#ifdef __TARGET_OS__Android
				_context.getFontDefault().set("FreeSerif", 19);
			#else
				_context.getFontDefault().set("FreeSerif;DejaVuSansMono",14);
			#endif
			
			// set the application icon ...
			_context.setIcon("DATA:icon.png");
			APPL_INFO("==> CREATE ... " PROJECT_NAME " (END)");
		}
		
		void onStart(ewol::Context& _context) override {
			APPL_INFO("==> START ... " PROJECT_NAME " (BEGIN)");
			// init internal global value
			globals::init();
			// init ALL Singleton :
			//()CTagsManager::getInstance();
			m_bufferManager = appl::BufferManager::create();
			m_pluginManager = appl::textPluginManager::create();
			appl::highlightManager::init();
			// Request load of the user configuration ...
			//ewol::userConfig::load();
			
			char cCurrentPath[FILENAME_MAX];
			// get the curent program folder
			if (!getcwd(cCurrentPath, FILENAME_MAX)) {
				return;
			}
			cCurrentPath[FILENAME_MAX - 1] = '\0';
			//APPL_INFO("The current working directory is " << cCurrentPath);
			
			ememory::SharedPtr<MainWindows> basicWindows = MainWindows::create();
			
			if (basicWindows == nullptr) {
				APPL_ERROR("Can not allocate the basic windows");
				_context.exit(-1);
				return;
			}
			// create the specific windows
			_context.setWindows(basicWindows);
			
			// need to add default plugin, because they depend on the Menu widget wich might be named : "appl-menu-interface"
			m_pluginManager->addDefaultPlugin();
			
			// add files
			APPL_INFO("show list of files : ");
			bool ctagDetected = false;
			for( int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
				std::string tmpppp = _context.getCmd().get(iii);
				if (tmpppp == "-t") {
					ctagDetected = true;
				} else if (ctagDetected == true) {
					etk::FSNode file(tmpppp);
					std::string name = file.getName();
					APPL_INFO("Load ctag file : \"" << name << "\"" );
					ctagDetected = false;
					//_context.getEObjectManager().multiCast().anonymousSend(ednMsgCtagsLoadFile, name);
				} else if (    tmpppp == "-h"
				            || tmpppp == "--help") {
					// nothing to do ...
				} else {
					etk::FSNode file(tmpppp);
					if (file.getNodeType() == etk::typeNode_file) {
						std::string name = file.getName();
						APPL_INFO("need load file : \"" << name << "\"" );
						m_bufferManager->open(name);
					} else if (file.getNodeType() == etk::typeNode_folder) {
						std::vector<std::string> listOfFiles = file.folderGetSub(false, true, ".*");
						for (auto &it: listOfFiles) {
							etk::FSNode file2(it);
							if (file2.getNodeType() == etk::typeNode_file) {
								std::string name = file2.getName();
								APPL_INFO("need load file : \"" << name << "\"" );
								m_bufferManager->open(name);
							}
						}
					}
				}
			}
			
			APPL_INFO("==> START ... " PROJECT_NAME " (END)");
			return;
		}
		void onStop(ewol::Context& _context) override {
			APPL_INFO("==> STOP ... " PROJECT_NAME " (START)");
			appl::highlightManager::unInit();
			//Kill all singleton
			m_pluginManager.reset();
			m_bufferManager.reset();
			APPL_INFO("==> STOP ... " PROJECT_NAME " (END)");
		}
		void onKillDemand(ewol::Context& _context) override {
			APPL_INFO("==> User demand kill ... " PROJECT_NAME " (START)");
			if (m_bufferManager == nullptr) {
				_context.exit(0);
			}
			APPL_TODO("Implement the check of buffer not saved ...");
			_context.exit(0);
			APPL_INFO("==> User demand kill ... " PROJECT_NAME " (END)");
		}
};



/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	/*
	APPL_ERROR(" base signature = " << typeid(&MainApplication::init).name());
	APPL_ERROR(" base signature = " << typeid(std::string).name());
	APPL_CRITICAL(" END ");
	*/
	// second possibility
	return ewol::run(new MainApplication(), _argc, _argv);
}


