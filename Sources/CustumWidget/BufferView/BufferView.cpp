/**
 *******************************************************************************
 * @file BufferViewer.cpp
 * @brief Editeur De N'ours : main textViewer diplayer
 * @author Edouard DUPIN
 * @date 04/12/2010
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
#include "Display.h"
#include "BufferView.h"
#include "BufferManager.h"
#include "ColorizeManager.h"
#include "MainWindows.h"

#undef __class__
#define __class__	"BufferView"

BufferView::BufferView(void) : MsgBroadcast("Buffer View", EDN_CAT_GUI)
{
	m_shawableAreaX = 0;
	m_shawableAreaY = 0;
	// Init link with the buffer Manager
	m_bufferManager = BufferManager::getInstance();
	m_colorManager = ColorizeManager::getInstance();
	m_menuContext = MenuContext::getInstance();

	m_widget = gtk_drawing_area_new();
	gtk_widget_set_size_request( m_widget, 250, 100);

	gtk_widget_add_events(	m_widget, 
								GDK_KEY_PRESS_MASK
							|	GDK_BUTTON_PRESS_MASK
							|	GDK_BUTTON_RELEASE_MASK
							|	GDK_POINTER_MOTION_MASK
							|	GDK_POINTER_MOTION_HINT_MASK);

#	ifdef USE_GTK_VERSION_3_0
	g_object_set(m_widget,"can-focus", TRUE, NULL);
#	elif defined( USE_GTK_VERSION_2_0 )
	GTK_WIDGET_SET_FLAGS(m_widget, GTK_CAN_FOCUS);
#	endif
	// Focus Event
	g_signal_connect(		G_OBJECT(m_widget), "focus_in_event",		G_CALLBACK(CB_focusGet),         this);
	g_signal_connect(		G_OBJECT(m_widget), "focus_out_event",		G_CALLBACK(CB_focusLost),        this);
	// Keyboard Event
	g_signal_connect_after(	G_OBJECT(m_widget), "key_press_event",		G_CALLBACK(CB_keyboardEvent),    this);
	g_signal_connect_after(	G_OBJECT(m_widget), "key_release_event",	G_CALLBACK(CB_keyboardEvent),    this);
	// Mouse Event
	g_signal_connect(		G_OBJECT(m_widget), "button_press_event",	G_CALLBACK(CB_mouseButtonEvent), this);
	g_signal_connect(		G_OBJECT(m_widget), "button_release_event",	G_CALLBACK(CB_mouseButtonEvent), this);
	g_signal_connect(		G_OBJECT(m_widget), "motion_notify_event",	G_CALLBACK(CB_mouseMotionEvent), this);
	// Display Event
	g_signal_connect(		G_OBJECT(m_widget), "realize",				G_CALLBACK(CB_displayInit),      this);
#	ifdef USE_GTK_VERSION_3_0
	g_signal_connect(		G_OBJECT(m_widget), "draw",					G_CALLBACK(CB_displayDraw),		this);
#	elif defined( USE_GTK_VERSION_2_0 )
	g_signal_connect(		G_OBJECT(m_widget), "expose_event",			G_CALLBACK(CB_displayDraw),		this);
#	endif
	m_selectedID = -1;

}

BufferView::~BufferView(void)
{

}

GtkWidget * BufferView::GetMainWidget(void)
{
	return m_widget;
}

void BufferView::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		case EDN_MSG__BUFFER_CHANGE_CURRENT:
			m_selectedID = dataID;
		case EDN_MSG__BUFFER_CHANGE_STATE:
		case EDN_MSG__BUFFER_CHANGE_NAME:
		case EDN_MSG__BUFFER_CHANGE_MODIFY:
			// change Title :
			gtk_widget_queue_draw(m_widget);
			break;
	}
}


gboolean BufferView::CB_displayDraw( GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	BufferView * self = reinterpret_cast<BufferView*>(data);

#	ifdef USE_GTK_VERSION_3_0
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	bool needRedrawAll = false;
	if (self->m_shawableAreaX != allocation.width) {
		needRedrawAll = true;
		self->m_shawableAreaX = allocation.width;
	}
	if (self->m_shawableAreaY != allocation.height) {
		needRedrawAll = true;
		self->m_shawableAreaY = allocation.height;
	}
#	elif defined( USE_GTK_VERSION_2_0)
	bool needRedrawAll = false;
	if (self->m_shawableAreaX != widget->allocation.width) {
		needRedrawAll = true;
		self->m_shawableAreaX = widget->allocation.width;
	}
	if (self->m_shawableAreaY != widget->allocation.height) {
		needRedrawAll = true;
		self->m_shawableAreaY = widget->allocation.height;
	}
#endif
	if (true == needRedrawAll) {
		//myBuffer->ForceReDraw(true);
	}
	EDN_INFO("==========================================================================");
	EDN_INFO("Request a display of : " << self->m_shawableAreaX << "px * "<<  self->m_shawableAreaY<<"px");
	DrawerManager monDrawer(widget, self->m_shawableAreaX, self->m_shawableAreaY);
	
	// get the number of buffer open
	int32_t nbBufferOpen = self->m_bufferManager->Size();
	int32_t i;
	uint32_t lineID = 0;
	uint32_t fontHeight = Display::GetFontHeight();
	basicColor_te selectFG = COLOR_LIST_TEXT_NORMAL;
	basicColor_te selectBG = COLOR_LIST_BG_1;
	for (i=0; i < nbBufferOpen; i++) {
		Edn::String name;
		bool isModify;
		if (self->m_bufferManager->Exist(i)) {
			isModify = self->m_bufferManager->Get(i)->IsModify();
			name = self->m_bufferManager->Get(i)->GetShortName();
			char *tmpModify = (char*)" ";
			if (true == isModify) {
				tmpModify = (char*)"M";
			}
			char name2[1024] = "";
			sprintf(name2, "[%2d](%s) %s", i, tmpModify, name.c_str() );
			
			if (true == isModify) {
				selectFG = COLOR_LIST_TEXT_MODIFY;
			} else {
				selectFG = COLOR_LIST_TEXT_NORMAL;
			}
			if (lineID%2==0) {
				selectBG = COLOR_LIST_BG_1;
			} else {
				selectBG = COLOR_LIST_BG_2;
			}
			if (self->m_selectedID == i) {
				selectBG = COLOR_LIST_BG_SELECTED;
			}
			EDN_INFO("color fg=" << selectFG << " bg="<<  selectBG);
			monDrawer.Rectangle(self->m_colorManager->Get(selectBG), 0, lineID*fontHeight, self->m_shawableAreaX, Display::GetFontHeight());
			
			monDrawer.Text(self->m_colorManager->Get(selectFG), 2, lineID*fontHeight, name2);
			monDrawer.Flush();
			lineID ++;
		}
		
	}
	return TRUE;

}




// sur : émis lors du premier affichage de la GtkDrawingArea
gboolean BufferView::CB_displayInit( GtkWidget *widget, gpointer data)
{
	BufferView * self = reinterpret_cast<BufferView*>(data);
	
#	ifdef USE_GTK_VERSION_3_0
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	int32_t size_x = allocation.width;
	int32_t size_y = allocation.height;
	
	self->m_shawableAreaX = allocation.width;
	self->m_shawableAreaY = allocation.height;
#	elif defined( USE_GTK_VERSION_2_0)
	int32_t size_x = widget->allocation.width;
	int32_t size_y = widget->allocation.height;
	
	self->m_shawableAreaX = widget->allocation.width;
	self->m_shawableAreaY = widget->allocation.height;
#	endif
	EDN_INFO("Request a diplay of : " << size_x << "px * " << size_y << "px");

	//Display::InitDisplayParam(self->m_displayParameters, widget, 700, 1200);

	gtk_widget_queue_draw( widget );
	return TRUE;
}

gint BufferView::CB_focusGet(	GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	//BufferView * self = reinterpret_cast<BufferView*>(data);
	
#	ifdef USE_GTK_VERSION_2_0
	GTK_WIDGET_SET_FLAGS (widget, GTK_HAS_FOCUS);
#	endif
	EDN_INFO("Focus - In");
	gtk_widget_queue_draw( widget );
	return FALSE;
}

gint BufferView::CB_focusLost(	GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	BufferView * self = reinterpret_cast<BufferView*>(data);
	
#	ifdef USE_GTK_VERSION_2_0
	GTK_WIDGET_UNSET_FLAGS (widget, GTK_HAS_FOCUS);
#	endif
	EDN_INFO("Focus - out");
	self->m_menuContext->Hide();
	return FALSE;
}

gint BufferView::CB_keyboardEvent(	GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	//BufferView * self = reinterpret_cast<BufferView*>(data);
	
	if(event->type == GDK_KEY_PRESS) {
		gtk_widget_queue_draw( widget );
	}
	return true;
}


gint BufferView::CB_mouseButtonEvent(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	BufferView * self = reinterpret_cast<BufferView*>(data);
	if (event->type != GDK_BUTTON_RELEASE) {
		self->m_menuContext->Hide();
	}
	// get focus on the widget
	gtk_widget_grab_focus(widget);
	if (event->button == 1) {
		if (event->type == GDK_BUTTON_PRESS) {
			EDN_INFO("mouse-event BT1  ==> One Clicked");
		}else if (event->type == GDK_2BUTTON_PRESS) {
			//EDN_INFO("mouse-event BT1  ==> Double Clicked %d, %d", (uint32_t)event->x, (uint32_t)event->y);
			uint32_t fontHeight = Display::GetFontHeight();
			int32_t selectBuf = self->m_bufferManager->WitchBuffer((event->y / fontHeight) + 1);
			//EDN_INFO(" plop %d / %d = %d ==> %d", (uint32_t)event->y, fontHeight, ((uint32_t)event->y / fontHeight), selectBuf);
			if ( 0 <= selectBuf) {
				self->SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, selectBuf);
				/*
				MainWindows *window = MainWindows::getInstance();
				EDN_INFO(" Event on Buffer " << selectBuf);
				// set the new seected Buffer
				window->SetSelected(selectBuf);
				*/
			}
		}/* else if (event->type == GDK_3BUTTON_PRESS) {
			EDN_INFO("mouse-event BT1  ==> Triple Clicked");
		}else if (event->type == GDK_BUTTON_RELEASE) {
			EDN_INFO("mouse-event BT1  ==> Realease");
		}*/
	} else if (event->button == 2) {
		/*
		if (event->type == GDK_BUTTON_PRESS) {
			EDN_INFO("mouse-event BT2 PRESS");
			self->m_menuContext->Show(event->x, event->y, false);
		}
		*/
	} else if (event->button == 3) {
		if (event->type == GDK_BUTTON_PRESS) {
			EDN_INFO("mouse-event BT3 PRESS");
			uint32_t fontHeight = Display::GetFontHeight();
			int32_t selectBuf = self->m_bufferManager->WitchBuffer((event->y / fontHeight) + 1);
			if ( 0 <= selectBuf) {
				// TODO : Find a simple methode
				int32_t windowsPosX, windowsPosY;
				gtk_window_get_position(GTK_WINDOW(gtk_widget_get_toplevel(widget)), &windowsPosX, &windowsPosY);
				//EDN_INFO("windowsPosX=" << windowsPosX << " windowsPosY=" << windowsPosY);
				int32_t widgetPosX, widgetPosY;
				gtk_widget_translate_coordinates(widget, gtk_widget_get_toplevel(widget), 0, 0, &widgetPosX, &widgetPosY);
				//EDN_INFO("widgetPosX=" << widgetPosX << " widgetPosY=" << widgetPosY);
				self->m_menuContext->Show(self->m_shawableAreaX+2+widgetPosX+windowsPosX, ((int32_t)(event->y / fontHeight)*fontHeight)+(fontHeight/2)+widgetPosY+windowsPosY, false);
			} else {
				self->m_menuContext->Hide();
			}
		}
	} else {
		EDN_INFO("mouse-event BT? PRESS");
	}
	gtk_widget_queue_draw( widget );
	return true;
}



gint BufferView::CB_mouseMotionEvent( GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	//BufferView * self = reinterpret_cast<BufferView*>(data);
	
/*
	if (true == ButtunOneSelected) {
		int x, y;
		GdkModifierType state;

		if (event->is_hint) {
			gdk_window_get_pointer(event->window, &x, &y, &state);
		} else {
			x = event->x;
			y = event->y;
			state = (GdkModifierType)event->state;
		}
		EDN_INFO("mouse-motion BT1 %d, %d", x, y);
	}
*/
	return true;
}


