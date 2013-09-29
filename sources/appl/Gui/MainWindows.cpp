/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Debug.h>
#include <appl/global.h>
#include <MainWindows.h>
#include <CodeView.h>
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
	etk::UString GetVersion(void)
	{
		#define FIRST_YEAR (2010)
		etk::UString tmpOutput = (date::GetYear()-FIRST_YEAR);
		tmpOutput += ".";
		tmpOutput += date::GetMonth();
		tmpOutput += ".";
		tmpOutput += date::GetDay();
		return tmpOutput;
	}
	
}


#undef __class__
#define __class__	"AboutGui"

#include <ewol/widget/Label.h>
#include <ewol/widget/Spacer.h>

class ParameterAboutGui : public widget::Sizer
{
	public :
		ParameterAboutGui(void) :
			widget::Sizer(widget::Sizer::modeVert)
		{
			widget::Spacer* mySpacer = NULL;
			
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				APPL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpand(bvec2(true,true));
				SubWidgetAdd(mySpacer);
			}
			etk::UString tmpLabel = "<left>";
			tmpLabel += "  <b>Editeur De N'ours</b> : v:";
			tmpLabel += appl::GetVersion();
			tmpLabel += "<br/>";
			tmpLabel += "  <b>Build Time</b> : ";
			tmpLabel += date::GetYear();
			tmpLabel += "/";
			tmpLabel += date::GetMonth();
			tmpLabel += "/";
			tmpLabel += date::GetDay();
			tmpLabel += " ";
			tmpLabel += date::GetHour();
			tmpLabel += "h";
			tmpLabel += date::GetMinute();
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
				APPL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myLabel->SetExpand(bvec2(true,false));
				SubWidgetAdd(myLabel);
			}
		};
		
		~ParameterAboutGui(void) { };
};


const char * l_smoothChick = "tmpEvent_smooth";
const char * l_smoothMin = "tmpEvent_minChange";
const char * l_smoothMax = "tmpEvent_maxChange";

#undef __class__
#define __class__	"MainWindows"

MainWindows::MainWindows(void)
{
	APPL_DEBUG("CREATE WINDOWS ... ");
	widget::Sizer * mySizerVert = NULL;
	widget::Sizer * mySizerVert2 = NULL;
	widget::Sizer * mySizerHori = NULL;
	//ewol::Button * myButton = NULL;
	CodeView * myCodeView = NULL;
	appl::TextViewer * myTextView = NULL;
	BufferView * myBufferView = NULL;
	widget::Menu * myMenu = NULL;
	
	mySizerVert = new widget::Sizer(widget::Sizer::modeVert);
	SetSubWidget(mySizerVert);
	
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		mySizerVert->SubWidgetAdd(mySizerHori);
			myBufferView = new BufferView();
			myBufferView->SetExpand(bvec2(false,true));
			myBufferView->SetFill(bvec2(true,true));
			mySizerHori->SubWidgetAdd(myBufferView);
			
			mySizerVert2 = new widget::Sizer(widget::Sizer::modeVert);
			mySizerHori->SubWidgetAdd(mySizerVert2);
				
				// main buffer Area :
				myTextView = new appl::TextViewer("FreeSerif;FreeMono;DejaVuSansMono", 11);
				myTextView->SetExpand(bvec2(true,true));
				myTextView->SetFill(bvec2(true,true));
				mySizerVert2->SubWidgetAdd(myTextView);
				/*
				myCodeView = new CodeView("FreeMono;DejaVuSansMono", 11);
				myCodeView->SetExpand(bvec2(true,true));
				myCodeView->SetFill(bvec2(true,true));
				mySizerVert2->SubWidgetAdd(myCodeView);
				*/
				// search area : 
				Search * mySearch = new Search();
				mySizerVert2->SubWidgetAdd(mySearch);
				#ifdef APPL_BUFFER_FONT_DISTANCE_FIELD
				{
					widget::Sizer * mySizerHori2 = new widget::Sizer(widget::Sizer::modeHori);
					mySizerVert2->SubWidgetAdd(mySizerHori2);
						
						widget::CheckBox* tmpCheck = new widget::CheckBox("smooth");
						mySizerHori2->SubWidgetAdd(tmpCheck);
						tmpCheck->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_smoothChick);
						
						widget::Slider* tmpSlider = new widget::Slider();
						mySizerHori2->SubWidgetAdd(tmpSlider);
						tmpSlider->RegisterOnEvent(this, ewolEventSliderChange, l_smoothMin);
						tmpSlider->SetExpand(bvec2(true,false));
						tmpSlider->SetMin(0);
						tmpSlider->SetMax(1000);
						tmpSlider->SetValue(0450);
						tmpSliderMin = tmpSlider;
						
						tmpSlider = new widget::Slider();
						mySizerHori2->SubWidgetAdd(tmpSlider);
						tmpSlider->RegisterOnEvent(this, ewolEventSliderChange, l_smoothMax);
						tmpSlider->SetExpand(bvec2(true,false));
						tmpSlider->SetMin(0);
						tmpSlider->SetMax(1000);
						tmpSlider->SetValue(0550);
						tmpSliderMax = tmpSlider;
				}
				#endif
			
		mySizerHori = new widget::Sizer(widget::Sizer::modeHori);
		mySizerVert->SubWidgetAdd(mySizerHori);
			
			myMenu = new widget::Menu();
			mySizerHori->SubWidgetAdd(myMenu);
			int32_t idMenuFile = myMenu->AddTitle("File");
				(void)myMenu->Add(idMenuFile, "New",          "", ednMsgGuiNew);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuFile, "Open",         "THEME:GUI:Load.svg", ednMsgGuiOpen);
				(void)myMenu->Add(idMenuFile, "Close",        "THEME:GUI:Close.svg", ednMsgGuiClose, "current");
				(void)myMenu->Add(idMenuFile, "Close (all)",  "", ednMsgGuiClose, "All");
				(void)myMenu->Add(idMenuFile, "Save",         "THEME:GUI:Save.svg", ednMsgGuiSave, "current");
				(void)myMenu->Add(idMenuFile, "Save As ...",  "", ednMsgGuiSaveAs);
				(void)myMenu->AddSpacer();
				//(void)myMenu->Add(idMenuFile, "Exit", "", ednMsgGuiExit);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuFile, "Properties",   "THEME:GUI:Parameter.svg", ednMsgProperties);
			int32_t idMenuEdit = myMenu->AddTitle("Edit");
				(void)myMenu->Add(idMenuEdit, "Undo",         "THEME:GUI:Undo.svg", ednMsgGuiUndo);
				(void)myMenu->Add(idMenuEdit, "Redo",         "THEME:GUI:Redo.svg", ednMsgGuiRedo);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuEdit, "Copy",         "", ednMsgGuiCopy, "STD");
				(void)myMenu->Add(idMenuEdit, "Cut",          "", ednMsgGuiCut, "STD");
				(void)myMenu->Add(idMenuEdit, "Paste",        "", ednMsgGuiPaste, "STD");
				(void)myMenu->Add(idMenuEdit, "Remove",       "", ednMsgGuiRm);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuEdit, "Select All","", ednMsgGuiSelect, "ALL");
				(void)myMenu->Add(idMenuEdit, "Un-Select","", ednMsgGuiSelect, "NONE");
				(void)myMenu->Add(idMenuEdit, "Goto line ...","", ednMsgGuiGotoLine, "???");
			int32_t idMenuSearch = myMenu->AddTitle("Search");
				(void)myMenu->Add(idMenuSearch, "Search",         "THEME:GUI:Search.svg", ednMsgGuiSearch);
				(void)myMenu->Add(idMenuSearch, "Replace",        "THEME:GUI:Replace.svg", ednMsgGuiReplace);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuSearch, "Find (previous)","", ednMsgGuiFind, "Previous");
				(void)myMenu->Add(idMenuSearch, "Find (next)",    "", ednMsgGuiFind, "Next");
				(void)myMenu->Add(idMenuSearch, "Find (all)",     "", ednMsgGuiFind, "All");
				(void)myMenu->Add(idMenuSearch, "Un-Select",      "", ednMsgGuiFind, "None");
			int32_t idMenuCTags = myMenu->AddTitle("C-tags");
				(void)myMenu->Add(idMenuCTags, "Load",      "", ednMsgGuiCtags, "Load");
				(void)myMenu->Add(idMenuCTags, "ReLoad",    "", ednMsgGuiCtags, "ReLoad");
				(void)myMenu->Add(idMenuCTags, "Jump",      "", ednMsgGuiCtags, "Jump");
				(void)myMenu->Add(idMenuCTags, "Back",      "", ednMsgGuiCtags, "Back");
			int32_t idMenugDisplay = myMenu->AddTitle("Display");
				(void)myMenu->Add(idMenugDisplay, "Charset UTF-8",        "", ednMsgGuiChangeCharset, "UTF-8");
				(void)myMenu->Add(idMenugDisplay, "Charset ISO-8859-1",   "", ednMsgGuiChangeCharset, "ISO-8859-1");
				(void)myMenu->Add(idMenugDisplay, "Charset ISO-8859-15",  "", ednMsgGuiChangeCharset, "ISO-8859-15");
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenugDisplay, "Color Black",          "", ednMsgGuiChangeColor, "Black");
				(void)myMenu->Add(idMenugDisplay, "Color White",          "", ednMsgGuiChangeColor, "White");
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenugDisplay, "Reload OpenGl Shader", "", ednMsgGuiReloadShader);
			
			m_widgetLabelFileName = new widget::Label("<left>FileName</left>");
			m_widgetLabelFileName->SetExpand(bvec2(true,false));
			m_widgetLabelFileName->SetFill(bvec2(false,true));;
			mySizerHori->SubWidgetAdd(m_widgetLabelFileName);
	
	
	// add generic shortcut ...
	//                 (shift, control, alt,  meta,  uniChar_t unicodeValue, const char * generateEventId, etk::UString& data)
	ShortCutAdd("ctrl+o",       ednMsgGuiOpen, "", true);
	ShortCutAdd("ctrl+n",       ednMsgGuiNew,  "", true);
	
	ShortCutAdd("ctrl+s",       ednMsgGuiSave, "current", true);
	ShortCutAdd("ctrl+shift+s", ednMsgGuiSave, "All", true);
	
	ShortCutAdd("ctrl+q",       ednMsgGuiClose, "current", true);
	ShortCutAdd("ctrl+shift+q", ednMsgGuiClose, "All", true);
	
	ShortCutAdd("ctrl+z",       ednMsgGuiUndo, "", true);
	ShortCutAdd("ctrl+shift+z", ednMsgGuiRedo, "", true);
	
	ShortCutAdd("ctrl+l",       ednMsgGuiGotoLine, "???", true);
	
	ShortCutAdd("ctrl+f",       ednMsgGuiSearch, "", true);
	ShortCutAdd("F12",          ednMsgGuiReloadShader, "", true);
	
	ShortCutAdd("ctrl+d",       ednMsgGuiCtags, "Jump", true);
	
	
	
	// Generic event ...
	RegisterMultiCast(ednMsgGuiSaveAs);
	RegisterMultiCast(ednMsgProperties);
	RegisterMultiCast(ednMsgGuiOpen);
	// to update the title ... 
	RegisterMultiCast(ednMsgBufferState);
	RegisterMultiCast(ednMsgBufferId);
	RegisterMultiCast(ednMsgGuiReloadShader);
}


MainWindows::~MainWindows(void)
{
	
}


const char *const ednEventPopUpFileSelected = "edn-mainWindows-openSelected";
const char *const ednEventPopUpFileSaveAs   = "edn-mainWindows-saveAsSelected";


void MainWindows::OnReceiveMessage(const ewol::EMessage& _msg)
{
	ewol::Windows::OnReceiveMessage(_msg);
	
	//APPL_INFO("Receive Event from the main windows ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	// Open file Section ...
	if (_msg.GetMessage() == ednMsgGuiOpen) {
		widget::FileChooser* tmpWidget = new widget::FileChooser();
		tmpWidget->SetTitle("Open Files ...");
		tmpWidget->SetValidateLabel("Open");
		if (BufferManager::GetSelected()!=-1) {
			BufferText * myBuffer = BufferManager::Get(BufferManager::GetSelected());
			if (NULL!=myBuffer) {
				etk::FSNode tmpFile = myBuffer->GetFileName();
				tmpWidget->SetFolder(tmpFile.GetNameFolder());
			}
		}
		PopUpWidgetPush(tmpWidget);
		tmpWidget->RegisterOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpFileSelected);
	} else if (_msg.GetMessage() == ednEventPopUpFileSelected) {
		APPL_DEBUG("Request opening the file : " << _msg.GetData());
		SendMultiCast(ednMsgOpenFile, _msg.GetData());
	} else if (_msg.GetMessage() == ednMsgGuiSaveAs) {
		if (_msg.GetData() == "") {
			APPL_ERROR("Null data for Save As file ... ");
		} else {
			m_currentSavingAsIdBuffer = -1;
			if (_msg.GetData() == "current") {
				m_currentSavingAsIdBuffer = BufferManager::GetSelected();
			} else {
				sscanf(_msg.GetData().c_str(), "%d", &m_currentSavingAsIdBuffer);
			}
			
			if (false == BufferManager::Exist(m_currentSavingAsIdBuffer)) {
				APPL_ERROR("Request saveAs on non existant Buffer ID=" << m_currentSavingAsIdBuffer);
			} else {
				BufferText* myBuffer = BufferManager::Get(m_currentSavingAsIdBuffer);
				widget::FileChooser* tmpWidget = new widget::FileChooser();
				if (NULL == tmpWidget) {
					APPL_ERROR("Can not allocate widget ==> display might be in error");
				} else {
					tmpWidget->SetTitle("Save Files As...");
					tmpWidget->SetValidateLabel("Save");
					etk::UString folder = "/home/";
					etk::UString fileName = "";
					if (true == myBuffer->HaveName()) {
						etk::FSNode tmpName = myBuffer->GetFileName();
						folder = tmpName.GetNameFolder();
						fileName = tmpName.GetNameFile();
					}
					tmpWidget->SetFolder(folder);
					tmpWidget->SetFileName(fileName);
					PopUpWidgetPush(tmpWidget);
					tmpWidget->RegisterOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpFileSaveAs);
				}
			}
		}
	} else if (_msg.GetMessage() == ednEventPopUpFileSaveAs) {
		// get the filename : 
		etk::UString tmpData = _msg.GetData();
		APPL_DEBUG("Request Saving As file : " << tmpData);
		
		BufferManager::Get(m_currentSavingAsIdBuffer)->SetFileName(tmpData);
		SendMultiCast(ednMsgGuiSave, m_currentSavingAsIdBuffer);
	} else if(    _msg.GetMessage() == ednMsgBufferState
	           || _msg.GetMessage() == ednMsgBufferId) {
		// the buffer change we need to update the widget string
		BufferText* tmpBuffer = BufferManager::Get(BufferManager::GetSelected());
		if (NULL != tmpBuffer) {
			etk::FSNode compleateName = tmpBuffer->GetFileName();
			bool isModify = tmpBuffer->IsModify();
			etk::UString directName = compleateName.GetName();
			if (true == isModify) {
				directName += " *";
			}
			if (NULL != m_widgetLabelFileName) {
				m_widgetLabelFileName->SetLabel(etk::UString("<left>") + directName + "</left>");
			}
			etk::UString windowsTitle = "edn - ";
			windowsTitle += directName;
			SetTitle(windowsTitle);
			return;
		} else {
			m_widgetLabelFileName->SetLabel("");
			SetTitle("edn");
		}
		return;
		// TODO : Set the Title ....
	} else if (_msg.GetMessage() == ednMsgProperties) {
		// Request the parameter GUI
		widget::Parameter* tmpWidget = new widget::Parameter();
		if (NULL == tmpWidget) {
			APPL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			tmpWidget->SetTitle("Properties");
			PopUpWidgetPush(tmpWidget);
			tmpWidget->MenuAddGroup("Editor");
			ewol::Widget* tmpSubWidget = new globals::ParameterGlobalsGui();
			tmpWidget->MenuAdd("Editor",          "", tmpSubWidget);
			tmpWidget->MenuAdd("Font & Color", "", NULL);
			tmpWidget->MenuAdd("Highlight",       "", NULL);
			tmpWidget->MenuAddGroup("General");
			tmpWidget->MenuAdd("Display",       "", NULL);
			tmpSubWidget = new ParameterAboutGui();
			tmpWidget->MenuAdd("About",           "", tmpSubWidget);
		}
	} else if (_msg.GetMessage() == ednMsgGuiReloadShader) {
		ewol::GetContext().GetResourcesManager().ReLoadResources();
		ewol::GetContext().ForceRedrawAll();
	} else if (_msg.GetMessage() == ednMsgGuiExit) {
		// TODO ...
	}
	
	return;
}

void MainWindows::OnObjectRemove(ewol::EObject * _removeObject)
{
	ewol::Windows::OnObjectRemove(_removeObject);
	if (m_widgetLabelFileName == _removeObject) {
		m_widgetLabelFileName = NULL;
	}
}
