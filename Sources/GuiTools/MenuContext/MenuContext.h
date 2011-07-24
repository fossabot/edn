/**
 *******************************************************************************
 * @file MenuContext.h
 * @brief Editeur De N'ours : special Menu (left button or normal menu) (header)
 * @author Edouard DUPIN
 * @date 22/07/2011
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
 
#ifndef __MENU_CONTEXT_H__
#define __MENU_CONTEXT_H__

#include "Singleton.h"
#include "MsgBroadcast.h"
#include "Edn.h"
#include "BufferManager.h"
#include "ColorizeManager.h"

class MenuContext: public Singleton<MenuContext>, public MsgBroadcast
{
	friend class Singleton<MenuContext>;
	// specific for sigleton system...
	private:
		// Constructeur
		MenuContext(void);
		~MenuContext(void);

	public:
		void    OnMessage(int32_t id, int32_t dataID);
		// sur : GTK+ callback :
		static gboolean CB_displayDraw( GtkWidget *widget, GdkEventExpose *event, gpointer data);
		static gboolean CB_displayDraw2( GtkWidget *widget, GdkEventExpose *event, gpointer data);
		static gboolean CB_displayInit( GtkWidget *widget, gpointer data);
		static gint     CB_focusGet( GtkWidget *widget, GdkEventFocus *event, gpointer data);
		static gint     CB_focusLost( GtkWidget *widget, GdkEventFocus *event, gpointer data);
		static gint     CB_keyboardEvent( GtkWidget *widget, GdkEventKey *event, gpointer data);
		static gint     CB_mouseButtonEvent(GtkWidget *widget, GdkEventButton *event, gpointer data);
		static gint     CB_mouseMotionEvent( GtkWidget *widget, GdkEventMotion *event, gpointer data);
		static void     CB_EventOnBufferManager(gpointer data);
		static void     CB_ScreenChange(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata);
	public:
		void    Clear(void);
		void    AddCommonElem(int32_t id);
		void    AddSpecificElem(Edn::String &text);
		void    Show(int32_t x, int32_t y, bool top);
		void    Hide(void);
	private:
		BufferManager *       m_bufferManager;
		ColorizeManager *     m_colorManager;
		GtkWidget *     m_dialog;
		GtkWidget *     m_widget;
		position_ts     m_requestedPos;
		position_ts     m_shawableSize;
};

#endif