/**
 *******************************************************************************
 * @file MainWindows.cpp
 * @brief Editeur De N'ours : main Windows diplayer (Sources)
 * @author Edouard DUPIN
 * @date 04/01/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <MainWindows.h>
#include <CodeView.h>
#include <BufferView.h>
#include <Search.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
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
#include <ewol/ResourceManager.h>
#include <ewol/eObject/EObject.h>




#undef __class__
#define __class__	"AboutGui"

#include <ewol/widget/Label.h>
#include <ewol/widget/Spacer.h>

class ParameterAboutGui : public ewol::SizerVert
{
	public :
		ParameterAboutGui(void)
		{
			ewol::Spacer* mySpacer = NULL;
			
			mySpacer = new ewol::Spacer();
			if (NULL == mySpacer) {
				APPL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpendX(true);
				mySpacer->SetExpendY(true);
				SubWidgetAdd(mySpacer);
			}
			AddElement("        libPng");
			AddElement("        ogg-tremor");
			AddElement("        portaudio");
			AddElement("        libZip");
			AddElement("        tinyXml");
			AddElement("        freetype");
			AddElement("        agg2.4");
			AddElement("        etk (BSD)");
			AddElement("    ewol is based on");
			AddElement("    Website : https://github.com/HeeroYui/ewol");
			AddElement("    Licence : BSD like");
			AddElement("    Copyright 2010 Edouard DUPIN, all right reserved");
			AddElement("    Supported OS : Linux, Windows, Android" );
			AddElement(etk::UString("    OpenGl librairy : v") + ewol::GetVersion() );
			AddElement("Ewol", true);
			AddElement("");
			AddElement("    Website : https://github.com/HeeroYui/edn");
			AddElement("    Licence : GPL");
			AddElement("    Copyright 2010 Edouard DUPIN, all right reserved");
			AddElement(etk::UString("    Build Time : ") + etk::UString(BUILD_TIME));
			AddElement("    Source Code Editor");
			AddElement(etk::UString("    Editeur De N'ours : v") + etk::UString(APPL_VERSION_TAG_NAME));
			AddElement("Edn", true);
			
		};
		
		~ParameterAboutGui(void) { };
		
		void AddElement(etk::UString label, bool bold=false, bool italic=false)
		{
			ewol::Label* myLabel = new ewol::Label(label);
			if (NULL == myLabel) {
				APPL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myLabel->SetExpendX(true);
				SubWidgetAdd(myLabel);
			}
		};
		
};


const char * l_smoothChick = "tmpEvent_smooth";
const char * l_smoothMin = "tmpEvent_minChange";
const char * l_smoothMax = "tmpEvent_maxChange";


extern float DF_SoftEdge_min;
extern float DF_SoftEdge_max;
extern int32_t DF_SoftEdge;
ewol::Slider* tmpSliderMin = NULL;
ewol::Slider* tmpSliderMax = NULL;

#undef __class__
#define __class__	"MainWindows"

MainWindows::MainWindows(void)
{
	APPL_DEBUG("CREATE WINDOWS ... ");
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerVert * mySizerVert2 = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	//ewol::Button * myButton = NULL;
	CodeView * myCodeView = NULL;
	BufferView * myBufferView = NULL;
	ewol::Menu * myMenu = NULL;
	
	mySizerVert = new ewol::SizerVert();
	SetSubWidget(mySizerVert);
	
		mySizerHori = new ewol::SizerHori();
		mySizerVert->SubWidgetAdd(mySizerHori);
			myBufferView = new BufferView();
			myBufferView->SetExpendX(false);
			myBufferView->SetExpendY(true);
			myBufferView->SetFillX(true);
			myBufferView->SetFillY(true);
			mySizerHori->SubWidgetAdd(myBufferView);
			
			mySizerVert2 = new ewol::SizerVert();
			mySizerHori->SubWidgetAdd(mySizerVert2);
				
				// main buffer Area :
				myCodeView = new CodeView("FreeMono", 11);
				myCodeView->SetExpendX(true);
				myCodeView->SetExpendY(true);
				myCodeView->SetFillX(true);
				myCodeView->SetFillY(true);
				mySizerVert2->SubWidgetAdd(myCodeView);
				
				// search area : 
				Search * mySearch = new Search();
				mySizerVert2->SubWidgetAdd(mySearch);
				#ifdef APPL_BUFFER_FONT_DISTANCE_FIELD
				{
					ewol::SizerHori * mySizerHori2 = new ewol::SizerHori();
					mySizerVert2->SubWidgetAdd(mySizerHori2);
						
						ewol::CheckBox* tmpCheck = new ewol::CheckBox("smooth");
						mySizerHori2->SubWidgetAdd(tmpCheck);
						tmpCheck->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_smoothChick);
						
						ewol::Slider* tmpSlider = new ewol::Slider();
						mySizerHori2->SubWidgetAdd(tmpSlider);
						tmpSlider->RegisterOnEvent(this, ewolEventSliderChange, l_smoothMin);
						tmpSlider->SetExpendX(true);
						tmpSlider->SetMin(0);
						tmpSlider->SetMax(1000);
						tmpSlider->SetValue(0450);
						tmpSliderMin = tmpSlider;
						
						tmpSlider = new ewol::Slider();
						mySizerHori2->SubWidgetAdd(tmpSlider);
						tmpSlider->RegisterOnEvent(this, ewolEventSliderChange, l_smoothMax);
						tmpSlider->SetExpendX(true);
						tmpSlider->SetMin(0);
						tmpSlider->SetMax(1000);
						tmpSlider->SetValue(0550);
						tmpSliderMax = tmpSlider;
				}
				#endif
			
		mySizerHori = new ewol::SizerHori();
		mySizerVert->SubWidgetAdd(mySizerHori);
			
			myMenu = new ewol::Menu();
			mySizerHori->SubWidgetAdd(myMenu);
			int32_t idMenuFile = myMenu->AddTitle("File");
				(void)myMenu->Add(idMenuFile, "New",          "", ednMsgGuiNew);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuFile, "Open",         "icon/Load.svg", ednMsgGuiOpen);
				(void)myMenu->Add(idMenuFile, "Close",        "icon/Close.svg", ednMsgGuiClose, "current");
				(void)myMenu->Add(idMenuFile, "Close (all)",  "", ednMsgGuiClose, "All");
				(void)myMenu->Add(idMenuFile, "Save",         "icon/Save.svg", ednMsgGuiSave, "current");
				(void)myMenu->Add(idMenuFile, "Save As ...",  "", ednMsgGuiSaveAs);
				(void)myMenu->AddSpacer();
				//(void)myMenu->Add(idMenuFile, "Exit", "", ednMsgGuiExit);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuFile, "Properties",   "icon/Parameter.svg", ednMsgProperties);
			int32_t idMenuEdit = myMenu->AddTitle("Edit");
				(void)myMenu->Add(idMenuEdit, "Undo",         "icon/Undo.svg", ednMsgGuiUndo);
				(void)myMenu->Add(idMenuEdit, "Redo",         "icon/Redo.svg", ednMsgGuiRedo);
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
				(void)myMenu->Add(idMenuSearch, "Search",         "icon/Search.svg", ednMsgGuiSearch);
				(void)myMenu->Add(idMenuSearch, "Replace",        "icon/Replace.svg", ednMsgGuiReplace);
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
			
			m_widgetLabelFileName = new ewol::Label("FileName");
			m_widgetLabelFileName->SetExpendX(true);
			m_widgetLabelFileName->SetFillY(true);
			mySizerHori->SubWidgetAdd(m_widgetLabelFileName);
		
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

/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void MainWindows::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	ewol::Windows::OnReceiveMessage(CallerObject, eventId, data);
	
	//APPL_INFO("Receive Event from the main windows ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	// Open file Section ...
	if (eventId == ednMsgGuiOpen) {
		ewol::FileChooser* tmpWidget = new ewol::FileChooser();
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
	} else if (eventId == ednEventPopUpFileSelected) {
		APPL_DEBUG("Request opening the file : " << data);
		SendMultiCast(ednMsgOpenFile, data);
	} else if (eventId == ednMsgGuiSaveAs) {
		if (data == "") {
			APPL_ERROR("Null data for Save As file ... ");
		} else {
			m_currentSavingAsIdBuffer = -1;
			if (data == "current") {
				m_currentSavingAsIdBuffer = BufferManager::GetSelected();
			} else {
				sscanf(data.c_str(), "%d", &m_currentSavingAsIdBuffer);
			}
			
			if (false == BufferManager::Exist(m_currentSavingAsIdBuffer)) {
				APPL_ERROR("Request saveAs on non existant Buffer ID=" << m_currentSavingAsIdBuffer);
			} else {
				BufferText* myBuffer = BufferManager::Get(m_currentSavingAsIdBuffer);
				ewol::FileChooser* tmpWidget = new ewol::FileChooser();
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
	} else if (eventId == ednEventPopUpFileSaveAs) {
		// get the filename : 
		etk::UString tmpData = data;
		APPL_DEBUG("Request Saving As file : " << tmpData);
		
		BufferManager::Get(m_currentSavingAsIdBuffer)->SetFileName(tmpData);
		SendMultiCast(ednMsgGuiSave, m_currentSavingAsIdBuffer);
	} else if(    eventId == ednMsgBufferState
	           || eventId == ednMsgBufferId) {
		// the buffer change we need to update the widget string
		BufferText* tmpBuffer = BufferManager::Get(BufferManager::GetSelected());
		if (NULL != tmpBuffer) {
			etk::FSNode compleateName = tmpBuffer->GetFileName();
			bool isModify = tmpBuffer->IsModify();
			etk::UString directName = compleateName.GetName();
			if (true == isModify) {
				directName += " *";
			}
			if (NULL == m_widgetLabelFileName) {
				return;
			}
			m_widgetLabelFileName->SetLabel(directName);
			etk::UString windowsTitle = "edn - ";
			windowsTitle += directName;
			ewol::SetTitle(windowsTitle);
			return;
		} else {
			m_widgetLabelFileName->SetLabel("");
			ewol::SetTitle("edn");
		}
		return;
		// TODO : Set the Title ....
	} else if (eventId == ednMsgProperties) {
		// Request the parameter GUI
		ewol::Parameter* tmpWidget = new ewol::Parameter();
		if (NULL == tmpWidget) {
			APPL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			tmpWidget->SetTitle("Properties");
			PopUpWidgetPush(tmpWidget);
			tmpWidget->MenuAddGroup("Editor");
			ewol::Widget* tmpSubWidget = new globals::ParameterGlobalsGui();
			tmpWidget->MenuAdd("Editor",          "", tmpSubWidget);
			tmpWidget->MenuAdd("Polices & Color", "", NULL);
			tmpWidget->MenuAdd("Highlight",       "", NULL);
			tmpWidget->MenuAddGroup("Genral");
			tmpWidget->MenuAdd("Affichage",       "", NULL);
			tmpSubWidget = new ParameterAboutGui();
			tmpWidget->MenuAdd("About",           "", tmpSubWidget);
		}
	} else if (eventId == l_smoothChick) {
		if (data == "true") {
			DF_SoftEdge = 1;
		} else {
			DF_SoftEdge = 0;
		}
	} else if (eventId == l_smoothMin) {
		int32_t newVal = 0;
		sscanf(data.c_str(), "%d", &newVal);
		DF_SoftEdge_min = (float)newVal / 1000.0;
		if (DF_SoftEdge_min>DF_SoftEdge_max) {
			DF_SoftEdge_max = DF_SoftEdge_min;
			tmpSliderMax->SetValue(DF_SoftEdge_max*1000.0);
		}
	} else if (eventId == l_smoothMax) {
		int32_t newVal = 0;
		sscanf(data.c_str(), "%d", &newVal);
		DF_SoftEdge_max = (float)newVal / 1000.0;
		if (DF_SoftEdge_min>DF_SoftEdge_max) {
			DF_SoftEdge_min = DF_SoftEdge_max;
			tmpSliderMin->SetValue(DF_SoftEdge_min*1000.0);
		}
	} else if (eventId == ednMsgGuiReloadShader) {
		ewol::resource::ReLoadResources();
		ewol::ForceRedrawAll();
	}
	
	return;
}


/**
* @brief Inform object that an other object is removed ...
* @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
* @note : Sub classes must call this class
* @return ---
*/
void MainWindows::OnObjectRemove(ewol::EObject * removeObject)
{
	ewol::Windows::OnObjectRemove(removeObject);
	if (m_widgetLabelFileName == removeObject) {
		m_widgetLabelFileName = NULL;
	}
}
