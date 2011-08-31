/**
 *******************************************************************************
 * @file MenuBar.cpp
 * @brief Editeur De N'ours : abstraction of the menu bar (Sources)
 * @author Edouard DUPIN
 * @date 17/06/2011
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
#include "MenuBar.h"
#include "ClipBoard.h"
#include "charset.h"
#include "ColorizeManager.h"

#define MENU_MSG
const char * MSG_TogleDisplayChar    = "Request a Togle of char displaying";
const char * MSG_TogleDisplayEOL     = "Request a Togle of displaying EndOfLine";
const char * MSG_TogleAutoIndent     = "Request a Togle of Auto Indent";
const char * MSG_SetCharsetIso559_1  = "Set ISO 5589-1";
const char * MSG_SetCharsetIso559_15 = "Set ISO 5589-15";
const char * MSG_SetCharsetUTF8      = "Set UTF 8";
const char * MSG_LoadColorBlack      = "Load Color Black";
const char * MSG_LoadColorWhite      = "Load Color White";
#define MSG_LINK(data)


static void CB_menuGenerique(GtkMenuItem *menu_item, gpointer data)
{
	//EDN_INFO("basic menue_event");
	messageData_ts * msg = (messageData_ts*)data;
	GeneralSendMessage(msg->msgId, msg->dataId);
}

static void CB_menuInternal(GtkMenuItem *menu_item, gpointer data)
{
	//EDN_INFO("basic menue_event");
	const char * myPointer = (const char*)data;
	if (myPointer == MSG_TogleDisplayChar) {
		if (globals::IsSetDisplaySpaceChar() == true) {
			globals::SetDisplaySpaceChar(false);
		} else {
			globals::SetDisplaySpaceChar(true);
		}
	} else if (myPointer == MSG_TogleDisplayEOL) {
		if (globals::IsSetDisplayEndOfLine() == true) {
			globals::SetDisplayEndOfLine(false);
		} else {
			globals::SetDisplayEndOfLine(true);
		}
	} else if (myPointer == MSG_TogleAutoIndent) {
		if (globals::IsSetAutoIndent() == true) {
			globals::SetAutoIndent(false);
		} else {
			globals::SetAutoIndent(true);
		}
	} else if (myPointer == MSG_SetCharsetIso559_1) {
		GeneralSendMessage(EDN_MSG__CURRENT_SET_CHARSET, EDN_CHARSET_ISO_8859_1);
	} else if (myPointer == MSG_SetCharsetIso559_15) {
		GeneralSendMessage(EDN_MSG__CURRENT_SET_CHARSET, EDN_CHARSET_ISO_8859_15);
	} else if (myPointer == MSG_SetCharsetUTF8) {
		GeneralSendMessage(EDN_MSG__CURRENT_SET_CHARSET, EDN_CHARSET_UTF8);
	} else if (myPointer == MSG_LoadColorWhite) {
		ColorizeManager * myColorSystem = ColorizeManager::getInstance();
		Edn::String homedir;
#		ifdef NDEBUG
			homedir = "/usr/share/edn/";
#		else
			homedir = "./data/";
#		endif
		homedir += "color_white.xml";
		myColorSystem->LoadFile(homedir);
	} else if (myPointer == MSG_LoadColorBlack) {
		ColorizeManager * myColorSystem = ColorizeManager::getInstance();
		Edn::String homedir;
#		ifdef NDEBUG
			homedir = "/usr/share/edn/";
#		else
			homedir = "./data/";
#		endif
		homedir += "color_black.xml";
		myColorSystem->LoadFile(homedir);
	}
}

#undef __class__
#define __class__	"MenuBarMain"

class MenuBarMain
{
	public:
		MenuBarMain(const char * title, GtkWidget * parent) : m_parent(NULL), m_menu(NULL), m_menuListe(NULL)
		{
			m_parent = parent;
			m_menu = gtk_menu_item_new_with_mnemonic(title);
			gtk_menu_shell_append( GTK_MENU_SHELL (parent), m_menu);
		}
		
		~MenuBarMain(void)
		{
			
		}
	private:
		void CheckSubMenu(void)
		{
			if (NULL == m_menuListe) {
				m_menuListe = gtk_menu_new();
				if (NULL != m_menuListe) {
					gtk_menu_item_set_submenu( GTK_MENU_ITEM (m_menu), m_menuListe);
				} else {
					EDN_ERROR("Can not create sub menu");
				}
			}
		}
		
	public:
	
		void AddInternal(const char * title, const char * accelKey, const char * msg, bool enable = false)
		{
			CheckSubMenu();
			if (NULL == m_menuListe) {
				return;
			}
			// create ITEM
			GtkWidget *tmpWidget = gtk_menu_item_new_with_mnemonic(title);
			// set grisage : 
			gtk_widget_set_sensitive(tmpWidget, enable);
			// add to the menu : 
			gtk_menu_shell_append(GTK_MENU_SHELL(m_menuListe), tmpWidget);
			// accel KEY : 
			AccelKey::getInstance()->SetAccel(tmpWidget, (char *)accelKey);
			// set callback
			g_signal_connect(G_OBJECT(tmpWidget), "activate", G_CALLBACK(CB_menuInternal), (void*)msg);
		}
		
		
		void Add(const char * title, const char * accelKey, messageType_te id = EDN_MSG__NONE, bool enable = false, int32_t dataSpecificID = -1)
		{
			CheckSubMenu();
			if (NULL == m_menuListe) {
				return;
			}
			messageData_ts * message = new messageData_ts;
			message->msgId      = id;
			message->dataId     = dataSpecificID;
			m_message.PushBack(message);
			// create ITEM
			GtkWidget *tmpWidget = gtk_menu_item_new_with_mnemonic(title);
			// set grisage : 
			gtk_widget_set_sensitive(tmpWidget, enable);
			// add to the menu : 
			gtk_menu_shell_append(GTK_MENU_SHELL(m_menuListe), tmpWidget);
			// accel KEY : 
			AccelKey::getInstance()->SetAccel(tmpWidget, (char *)accelKey);
			// set callback
			if(EDN_MSG__NONE!=id) {
				g_signal_connect(G_OBJECT(tmpWidget), "activate", G_CALLBACK(CB_menuGenerique), message);
			}
		}
		
		void AddGen(const char * title, const char * accelKey, messageType_te id = EDN_MSG__NONE, bool enable = false, int32_t dataSpecificID = -1)
		{
			CheckSubMenu();
			if (NULL == m_menuListe) {
				return;
			}
			messageData_ts * message = new messageData_ts;
			message->msgId      = id;
			message->dataId     = dataSpecificID;
			m_message.PushBack(message);
			// create ITEM
			GtkWidget *tmpWidget = gtk_image_menu_item_new_from_stock( title, AccelKey::getInstance()->GetAccel() );
			// set grisage : 
			gtk_widget_set_sensitive(tmpWidget, enable);
			// add to the menu : 
			gtk_menu_shell_append(GTK_MENU_SHELL(m_menuListe), tmpWidget);
			// accel KEY : 
			AccelKey::getInstance()->SetAccel(tmpWidget, (char *)accelKey);
			// set callback
			if(EDN_MSG__NONE!=id) {
				g_signal_connect(G_OBJECT(tmpWidget), "activate", G_CALLBACK(CB_menuGenerique), message);
			}
		}
		
		void AddSeparator(void)
		{
			CheckSubMenu();
			if (NULL == m_menuListe) {
				return;
			}
			gtk_menu_shell_append(GTK_MENU_SHELL (m_menuListe), gtk_separator_menu_item_new() );
		}
		
	private:
		GtkWidget *       m_parent;
		GtkWidget *       m_menu;
		GtkWidget *       m_menuListe;
		Edn::VectorType<messageData_ts*>    m_message;
};

#undef __class__
#define __class__	"MenuBar"

MenuBar::MenuBar(void) : MsgBroadcast("Menu bar", EDN_CAT_GUI)
{
	m_mainWidget = gtk_menu_bar_new();
	MenuBarMain *tmp = NULL;
	
	tmp = new MenuBarMain("_File", m_mainWidget);
		tmp->AddGen(GTK_STOCK_NEW,                 "ctrl+n",       EDN_MSG__NEW, true);
		tmp->AddGen(GTK_STOCK_OPEN,                "ctrl+o",       EDN_MSG__GUI_SHOW_OPEN_FILE, true);
		tmp->AddSeparator();
		tmp->AddGen("Close file",                  "ctrl+q",       EDN_MSG__CURRENT_CLOSE, true);
		tmp->AddSeparator();
		tmp->AddGen(GTK_STOCK_SAVE,                "ctrl+s",       EDN_MSG__CURRENT_SAVE, true);
		tmp->AddGen(GTK_STOCK_SAVE_AS,             "ctrl+shift+s", EDN_MSG__CURRENT_SAVE_AS, true);
		tmp->AddSeparator();
		tmp->AddGen(GTK_STOCK_QUIT,                "ctrl+shift+q", EDN_MSG__QUIT, true);
	m_listMenu.PushBack(tmp);
	
	tmp = new MenuBarMain("_Edit", m_mainWidget);
		tmp->AddGen(GTK_STOCK_UNDO,                "ctrl+z",       EDN_MSG__CURRENT_UNDO, true);
		tmp->AddGen(GTK_STOCK_REDO,                "ctrl+shift+z", EDN_MSG__CURRENT_REDO, true);
		tmp->AddSeparator();
		tmp->AddGen(GTK_STOCK_CUT,                 "ctrl+x",       EDN_MSG__CURRENT_CUT, true, COPY_STD);
		tmp->AddGen(GTK_STOCK_COPY,                "ctrl+c",       EDN_MSG__CURRENT_COPY, true, COPY_STD);
		tmp->AddGen(GTK_STOCK_PASTE,               "ctrl+v",       EDN_MSG__CURRENT_PASTE, true, COPY_STD);
//		tmp->AddGen(GTK_STOCK_DELETE,              NULL);
		tmp->AddGen("Remove line",                 "ctrl+w",       EDN_MSG__CURRENT_REMOVE_LINE, true);
		tmp->AddSeparator();
		tmp->AddGen(GTK_STOCK_SELECT_ALL,          "ctrl+a",       EDN_MSG__CURRENT_SELECT_ALL, true);
		tmp->AddGen("Unselect",                    "ctrl+shift+a", EDN_MSG__CURRENT_UN_SELECT, true);
		tmp->AddSeparator();
		tmp->AddGen("Goto Line number ...",        "ctrl+l",       EDN_MSG__GUI_SHOW_GOTO_LINE, true);
//		tmp->AddSeparator();
//		tmp->AddGen(GTK_STOCK_PREFERENCES,         NULL,           EDN_MSG__GUI_SHOW_PREFERENCE, true);
	m_listMenu.PushBack(tmp);
	
	tmp = new MenuBarMain("_Search", m_mainWidget);
		tmp->AddGen(GTK_STOCK_FIND,                 "ctrl+f",       EDN_MSG__GUI_SHOW_SEARCH, true);
		tmp->AddGen(GTK_STOCK_FIND_AND_REPLACE,     "ctrl+r",       EDN_MSG__GUI_SHOW_REPLACE, true);
		tmp->AddSeparator();
		tmp->AddGen("Find next",                    "ctrl+h",       EDN_MSG__CURRENT_FIND_NEXT, true);
		tmp->AddGen("Find previous",                "ctrl+shift+h", EDN_MSG__CURRENT_FIND_PREVIOUS, true);
//		tmp->AddGen("Find old next",                "ctrl+g",       EDN_MSG__CURRENT_FIND_OLD_NEXT, true);
//		tmp->AddGen("Find old previous",            "ctrl+shift+g", EDN_MSG__CURRENT_FIND_OLD_PREVIOUS, true);
//		tmp->AddSeparator();
//		tmp->AddGen("Suprimer colorisation",        NULL);
//		tmp->AddSeparator();
//		tmp->AddGen("Goto Line",                    "ctrl+l",       EDN_MSG__CURRENT_GOTO_LINE, true);
	m_listMenu.PushBack(tmp);
	
	tmp = new MenuBarMain("Display", m_mainWidget);
		tmp->AddInternal("Show space & tabs",                    NULL, MSG_TogleDisplayChar, true);
		tmp->AddInternal("Show end of lines",                    NULL, MSG_TogleDisplayEOL, true);
		tmp->AddInternal("Audo Indent",                          NULL, MSG_TogleAutoIndent, true);
		tmp->AddSeparator();
		tmp->AddInternal("Set charset Occidental (ISO-8859-1)",  NULL, MSG_SetCharsetIso559_1, true);
		tmp->AddInternal("Set charset Occidental (ISO-8859-15)", NULL, MSG_SetCharsetIso559_15, true);
		tmp->AddInternal("Set charset Internationnal (UTF 8)",   NULL, MSG_SetCharsetUTF8, true);
		tmp->AddSeparator();
		tmp->AddGen(     "Reload Color File",                    NULL, EDN_MSG__RELOAD_COLOR_FILE, true);
		tmp->AddSeparator();
		tmp->AddInternal("Set Color Black",                      NULL, MSG_LoadColorBlack, true);
		tmp->AddInternal("Set Color White",                      NULL, MSG_LoadColorWhite, true);
	m_listMenu.PushBack(tmp);
	/*
	tmp = new MenuBarMain("Project", m_mainWidget);
	m_listMenu.PushBack(tmp);
	*/
	tmp = new MenuBarMain("Ctags", m_mainWidget);
		tmp->AddGen("load Ctags file",              NULL,           EDN_MSG__OPEN_CTAGS, true);
		tmp->AddGen("re-load Ctags file",           NULL,           EDN_MSG__RELOAD_CTAGS, true);
		tmp->AddSeparator();
		tmp->AddGen("Find Definition",              "ctrl+d",       EDN_MSG__JUMP_TO_CURRENT_SELECTION, true);
		tmp->AddGen("Back previous",                "ctrl+shift+d", EDN_MSG__JUMP_BACK, true);
	/*
		tmp->AddGen("Gestion Ctags",                NULL);
		tmp->AddGen("Add Ctags Folder",             NULL);
		tmp->AddSeparator();
		tmp->AddGen("Trouver la definition",        NULL);
		tmp->AddGen("Trouver la declaration",       NULL);
		tmp->AddGen("Retour...",                    NULL);
		tmp->AddSeparator();
		tmp->AddGen("Tout desactiver",              NULL);
	*/
	m_listMenu.PushBack(tmp);
	/*
	tmp = new MenuBarMain("Macros", m_mainWidget);
	m_listMenu.PushBack(tmp);
	*/
	/*
	tmp = new MenuBarMain("Tools", m_mainWidget);
		tmp->AddGen("Execute Commande",             "ctrl+e");
		tmp->AddGen("Set Command",                  NULL);
	m_listMenu.PushBack(tmp);
	*/
	tmp = new MenuBarMain("Internal ClipBoard", m_mainWidget);
//		tmp->AddGen("Cut 1",                 "ctrl+alt+1",      EDN_MSG__CURRENT_CUT,   true, 1);
		tmp->AddGen("Copy 1",                "ctrl+1",       EDN_MSG__CURRENT_COPY,  true, 1);
		tmp->AddGen("Paste 1",               "alt+1",        EDN_MSG__CURRENT_PASTE, true, 1);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 2",                 "ctrl+alt+2",      EDN_MSG__CURRENT_CUT,   true, 2);
		tmp->AddGen("Copy 2",                "ctrl+2",       EDN_MSG__CURRENT_COPY,  true, 2);
		tmp->AddGen("Paste 2",               "alt+2",        EDN_MSG__CURRENT_PASTE, true, 2);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 3",                 "ctrl+alt+3",      EDN_MSG__CURRENT_CUT,   true, 3);
		tmp->AddGen("Copy 3",                "ctrl+3",       EDN_MSG__CURRENT_COPY,  true, 3);
		tmp->AddGen("Paste 3",               "alt+3",        EDN_MSG__CURRENT_PASTE, true, 3);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 4",                 "ctrl+alt+4",      EDN_MSG__CURRENT_CUT,   true, 4);
		tmp->AddGen("Copy 4",                "ctrl+4",       EDN_MSG__CURRENT_COPY,  true, 4);
		tmp->AddGen("Paste 4",               "alt+4",        EDN_MSG__CURRENT_PASTE, true, 4);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 5",                 "ctrl+alt+5",      EDN_MSG__CURRENT_CUT,   true, 5);
		tmp->AddGen("Copy 5",                "ctrl+5",       EDN_MSG__CURRENT_COPY,  true, 5);
		tmp->AddGen("Paste 5",               "alt+5",        EDN_MSG__CURRENT_PASTE, true, 5);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 6",                 "ctrl+alt+6",      EDN_MSG__CURRENT_CUT,   true, 6);
		tmp->AddGen("Copy 6",                "ctrl+6",       EDN_MSG__CURRENT_COPY,  true, 6);
		tmp->AddGen("Paste 6",               "alt+6",        EDN_MSG__CURRENT_PASTE, true, 6);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 7",                 "ctrl+alt+7",      EDN_MSG__CURRENT_CUT,   true, 7);
		tmp->AddGen("Copy 7",                "ctrl+7",       EDN_MSG__CURRENT_COPY,  true, 7);
		tmp->AddGen("Paste 7",               "alt+7",        EDN_MSG__CURRENT_PASTE, true, 7);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 8",                 "ctrl+alt+8",      EDN_MSG__CURRENT_CUT,   true, 8);
		tmp->AddGen("Copy 8",                "ctrl+8",       EDN_MSG__CURRENT_COPY,  true, 8);
		tmp->AddGen("Paste 8",               "alt+8",        EDN_MSG__CURRENT_PASTE, true, 8);
		tmp->AddSeparator();
//		tmp->AddGen("Cut 9",                 "ctrl+alt+9",      EDN_MSG__CURRENT_CUT,   true, 9);
		tmp->AddGen("Copy 9",                "ctrl+9",       EDN_MSG__CURRENT_COPY,  true, 9);
		tmp->AddGen("Paste 9",               "alt+9",        EDN_MSG__CURRENT_PASTE, true, 9);
	m_listMenu.PushBack(tmp);
	
	tmp = new MenuBarMain("Help", m_mainWidget);
		tmp->AddGen("help display",              NULL,           EDN_MSG__OPEN_CTAGS, false);
		tmp->AddSeparator();
		tmp->AddGen("About ...",                 NULL,       EDN_MSG__GUI_SHOW_ABOUT, true);
	/*
		tmp->AddGen("Gestion Ctags",                NULL);
		tmp->AddGen("Add Ctags Folder",             NULL);
		tmp->AddSeparator();
		tmp->AddGen("Trouver la definition",        NULL);
		tmp->AddGen("Trouver la declaration",       NULL);
		tmp->AddGen("Retour...",                    NULL);
		tmp->AddSeparator();
		tmp->AddGen("Tout desactiver",              NULL);
	*/
	m_listMenu.PushBack(tmp);
	
}

MenuBar::~MenuBar(void)
{
	
}

void MenuBar::OnMessage(int32_t id, int32_t dataID)
{
	//EDN_INFO("ReceiveMessage");
}


