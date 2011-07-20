/**
 *******************************************************************************
 * @file ToolBar.h
 * @brief Editeur De N'ours : abstraction of the tool bar (Header)
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

#include "MsgBroadcast.h"

#ifndef __TOOL_BAR_H__
#define __TOOL_BAR_H__


class ToolBar: public MsgBroadcast
{
	public:
		// Constructeur
		ToolBar(void);
		~ToolBar(void);
		GtkWidget	* GetWidget(void) { return m_mainWidget; };
		void          OnMessage(int32_t id, int32_t dataID);
	
	private:
		void AddSeparator(void);
		void Add(const char * title, const char * labelHelp, messageType_te id = EDN_MSG__NONE, bool enable = false);
		GtkWidget *                      m_mainWidget;
		EdnVectorBin<messageData_ts*>    m_message;
};


#endif
