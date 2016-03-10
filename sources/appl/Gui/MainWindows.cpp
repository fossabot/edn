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
			propertyMode.setDirectCheck(ewol::widget::Sizer::modeVert);
			ewol::widget::Sizer::init();
			std::shared_ptr<ewol::widget::Spacer> mySpacer;
			
			mySpacer = ewol::widget::Spacer::create();
			if (nullptr == mySpacer) {
				APPL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				mySpacer->propertyExpand.set(bvec2(true,true));
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
			std::shared_ptr<ewol::widget::Label> myLabel = ewol::widget::Label::create();
			if (nullptr == myLabel) {
				APPL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				myLabel->propertyValue.set(tmpLabel);
				myLabel->propertyExpand.set(bvec2(true,false));
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
	ewol::widget::SizerShared mySizerVert;
	ewol::widget::SizerShared mySizerVert2;
	ewol::widget::SizerShared mySizerHori;
	std::shared_ptr<appl::TextViewer> myTextView;
	std::shared_ptr<appl::TextViewer> myTextView2;
	std::shared_ptr<BufferView> myBufferView;
	ewol::widget::MenuShared myMenu;
	
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
	m_viewerManager = appl::ViewerManager::create();
	
	mySizerVert = ewol::widget::Sizer::create();
	mySizerVert->propertyName.set("plop 1111111");
	mySizerVert->propertyMode.set(ewol::widget::Sizer::modeVert);
	setSubWidget(mySizerVert);
		mySizerHori = ewol::widget::Sizer::create();
		mySizerHori->propertyName.set("plop 222222222");
		mySizerHori->propertyMode.set(ewol::widget::Sizer::modeHori);
		mySizerVert->subWidgetAdd(mySizerHori);
			myBufferView = BufferView::create();
			myBufferView->propertyName.set("plop 3333333");
			myBufferView->propertyExpand.set(bvec2(false,true));
			myBufferView->propertyFill.set(bvec2(true,true));
			mySizerHori->subWidgetAdd(myBufferView);
			
			mySizerVert2 = ewol::widget::Sizer::create();
			mySizerVert2->propertyMode.set(ewol::widget::Sizer::modeVert);
			mySizerHori->subWidgetAdd(mySizerVert2);
				mySizerVert2->propertyName.set("appl-view-code-sizer");
				// main buffer Area :
				#if defined(__TARGET_OS__Android)
					int32_t sizeText = 16;
				#else
					int32_t sizeText = 11;
				#endif
				myTextView2 = appl::TextViewer::create("font-size", sizeText);
				myTextView2->propertyName.set("appl-text-viewer2");
				myTextView2->propertyExpand.set(bvec2(true,true));
				myTextView2->propertyFill.set(bvec2(true,true));
				myTextView2->propertyHide.set(true);
				mySizerVert2->subWidgetAdd(myTextView2);
				
				myTextView = appl::TextViewer::create("font-size", sizeText);
				myTextView->propertyName.set("appl-text-viewer1");
				myTextView->propertyExpand.set(bvec2(true,true));
				myTextView->propertyFill.set(bvec2(true,true));
				mySizerVert2->subWidgetAdd(myTextView);
				
				// search area:
				m_widgetSearch = appl::widget::Search::create();
				mySizerVert2->subWidgetAdd(m_widgetSearch);
			
		mySizerHori = ewol::widget::Sizer::create();
		mySizerHori->propertyName.set("plop 555555");
		mySizerHori->propertyMode.set(ewol::widget::Sizer::modeHori);
		mySizerVert->subWidgetAdd(mySizerHori);
			
			myMenu = ewol::widget::Menu::create();
			myMenu->propertyName.set("appl-menu-interface");
			mySizerHori->subWidgetAdd(myMenu);
			int32_t idMenuFile = myMenu->addTitle("_T{File}");
				myMenu->add(idMenuFile, "_T{New}",          "", "menu:new");
				myMenu->addSpacer();
				myMenu->add(idMenuFile, "_T{Open}",         "THEME:GUI:Load.edf", "menu:open");
				myMenu->add(idMenuFile, "_T{Close}",        "THEME:GUI:Close.edf", "menu:close");
				myMenu->add(idMenuFile, "_T{Close (all)}",  "", "menu:close-all");
				myMenu->add(idMenuFile, "_T{Save}",         "THEME:GUI:Save.edf", "menu:save");
				myMenu->add(idMenuFile, "_T{Save As ...}",  "", "menu:save-as");
				myMenu->addSpacer();
				myMenu->add(idMenuFile, "_T{Properties}",   "THEME:GUI:Parameter.edf", "menu:property");
			int32_t idMenuEdit = myMenu->addTitle("_T{Edit}");
				myMenu->add(idMenuEdit, "_T{Goto line ...}","", "menu:goto-line");
			int32_t idMenuSearch = myMenu->addTitle("_T{Search}");
				myMenu->add(idMenuSearch, "_T{Search}",         "THEME:GUI:Search.edf", "menu:search");
				myMenu->add(idMenuSearch, "_T{Replace}",        "THEME:GUI:Replace.edf", "menu:replace");
				myMenu->addSpacer();
				myMenu->add(idMenuSearch, "_T{Find (previous)}","", "menu:find:previous");
				myMenu->add(idMenuSearch, "_T{Find (next)}",    "", "menu:find:next");
				myMenu->add(idMenuSearch, "_T{Find (all)}",     "", "menu:find:all");
				myMenu->add(idMenuSearch, "_T{Un-Select}",      "", "menu:find:none");
			int32_t idMenugDisplay = myMenu->addTitle("_T{Display}");
				myMenu->add(idMenugDisplay, "_T{Color Black}",          "", "menu:color:color/black/");
				myMenu->add(idMenugDisplay, "_T{Color White}",          "", "menu:color:color/white/");
				myMenu->add(idMenugDisplay, "_T{Shape square}",         "", "menu:shape:shape/square/");
				myMenu->add(idMenugDisplay, "_T{Shape round}",          "", "menu:shape:shape/round/");
				myMenu->addSpacer();
				myMenu->add(idMenugDisplay, "_T{Reload openGl Shader}", "", "menu:reloadShape");
				myMenu->addSpacer();
				myMenu->add(idMenugDisplay, "_T{Split}", "", "menu:split:enable");
				myMenu->add(idMenugDisplay, "_T{Unsplit}", "", "menu:split:disable");
				myMenu->add(idMenugDisplay, "_T{Vertical}", "", "menu:split:vert");
				myMenu->add(idMenugDisplay, "_T{Horizontal}", "", "menu:split:hori");
			myMenu->signalSelect.connect(shared_from_this(), &MainWindows::onCallbackMenuEvent);
			m_widgetLabelFileName = ewol::widget::Label::create();
			m_widgetLabelFileName->propertyValue.set("FileName");
			m_widgetLabelFileName->propertyName.set("appl-widget-display-name");
			m_widgetLabelFileName->propertyExpand.set(bvec2(true,false));
			m_widgetLabelFileName->propertyFill.set(bvec2(true,false));;
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
	// TODO : auto-connect on shortcut event ==> maybe do beter later ...
	signalShortcut.connect(shared_from_this(), &MainWindows::onCallbackShortCut);
	m_bufferManager->signalSelectFile.connect(shared_from_this(), &MainWindows::onCallbackselectNewFile);
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
		if (m_widgetSearch->propertyHide.get() == true) {
			m_widgetSearch->propertyHide.set(false);
			m_widgetSearch->selectSearch();
		} else {
			if (m_widgetSearch->isSelectSearch()) {
				m_widgetSearch->propertyHide.set(true);
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
		if (m_widgetSearch->propertyHide.get() == true) {
			m_widgetSearch->propertyHide.set(false);
			m_widgetSearch->selectReplace();
		} else {
			if (m_widgetSearch->isSelectReplace()) {
				m_widgetSearch->propertyHide.set(true);
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
		propertySetOnWidgetNamed("appl-text-viewer2", "hide", "false");
	} else if (_value == "menu:split:disable") {
		propertySetOnWidgetNamed("appl-text-viewer2", "hide", "true");
	} else if (_value == "menu:split:vert") {
		propertySetOnWidgetNamed("appl-view-code-sizer", "mode", "vert");
	} else if (_value == "menu:split:hori") {
		propertySetOnWidgetNamed("appl-view-code-sizer", "mode", "hori");
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
	tmpWidget->propertyLabelTitle.set("_T{Open files ...}");
	tmpWidget->propertyLabelValidate.set("_T{Open}");
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		return;
	}
	// Get a ref on the buffer selected (if null, no buffer was selected ...)
	std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->getBufferSelected();
	if (tmpBuffer != nullptr) {
		etk::FSNode tmpFile = tmpBuffer->getFileName();
		tmpWidget->propertyPath.set(tmpFile.getNameFolder());
	}
	// apply widget pop-up ...
	popUpWidgetPush(tmpWidget);
	tmpWidget->signalValidate.connect(shared_from_this(), &MainWindows::onCallbackPopUpFileSelected);
}

void MainWindows::displayProperty() {
	// Request the parameter GUI
	std::shared_ptr<ewol::widget::Parameter> tmpWidget = ewol::widget::Parameter::create();
	if (nullptr == tmpWidget) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
	} else {
		#if 0
			std::string menuDescription = "<title>Properties</title>\n";
			menuDescription += "<group title='_T{Editor}'>\n";
			menuDescription += "	<menu title='_T{Editor Interface}' short-title='Editor' widget='appl-text-viewer'>\n";
			menuDescription += "</group>\n";
			menuDescription += "<group title='_T{Gui}'>\n";
			menuDescription += "	<menu title='Font selection' short-title='Font' widget=''>\n";
			menuDescription += "	<menu title='Color selection' short-title='Color' widget=''>\n";
			menuDescription += "	<menu title='Theme selection' short-title='Theme' widget=''>\n";
			menuDescription += "</group>\n";
			
			tmpWidget->setMenu(menuDescription);
		#else
			tmpWidget->propertyLabelTitle.set("_T{Properties}");
			popUpWidgetPush(tmpWidget);
			tmpWidget->menuAddGroup("_T{Editor}");
			std::shared_ptr<ewol::Widget> tmpSubWidget = globals::ParameterGlobalsGui::create();
			tmpWidget->menuAdd("_T{Editor}",          "", tmpSubWidget);
			tmpWidget->menuAdd("_T{Font & Color}", "", nullptr);
			tmpWidget->menuAdd("_T{Highlight}",       "", nullptr);
			tmpWidget->menuAddGroup("_T{General}");
			tmpWidget->menuAdd("_T{Display}",       "", nullptr);
			tmpSubWidget = ParameterAboutGui::create();
			tmpWidget->menuAdd("_T{About}",           "", tmpSubWidget);
		#endif
	}
}

void MainWindows::onCallbackselectNewFile(const std::string& _value) {
	APPL_INFO("onCallbackselectNewFile(" << _value << ")");
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		return;
	}
	m_connectionSave.disconnect();
	m_connectionModify.disconnect();
	m_connectionSaveName.disconnect();
	onCallbackTitleUpdate();
	std::shared_ptr<appl::Buffer> tmpp = m_bufferManager->getBufferSelected();
	if (tmpp != nullptr) {
		m_connectionSave = tmpp->signalIsSave.connect(this, &MainWindows::onCallbackTitleUpdate);
		m_connectionModify = tmpp->signalIsModify.connect(this, &MainWindows::onCallbackTitleUpdate);
		m_connectionSaveName = tmpp->signalChangeName.connect(this, &MainWindows::onCallbackTitleUpdate);
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
			m_widgetLabelFileName->propertyValue.set("");
		}
	} else {
		std::string nameFileSystem = etk::FSNode(tmpp->getFileName()).getFileSystemName();
		setTitle(std::string("Edn : ") + (tmpp->isModify()==true?" *":"") + nameFileSystem);
		if (m_widgetLabelFileName != nullptr) {
			m_widgetLabelFileName->propertyValue.set(nameFileSystem + (tmpp->isModify()==true?" *":""));
		}
	}
}

void MainWindows::saveAsPopUp(const std::shared_ptr<appl::Buffer>& _buffer) {
	if (_buffer == nullptr) {
		APPL_ERROR("Call With nullptr input...");
		return;
	}
	std::shared_ptr<appl::WorkerSaveFile> tmpObject = appl::WorkerSaveFile::create("buffer-name", _buffer->getFileName());
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
	tmpPopUp->setTitle("<bold>_T{Close un-saved file:}</bold>");
	tmpPopUp->setComment("_T{The file named:} <i>\"" + _buffer->getFileName() + "\"</i> _T{is curently modify.}   <br/>_T{If you don't saves these modifications,<br/>they will be definitly lost...}");
	std::shared_ptr<ewol::widget::Button> bt = nullptr;
	if (_buffer->hasFileName() == true) {
		bt = tmpPopUp->addButton("_T{Save}", true);
		if (bt != nullptr) {
			// TODO : The element is removed before beeing pressed
			// TODO : bt->signalPressed.connect(shared_from_this(), mainWindowsRequestSaveFile, _buffer->getFileName());
			// TODO : bt->signalPressed.connect(shared_from_this(), mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
		}
	}
	bt = tmpPopUp->addButton("_T{Save As}", true);
	if (bt != nullptr) {
		// TODO : bt->signalPressed.connect(shared_from_this(), mainWindowsRequestSaveFileAs, _buffer->getFileName());
		//bt->signalPressed.connect(shared_from_this(), mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
		// TODO : Request the close when saved ...
	}
	bt = tmpPopUp->addButton("_T{Close}", true);
	if (bt != nullptr) {
		// TODO : bt->signalPressed.connect(shared_from_this(), mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
	}
	tmpPopUp->addButton("_T{Cancel}", true);
	tmpPopUp->propertyCloseOutEvent.set(true);
	popUpWidgetPush(tmpPopUp);
}
