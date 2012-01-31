/**
 *******************************************************************************
 * @file StatusBar.cpp
 * @brief Editeur De N'ours : abstraction of the status bar (Sources)
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
#include <StatusBar.h>



StatusBar::StatusBar(void)
{
/*
	m_mainWidget = gtk_statusbar_new();
	// pas très bien compris pourquoi mais ca marche...
	m_iContextId = gtk_statusbar_get_context_id(GTK_STATUSBAR(m_mainWidget), "ExitMsg");
	// ajout d'un message :
	gtk_statusbar_push(GTK_STATUSBAR (m_mainWidget), GPOINTER_TO_INT(m_iContextId), "Edn Editeur de n'ours");
	// Supression du précédent message :
	//gtk_statusbar_pop(GTK_STATUSBAR(m_mainWidget), GPOINTER_TO_INT(m_iContextId));
*/
}

StatusBar::~StatusBar(void)
{
	
}

void StatusBar::OnMessage(int32_t id, int32_t dataID)
{
	//EDN_INFO("ReceiveMessage");
}


