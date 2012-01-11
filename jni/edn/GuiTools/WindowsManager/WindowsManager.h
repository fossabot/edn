/**
 *******************************************************************************
 * @file WindowsManager.h
 * @brief Editeur De N'ours : Windows creation, pop up, destruction ... (header)
 * @author Edouard DUPIN
 * @date 20/06/2011
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
 
#ifndef __WINDOWS_MANAGER_H__
#define __WINDOWS_MANAGER_H__

#include <etk/Singleton.h>
#include <MsgBroadcast.h>
#include <MainWindows.h>

class WindowsManager: public etk::Singleton<WindowsManager>, public MsgBroadcast
{
	friend class etk::Singleton<WindowsManager>;
	// specific for sigleton system...
	private:
		// Constructeur
		WindowsManager(void);
		~WindowsManager(void);

	public:
		void    OnMessage(int32_t id, int32_t dataID);
	private:
		MainWindows * m_mainWindow;
		int32_t       m_currentBufferID;
};

#endif

