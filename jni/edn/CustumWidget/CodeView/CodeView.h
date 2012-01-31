/**
 *******************************************************************************
 * @file CodeView.h
 * @brief Editeur De N'ours : Code Viewer Widget (header)
 * @author Edouard DUPIN
 * @date 05/01/2011
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

#ifndef __CODE_VIEW_H__
#define __CODE_VIEW_H__

#include <tools_debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <Display.h>
#include <MsgBroadcast.h>

#include <etk/Types.h>
#include <ewol/Widget.h>

class CodeView :public ewol::Widget
{
	public:
		         CodeView(void);
		virtual ~CodeView(void);
		virtual bool   CalculateMinSize(void);
	private:
		etk::String    m_label;
		color_ts       m_textColorFg;  //!< Text color
		color_ts       m_textColorBg;  //!< Background color
		BufferManager *     m_bufferManager;
		ColorizeManager *   m_colorManager;
		int32_t             m_bufferID;
		bool                m_buttunOneSelected;
	public:
		virtual void   OnRegenerateDisplay(void);
		bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
	public:
		virtual bool OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
		virtual bool OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y);
		virtual bool OnEventKb(ewol::eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE]);
		virtual bool OnEventKbMove(ewol::eventKbType_te typeEvent, ewol::eventKbMoveType_te moveTypeEvent);
		virtual void OnGetFocus(void);
		virtual void OnLostFocus(void);
};



#if 0
class CodeView : public MsgBroadcast
{
	public:
		// Constructeur
		CodeView(void);
		~CodeView(void);
		void OnMessage(int32_t id, int32_t dataID);
		/*
		GtkWidget	*GetMainWidget(void);
		// sur : GTK+ callback :
		static gboolean		CB_displayDraw( GtkWidget *widget, GdkEventExpose *event, gpointer data);
		static gboolean		CB_displayInit( GtkWidget *widget, gpointer data);
		static gint			CB_focusGet( GtkWidget *widget, GdkEventFocus *event, gpointer data);
		static gint			CB_focusLost( GtkWidget *widget, GdkEventFocus *event, gpointer data);
		static gint			CB_keyboardEvent( GtkWidget *widget, GdkEventKey *event, gpointer data);
		static gint			CB_mouseButtonEvent(GtkWidget *widget, GdkEventButton *event, gpointer data);
		static gint			CB_mouseMotionEvent( GtkWidget *widget, GdkEventMotion *event, gpointer data);
		static gint			CB_mouseScrollEvent( GtkWidget *widget, GdkEventScroll *event, gpointer data);
		*/

	private:
		// main windows widget : 
		//GtkWidget *         m_widget;
		// récupération des proprieter général...
		BufferManager *     m_bufferManager;
		ColorizeManager *   m_colorManager;
		int32_t             m_shawableAreaX;
		int32_t             m_shawableAreaY;
		int32_t             m_bufferID;
		bool                m_buttunOneSelected;
};

#endif

#endif


