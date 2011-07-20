/**
 *******************************************************************************
 * @file MenuBar.h
 * @brief Editeur De N'ours : abstraction of the menu bar (Header)
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
#include "tools_globals.h"
#include "MsgBroadcast.h"
#include "EdnVectorBin.h"
#include "AccelKey.h"

#ifndef __MENU_BAR_H__
#define __MENU_BAR_H__

class MenuBarMain;

class MenuBar: public MsgBroadcast
{
	public:
		// Constructeur
		MenuBar(void);
		~MenuBar(void);
		GtkWidget	* GetWidget(void) { return m_mainWidget; };
		void          OnMessage(int32_t id, int32_t dataID);
	
	private:
		GtkWidget *                     m_mainWidget;
		GtkAccelGroup *                 m_accelGroup;
		EdnVectorBin<MenuBarMain*>      m_listMenu;
};


#endif
