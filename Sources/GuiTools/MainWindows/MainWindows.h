/**
 *******************************************************************************
 * @file MainWindows.h
 * @brief Editeur De N'ours : main Windows diplayer (header)
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
#include "Singleton.h"
#include "MsgBroadcast.h"

#include "CodeView.h"
#include "BufferView.h"
#include "BufferManager.h"
#include "MenuBar.h"
#include "StatusBar.h"
#include "ToolBar.h"


#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__

class MainWindows: public Singleton<MainWindows>, public MsgBroadcast
{
	friend class Singleton<MainWindows>;
	// specific for sigleton system...
	private:
		// Constructeur
		MainWindows(void);
		~MainWindows(void);
	public:
		GtkWidget *     GetWidget(void) { return m_mainWindow;};
		void            OnMessage(int32_t id, int32_t dataID);
		static bool     OnQuit(GtkWidget *widget, gpointer data);
	
	private:
		void            SetTitle(Edn::File &fileName, bool isModify);
		void            SetNoTitle(void);
		// main windows widget : 
		GtkWidget *     m_mainWindow;
		BufferView      m_BufferView;
		CodeView        m_CodeView;
		MenuBar         m_MenuBar;
		StatusBar       m_StatusBar;
		ToolBar         m_ToolBar;
};

#endif


