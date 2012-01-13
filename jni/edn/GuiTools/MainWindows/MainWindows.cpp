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
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widgetMeta/FileChooser.h>
#include <ewol/WidgetManager.h>

#undef __class__
#define __class__	"MainWindows"



MainWindows::MainWindows(void)
{
	EDN_DEBUG("CREATE WINDOWS ... ");
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Button * myButton = NULL;
	ewol::Label * myLabel = NULL;
	CodeView * myCodeView = NULL;
	
	mySizerVert = new ewol::SizerVert();
	SetSubWidget(mySizerVert);
	
		mySizerHori = new ewol::SizerHori();
		mySizerVert->SubWidgetAdd(mySizerHori);
			
			myButton = new ewol::Button("Open");
			mySizerHori->SubWidgetAdd(myButton);
			myButton = new ewol::Button("Close");
			mySizerHori->SubWidgetAdd(myButton);
			myButton = new ewol::Button("Save");
			mySizerHori->SubWidgetAdd(myButton);
			myButton = new ewol::Button("Save As ...");
			mySizerHori->SubWidgetAdd(myButton);
			
			myLabel = new ewol::Label("FileName");
			myLabel->SetExpendX(true);
			myLabel->SetFillY(true);
			mySizerHori->SubWidgetAdd(myLabel);
		
		mySizerHori = new ewol::SizerHori();
		mySizerVert->SubWidgetAdd(mySizerHori);
			myCodeView = new CodeView();
			myCodeView->SetExpendX(true);
			myCodeView->SetExpendY(true);
			myCodeView->SetFillX(true);
			myCodeView->SetFillY(true);
			mySizerHori->SubWidgetAdd(myCodeView);
			
}


MainWindows::~MainWindows(void)
{
	
}
















#ifdef SDFGSDFGSDF___SDFGSDFG
MainWindows::MainWindows(void) : MsgBroadcast("Main Windows", EDN_CAT_GUI)
{
	m_mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// select the program icone
	GError *err = NULL;
	etk::String iconeFile;
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
	etk::String ExitIconeFile;
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
	etk::String tmp = "";
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
	etk::String tmp = "Edn";
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



