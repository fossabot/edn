/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/global.h>
#include <MainWindows.h>
#include <BufferView.h>
#include <TextViewer.h>
#include <Search.h>

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
#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/eContext.h>
#include <date/date.h>
#include <ewol/widget/meta/StdPopUp.h>
#include <appl/Gui/WorkerSaveFile.h>
#include <appl/Gui/WorkerSaveAllFile.h>
#include <appl/Gui/WorkerCloseFile.h>
#include <appl/Gui/WorkerCloseAllFile.h>

namespace appl 
{
	std::string getVersion(void)
	{
		#define FIRST_YEAR (2010)
		std::string tmpOutput = std::to_string(date::getYear()-FIRST_YEAR);
		tmpOutput += ".";
		tmpOutput += std::to_string(date::getMonth());
		tmpOutput += ".";
		tmpOutput += std::to_string(date::getDay());
		return tmpOutput;
	}
	
}


#undef __class__
#define __class__	"AboutGui"

#include <ewol/widget/Label.h>
#include <ewol/widget/Spacer.h>

class ParameterAboutGui : public widget::Sizer {
	public :
		ParameterAboutGui(void) :
		  widget::Sizer(widget::Sizer::modeVert) {
			widget::Spacer* mySpacer = NULL;
			
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
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
			widget::Label* myLabel = new widget::Label(tmpLabel);
			if (NULL == myLabel) {
				APPL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				myLabel->setExpand(bvec2(true,false));
				subWidgetAdd(myLabel);
			}
		};
		
		~ParameterAboutGui(void) {
			
		};
};
// Local main windows event :
static const char* mainWindowsRequestSaveFile = "appl-event-main-windows-save-file";
static const char* mainWindowsRequestSaveFileAs = "appl-event-main-windows-save-file-as";
static const char* mainWindowsRequestcloseFileNoCheck = "appl-event-main-windows-close-file-no-check";

const char* l_smoothChick = "tmpEvent_smooth";
const char* l_smoothMin = "tmpEvent_minChange";
const char* l_smoothMax = "tmpEvent_maxChange";

#undef __class__
#define __class__ "MainWindows"

MainWindows::MainWindows(void) {
	APPL_DEBUG("CREATE WINDOWS ... ");
	widget::Sizer * mySizerVert = NULL;
	widget::Sizer * mySizerVert2 = NULL;
	widget::Sizer * mySizerHori = NULL;
	//ewol::Button * myButton = NULL;
	appl::TextViewer * myTextView = NULL;
	BufferView * myBufferView = NULL;
	widget::Menu * myMenu = NULL;
	
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	mySizerVert = new widget::Sizer(widget::Sizer::modeVert);
	setSubWidget(mySizerVert);
	
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		mySizerVert->subWidgetAdd(mySizerHori);
			myBufferView = new BufferView();
			myBufferView->setExpand(bvec2(false,true));
			myBufferView->setFill(bvec2(true,true));
			mySizerHori->subWidgetAdd(myBufferView);
			
			mySizerVert2 = new widget::Sizer(widget::Sizer::modeVert);
			mySizerHori->subWidgetAdd(mySizerVert2);
				
				// main buffer Area :
				myTextView = new appl::TextViewer("FreeMono;DejaVuSansMono;FreeSerif", 11);
				myTextView->setExpand(bvec2(true,true));
				myTextView->setFill(bvec2(true,true));
				mySizerVert2->subWidgetAdd(myTextView);
				/*
				myTextView = new appl::TextViewer("FreeMono;DejaVuSansMono;FreeSerif", 11);
				myTextView->setExpand(bvec2(true,true));
				myTextView->setFill(bvec2(true,true));
				mySizerVert2->subWidgetAdd(myTextView);
				*/
				// search area : 
				Search * mySearch = new Search();
				mySizerVert2->subWidgetAdd(mySearch);
				#ifdef APPL_BUFFER_FONT_DISTANCE_FIELD
				{
					widget::Sizer * mySizerHori2 = new widget::Sizer(widget::Sizer::modeHori);
					mySizerVert2->subWidgetAdd(mySizerHori2);
						
						widget::CheckBox* tmpCheck = new widget::CheckBox("smooth");
						mySizerHori2->subWidgetAdd(tmpCheck);
						tmpCheck->registerOnEvent(this, ewolEventCheckBoxClicked, l_smoothChick);
						
						widget::Slider* tmpSlider = new widget::Slider();
						mySizerHori2->subWidgetAdd(tmpSlider);
						tmpSlider->registerOnEvent(this, ewolEventSliderChange, l_smoothMin);
						tmpSlider->setExpand(bvec2(true,false));
						tmpSlider->setMin(0);
						tmpSlider->setMax(1000);
						tmpSlider->setValue(0450);
						tmpSliderMin = tmpSlider;
						
						tmpSlider = new widget::Slider();
						mySizerHori2->subWidgetAdd(tmpSlider);
						tmpSlider->registerOnEvent(this, ewolEventSliderChange, l_smoothMax);
						tmpSlider->setExpand(bvec2(true,false));
						tmpSlider->setMin(0);
						tmpSlider->setMax(1000);
						tmpSlider->setValue(0550);
						tmpSliderMax = tmpSlider;
				}
				#endif
			
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		mySizerVert->subWidgetAdd(mySizerHori);
			
			myMenu = new widget::Menu();
			mySizerHori->subWidgetAdd(myMenu);
			int32_t idMenuFile = myMenu->addTitle("File");
				(void)myMenu->add(idMenuFile, "New",          "", ednMsgGuiNew);
				(void)myMenu->addSpacer();
				(void)myMenu->add(idMenuFile, "Open",         "THEME:GUI:Load.svg", ednMsgGuiOpen);
				(void)myMenu->add(idMenuFile, "Close",        "THEME:GUI:Close.svg", ednMsgGuiClose, "current");
				(void)myMenu->add(idMenuFile, "Close (all)",  "", ednMsgGuiClose, "All");
				(void)myMenu->add(idMenuFile, "Save",         "THEME:GUI:Save.svg", ednMsgGuiSave, "current");
				(void)myMenu->add(idMenuFile, "Save As ...",  "", ednMsgGuiSaveAs);
				(void)myMenu->addSpacer();
				//(void)myMenu->add(idMenuFile, "Exit", "", ednMsgGuiExit);
				(void)myMenu->addSpacer();
				(void)myMenu->add(idMenuFile, "Properties",   "THEME:GUI:Parameter.svg", ednMsgProperties);
			int32_t idMenuEdit = myMenu->addTitle("Edit");
				(void)myMenu->add(idMenuEdit, "Undo",         "THEME:GUI:Undo.svg", ednMsgGuiUndo);
				(void)myMenu->add(idMenuEdit, "Redo",         "THEME:GUI:Redo.svg", ednMsgGuiRedo);
				(void)myMenu->addSpacer();
				(void)myMenu->add(idMenuEdit, "Copy",         "", ednMsgGuiCopy, "STD");
				(void)myMenu->add(idMenuEdit, "Cut",          "", ednMsgGuiCut, "STD");
				(void)myMenu->add(idMenuEdit, "Paste",        "", ednMsgGuiPaste, "STD");
				(void)myMenu->add(idMenuEdit, "Remove",       "", ednMsgGuiRm);
				(void)myMenu->addSpacer();
				(void)myMenu->add(idMenuEdit, "Select All","", ednMsgGuiSelect, "ALL");
				(void)myMenu->add(idMenuEdit, "Un-Select","", ednMsgGuiSelect, "NONE");
				(void)myMenu->add(idMenuEdit, "Goto line ...","", ednMsgGuiGotoLine, "???");
			int32_t idMenuSearch = myMenu->addTitle("Search");
				(void)myMenu->add(idMenuSearch, "Search",         "THEME:GUI:Search.svg", ednMsgGuiSearch);
				(void)myMenu->add(idMenuSearch, "Replace",        "THEME:GUI:Replace.svg", ednMsgGuiReplace);
				(void)myMenu->addSpacer();
				(void)myMenu->add(idMenuSearch, "Find (previous)","", ednMsgGuiFind, "Previous");
				(void)myMenu->add(idMenuSearch, "Find (next)",    "", ednMsgGuiFind, "Next");
				(void)myMenu->add(idMenuSearch, "Find (all)",     "", ednMsgGuiFind, "All");
				(void)myMenu->add(idMenuSearch, "Un-Select",      "", ednMsgGuiFind, "None");
			int32_t idMenuCTags = myMenu->addTitle("C-tags");
				(void)myMenu->add(idMenuCTags, "Load",      "", ednMsgGuiCtags, "Load");
				(void)myMenu->add(idMenuCTags, "ReLoad",    "", ednMsgGuiCtags, "ReLoad");
				(void)myMenu->add(idMenuCTags, "Jump",      "", ednMsgGuiCtags, "Jump");
				(void)myMenu->add(idMenuCTags, "Back",      "", ednMsgGuiCtags, "Back");
			int32_t idMenugDisplay = myMenu->addTitle("Display");
				(void)myMenu->add(idMenugDisplay, "Charset UTF-8",        "", ednMsgGuiChangeCharset, "UTF-8");
				(void)myMenu->add(idMenugDisplay, "Charset ISO-8859-1",   "", ednMsgGuiChangeCharset, "ISO-8859-1");
				(void)myMenu->add(idMenugDisplay, "Charset ISO-8859-15",  "", ednMsgGuiChangeCharset, "ISO-8859-15");
				(void)myMenu->addSpacer();
				(void)myMenu->add(idMenugDisplay, "Color Black",          "", ednMsgGuiChangeColor, "Black");
				(void)myMenu->add(idMenugDisplay, "Color White",          "", ednMsgGuiChangeColor, "White");
				(void)myMenu->addSpacer();
				(void)myMenu->add(idMenugDisplay, "Reload openGl Shader", "", ednMsgGuiReloadShader);
			
			m_widgetLabelFileName = new widget::Label("<left>FileName</left>");
			m_widgetLabelFileName->setExpand(bvec2(true,false));
			m_widgetLabelFileName->setFill(bvec2(false,true));;
			mySizerHori->subWidgetAdd(m_widgetLabelFileName);
	
	
	// add generic shortcut ...
	//                 (shift, control, alt,  meta,  char32_t unicodeValue, const char * generateEventId, std::string& data)
	shortCutAdd("ctrl+o",       ednMsgGuiOpen, "", true);
	shortCutAdd("ctrl+n",       ednMsgGuiNew,  "", true);
	
	shortCutAdd("ctrl+s",       ednMsgGuiSave, "current", true);
	shortCutAdd("ctrl+shift+s", ednMsgGuiSave, "All", true);
	
	shortCutAdd("ctrl+q",       ednMsgGuiClose, "current", true);
	shortCutAdd("ctrl+shift+q", ednMsgGuiClose, "All", true);
	
	shortCutAdd("ctrl+z",       ednMsgGuiUndo, "", true);
	shortCutAdd("ctrl+shift+z", ednMsgGuiRedo, "", true);
	
	shortCutAdd("ctrl+l",       ednMsgGuiGotoLine, "???", true);
	
	shortCutAdd("ctrl+f",       ednMsgGuiSearch, "", true);
	shortCutAdd("F12",          ednMsgGuiReloadShader, "", true);
	
	shortCutAdd("ctrl+d",       ednMsgGuiCtags, "Jump", true);
	
	
	
	// Generic event ...
	registerMultiCast(ednMsgGuiSave);
	registerMultiCast(ednMsgGuiSaveAs);
	registerMultiCast(ednMsgProperties);
	registerMultiCast(ednMsgGuiNew);
	registerMultiCast(ednMsgGuiOpen);
	registerMultiCast(ednMsgGuiClose);
	// to update the title ... 
	registerMultiCast(ednMsgBufferState);
	registerMultiCast(ednMsgBufferId);
	registerMultiCast(ednMsgGuiReloadShader);
}


MainWindows::~MainWindows(void) {
	appl::BufferManager::release(m_bufferManager);
}


const char *const ednEventPopUpFileSelected = "edn-mainWindows-openSelected";
const char *const ednEventPopUpFileSaveAs   = "edn-mainWindows-saveAsSelected";


void MainWindows::onReceiveMessage(const ewol::EMessage& _msg) {
	ewol::Windows::onReceiveMessage(_msg);
	
	APPL_INFO("Receive Event from the main windows: " << _msg );
	// open file Section ...
	if (_msg.getMessage() == ednMsgGuiOpen) {
		widget::FileChooser* tmpWidget = new widget::FileChooser();
		if (tmpWidget == NULL) {
			APPL_ERROR("Can not open File chooser !!! ");
			return;
		}
		tmpWidget->setTitle("Open files ...");
		tmpWidget->setValidateLabel("Open");
		if (m_bufferManager == NULL) {
			APPL_ERROR("can not call unexistant buffer manager ... ");
			return;
		}
		// Get a ref on the buffer selected (if null, no buffer was selected ...)
		appl::Buffer* tmpBuffer = m_bufferManager->getBufferSelected();
		if (tmpBuffer != NULL) {
			etk::FSNode tmpFile = tmpBuffer->getFileName();
			tmpWidget->setFolder(tmpFile.getNameFolder());
		}
		// apply widget pop-up ...
		popUpWidgetPush(tmpWidget);
		tmpWidget->registerOnEvent(this, widget::FileChooser::eventValidate, ednEventPopUpFileSelected);
	} else if (_msg.getMessage() == ednMsgProperties) {
		// Request the parameter GUI
		widget::Parameter* tmpWidget = new widget::Parameter();
		if (NULL == tmpWidget) {
			APPL_ERROR("Can not allocate widget  == > display might be in error");
		} else {
			tmpWidget->setTitle("Properties");
			popUpWidgetPush(tmpWidget);
			tmpWidget->menuAddGroup("Editor");
			ewol::Widget* tmpSubWidget = new globals::ParameterGlobalsGui();
			tmpWidget->menuAdd("Editor",          "", tmpSubWidget);
			tmpWidget->menuAdd("Font & Color", "", NULL);
			tmpWidget->menuAdd("Highlight",       "", NULL);
			tmpWidget->menuAddGroup("General");
			tmpWidget->menuAdd("Display",       "", NULL);
			tmpSubWidget = new ParameterAboutGui();
			tmpWidget->menuAdd("About",           "", tmpSubWidget);
		}
	} else if (_msg.getMessage() == ednMsgGuiReloadShader) {
		ewol::getContext().getResourcesManager().reLoadResources();
		ewol::getContext().forceRedrawAll();
	} else if (_msg.getMessage() == ednMsgGuiExit) {
		// TODO : ...
	}
	// Note : Fore all next message we need to acces to the buffer manager ==> just check one time ...
	if (m_bufferManager == NULL) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		return;
	}
	if (_msg.getMessage() == ednMsgGuiNew) {
		if (m_bufferManager == NULL) {
			APPL_ERROR("can not call unexistant buffer manager ... ");
			return;
		}
		(void)m_bufferManager->createNewBuffer();
	} else if (_msg.getMessage() == ednEventPopUpFileSelected) {
		APPL_DEBUG("Request opening the file : " << _msg.getData());
		m_bufferManager->open(_msg.getData());
	} else if (_msg.getMessage() == ednMsgGuiSave) {
		APPL_DEBUG("Request saving the file : " << _msg.getData());
		if (to_lower(_msg.getData()) == "current") {
			appl::WorkerSaveFile* tmpWorker = new appl::WorkerSaveFile("", false);
			return;
		} else if (to_lower(_msg.getData()) == "all") {
			appl::WorkerSaveAllFile* tmpWorker = new appl::WorkerSaveAllFile();
			return;
		} else {
			APPL_ERROR("UNKNOW request : " << _msg);
		}
	} else if (_msg.getMessage() == ednMsgGuiSaveAs) {
		appl::WorkerSaveFile* tmpWorker = new appl::WorkerSaveFile("", true);
	} else if (_msg.getMessage() == ednMsgGuiClose) {
		// Get a ref on the buffer selected (if null, no buffer was selected ...)
		if (_msg.getData() == "current") {
			appl::WorkerCloseFile* tmpWorker = new appl::WorkerCloseFile("");
		} else {
			appl::WorkerCloseAllFile* tmpWorker = new appl::WorkerCloseAllFile();
		}
	} else if (_msg.getMessage() == mainWindowsRequestSaveFile) { // return after a choice of close...
		if (m_bufferManager->exist(_msg.getData()) == false) {
			APPL_ERROR("Try to save an non-existant file :" << _msg.getData());
			return;
		}
		appl::Buffer* tmpBuffer = m_bufferManager->get(_msg.getData());
		if (tmpBuffer == NULL) {
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
		appl::Buffer* tmpBuffer = m_bufferManager->get(_msg.getData());
		if (tmpBuffer == NULL) {
			APPL_ERROR("Error to get the buffer : " << _msg.getData());
			return;
		}
		saveAsPopUp(tmpBuffer);
	} else if (_msg.getMessage() == mainWindowsRequestcloseFileNoCheck) { // return after a choice of close...
		if (m_bufferManager->exist(_msg.getData()) == false) {
			APPL_ERROR("Try to save an non-existant file :" << _msg.getData());
			return;
		}
		appl::Buffer* tmpBuffer = m_bufferManager->get(_msg.getData());
		if (tmpBuffer == NULL) {
			APPL_ERROR("Error to get the buffer : " << _msg.getData());
			return;
		}
		// note: just remove ==> no check : normal case ...
		tmpBuffer->removeObject();
	}
	return;
}

void MainWindows::saveAsPopUp(appl::Buffer* _buffer) {
	if (_buffer == NULL) {
		APPL_ERROR("Call With NULL input...");
		return;
	}
	appl::WorkerSaveFile* tmpObject = new appl::WorkerSaveFile(_buffer->getFileName());
}

void MainWindows::closeNotSavedFile(appl::Buffer* _buffer) {
	if (_buffer == NULL) {
		APPL_ERROR("Call With NULL input...");
		return;
	}
	widget::StdPopUp* tmpPopUp = new widget::StdPopUp();
	if (tmpPopUp == NULL) {
		APPL_ERROR("Can not create a simple pop-up");
		return;
	}
	tmpPopUp->setTitle("<bold>Close un-saved file:</bold>");
	tmpPopUp->setComment("The file named : <i>\"" + _buffer->getFileName() + "\"</i> is curently modify.   <br/>If you don't saves these modifications,<br/>they will be definitly lost...");
	ewol::Widget* bt = NULL;
	if (_buffer->hasFileName() == true) {
		bt = tmpPopUp->addButton("Save", true);
		if (bt != NULL) {
			// TODO : The element is removed before beeing pressed
			bt->registerOnEvent(this, widget::Button::eventPressed, mainWindowsRequestSaveFile, _buffer->getFileName());
			bt->registerOnEvent(this, widget::Button::eventPressed, mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
		}
	}
	bt = tmpPopUp->addButton("Save As", true);
	if (bt != NULL) {
		bt->registerOnEvent(this, widget::Button::eventPressed, mainWindowsRequestSaveFileAs, _buffer->getFileName());
		//bt->registerOnEvent(this, widget::Button::eventPressed, mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
		// TODO : Request the close when saved ...
	}
	bt = tmpPopUp->addButton("Close", true);
	if (bt != NULL) {
		bt->registerOnEvent(this, widget::Button::eventPressed, mainWindowsRequestcloseFileNoCheck, _buffer->getFileName());
	}
	tmpPopUp->addButton("Cancel", true);
	tmpPopUp->setRemoveOnExternClick(true);
	popUpWidgetPush(tmpPopUp);
}

void MainWindows::onObjectRemove(ewol::EObject * _removeObject) {
	ewol::Windows::onObjectRemove(_removeObject);
	if (m_widgetLabelFileName == _removeObject) {
		m_widgetLabelFileName = NULL;
	}
}
