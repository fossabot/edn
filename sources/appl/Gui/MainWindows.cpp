/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/global.h>
#include <appl/Gui/MainWindows.h>
#include <appl/Gui/BufferView.h>
#include <appl/Gui/TextViewer.h>
#include <appl/Gui/Search.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/Manager.h>
#include <ewol/object/Object.h>
#include <ewol/context/Context.h>
#include <date/date.h>
#include <ewol/widget/meta/StdPopUp.h>
#include <appl/Gui/WorkerSaveFile.h>
#include <appl/Gui/WorkerSaveAllFile.h>
#include <appl/Gui/WorkerCloseFile.h>
#include <appl/Gui/WorkerCloseAllFile.h>

namespace appl {
	std::string getVersion() {
		#define FIRST_YEAR (2010)
		std::string tmpOutput = etk::to_string(date::getYear()-FIRST_YEAR);
		tmpOutput += ".";
		tmpOutput += etk::to_string(date::getMonth());
		tmpOutput += ".";
		tmpOutput += etk::to_string(date::getDay());
		return tmpOutput;
	}
	
}


#undef __class__
#define __class__	"AboutGui"

#include <ewol/widget/Label.h>
#include <ewol/widget/Spacer.h>

class ParameterAboutGui : public ewol::widget::Sizer {
	public :
		ParameterAboutGui() {
			addObjectType("appl::ParameterAboutGui");
		}
		void init() {
			ewol::widget::Sizer::init(ewol::widget::Sizer::modeVert);
			std::shared_ptr<ewol::widget::Spacer> mySpacer;
			
			mySpacer = ewol::widget::Spacer::create();
			if (nullptr == mySpacer) {
				APPL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->setExpand(bvec2(true,true));
				subWidgetAdd(mySpacer);
			}
			std::string tmpLabel = "<left>";
			tmpLabel += "  <b>Editeur De N'ours</b> : v:";
			tmpLabel += appl::getVersion();
			tmpLabel += "<br/>";
			tmpLabel += "  <b>Build Time</b> : ";
			tmpLabel += date::getYear();
			tmpLabel += "/";
			tmpLabel += date::getMonth();
			tmpLabel += "/";
			tmpLabel += date::getDay();
			tmpLabel += " ";
			tmpLabel += date::getHour();
			tmpLabel += "h";
			tmpLabel += date::getMinute();
			tmpLabel += "<br/>";
			tmpLabel += "  <b>Website</b> : https://github.com/HeeroYui/edn<br/>";
			tmpLabel += "  <b>License</b> : GPL v3<br/>";
			tmpLabel += "  <b>Copyright</b> : 2010 Edouard DUPIN<br/>";
			tmpLabel += "<br/>";
			tmpLabel += "  dependency librairies :<br/>";
			tmpLabel += "    libPng, ogg-tremor, portaudio, libZip<br/>";
			tmpLabel += "    tinyXml, freetype, agg2.4, etk<br/>";
			tmpLabel += "</left>";
			std::shared_ptr<ewol::widget::Label> myLabel = ewol::widget::Label::create(tmpLabel);
			if (nullptr == myLabel) {
				APPL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				myLabel->setExpand(bvec2(true,false));
				subWidgetAdd(myLabel);
			}
		};
		
		~ParameterAboutGui() {
			
		};
};
// Local main windows event :
static const char* mainWindowsRequestSaveFile = "appl-event-main-windows-save-file";
static const char* mainWindowsRequestSaveFileAs = "appl-event-main-windows-save-file-as";
static const char* mainWindowsRequestcloseFileNoCheck = "appl-event-main-windows-close-file-no-check";
static const char* l_MsgNameGuiChangeShape = "appl-event-main-windows-Change-shape";

const char* l_smoothChick = "tmpEvent_smooth";
const char* l_smoothMin = "tmpEvent_minChange";
const char* l_smoothMax = "tmpEvent_maxChange";

#undef __class__
#define __class__ "MainWindows"

MainWindows::MainWindows() {
	addObjectType("appl::MainWindows");
}

void MainWindows::init() {
	ewol::widget::Windows::init();
	APPL_DEBUG("CREATE WINDOWS ... ");
	std::shared_ptr<ewol::widget::Sizer> mySizerVert;
	std::shared_ptr<ewol::widget::Sizer> mySizerVert2;
	std::shared_ptr<ewol::widget::Sizer> mySizerHori;
	std::shared_ptr<appl::TextViewer> myTextView;
	std::shared_ptr<appl::TextViewer> myTextView2;
	std::shared_ptr<BufferView> myBufferView;
	std::shared_ptr<ewol::widget::Menu> myMenu;
	
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
	m_viewerManager = appl::ViewerManager::create();
	
	mySizerVert = ewol::widget::Sizer::create(ewol::widget::Sizer::modeVert);
	mySizerVert->setName("plop 1111111");
	setSubWidget(mySizerVert);
	
		mySizerHori = ewol::widget::Sizer::create(ewol::widget::Sizer::modeHori);
		mySizerHori->setName("plop 222222222");
		mySizerVert->subWidgetAdd(mySizerHori);
			myBufferView = BufferView::create();
			myBufferView->setName("plop 3333333");
			myBufferView->setExpand(bvec2(false,true));
			myBufferView->setFill(bvec2(true,true));
			mySizerHori->subWidgetAdd(myBufferView);
			
			mySizerVert2 = ewol::widget::Sizer::create(ewol::widget::Sizer::modeVert);
			mySizerHori->subWidgetAdd(mySizerVert2);
				mySizerVert2->setName("appl-view-code-sizer");
				// main buffer Area :
				#if defined(__TARGET_OS__Android)
					int32_t sizeText = 16;
				#else
					int32_t sizeText = 11;
				#endif
				myTextView2 = appl::TextViewer::create("FreeMono;DejaVuSansMono;FreeSerif", sizeText);
				myTextView2->setName("appl-text-viewer2");
				myTextView2->setExpand(bvec2(true,true));
				myTextView2->setFill(bvec2(true,true));
				myTextView2->hide();
				mySizerVert2->subWidgetAdd(myTextView2);
				
				myTextView = appl::TextViewer::create("FreeMono;DejaVuSansMono;FreeSerif", sizeText);
				myTextView->setName("appl-text-viewer1");
				myTextView->setExpand(bvec2(true,true));
				myTextView->setFill(bvec2(true,true));
				mySizerVert2->subWidgetAdd(myTextView);
				
				// search area : 
				m_widgetSearch = appl::widget::Search::create();
				mySizerVert2->subWidgetAdd(m_widgetSearch);
			
		mySizerHori = ewol::widget::Sizer::create(ewol::widget::Sizer::modeHori);
		mySizerHori->setName("plop 555555");
		mySizerVert->subWidgetAdd(mySizerHori);
			
			myMenu = ewol::widget::Menu::create();
			myMenu->setName("appl-menu-interface");
			mySizerHori->subWidgetAdd(myMenu);
			int32_t idMenuFile = myMenu->addTitle("File");
				myMenu->add(idMenuFile, "New",          "", "menu:new");
				myMenu->addSpacer();
				myMenu->add(idMenuFile, "Open",         "THEME:GUI:Load.edf", "menu:open");
				myMenu->add(idMenuFile, "Close",        "THEME:GUI:Close.edf", "menu:close");
				myMenu->add(idMenuFile, "Close (all)",  "", "menu:close-all");
				myMenu->add(idMenuFile, "Save",         "THEME:GUI:Save.edf", "menu:save");
				myMenu->add(idMenuFile, "Save As ...",  "", "menu:save-as");
				myMenu->addSpacer();
				myMenu->add(idMenuFile, "Properties",   "THEME:GUI:Parameter.edf", "menu:property");
			int32_t idMenuEdit = myMenu->addTitle("Edit");
				myMenu->add(idMenuEdit, "Goto line ...","", "menu:goto-line");
			int32_t idMenuSearch = myMenu->addTitle("Search");
				myMenu->add(idMenuSearch, "Search",         "THEME:GUI:Search.edf", "menu:search");
				myMenu->add(idMenuSearch, "Replace",        "THEME:GUI:Replace.edf", "menu:replace");
				myMenu->addSpacer();
				myMenu->add(idMenuSearch, "Find (previous)","", "menu:find:previous");
				myMenu->add(idMenuSearch, "Find (next)",    "", "menu:find:next");
				myMenu->add(idMenuSearch, "Find (all)",     "", "menu:find:all");
				myMenu->add(idMenuSearch, "Un-Select",      "", "menu:find:none");
			int32_t idMenugDisplay = myMenu->addTitle("Display");
				myMenu->add(idMenugDisplay, "Color Black",          "", "menu:color:color/black/");
				myMenu->add(idMenugDisplay, "Color White",          "", "menu:color:color/white/");
				myMenu->add(idMenugDisplay, "Shape square",         "", "menu:shape:shape/square/");
				myMenu->add(idMenugDisplay, "Shape round",          "", "menu:shape:shape/round/");
				myMenu->addSpacer();
				myMenu->add(idMenugDisplay, "Reload openGl Shader", "", "menu:reloadShape");
				myMenu->addSpacer();
				myMenu->add(idMenugDisplay, "Split", "", "menu:split:enable");
				myMenu->add(idMenugDisplay, "Unsplit", "", "menu:split:disable");
				myMenu->add(idMenugDisplay, "Vertical", "", "menu:split:vert");
				myMenu->add(idMenugDisplay, "Horizontal", "", "menu:split:hori");
			myMenu->signalSelect.bind(shared_from_this(), &MainWindows::onCallbackMenuEvent);
			m_widgetLabelFileName = ewol::widget::Label::create("FileName");
			m_widgetLabelFileName->setName("appl-widget-display-name");
			m_widgetLabelFileName->setExpand(bvec2(true,false));
			m_widgetLabelFileName->setFill(bvec2(true,false));;
			mySizerHori->subWidgetAdd(m_widgetLabelFileName);
	
	// add generic shortcut ...
	shortCutAdd("ctrl+o",       "menu:open");
	shortCutAdd("ctrl+n",       "menu:new");
	
	shortCutAdd("ctrl+s",       "menu:save");
	shortCutAdd("ctrl+shift+s", "menu:save-all");
	
	shortCutAdd("ctrl+q",       "menu:close");
	shortCutAdd("ctrl+shift+q", "menu:close-all");
	
	shortCutAdd("ctrl+l",       "menu:goto-line");
	
	shortCutAdd("ctrl+f",       "menu:search");
	shortCutAdd("F12",          "menu:reloade-shader");
	// TODO : auto-bind on shortcut event ==> maybe do beter later ...
	signalShortcut.bind(shared_from_this(), &MainWindows::onCallbackShortCut);
	m_bufferManager->signalSelectFile.bind(shared_from_this(), &MainWindows::onCallbackShortCut);
}


MainWindows::~MainWindows() {
	
}



void MainWindows::onCallbackShortCut(const std::string& _value) {
	APPL_WARNING("Event from ShortCut : " << _value);
	onCallbackMenuEvent(_value);
}

void MainWindows::onCallbackMenuEvent(const std::string& _value) {
	APPL_WARNING("Event from Menu : " << _value);
	if (_value == "menu:new") {
		if (m_bufferManager != nullptr) {
			m_bufferManager->createNewBuffer();
		}
	} else if (_value == "menu:open") {
		displayOpen();
	} else if (_value == "menu:close") {
		std::shared_ptr<appl::WorkerCloseFile> worker = appl::WorkerCloseFile::create();
		worker->startAction("");
	} else if (_value == "menu:close-all") {
		appl::WorkerCloseAllFile::create();
	} else if (_value == "menu:save") {
		appl::WorkerSaveFile::create("", false);
	} else if (_value == "menu:save-all") {
		appl::WorkerSaveAllFile::create();
	} else if (_value == "menu:save-as") {
		appl::WorkerSaveFile::create("", true);
	} else if (_value == "menu:property") {
		displayProperty();
	} else if (_value == "menu:search") {
		if (m_widgetSearch == nullptr) {
			return;
		}
		if (m_widgetSearch->isHide()) {
			m_widgetSearch->show();
			m_widgetSearch->selectSearch();
		} else {
			if (m_widgetSearch->isSelectSearch()) {
				m_widgetSearch->hide();
				if (m_viewerManager != nullptr) {
					std::shared_ptr<appl::TextViewer> tmp = m_viewerManager->getViewerSelected();
					if (tmp != nullptr) {
						tmp->keepFocus();
					}
				}
			} else {
				m_widgetSearch->selectSearch();
			}
		}
	} else if (_value == "menu:replace") {
		if (m_widgetSearch == nullptr) {
			return;
		}
		if (m_widgetSearch->isHide()) {
			m_widgetSearch->show();
			m_widgetSearch->selectReplace();
		} else {
			if (m_widgetSearch->isSelectReplace()) {
				m_widgetSearch->hide();
				if (m_viewerManager != nullptr) {
					std::shared_ptr<appl::TextViewer> tmp = m_viewerManager->getViewerSelected();
					if (tmp != nullptr) {
						tmp->keepFocus();
					}
				}
			} else {
				m_widgetSearch->selectReplace();
			}
		}
	} else if (_value == "menu:find:previous") {
		APPL_TODO("Event from Menu : " << _value);
	} else if (_value == "menu:find:next") {
		APPL_TODO("Event from Menu : " << _value);
	} else if (_value == "menu:find:all") {
		APPL_TODO("Event from Menu : " << _value);
	} else if (_value == "menu:find:none") {
		APPL_TODO("Event from Menu : " << _value);
	} else if (    _value == "menu:color:color/black/"
	            || _value == "menu:color:color/white/") {
		etk::theme::setName("COLOR", std::string(_value, 12));
		ewol::getContext().getResourcesManager().reLoadResources();
		ewol::getContext().forceRedrawAll();
	} else if (    _value == "menu:shape:shape/square/"
	            || _value == "menu:shape:shape/round/") {
		etk::theme::setName("GUI", std::string(_value, 12));
		ewol::getContext().getResourcesManager().reLoadResources();
		ewol::getContext().forceRedrawAll();
	} else if (_value == "menu:reloadShape") {
		ewol::getContext().getResourcesManager().reLoadResources();
		ewol::getContext().forceRedrawAll();
	} else if (_value == "menu:split:enable") {
		parameterSetOnWidgetNamed("appl-text-viewer2", "hide", "false");
	} else if (_value == "menu:split:disable") {
		parameterSetOnWidgetNamed("appl-text-viewer2", "hide", "true");
	} else if (_value == "menu:split:vert") {
		parameterSetOnWidgetNamed("appl-view-code-sizer", "mode", "vert");
	} else if (_value == "menu:split:hori") {
		parameterSetOnWidgetNamed("appl-view-code-sizer", "mode", "hori");
	} else {
		APPL_ERROR("Event from Menu UNKNOW : '" << _value << "'");
	}
}
/* TODO :
	} else if (_msg.getMessage() == mainWindowsRequestSaveFile) { // return after a choice of close...
		if (m_bufferManager->exist(_msg.getData()) == false) {
			APPL_ERROR("Try to save an non-existant file :" << _msg.getData());
			return;
		}
		std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(_msg.getData());
		if (tmpBuffer == nullptr) {
			APPL_ERROR("Error to get the buffer : " << _msg.getData());
			return;
		}
		if (tmpBuffer->hasFileName() == false) {
			APPL_ERROR("Will never arrive");
			saveAsPopUp(tmpBuffer);
		} else {
			if (tmpBuffer->storeFile() == false) {
				APPL_ERROR("Error when loading the file " << _msg.getData());
				displayErrorMessage("Error when loading the file <br/><i>" + _msg.getData() + "</i>");
			}
		}
	} else if (_msg.getMessage() == mainWindowsRequestSaveFileAs) { // return after a choice of close...
		if (m_bufferManager->exist(_msg.getData()) == false) {
			APPL_ERROR("Try to save an non-existant file :" << _msg.getData());
			return;
		}
		std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(_msg.getData());
		if (tmpBuffer == nullptr) {
			APPL_ERROR("Error to get the buffer : " << _msg.getData());
			return;
		}
		saveAsPopUp(tmpBuffer);
	} else if (_msg.getMessage() == mainWindowsRequestcloseFileNoCheck) { // return after a choice of close...
		if (m_bufferManager->exist(_msg.getData()) == false) {
			APPL_ERROR("Try to save an non-existant file :" << _msg.getData());
			return;
		}
		std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(_msg.getData());
		if (tmpBuffer == nullptr) {
			APPL_ERROR("Error to get the buffer : " << _msg.getData());
			return;
		}
		// note: just remove ==> no check : normal case ...
		//tmpBuffer->removeObject();
		EWOL_TODO("call remove buffer ...");
	}
*/

void MainWindows::displayOpen() {
	std::shared_ptr<ewol::widget::FileChooser> tmpWidget = ewol::widget::FileChooser::create();
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not open File chooser !!! ");
		return;
	}
	tmpWidget->setTitle("Open files ...");
	tmpWidget->setValidateLabel("Open");
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		return;
	}
	// Get a ref on the buffer selected (if null, no buffer was selected ...)
	std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->getBufferSelected();
	if (tmpBuffer != nullptr) {
		etk::FSNode tmpFile = tmpBuffer->getFileName();
		tmpWidget->setFolder(tmpFile.getNameFolder());
	}
	// apply widget pop-up ...
	popUpWidgetPush(tmpWidget);
	tmpWidget->signalValidate.bind(shared_from_this(), &MainWindows::onCallbackPopUpFileSelected);
}

void MainWindows::displayProperty() {
	// Request the parameter GUI
	std::shared_ptr<ewol::widget::Parameter> tmpWidget = ewol::widget::Parameter::create();
	if (nullptr == tmpWidget) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		#ifdef SDGSDFGSDFGSDFGSDFGSTERGDHFGHFDS
		std::string menuDescription = "<title>Properties</title>\n";
		menuDescription += "<group title='Editor'>\n";
		menuDescription += "	<menu title='Editor Interface' short-title='Editor' widget='appl-text-viewer'>\n";
		menuDescription += "</group>\n";
		menuDescription += "<group title='Gui'>\n";
		menuDescription += "	<menu title='Font selection' short-title='Font' widget=''>\n";
		menuDescription += "	<menu title='Color selection' short-title='Color' widget=''>\n";
		menuDescription += "	<menu title='Theme selection' short-title='Theme' widget=''>\n";
		menuDescription += "</group>\n";
		
		tmpWidget->setMenu(menuDescription);
		#endif
		tmpWidget->setTitle("Properties");
		popUpWidgetPush(tmpWidget);
		tmpWidget->menuAddGroup("Editor");
		std::shared_ptr<ewol::Widget> tmpSubWidget = globals::ParameterGlobalsGui::create();
		tmpWidget->menuAdd("Editor",          "", tmpSubWidget);
		tmpWidget->menuAdd("Font & Color", "", nullptr);
		tmpWidget->menuAdd("Highlight",       "", nullptr);
		tmpWidget->menuAddGroup("General");
		tmpWidget->menuAdd("Display",       "", nullptr);
		tmpSubWidget = ParameterAboutGui::create();
		tmpWidget->menuAdd("About",           "", tmpSubWidget);
	}
}

void MainWindows::onCallbackselectNewFile(const std::string& _value) {
	APPL_INFO("onCallbackselectNewFile(" << _value << ")");
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		return;
	}
	// TODO : Remove all previous binding from the old buffer ...
	onCallbackTitleUpdate();
	std::shared_ptr<appl::Buffer> tmpp = m_bufferManager->getBufferSelected();
	if (tmpp != nullptr) {
		tmpp->signalIsSave.bind(shared_from_this(), &MainWindows::onCallbackTitleUpdate);
		tmpp->signalIsModify.bind(shared_from_this(), &MainWindows::onCallbackTitleUpdate);
		tmpp->signalChangeName.bind(shared_from_this(), &MainWindows::onCallbackTitleUpdate);
	}
}

void MainWindows::onCallbackPopUpFileSelected(const std::string& _value) {
	APPL_INFO("onCallbackPopUpFileSelected(" << _value << ")");
	APPL_DEBUG("Request opening the file : " << _value);
	m_bufferManager->open(_value);
}

void MainWindows::onCallbackTitleUpdate() {
	APPL_INFO("onCallbackTitleUpdate()");
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		return;
	}
	// select a new Buffer ==> change title:
	std::shared_ptr<appl::Buffer> tmpp = m_bufferManager->getBufferSelected();
	if (tmpp == nullptr) {
		setTitle("Edn");
		if (m_widgetLabelFileName != nullptr) {
			m_widgetLabelFileName->setLabel("");
		}
	} else {
		std::string nameFileSystem = etk::FSNode(tmpp->getFileName()).getFileSystemName();
		setTitle(std::string("Edn : ") + (tmpp->isModify()==true?" *":"") + nameFileSystem);
		if (m_widgetLabelFileName != nullptr) {
			m_widgetLabelFileName->setLabel(nameFileSystem + (tmpp->isModify()==true?" *":""));
		}
	}
}

void MainWindows::saveAsPopUp(const std::shared_ptr<appl::Buffer>& _buffer) {
	if (_buffer == nullptr) {
		APPL_ERROR("Call With nullptr input...");
		return;
	}
	std::shared_ptr<appl::WorkerSaveFile> tmpObject = appl::WorkerSaveFile::create(_buffer->getFileName());
}

void MainWindows::closeNotSavedFile(const std::shared_ptr<appl::Buffer>& _buffer) {
	if (_buffer == nullptr) {
		APPL_ERROR("Call With nullptr input...");
		return;
	}
	std::shared_ptr<ewol::widget::StdPopUp> tmpPopUp = ewol::widget::StdPopUp::create();
	if (tmpPopUp == nullptr) {
		APPL_ERROR("Can not create a simple pop-up");
		return;
	}
	tmpPopUp->setTitle("<bold>Close un-saved file:</bold>");
	tmpPopUp->setComment("The file named : <i>\"" + _buffer->getFileName() + "\"</i> is curently modify.   <br/>If you don't saves these modifications,<br/>they will be definitly lost...");
	std::shared_ptr<ewol::widget::Button> bt = nullptr;
	if (_buffer->hasFileName() == true) {
		bt = tmpPopUp->addButton("Save", true);
		if (bt != nullptr) {
			// TODO : The element is removed before beeing pressed
			// TODO : bt->signalPressed.bind(shared_from_this(), mainWindowsRequestSaveFile, _buffer->getFileName());
			// TODO : bt->signalPressed.bind(shared_from_this(), mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
		}
	}
	bt = tmpPopUp->addButton("Save As", true);
	if (bt != nullptr) {
		// TODO : bt->signalPressed.bind(shared_from_this(), mainWindowsRequestSaveFileAs, _buffer->getFileName());
		//bt->signalPressed.bind(shared_from_this(), mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
		// TODO : Request the close when saved ...
	}
	bt = tmpPopUp->addButton("Close", true);
	if (bt != nullptr) {
		// TODO : bt->signalPressed.bind(shared_from_this(), mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
	}
	tmpPopUp->addButton("Cancel", true);
	tmpPopUp->setRemoveOnExternClick(true);
	popUpWidgetPush(tmpPopUp);
}
