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

#include <tools_debug.h>
#include <tools_globals.h>
#include <MainWindows.h>
#include <CodeView.h>
#include <ClipBoard.h>
#include <BufferView.h>

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
#include <ewol/widget/Menu.h>
#include <ewol/widgetMeta/FileChooser.h>
#include <ewol/WidgetManager.h>
#include <ewol/EObject.h>

#undef __class__
#define __class__	"MainWindows"

extern const char * const TYPE_EOBJECT_EDN_MAIN_WINDOWS = "MainWindows";

MainWindows::MainWindows(void)
{
	EDN_DEBUG("CREATE WINDOWS ... ");
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	//ewol::Button * myButton = NULL;
	CodeView * myCodeView = NULL;
	BufferView * myBufferView = NULL;
	ewol::Menu * myMenu = NULL;
	
	mySizerVert = new ewol::SizerVert();
	SetSubWidget(mySizerVert);
	
		mySizerHori = new ewol::SizerHori();
		mySizerVert->SubWidgetAdd(mySizerHori);
			
			myMenu = new ewol::Menu();
			mySizerHori->SubWidgetAdd(myMenu);
			int32_t idMenuFile = myMenu->AddTitle("File");
				(void)myMenu->Add(idMenuFile, "New",          "iconEdn.bmp", ednMsgGuiNew);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuFile, "Open",         "icon/Load.svg", ednMsgGuiOpen);
				(void)myMenu->Add(idMenuFile, "Close",        "icon/Close.svg", ednMsgGuiClose, "current");
				(void)myMenu->Add(idMenuFile, "Close (all)",  "", ednMsgGuiClose, "All");
				(void)myMenu->Add(idMenuFile, "Save",         "icon/Save.svg", ednMsgGuiSave, "current");
				(void)myMenu->Add(idMenuFile, "Save As ...",  "", ednMsgGuiSaveAs);
				(void)myMenu->AddSpacer();
				//(void)myMenu->Add(idMenuFile, "Exit", "", ednMsgGuiExit);
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
				(void)myMenu->Add(idMenuSearch, "Replace",        "", ednMsgGuiReplace);
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
			(void)myMenu->AddTitle("?", "", ednMsgGuiAbout);
			
			m_widgetLabelFileName = new ewol::Label("FileName");
			m_widgetLabelFileName->SetExpendX(true);
			m_widgetLabelFileName->SetFillY(true);
			mySizerHori->SubWidgetAdd(m_widgetLabelFileName);
		
		mySizerHori = new ewol::SizerHori();
		mySizerVert->SubWidgetAdd(mySizerHori);
			myBufferView = new BufferView();
			myBufferView->SetExpendX(false);
			myBufferView->SetExpendY(true);
			myBufferView->SetFillX(true);
			myBufferView->SetFillY(true);
			mySizerHori->SubWidgetAdd(myBufferView);
			myCodeView = new CodeView();
			myCodeView->SetExpendX(true);
			myCodeView->SetExpendY(true);
			myCodeView->SetFillX(true);
			myCodeView->SetFillY(true);
			
			myCodeView->SetFontSize(11);
			myCodeView->SetFontNameNormal(    "freefont/FreeMono.ttf");
			myCodeView->SetFontNameBold(      "freefont/FreeMonoBold.ttf");
			myCodeView->SetFontNameItalic(    "freefont/FreeMonoOblique.ttf");
			myCodeView->SetFontNameBoldItalic("freefont/FreeMonoBoldOblique.ttf");
			/*
			myCodeView->SetFontSize(11);
			myCodeView->SetFontNameNormal(    "ubuntu/UbuntuMono-R.ttf");
			myCodeView->SetFontNameBold(      "ubuntu/UbuntuMono-B.ttf");
			myCodeView->SetFontNameItalic(    "ubuntu/UbuntuMono-RI.ttf");
			myCodeView->SetFontNameBoldItalic("ubuntu/UbuntuMono-BI.ttf");
			*/
			mySizerHori->SubWidgetAdd(myCodeView);
			
	// Generic event ...
	RegisterMultiCast(ednMsgGuiSaveAs);
	RegisterMultiCast(ednMsgGuiOpen);
	RegisterMultiCast(ednMsgGuiAbout);
	// to update the title ... 
	RegisterMultiCast(ednMsgBufferState);
	RegisterMultiCast(ednMsgBufferId);
}


MainWindows::~MainWindows(void)
{
	
}

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool MainWindows::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		EWOL_ERROR("check error : \"" << TYPE_EOBJECT_EDN_MAIN_WINDOWS << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == TYPE_EOBJECT_EDN_MAIN_WINDOWS) {
		return true;
	} else {
		if(true == ewol::Windows::CheckObjectType(objectType)) {
			return true;
		}
		EWOL_ERROR("check error : \"" << TYPE_EOBJECT_EDN_MAIN_WINDOWS << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const MainWindows::GetObjectType(void)
{
	return TYPE_EOBJECT_EDN_MAIN_WINDOWS;
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
	
	//EDN_INFO("Receive Event from the main windows ... : widgetid=" << CallerObject << "\"" << eventId << "\" ==> data=\"" << data << "\"" );
	// Open file Section ...
	if (eventId == ednMsgGuiOpen) {
		ewol::FileChooser* tmpWidget = new ewol::FileChooser();
		tmpWidget->SetTitle("Open Files ...");
		tmpWidget->SetValidateLabel("Open");
		// TODO : Set the good folder ...
		//tmpWidget->SetFolder("/");
		PopUpWidgetPush(tmpWidget);
		tmpWidget->RegisterOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpFileSelected);
	} else if (eventId == ednEventPopUpFileSelected) {
		// get widget:
		ewol::FileChooser * tmpWidget = EWOL_CAST_WIDGET_FILE_CHOOSER(CallerObject);
		if (NULL == tmpWidget) {
			EDN_ERROR("impossible to get pop_upWidget " << CallerObject);
			return;
		}
		// get the filename : 
		etk::UString tmpData = tmpWidget->GetCompleateFileName();
		EDN_DEBUG("Request opening the file : " << tmpData);
		SendMultiCast(ednMsgOpenFile, tmpData);
	} else if (eventId == ednMsgGuiSaveAs) {
		if (data == "") {
			EDN_ERROR("Null data for Save As file ... ");
		} else {
			m_currentSavingAsIdBuffer = -1;
			if (data == "current") {
				m_currentSavingAsIdBuffer = BufferManager::GetSelected();
			} else {
				sscanf(data.Utf8Data(), "%d", &m_currentSavingAsIdBuffer);
			}
			
			if (false == BufferManager::Exist(m_currentSavingAsIdBuffer)) {
				EDN_ERROR("Request saveAs on non existant Buffer ID=" << m_currentSavingAsIdBuffer);
			} else {
				Buffer * myBuffer = BufferManager::Get(m_currentSavingAsIdBuffer);
				ewol::FileChooser* tmpWidget = new ewol::FileChooser();
				tmpWidget->SetTitle("Save Files As...");
				tmpWidget->SetValidateLabel("Save");
				etk::UString folder = "/home/";
				etk::UString fileName = "";
				if (true == myBuffer->HaveName()) {
					etk::File tmpName = myBuffer->GetFileName();
					folder = tmpName.GetFolder();
					fileName = tmpName.GetShortFilename();
				}
				tmpWidget->SetFolder(folder);
				tmpWidget->SetFileName(fileName);
				PopUpWidgetPush(tmpWidget);
				tmpWidget->RegisterOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpFileSaveAs);
			}
		}
	} else if (eventId == ednEventPopUpFileSaveAs) {
		// get widget:
		ewol::FileChooser * tmpWidget = EWOL_CAST_WIDGET_FILE_CHOOSER(CallerObject);
		if (NULL == tmpWidget) {
			EDN_ERROR("impossible to get pop_upWidget " << CallerObject);
			return;
		}
		// get the filename : 
		etk::UString tmpData = tmpWidget->GetCompleateFileName();
		EDN_DEBUG("Request Saving As file : " << tmpData);
		
		BufferManager::Get(m_currentSavingAsIdBuffer)->SetFileName(tmpData);
		SendMultiCast(ednMsgGuiSave, m_currentSavingAsIdBuffer);
	} else if(    eventId == ednMsgBufferState
	           || eventId == ednMsgBufferId) {
		// the buffer change we need to update the widget string
		Buffer* tmpBuffer = BufferManager::Get(BufferManager::GetSelected());
		if (NULL != tmpBuffer) {
			etk::File compleateName = tmpBuffer->GetFileName();
			bool isModify = tmpBuffer->IsModify();
			etk::UString directName = compleateName.GetCompleateName();
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
	} else if (eventId == ednMsgGuiAbout) {
	/*
		//Title 
			"Edn"
		// version
			VERSION_TAG_NAME
		// comments:
			"Editeur De N'ours\n"
			"L'Editeur Desoxyribo-Nucleique.\n"
			"Source Code Editor\n"
			"Build Time : " VERSION_BUILD_TIME;
		// copyright
			"Copyright 2010 Edouard DUPIN, all right reserved";
		// licence
			"This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY\n\n"
			"You can:\n"
			"    * Redistribute the sources code and binaries.\n"
			"    * Modify the Sources code.\n"
			"    * Use a part of the sources (less than 50%) in an other software, just write somewhere \"Edn is great\" visible by the user (on your product or on your website with a link to my page).\n"
			"    * Redistribute the modification only if you want.\n"
			"    * Send me the bug-fix (it could be great).\n"
			"    * Pay me a beer or some other things.\n"
			"    * Print the source code on WC paper ...\n\n"
			"You can NOT:\n"
			"    * Earn money with this Software (But I can).\n"
			"    * Add malware in the Sources.\n"
			"    * Do something bad with the sources.\n"
			"    * Use it to travel in the space with a toaster.\n\n"
			"I reserve the right to change this licence. If it change the version of the copy you have keep its own license."
		*/
	}
	
	return;
}

