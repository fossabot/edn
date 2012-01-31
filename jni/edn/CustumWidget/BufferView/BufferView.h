/**
 *******************************************************************************
 * @file BufferView.h
 * @brief Editeur De N'ours : main List Buffer Viewer (header)
 * @author Edouard DUPIN
 * @date 09/12/2010
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
#ifndef __BUFFER_VIEW_H__
#define __BUFFER_VIEW_H__

#include <tools_debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <Display.h>
#include <MsgBroadcast.h>



class BufferView : public ewol::Widget
{
	public:
		// Constructeur
		BufferView(void);
		~BufferView(void);
		//GtkWidget	*GetMainWidget(void);
		bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
		/*
		// sur : GTK+ callback :
		static gboolean	CB_displayDraw( GtkWidget *widget, GdkEventExpose *event, gpointer data);
		static gboolean	CB_displayInit( GtkWidget *widget, gpointer data);
		static gint		CB_focusGet( GtkWidget *widget, GdkEventFocus *event, gpointer data);
		static gint		CB_focusLost( GtkWidget *widget, GdkEventFocus *event, gpointer data);
		static gint		CB_keyboardEvent( GtkWidget *widget, GdkEventKey *event, gpointer data);
		static gint		CB_mouseButtonEvent(GtkWidget *widget, GdkEventButton *event, gpointer data);
		static gint		CB_mouseMotionEvent( GtkWidget *widget, GdkEventMotion *event, gpointer data);
		static void		CB_EventOnBufferManager(gpointer data);

		static void     OnPopupEventShow(GtkWidget *menuitem, gpointer data);
		static void     OnPopupEventClose(GtkWidget *menuitem, gpointer data);
		static void     OnPopupEventSave(GtkWidget *menuitem, gpointer data);
		static void     OnPopupEventSaveAs(GtkWidget *menuitem, gpointer data);
		*/
	
	private:
		//void        ViewPopupMenu(GtkWidget *parrent, GdkEventButton *event, int32_t BufferID);
		// main windows widget : 
		//GtkWidget *           m_widget;
		// récupération des proprieter général...
		BufferManager *       m_bufferManager;
		ColorizeManager *     m_colorManager;
		int32_t               m_shawableAreaX;
		int32_t               m_shawableAreaY;
		int32_t               m_selectedID;
		int32_t               m_contectMenuSelectID;
};


#endif


