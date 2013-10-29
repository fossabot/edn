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

namespace appl 
{
	etk::UString getVersion(void)
	{
		#define FIRST_YEAR (2010)
		etk::UString tmpOutput = (date::getYear()-FIRST_YEAR);
		tmpOutput += ".";
		tmpOutput += date::getMonth();
		tmpOutput += ".";
		tmpOutput += date::getDay();
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
			etk::UString tmpLabel = "<left>";
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


const char * l_smoothChick = "tmpEvent_smooth";
const char * l_smoothMin = "tmpEvent_minChange";
const char * l_smoothMax = "tmpEvent_maxChange";

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
	//                 (shift, control, alt,  meta,  etk::UChar unicodeValue, const char * generateEventId, etk::UString& data)
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
	registerMultiCast(ednMsgGuiSaveAs);
	registerMultiCast(ednMsgProperties);
	registerMultiCast(ednMsgGuiOpen);
	// to update the title ... 
	registerMultiCast(ednMsgBufferState);
	registerMultiCast(ednMsgBufferId);
	registerMultiCast(ednMsgGuiReloadShader);
}


MainWindows::~MainWindows(void) {
	
}


const char *const ednEventPopUpFileSelected = "edn-mainWindows-openSelected";
const char *const ednEventPopUpFileSaveAs   = "edn-mainWindows-saveAsSelected";


void MainWindows::onReceiveMessage(const ewol::EMessage& _msg) {
	ewol::Windows::onReceiveMessage(_msg);
	
	//APPL_INFO("Receive Event from the main windows ... : \"" << eventId << "\"  == > data=\"" << data << "\"" );
	// open file Section ...
	if (_msg.getMessage() == ednMsgGuiOpen) {
		widget::FileChooser* tmpWidget = new widget::FileChooser();
		tmpWidget->setTitle("Open files ...");
		tmpWidget->setValidateLabel("Open");
		/*
		if (BufferManager::getSelected()!=-1) {
			BufferText * myBuffer = BufferManager::get(BufferManager::getSelected());
			if (NULL!=myBuffer) {
				etk::FSNode tmpFile = myBuffer->getFileName();
				tmpWidget->setFolder(tmpFile.getNameFolder());
			}
		}
		*/
		popUpWidgetPush(tmpWidget);
		tmpWidget->registerOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpFileSelected);
	} else if (_msg.getMessage() == ednEventPopUpFileSelected) {
		APPL_DEBUG("Request opening the file : " << _msg.getData());
		sendMultiCast(ednMsgOpenFile, _msg.getData());
	} else if (_msg.getMessage() == ednMsgGuiSaveAs) {
		if (_msg.getData() == "") {
			APPL_ERROR("Null data for Save As file ... ");
		} else {
			m_currentSavingAsIdBuffer = -1;
			if (_msg.getData() == "current") {
				m_currentSavingAsIdBuffer = -1;//BufferManager::getSelected();
			} else {
				sscanf(_msg.getData().c_str(), "%d", &m_currentSavingAsIdBuffer);
			}
			/*
			if (false == BufferManager::exist(m_currentSavingAsIdBuffer)) {
				APPL_ERROR("Request saveAs on non existant Buffer ID=" << m_currentSavingAsIdBuffer);
			} else {
				BufferText* myBuffer = BufferManager::get(m_currentSavingAsIdBuffer);
				widget::FileChooser* tmpWidget = new widget::FileChooser();
				if (NULL == tmpWidget) {
					APPL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					tmpWidget->setTitle("Save files As...");
					tmpWidget->setValidateLabel("Save");
					etk::UString folder = "/home/";
					etk::UString fileName = "";
					if (true == myBuffer->haveName()) {
						etk::FSNode tmpName = myBuffer->getFileName();
						folder = tmpName.getNameFolder();
						fileName = tmpName.getNameFile();
					}
					tmpWidget->setFolder(folder);
					tmpWidget->setFileName(fileName);
					popUpWidgetPush(tmpWidget);
					tmpWidget->registerOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpFileSaveAs);
				}
			}
			*/
		}
	} else if (_msg.getMessage() == ednEventPopUpFileSaveAs) {
		// get the filename : 
		etk::UString tmpData = _msg.getData();
		APPL_DEBUG("Request Saving As file : " << tmpData);
		/*
		BufferManager::get(m_currentSavingAsIdBuffer)->setFileName(tmpData);
		sendMultiCast(ednMsgGuiSave, m_currentSavingAsIdBuffer);
		*/
	} else if(    _msg.getMessage() == ednMsgBufferState
	           || _msg.getMessage() == ednMsgBufferId) {
		// the buffer change we need to update the widget string
		/*
		BufferText* tmpBuffer = BufferManager::get(BufferManager::getSelected());
		if (NULL != tmpBuffer) {
			etk::FSNode compleateName = tmpBuffer->getFileName();
			bool isModify = tmpBuffer->isModify();
			etk::UString directName = compleateName.getName();
			if (true == isModify) {
				directName += " *";
			}
			if (NULL != m_widgetLabelFileName) {
				m_widgetLabelFileName->setLabel(etk::UString("<left>") + directName + "</left>");
			}
			etk::UString windowsTitle = "edn - ";
			windowsTitle += directName;
			setTitle(windowsTitle);
			return;
		} else {
			m_widgetLabelFileName->setLabel("");
			setTitle("edn");
		}
		*/
		return;
		// TODO : set the Title ....
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
		// TODO ...
	}
	
	return;
}

void MainWindows::onObjectRemove(ewol::EObject * _removeObject) {
	ewol::Windows::onObjectRemove(_removeObject);
	if (m_widgetLabelFileName == _removeObject) {
		m_widgetLabelFileName = NULL;
	}
}
