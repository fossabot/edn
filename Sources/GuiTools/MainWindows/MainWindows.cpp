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

#include "tools_debug.h"
#include "tools_globals.h"
#include "MainWindows.h"
#include "CodeView.h"
#include "ClipBoard.h"
#include "BufferView.h"
#include "AccelKey.h"

#undef __class__
#define __class__	"MainWindows"

MainWindows::MainWindows(void) : MsgBroadcast("Main Windows", EDN_CAT_GUI)
{

	m_mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// select the program icone
	GError *err = NULL;
	Edn::String iconeFile;
#ifdef NDEBUG
	iconeFile = "/usr/share/edn/images/icone.png";
#else
	iconeFile = "./data/data/imagesSources/icone.png";
#endif

	GdkPixbuf * icone = gdk_pixbuf_new_from_file(iconeFile.c_str(), &err);
	if (err != NULL) {
		gtk_window_set_default_icon_name("text-editor");
	} else {
		gtk_window_set_icon(GTK_WINDOW(m_mainWindow), icone);
	}
	// Default size open windows
	gtk_window_set_default_size(GTK_WINDOW(m_mainWindow), 800, 600);

	// enable the close signal of the windows 
	g_signal_connect(G_OBJECT(m_mainWindow), "delete-event", G_CALLBACK(OnQuit), this);
	g_signal_connect(G_OBJECT(m_mainWindow), "window-state-event", G_CALLBACK(OnStateChange), this);
	//g_signal_connect(G_OBJECT(m_mainWindow), "destroy", G_CALLBACK(OnQuit), this);

	// Create a vertical box for stacking the menu and editor widgets in.
	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add(GTK_CONTAINER(m_mainWindow), vbox);

	// Set key Accelerator :
	AccelKey::getInstance()->LinkCommonAccel(GTK_WINDOW(m_mainWindow));

	// Create the menu bar.
	gtk_box_pack_start(	GTK_BOX (vbox), m_MenuBar.GetWidget(), FALSE, FALSE, 0);

	// **********************************************************
	// *                Horizontal ELEMENTS :                   *
	// **********************************************************
		// Create a vertical box for stacking the menu and editor widgets in.
		GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
		gtk_container_add(GTK_CONTAINER (vbox), hbox);
		// create the toolbar :
#		if USE_GTK_VERSION_2_0
		gtk_box_pack_start(GTK_BOX(hbox), m_ToolBar.GetWidget(), FALSE, FALSE, 0);
#		endif
		// TreeView :
		gtk_box_pack_start(GTK_BOX(hbox), m_BufferView.GetMainWidget(), FALSE, TRUE, 1);

		// Text displayer : 
		gtk_box_pack_start(	GTK_BOX (hbox), m_CodeView.GetMainWidget(), TRUE, TRUE, 0);

	// Create the status bar
	//gtk_box_pack_end(GTK_BOX(vbox), m_StatusBar.GetWidget(), FALSE, FALSE, 0);

	// recursive version of gtk_widget_show
	gtk_widget_show_all(m_mainWindow); 

}

MainWindows::~MainWindows(void)
{
	/*
	if (NULL != m_DlgSearch) {
		delete m_DlgSearch;
	}
	*/
}



void MainWindows::SetTitle(Edn::File &fileName, bool isModify)
{
	Edn::String tmp = "";
	if (fileName.GetShortFilename() != "") {
		tmp += fileName.GetShortFilename();
		tmp += " - ";
		tmp += fileName.GetFolder();
		tmp += " - ";
	}
	tmp += "Edn";
	gtk_window_set_title(GTK_WINDOW(m_mainWindow), tmp.c_str());
}

void MainWindows::SetNoTitle(void)
{
	Edn::String tmp = "Edn";
	gtk_window_set_title(GTK_WINDOW(m_mainWindow), tmp.c_str());
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
				Buffer *mybuf = BufferManager::getInstance()->Get(dataID);
				if (NULL != mybuf) {
					Edn::File plop = mybuf->GetFileName();
					SetTitle(plop, mybuf->IsModify() );
				}
			}
			break;
		case EDN_MSG__QUIT:
			OnQuit(m_mainWindow, this);
			break;
	}
}

bool MainWindows::OnQuit(GtkWidget *widget, gpointer data)
{
	EDN_INFO("quit requested");
	// dlg to confirm the quit event : 
	GtkWidget *p_dialog = gtk_dialog_new_with_buttons("Exit",
	                                                  GTK_WINDOW(widget),
	                                                  GTK_DIALOG_MODAL,
	                                                  GTK_STOCK_YES, GTK_RESPONSE_YES,
	                                                  GTK_STOCK_NO, GTK_RESPONSE_NO,
	                                                  NULL);
	GtkWidget *p_label =  gtk_label_new ("Do you want exit Edn?");
	gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area( GTK_DIALOG(p_dialog) )), p_label, TRUE, TRUE, 0);
	
	gtk_widget_show(p_label);
	
	switch (gtk_dialog_run (GTK_DIALOG (p_dialog)))
	{
		case GTK_RESPONSE_YES:
			gtk_widget_destroy (p_dialog);
			
			break;
		case GTK_RESPONSE_NO:
			gtk_widget_destroy (p_dialog);
			// do not close the windows
			return true;
			break;
	}
	
	gtk_main_quit();
	return false;
}


gboolean MainWindows::OnStateChange(GtkWidget *widget, GdkEvent* event, gpointer data)
{
	MainWindows * self = reinterpret_cast<MainWindows*>(data);
	EDN_WARNING("State change");
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

#if 0

void MainWindows::OnMenuAbout(wxCommandEvent & WXUNUSED(event))
{
	//EDN_INFO("MainWindows::OnMenuAbout (event)");
	wxAboutDialogInfo info;
	info.SetName(wxT("edn"));
	info.SetVersion(wxT("0.1.0 pre-Beta"));
	info.SetDescription(wxT("Editeur De N'ours, l'Editeur Desoxyribo-Nucleique\n"
							"Source Code Editor"));
	info.SetCopyright(wxT(	"Copyright 2010 Edouard DUPIN, all right reserved\n"
							"This software is distributed in the hope that it will be useful, but WITHOUT\n"
							"ANY WARRANTY\n\n"
							"Licence summary : \n"
							"   You can modify and redistribute the sources code and binaries.\n"
							"   You can send me the bug-fix\n"
							"   You can not earn money with this Software (if the source extract from Edn\n"
							"       represent less than 50% of original Sources)\n"
							"Term of the licence in in the file licence.txt"));
	wxAboutBox(info);

}

#endif




