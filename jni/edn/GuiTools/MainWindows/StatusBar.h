/**
 *******************************************************************************
 * @file StatusBar.h
 * @brief Editeur De N'ours : abstraction of the status bar (Header)
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

#include <MsgBroadcast.h>

#ifndef __STATUS_BAR_H__
#define __STATUS_BAR_H__

class StatusBar: public MsgBroadcast
{
	public:
		// Constructeur
		StatusBar(void);
		~StatusBar(void);
		//GtkWidget	* GetWidget(void) { return m_mainWidget; };
		void          OnMessage(int32_t id, int32_t dataID);
	
	private:
		//GtkWidget *     m_mainWidget;
		//guint           m_iContextId;
};


#endif
