/**
 *******************************************************************************
 * @file ToolBar.cpp
 * @brief Editeur De N'ours : abstraction of the tool bar (Sources)
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

#include <tools_debug.h>
#include <ToolBar.h>
#include <ClipBoard.h>

/*
static void CB_menuGenerique(GtkMenuItem *menu_item, gpointer data)
{
	//EDN_INFO("basic menue_event");
	messageData_ts * msg = (messageData_ts*)data;
	// broacast message : 
	GeneralSendMessage(msg->msgId, msg->dataId);
}
*/

ToolBar::ToolBar(void) : MsgBroadcast("Tool bar", EDN_CAT_GUI)
{
/*
	m_mainWidget = gtk_toolbar_new();
#	ifdef USE_GTK_VERSION_2_0
	gtk_toolbar_set_orientation(GTK_TOOLBAR(m_mainWidget), GTK_ORIENTATION_VERTICAL);
#	endif
	// Modification de la taille des icones
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(m_mainWidget), GTK_ICON_SIZE_BUTTON);
	// Affichage uniquement des icones
	gtk_toolbar_set_style(GTK_TOOLBAR(m_mainWidget), GTK_TOOLBAR_ICONS);
	
	Add(GTK_STOCK_NEW, "Nouveau",              EDN_MSG__NEW);
	Add(GTK_STOCK_OPEN, "Ouvrir",              EDN_MSG__GUI_SHOW_OPEN_FILE);
	AddSeparator();
	Add(GTK_STOCK_SAVE, "Enregistrer",         EDN_MSG__CURRENT_SAVE);
	Add(GTK_STOCK_SAVE_AS, "Enregistrer sous", EDN_MSG__CURRENT_SAVE_AS);
	AddSeparator();
	Add(GTK_STOCK_QUIT, "Quitter");
*/
}

ToolBar::~ToolBar(void)
{
	
}

void ToolBar::OnMessage(int32_t id, int32_t dataID)
{
	//EDN_INFO("ReceiveMessage");
}
/*
void ToolBar::AddSeparator(void)
{
#	ifdef USE_GTK_VERSION_3_0
#	elif defined( USE_GTK_VERSION_2_0)
	gtk_toolbar_append_space(GTK_TOOLBAR(m_mainWidget));
#	endif
}


void ToolBar::Add(const char * title, const char * labelHelp, messageType_te id, bool enable)
{
	messageData_ts * message = new messageData_ts;
	message->msgId      = id;
	message->dataId     = -1;
	m_message.PushBack(message);
#	ifdef USE_GTK_VERSION_3_0
#	elif defined( USE_GTK_VERSION_2_0)
	gtk_toolbar_insert_stock(GTK_TOOLBAR(m_mainWidget), title, labelHelp, NULL, G_CALLBACK(CB_menuGenerique), message, -1);
#	endif
}
*/



