/**
 *******************************************************************************
 * @file Search.cpp
 * @brief Editeur De N'ours : Search system
 * @author Edouard DUPIN
 * @date 03/01/2011
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


#include "tools_globals.h"
#include "Search.h"
#include "SearchData.h"
#include "BufferManager.h"
#include "MainWindows.h"
#include "MsgBroadcast.h"

#undef __class__
#define __class__	"Search"


Search::Search(void)
	/*:	m_localDialog(NULL),
		m_searchLabel(NULL),
		m_searchEntry(NULL),
		m_replaceLabel(NULL),
		m_replaceEntry(NULL),
		m_CkMatchCase(NULL),
		m_CkWrapAround(NULL) */
{
	// nothing to do ...
}

Search::~Search(void)
{
/*
	if (NULL!=m_localDialog) {
		gtk_widget_hide(m_localDialog);
		gtk_widget_destroy(m_localDialog);
		m_localDialog = NULL;
	}
*/
}
/*
void Search::Display(GtkWindow *parent)
{
	if(NULL == m_localDialog) {
		m_localDialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		// set dialog not resisable
		gtk_window_set_resizable(GTK_WINDOW(m_localDialog), FALSE);
		// set the windows alwais on top of every all system windows
		//gtk_window_set_keep_above(GTK_WINDOW(m_localDialog), TRUE);
		// Set the dialog on top of the selected windows
		gtk_window_set_transient_for(GTK_WINDOW(m_localDialog), parent);
		// set the dialog on the top right
		gtk_window_set_gravity(GTK_WINDOW(m_localDialog), GDK_GRAVITY_NORTH_EAST);
		// Remove the dialogue from the task bar
		gtk_window_set_skip_taskbar_hint(GTK_WINDOW(m_localDialog), TRUE);
		// this element did not apear in the miniature of the windows
		gtk_window_set_skip_pager_hint(GTK_WINDOW(m_localDialog), TRUE);
		// select the program icone
		//gtk_window_set_default_icon_name("Replace");
		
		// set default title : 
		gtk_window_set_title(GTK_WINDOW(m_localDialog), "Search / Replace");
		
		// Default size open windows
		//gtk_window_set_default_size(GTK_WINDOW(m_localDialog), 300, 400);
		
		// enable the close signal of the windows 
		g_signal_connect(G_OBJECT(m_localDialog), "destroy", G_CALLBACK(OnButtonQuit), this);
		
		// Create a vertical box for stacking the menu and editor widgets in.
		GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
		gtk_container_add(GTK_CONTAINER(m_localDialog), vbox);
		
		// Set key Accelerator :
		//AccelKey::getInstance()->LinkCommonAccel(GTK_WINDOW(m_localDialog));
		
		// **********************************************************
		// *  Search Entry
		// **********************************************************
		{
			GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
			gtk_container_add(GTK_CONTAINER (vbox), hbox);
			// search label
			m_searchLabel = gtk_label_new("Search  ");
			gtk_box_pack_start(GTK_BOX(hbox), m_searchLabel, FALSE, TRUE, 0);
			// Search entry
			m_searchEntry = gtk_entry_new();
			gtk_box_pack_start(GTK_BOX(hbox), m_searchEntry, TRUE, TRUE, 0);
			// Connect signals :
			g_signal_connect(G_OBJECT(m_searchEntry),          "activate", G_CALLBACK(OnButtonNext), this);
			g_signal_connect(G_OBJECT(m_searchEntry),          "changed",  G_CALLBACK(OnEntrySearchChange), this);
		}
		// **********************************************************
		// *  Replace Entry
		// **********************************************************
		{
			GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
			gtk_container_add(GTK_CONTAINER (vbox), hbox);
			// search label
			m_replaceLabel = gtk_label_new("Replace");
			gtk_box_pack_start(GTK_BOX(hbox), m_replaceLabel, FALSE, TRUE, 0);
			// Search entry
			m_replaceEntry = gtk_entry_new();
			gtk_box_pack_start(GTK_BOX(hbox), m_replaceEntry, TRUE, TRUE, 0);
			// Connect signals :
			g_signal_connect(G_OBJECT(m_replaceEntry),         "changed",  G_CALLBACK(OnEntryReplaceChange), this);
		}
		
		// **********************************************************
		// *  mode Selection
		// **********************************************************
		m_CkMatchCase = gtk_check_button_new_with_label("Case Sensitive");
		gtk_container_add(GTK_CONTAINER (vbox), m_CkMatchCase);
		m_CkWrapAround = gtk_check_button_new_with_label("Wrap arround");
		gtk_container_add(GTK_CONTAINER (vbox), m_CkWrapAround);
		m_CkRegularExpression = gtk_check_button_new_with_label("Regular Expression");
		gtk_container_add(GTK_CONTAINER (vbox), m_CkRegularExpression);
		
		// connect signals :
		g_signal_connect(G_OBJECT(m_CkMatchCase),          "activate", G_CALLBACK(OnCheckBoxEventCase), this);
		g_signal_connect(G_OBJECT(m_CkMatchCase),          "clicked",  G_CALLBACK(OnCheckBoxEventCase), this);
		g_signal_connect(G_OBJECT(m_CkWrapAround),         "activate", G_CALLBACK(OnCheckBoxEventWrap), this);
		g_signal_connect(G_OBJECT(m_CkWrapAround),         "clicked",  G_CALLBACK(OnCheckBoxEventWrap), this);
		g_signal_connect(G_OBJECT(m_CkRegularExpression),  "activate", G_CALLBACK(OnCheckBoxEventRegExp), this);
		g_signal_connect(G_OBJECT(m_CkRegularExpression),  "clicked",  G_CALLBACK(OnCheckBoxEventRegExp), this);
		
		
		// **********************************************************
		// *  Search / Replace button
		// **********************************************************
		{
			GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
			gtk_container_add(GTK_CONTAINER (vbox), hbox);
			// Find previous
			m_BtPrevious = gtk_button_new_with_label("Previous");
			gtk_container_add(GTK_CONTAINER (hbox), m_BtPrevious);
			// Find Next
			m_BtNext = gtk_button_new_with_label("Next");
			gtk_container_add(GTK_CONTAINER (hbox), m_BtNext);
			// Replace
			m_BtReplace = gtk_button_new_with_label("Replace");
			gtk_container_add(GTK_CONTAINER (hbox), m_BtReplace);
			// Replace & next
			m_BtReplaceAndNext = gtk_button_new_with_label("Replace & Find");
			gtk_container_add(GTK_CONTAINER (hbox), m_BtReplaceAndNext);
			// Exit
			m_BtQuit = gtk_button_new_with_label("Close");
			gtk_container_add(GTK_CONTAINER (hbox), m_BtQuit);
			
			// Connect signals :
			g_signal_connect(G_OBJECT(m_BtPrevious),           "activate", G_CALLBACK(OnButtonPrevious), this);
			g_signal_connect(G_OBJECT(m_BtPrevious),           "released", G_CALLBACK(OnButtonPrevious), this);
			g_signal_connect(G_OBJECT(m_BtNext),               "activate", G_CALLBACK(OnButtonNext), this);
			g_signal_connect(G_OBJECT(m_BtNext),               "released", G_CALLBACK(OnButtonNext), this);
			g_signal_connect(G_OBJECT(m_BtReplace),            "activate", G_CALLBACK(OnButtonReplace), this);
			g_signal_connect(G_OBJECT(m_BtReplace),            "released", G_CALLBACK(OnButtonReplace), this);
			g_signal_connect(G_OBJECT(m_BtReplaceAndNext),     "activate", G_CALLBACK(OnButtonReplaceAndNext), this);
			g_signal_connect(G_OBJECT(m_BtReplaceAndNext),     "released", G_CALLBACK(OnButtonReplaceAndNext), this);
			g_signal_connect(G_OBJECT(m_BtQuit),               "activate", G_CALLBACK(OnButtonQuit), this);
			g_signal_connect(G_OBJECT(m_BtQuit),               "released", G_CALLBACK(OnButtonQuit), this);
		}
		
		// recursive version of gtk_widget_show
		gtk_widget_show_all(m_localDialog);
	}
	if(NULL == m_localDialog) {
		EDN_ERROR("No search-dialog instance");
	} else {
		// update datas :
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_CkWrapAround), SearchData::GetWrap());
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_CkMatchCase), SearchData::GetCase());
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_CkRegularExpression), SearchData::GetRegExp());
		if (true == SearchData::GetRegExp()) {
			gtk_widget_set_sensitive(m_CkMatchCase, false);
		} else {
			gtk_widget_set_sensitive(m_CkMatchCase, true);
		}
		// Remove data form the search
		etk::UString myDataString = "";
		SearchData::SetSearch(myDataString);
		gtk_entry_set_text(GTK_ENTRY(m_searchEntry), myDataString.c_str());
		if (0 == strlen(myDataString.c_str())) {
			m_haveSearchData = false;
		} else {
			m_haveSearchData = true;
		}
		
		SearchData::GetReplace(myDataString);
		gtk_entry_set_text(GTK_ENTRY(m_replaceEntry), myDataString.c_str());
		if (0 == strlen(myDataString.c_str())) {
			m_haveReplaceData = false;
		} else {
			m_haveReplaceData = true;
		}
		
		gtk_widget_set_sensitive(m_BtPrevious, m_haveSearchData);
		gtk_widget_set_sensitive(m_BtNext, m_haveSearchData);
		// basic no search data
		gtk_widget_set_sensitive(m_BtReplace, false);
		gtk_widget_set_sensitive(m_BtReplaceAndNext, false);
		
		
		// set focus on a specific widget :
		//gtk_window_set_focus(parent, m_searchEntry);
		gtk_widget_grab_focus(m_searchEntry);
		
		// display the dialogue box
		gtk_widget_show_all(m_localDialog);
		
		// hide regular expression
		gtk_widget_hide(m_CkRegularExpression);
	}
}
*/

void Search::Destroy(void)
{
/*
	if (NULL!=m_localDialog) {
		gtk_widget_destroy(m_localDialog);
		m_localDialog = NULL;
	}
*/
}


void Search::Hide(void)
{
	//gtk_widget_hide(m_localDialog);
}

/*
void Search::OnButtonPrevious(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	GeneralSendMessage(EDN_MSG__CURRENT_FIND_PREVIOUS);
}

void Search::OnButtonNext(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	GeneralSendMessage(EDN_MSG__CURRENT_FIND_NEXT);
}

void Search::OnButtonReplace(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	GeneralSendMessage(EDN_MSG__CURRENT_REPLACE);
}

void Search::OnButtonReplaceAndNext(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	GeneralSendMessage(EDN_MSG__CURRENT_REPLACE);
	GeneralSendMessage(EDN_MSG__CURRENT_FIND_NEXT);
}

void Search::OnButtonQuit(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	Search * self = dynamic_cast<Search*>(data);
	self->Destroy();
}

void Search::OnCheckBoxEventWrap(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		SearchData::SetWrap(true);
	} else {
		SearchData::SetWrap(false);
	}
}

void Search::OnCheckBoxEventCase(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		SearchData::SetCase(true);
	} else {
		SearchData::SetCase(false);
	}
}

void Search::OnCheckBoxEventRegExp(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	Search * self = dynamic_cast<Search*>(data);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		SearchData::SetRegExp(true);
		gtk_widget_set_sensitive(self->m_CkMatchCase, false);
	} else {
		SearchData::SetRegExp(false);
		gtk_widget_set_sensitive(self->m_CkMatchCase, true);
	}
}

void Search::OnEntrySearchChange(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	Search * self = dynamic_cast<Search*>(data);
	// update research data
	const char *testData = gtk_entry_get_text(GTK_ENTRY(widget));
	if (NULL !=  testData) {
		etk::UString myDataString = testData;
		SearchData::SetSearch(myDataString);
		if (0 == strlen(testData)) {
			self->m_haveSearchData = false;
		} else {
			self->m_haveSearchData = true;
		}
		gtk_widget_set_sensitive(self->m_BtPrevious, self->m_haveSearchData);
		gtk_widget_set_sensitive(self->m_BtNext, self->m_haveSearchData);
		if (false == self->m_haveSearchData) {
			gtk_widget_set_sensitive(self->m_BtReplace, false);
			gtk_widget_set_sensitive(self->m_BtReplaceAndNext, false);
		} else {
			gtk_widget_set_sensitive(self->m_BtReplace, true);
			gtk_widget_set_sensitive(self->m_BtReplaceAndNext, true);
		}
	}
}

void Search::OnEntryReplaceChange(GtkWidget *widget, gpointer data)
{
	//EDN_INFO("CALLBACK");
	Search * self = dynamic_cast<Search*>(data);
	// update replace data
	const char *testData = gtk_entry_get_text(GTK_ENTRY(widget));
	if (NULL !=  testData) {
		etk::UString myDataString = testData;
		SearchData::SetReplace(myDataString);
		if (0 == strlen(testData)) {
			self->m_haveReplaceData = false;
		} else {
			self->m_haveReplaceData = true;
		}
		if (false == self->m_haveSearchData) {
			gtk_widget_set_sensitive(self->m_BtReplace, false);
			gtk_widget_set_sensitive(self->m_BtReplaceAndNext, false);
		} else {
			gtk_widget_set_sensitive(self->m_BtReplace, true);
			gtk_widget_set_sensitive(self->m_BtReplaceAndNext, true);
		}
	}
}
*/
