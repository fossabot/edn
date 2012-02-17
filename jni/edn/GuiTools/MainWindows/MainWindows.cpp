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
#include <AccelKey.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/Test.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Menu.h>
#include <ewol/widgetMeta/FileChooser.h>
#include <ewol/WidgetManager.h>
#include <ewol/WidgetMessageMultiCast.h>

#undef __class__
#define __class__	"MainWindows"

MainWindows::MainWindows(void)
{
	EDN_DEBUG("CREATE WINDOWS ... ");
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	//ewol::Button * myButton = NULL;
	ewol::Label * myLabel = NULL;
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
				(void)myMenu->Add(idMenuFile, "New", "", ednMsgGuiNew);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuFile, "Open", "", ednMsgGuiOpen);
				(void)myMenu->Add(idMenuFile, "Close", "", ednMsgGuiClose, "current");
				(void)myMenu->Add(idMenuFile, "Close (all)", "", ednMsgGuiClose, "All");
				(void)myMenu->Add(idMenuFile, "Save", "", ednMsgGuiSave, "current");
				(void)myMenu->Add(idMenuFile, "Save As ...", "", ednMsgGuiSaveAs);
				(void)myMenu->AddSpacer();
				//(void)myMenu->Add(idMenuFile, "Exit", "", ednMsgGuiExit);
			int32_t idMenuEdit = myMenu->AddTitle("Edit");
				(void)myMenu->Add(idMenuEdit, "Undo", "", ednMsgGuiUndo);
				(void)myMenu->Add(idMenuEdit, "Redo", "", ednMsgGuiRedo);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuEdit, "Copy",  "", ednMsgGuiCopy, "STD");
				(void)myMenu->Add(idMenuEdit, "Cut",   "", ednMsgGuiCut, "STD");
				(void)myMenu->Add(idMenuEdit, "Paste", "", ednMsgGuiPaste, "STD");
				(void)myMenu->Add(idMenuEdit, "Redo",  "", ednMsgGuiRedo);
				(void)myMenu->Add(idMenuEdit, "Remove","", ednMsgGuiRm);
				(void)myMenu->AddSpacer();
				(void)myMenu->Add(idMenuEdit, "Select All","", ednMsgGuiSelect, "ALL");
				(void)myMenu->Add(idMenuEdit, "Un-Select","", ednMsgGuiSelect, "NONE");
				(void)myMenu->Add(idMenuEdit, "Goto line ...","", ednMsgGuiGotoLine, "???");
			int32_t idMenuSearch = myMenu->AddTitle("Search");
				(void)myMenu->Add(idMenuSearch, "Search",         "", ednMsgGuiSearch);
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
			
			myLabel = new ewol::Label("FileName");
			myLabel->SetExpendX(true);
			myLabel->SetFillY(true);
			mySizerHori->SubWidgetAdd(myLabel);
		
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
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgGuiSaveAs);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgGuiOpen);
}


MainWindows::~MainWindows(void)
{
	
}

const char *const ednEventPopUpFileSelected = "edn-mainWindows-openSelected";
const char *const ednEventPopUpFileSaveAs   = "edn-mainWindows-saveAsSelected";

bool MainWindows::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y)
{
	if (true == ewol::Windows::OnEventAreaExternal(widgetID, generateEventId, data, x, y) ) {
		return true;
	}
	//EDN_INFO("Receive Event from the main windows ... : widgetid=" << widgetID << "\"" << generateEventId << "\" ==> data=\"" << data << "\"" );
	// Open file Section ...
	if (generateEventId == ednMsgGuiOpen) {
		ewol::FileChooser* tmpWidget = new ewol::FileChooser();
		tmpWidget->SetTitle("Open Files ...");
		tmpWidget->SetValidateLabel("Open");
		// TODO : Set the good folder ...
		//tmpWidget->SetFolder("/");
		PopUpWidgetPush(tmpWidget);
		if (false == tmpWidget->ExternLinkOnEvent(ewolEventFileChooserValidate, GetWidgetId(), ednEventPopUpFileSelected) ) {
			EDN_CRITICAL("link with an entry event");
		}
	} else if (generateEventId == ednEventPopUpFileSelected) {
		// get widget:
		ewol::FileChooser * tmpWidget = (ewol::FileChooser*)ewol::widgetManager::Get(widgetID);
		if (NULL == tmpWidget) {
			EDN_ERROR("impossible to get pop_upWidget " << widgetID);
			return false;
		}
		// get the filename : 
		etk::UString tmpData = tmpWidget->GetCompleateFileName();
		EDN_DEBUG("Request opening the file : " << tmpData);
		ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgOpenFile, tmpData);
	} else if (generateEventId == ednMsgGuiSaveAs) {
		if (NULL == data) {
			EDN_ERROR("Null data for Save As file ... ");
		} else {
			BufferManager * myMng = BufferManager::getInstance();
			m_currentSavingAsIdBuffer = -1;
			if (0 == strcmp(data , "current")) {
				m_currentSavingAsIdBuffer = myMng->GetSelected();
			} else {
				sscanf(data, "%d", &m_currentSavingAsIdBuffer);
			}
			
			if (false == myMng->Exist(m_currentSavingAsIdBuffer)) {
				EDN_ERROR("Request saveAs on non existant Buffer ID=" << m_currentSavingAsIdBuffer);
			} else {
				Buffer * myBuffer = myMng->Get(m_currentSavingAsIdBuffer);
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
				if (false == tmpWidget->ExternLinkOnEvent(ewolEventFileChooserValidate, GetWidgetId(), ednEventPopUpFileSaveAs) ) {
					EDN_CRITICAL("link with an entry event");
				}
			}
		}
	} else if (generateEventId == ednEventPopUpFileSaveAs) {
		// get widget:
		ewol::FileChooser * tmpWidget = (ewol::FileChooser*)ewol::widgetManager::Get(widgetID);
		if (NULL == tmpWidget) {
			EDN_ERROR("impossible to get pop_upWidget " << widgetID);
			return false;
		}
		// get the filename : 
		etk::UString tmpData = tmpWidget->GetCompleateFileName();
		EDN_DEBUG("Request Saving As file : " << tmpData);
		
		BufferManager * myMng = BufferManager::getInstance();
		myMng->Get(m_currentSavingAsIdBuffer)->SetFileName(tmpData);
		ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgGuiSave, m_currentSavingAsIdBuffer);
	}
	
	return true;
}

