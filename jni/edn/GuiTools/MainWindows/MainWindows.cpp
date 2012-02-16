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


const char * const ednEventNewFile = "edn-New-File";
const char * const ednEventOpenFile = "edn-Open-File";
const char * const ednEventCloseFile = "edn-Close-File";
const char * const ednEventSaveFile = "edn-Save-File";
const char * const ednEventSaveAsFile = "edn-SaveAs-File";
const char * const ednEventPopUpFileSelected = "edn-PopUp-FileSelected";
const char * const ednEventPopUpFileSaveAs = "edn-PopUp-FileSaveAs";
const char * const ednEventContextMenuOther = "edn-Other";

MainWindows::MainWindows(void)
{
	EDN_DEBUG("CREATE WINDOWS ... ");
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Button * myButton = NULL;
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
				myMenu->Add(idMenuFile, "New", "", ednMsgGuiNew);
				myMenu->AddSpacer();
				myMenu->Add(idMenuFile, "Open", "", ednMsgGuiOpen);
				myMenu->Add(idMenuFile, "Close", "", ednMsgGuiClose, "current");
				myMenu->Add(idMenuFile, "Close (all)", "", ednMsgGuiClose, "All");
				myMenu->Add(idMenuFile, "Save", "", ednMsgGuiSave, "current");
				myMenu->Add(idMenuFile, "Save As ...", "", ednMsgGuiSaveAs);
				myMenu->AddSpacer();
				//myMenu->Add(idMenuFile, "Exit", "", ednMsgGuiExit);
			int32_t idMenuEdit = myMenu->AddTitle("Edit");
				myMenu->Add(idMenuEdit, "Undo", "", ednMsgGuiUndo);
				myMenu->Add(idMenuEdit, "Redo", "", ednMsgGuiRedo);
				myMenu->AddSpacer();
				myMenu->Add(idMenuEdit, "Copy",  "", ednMsgGuiCopy, "STD");
				myMenu->Add(idMenuEdit, "Cut",   "", ednMsgGuiCut, "STD");
				myMenu->Add(idMenuEdit, "Paste", "", ednMsgGuiPaste, "STD");
				myMenu->Add(idMenuEdit, "Redo",  "", ednMsgGuiRedo);
				myMenu->Add(idMenuEdit, "Remove","", ednMsgGuiRm);
				myMenu->AddSpacer();
				myMenu->Add(idMenuEdit, "Select All","", ednMsgGuiSelect, "ALL");
				myMenu->Add(idMenuEdit, "Un-Select","", ednMsgGuiSelect, "NONE");
				myMenu->Add(idMenuEdit, "Goto line ...","", ednMsgGuiGotoLine, "???");
			int32_t idMenuSearch = myMenu->AddTitle("Search");
				myMenu->Add(idMenuEdit, "Search",         "", ednMsgGuiSearch);
				myMenu->Add(idMenuEdit, "Replace",        "", ednMsgGuiReplace);
				myMenu->AddSpacer();
				myMenu->Add(idMenuEdit, "Find (previous)","", ednMsgGuiFind, "Previous");
				myMenu->Add(idMenuEdit, "Find (next)",    "", ednMsgGuiFind, "Next");
				myMenu->Add(idMenuEdit, "Find (all)",     "", ednMsgGuiFind, "All");
				myMenu->Add(idMenuEdit, "Un-Select",      "", ednMsgGuiFind, "None");
			int32_t idMenuCTags = myMenu->AddTitle("C-tags");
				myMenu->Add(idMenuEdit, "Load",      "", ednMsgGuiCtags, "Load");
				myMenu->Add(idMenuEdit, "ReLoad",    "", ednMsgGuiCtags, "ReLoad");
				myMenu->Add(idMenuEdit, "Jump",      "", ednMsgGuiCtags, "Jump");
				myMenu->Add(idMenuEdit, "Back",      "", ednMsgGuiCtags, "Back");
			int32_t idMenuAbout = myMenu->AddTitle("?", "", ednMsgGuiAbout);
			
			/*
			myButton = new ewol::Button("New");
			mySizerHori->SubWidgetAdd(myButton);
			if (false == myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ednEventNewFile) ) {
				EDN_CRITICAL("link with an entry event");
			}
			
			myButton = new ewol::Button("Open");
			mySizerHori->SubWidgetAdd(myButton);
			if (false == myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ednEventOpenFile) ) {
				EDN_CRITICAL("link with an entry event");
			}
			
			myButton = new ewol::Button("Close");
			mySizerHori->SubWidgetAdd(myButton);
			if (false == myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ednEventCloseFile) ) {
				EDN_CRITICAL("link with an entry event");
			}
			
			myButton = new ewol::Button("Save");
			mySizerHori->SubWidgetAdd(myButton);
			if (false == myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ednEventSaveFile) ) {
				EDN_CRITICAL("link with an entry event");
			}
			
			myButton = new ewol::Button("Save As ...");
			mySizerHori->SubWidgetAdd(myButton);
			if (false == myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ednEventSaveAsFile) ) {
				EDN_CRITICAL("link with an entry event");
			}
			*/
			
			myButton = new ewol::Button(" * Other * ");
			mySizerHori->SubWidgetAdd(myButton);
			if (false == myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ednEventContextMenuOther) ) {
				EDN_CRITICAL("link with an entry event");
			}
			
			
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
}


MainWindows::~MainWindows(void)
{
	
}



bool MainWindows::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y)
{
	if (true == ewol::Windows::OnEventAreaExternal(widgetID, generateEventId, data, x, y) ) {
		return true;
	}
	//EDN_INFO("Receive Event from the main windows ... : widgetid=" << widgetID << "\"" << generateEventId << "\" ==> data=\"" << data << "\"" );
	// newFile section ...
	if (generateEventId == ednEventNewFile) {
		ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgGuiNew);
	}
	else if (generateEventId == ednEventCloseFile) {
		ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgGuiClose, "current");
	}
	else if (generateEventId == ednEventSaveFile) {
		ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgGuiSave, "current");
	}
	else if (generateEventId == ednEventSaveAsFile) {
		OnEventAreaExternal(GetWidgetId(), ednMsgGuiSaveAs, "current", x, y);
	}
	// Open file Section ...
	else if (generateEventId == ednEventOpenFile) {
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
		
		//ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgOpenFile, tmpData);
	} else if (generateEventId == ednEventContextMenuOther) {
		ewol::ContextMenu * tmpWidget = new ewol::ContextMenu();
		if (NULL == tmpWidget) {
			return false;
		}
		tmpWidget->SetPositionMark(ewol::CONTEXT_MENU_MARK_TOP, {x, y} );
		
		ewol::SizerVert * mySizerVert = NULL;
		ewol::Button * myButton = NULL;
		
		mySizerVert = new ewol::SizerVert();
			mySizerVert->LockExpendContamination(true);
			// set it in the pop-up-system : 
			tmpWidget->SubWidgetSet(mySizerVert);
			
			myButton = new ewol::Button("plop");
				//m_widgetValidateId = myButton->GetWidgetId();
				//myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ewolEventFileChooserValidate);
				myButton->SetExpendX(true);
				myButton->SetFillX(true);
				mySizerVert->SubWidgetAdd(myButton);
			myButton = new ewol::Button("plop2");
				//m_widgetCancelId = myButton->GetWidgetId();
				//myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ewolEventFileChooserCancel);
				myButton->SetExpendX(true);
				myButton->SetFillX(true);
				mySizerVert->SubWidgetAdd(myButton);
			myButton = new ewol::Button("plop3");
				//m_widgetCancelId = myButton->GetWidgetId();
				//myButton->ExternLinkOnEvent(ewolEventButtonPressed, GetWidgetId(), ewolEventFileChooserCancel);
				myButton->SetExpendX(true);
				myButton->SetFillX(true);
				mySizerVert->SubWidgetAdd(myButton);
		
		PopUpWidgetPush(tmpWidget);
	}
	return true;
}













#ifdef SDFGSDFGSDF___SDFGSDFG
MainWindows::MainWindows(void) : MsgBroadcast("Main Windows", EDN_CAT_GUI)
{
	m_mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// select the program icone
	GError *err = NULL;
	etk::UString iconeFile;
	iconeFile = "/usr/share/edn/images/icone.png";

	GdkPixbuf * icone = gdk_pixbuf_new_from_file(iconeFile.c_str(), &err);
	if (err != NULL) {
		gtk_window_set_default_icon_name("text-editor");
	} else {
		gtk_window_set_icon(GTK_WINDOW(m_mainWindow), icone);
	}

	// enable the close signal of the windows 
	g_signal_connect(G_OBJECT(m_mainWindow), "delete-event", G_CALLBACK(OnQuit), this);
	g_signal_connect(G_OBJECT(m_mainWindow), "window-state-event", G_CALLBACK(OnStateChange), this);

	// Create a vertical box for stacking the menu and editor widgets in.
	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add(GTK_CONTAINER(m_mainWindow), vbox);

	// Set key Accelerator :
	AccelKey::getInstance()->LinkCommonAccel(GTK_WINDOW(m_mainWindow));

	// Create the menu bar.
	GtkWidget *hboxMenu = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start(	GTK_BOX (vbox), hboxMenu, FALSE, FALSE, 0);
	// Add Exit boutton
	etk::UString ExitIconeFile;
	ExitIconeFile = "/usr/share/edn/images/delete-24px.png";
	// TODO : find a good way to change the size of an image
	GtkWidget *myImageQuit = gtk_image_new_from_file(ExitIconeFile.c_str());
	GtkIconSize mySize = GTK_ICON_SIZE_SMALL_TOOLBAR;
	//EDN_DEBUG(" plop : " << mySize);
	//gtk_image_get_icon_set( GTK_IMAGE(myImageQuit), NULL, &mySize);
	//EDN_DEBUG(" plop : " << mySize);
	GtkWidget *event_box = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(event_box),myImageQuit);
	gtk_widget_add_events(event_box,GDK_BUTTON_PRESS_MASK);
	g_signal_connect(G_OBJECT(event_box), "button_press_event", G_CALLBACK(OnQuit), this);
	
	gtk_box_pack_start(	GTK_BOX(hboxMenu), event_box, FALSE, FALSE, 0);
	
	// add the real menu bar
	gtk_box_pack_start(	GTK_BOX (hboxMenu), m_MenuBar.GetWidget(), FALSE, FALSE, 0);

	// Add title
	m_internalTitleLabel = gtk_label_new("Edn");
	gtk_box_pack_start(	GTK_BOX (hboxMenu), m_internalTitleLabel, FALSE, FALSE, 0);
	// **********************************************************
	// *                Horizontal ELEMENTS :                   *
	// **********************************************************
		// Create a vertical box for stacking the menu and editor widgets in.
		GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
		gtk_container_add(GTK_CONTAINER (vbox), hbox);
		// create the toolbar :
		// TreeView :
		gtk_box_pack_start(GTK_BOX(hbox), m_BufferView.GetMainWidget(), FALSE, TRUE, 1);

		// Text displayer : 
		gtk_box_pack_start(	GTK_BOX (hbox), m_CodeView.GetMainWidget(), TRUE, TRUE, 0);

	// Create the status bar
	//gtk_box_pack_end(GTK_BOX(vbox), m_StatusBar.GetWidget(), FALSE, FALSE, 0);

	// recursive version of gtk_widget_show
	gtk_widget_show_all(m_mainWindow); 


MainWindows::~MainWindows(void)
{
	/*
	if (NULL != m_DlgSearch) {
		delete m_DlgSearch;
	}
	*/
}



void MainWindows::SetTitle(etk::File &fileName, bool isModify)
{
	etk::UString tmp = "";
	if (fileName.GetShortFilename() != "") {
		tmp += fileName.GetShortFilename();
		tmp += " - ";
		tmp += fileName.GetFolder();
		tmp += " - ";
	}
	tmp += "Edn";
	//gtk_window_set_title(GTK_WINDOW(m_mainWindow), tmp.c_str());
	tmp = " ";
	if (fileName.GetShortFilename() != "") {
		tmp += fileName.GetFolder();
		tmp += "/";
		tmp += fileName.GetShortFilename();
	} else {
		tmp += "Edn";
	}
	tmp += " ";
	//gtk_label_set_text(GTK_LABEL(m_internalTitleLabel), tmp.c_str());
	
}

void MainWindows::SetNoTitle(void)
{
	etk::UString tmp = "Edn";
	//gtk_window_set_title(GTK_WINDOW(m_mainWindow), tmp.c_str());
	//gtk_label_set_text(GTK_LABEL(m_internalTitleLabel), tmp.c_str());
}

void MainWindows::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		case EDN_MSG__BUFFER_CHANGE_CURRENT:
			// change Title :
			// TODO : String error when remove the error with -1;
			if (-1 == dataID) {
				SetNoTitle();
			} else {
				/*
				Buffer *mybuf = BufferManager::getInstance()->Get(dataID);
				if (NULL != mybuf) {
					etk::File plop = mybuf->GetFileName();
					SetTitle(plop, mybuf->IsModify() );
				}
				*/
			}
			break;
	}
}
bool MainWindows::OnQuit(GtkWidget *widget, gpointer data)
{
	//MainWindows * self = reinterpret_cast<MainWindows*>(data);
	GeneralSendMessage(EDN_MSG__GUI_SHOW_EXIT_CONFIRMATION);
	// Close is not managed here ...
	return true;
}


gboolean MainWindows::OnStateChange(GtkWidget *widget, GdkEvent* event, gpointer data)
{
	MainWindows * self = reinterpret_cast<MainWindows*>(data);
	EDN_DEBUG("State change");
	EDN_INFO(" change state mask : " << event->window_state.changed_mask);
	EDN_INFO(" change state new val : " << event->window_state.new_window_state);
	if (event->window_state.changed_mask == GDK_WINDOW_STATE_MAXIMIZED) {
		EDN_INFO("   ==> Maximisation change...");
		if (event->window_state.new_window_state == GDK_WINDOW_STATE_MAXIMIZED) {
			EDN_INFO("       ==> ENABLE");
			gtk_window_set_decorated(GTK_WINDOW(self->m_mainWindow), FALSE);
		} else {
			EDN_INFO("       ==> DISABLE");
			gtk_window_set_decorated(GTK_WINDOW(self->m_mainWindow), TRUE);
			
		}
	}
}
#endif



