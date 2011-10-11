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
	iconeFile = "./data/imagesSources/icone.png";
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
	
	// remove decoration
	//gtk_window_set_decorated(GTK_WINDOW(m_mainWindow), FALSE);
	
	// Create a vertical box for stacking the menu and editor widgets in.
	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add(GTK_CONTAINER(m_mainWindow), vbox);

	// Set key Accelerator :
	AccelKey::getInstance()->LinkCommonAccel(GTK_WINDOW(m_mainWindow));

	// Create the menu bar.
#if 0
	gtk_box_pack_start(	GTK_BOX (vbox), m_MenuBar.GetWidget(), FALSE, FALSE, 0);
#else
	GtkWidget *hboxMenu = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start(	GTK_BOX (vbox), hboxMenu, FALSE, FALSE, 0);
	// Add Exit boutton
	Edn::String ExitIconeFile;
#ifdef NDEBUG
	ExitIconeFile = "/usr/share/edn/images/delete-24px.png";
#else
	ExitIconeFile = "./data/imagesSources/delete-24px.png";
#endif
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
	GdkColor color;
	GtkStateType tmpppppp = GTK_STATE_NORMAL;
	//GtkStateType tmpppppp = GTK_STATE_ACTIVE;
	//GtkStateType tmpppppp = GTK_STATE_PRELIGHT;
	//GtkStateType tmpppppp = GTK_STATE_SELECTED;
	//GtkStateType tmpppppp = GTK_STATE_INSENSITIVE;
	//gdk_color_parse ("green", &color);
	//gtk_widget_modify_fg(m_MenuBar.GetWidget(), tmpppppp, &color);
	//gdk_color_parse ("blue", &color);
	//gtk_widget_modify_bg(m_MenuBar.GetWidget(), tmpppppp, &color);
	gdk_color_parse ("red", &color);
	gtk_widget_modify_text(m_MenuBar.GetWidget(), tmpppppp, &color);
	//gdk_color_parse ("orange", &color);
	gtk_widget_modify_base(m_MenuBar.GetWidget(), tmpppppp, &color);

	// Add title
	m_internalTitleLabel = gtk_label_new("Edn");
	gtk_box_pack_start(	GTK_BOX (hboxMenu), m_internalTitleLabel, FALSE, FALSE, 0);
#endif
	// **********************************************************
	// *                Horizontal ELEMENTS :                   *
	// **********************************************************
		// Create a vertical box for stacking the menu and editor widgets in.
		GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
		gtk_container_add(GTK_CONTAINER (vbox), hbox);
		// create the toolbar :
#		if USE_GTK_VERSION_2_0
		//gtk_box_pack_start(GTK_BOX(hbox), m_ToolBar.GetWidget(), FALSE, FALSE, 0);
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
	tmp = " ";
	if (fileName.GetShortFilename() != "") {
		tmp += fileName.GetFolder();
		tmp += "/";
		tmp += fileName.GetShortFilename();
	} else {
		tmp += "Edn";
	}
		tmp += " ";
	gtk_label_set_text(GTK_LABEL(m_internalTitleLabel), tmp.c_str());
	
}

void MainWindows::SetNoTitle(void)
{
	Edn::String tmp = "Edn";
	gtk_window_set_title(GTK_WINDOW(m_mainWindow), tmp.c_str());
	gtk_label_set_text(GTK_LABEL(m_internalTitleLabel), tmp.c_str());
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




