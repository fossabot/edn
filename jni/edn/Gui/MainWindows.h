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


#include <tools_debug.h>
#include <MsgBroadcast.h>

#include <CodeView.h>
#include <BufferView.h>
#include <BufferManager.h>


#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__
class MainWindows : public ewol::Windows
{
	private:
		int32_t m_currentSavingAsIdBuffer;
	public:
		// Constructeur
		MainWindows(void);
		~MainWindows(void);
		virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y);
};
#endif


