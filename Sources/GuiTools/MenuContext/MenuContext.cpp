/**
 *******************************************************************************
 * @file MenuContext.cpp
 * @brief Editeur De N'ours : special Menu (left button or normal menu) (Sources)
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


#include "tools_debug.h"
#include "tools_globals.h"
#include "MenuContext.h"


#undef  __class__
#define __class__    "MenuContext"

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
MenuContext::MenuContext(void) : MsgBroadcast("Menu Context", EDN_CAT_MENU_CONTEXT)
{
	//m_currentBufferID = -1;
	m_dialog = NULL;
	m_shawableSize.x=200;
	m_shawableSize.y=200;
	// Init link with the buffer Manager
	m_bufferManager = BufferManager::getInstance();
	m_colorManager = ColorizeManager::getInstance();
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
MenuContext::~MenuContext(void)
{
	if(NULL != m_dialog) {
		gtk_widget_destroy(m_dialog);
		m_dialog = NULL;
	}
}

void MenuContext::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		/*
		case EDN_MSG__BUFFER_CHANGE_CURRENT:
			m_currentBufferID = dataID;
			break;
		*/
	}
}



void MenuContext::Clear(void)
{
	
}

void MenuContext::AddCommonElem(int32_t id)
{
	
}

void MenuContext::AddSpecificElem(Edn::String &text)
{
	
}
bool supports_alpha = false;

void MenuContext::CB_ScreenChange(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
/*
  // To check if the display supports alpha channels, get the colormap 
  GdkScreen *screen = NULL;
  GdkColormap *colormap = NULL;

  screen = gtk_widget_get_screen(widget);
  colormap = gdk_screen_get_rgba_colormap (screen);
  if (colormap == NULL)
  {
    EDN_WARNING("Your screen does not support alpha channels!\n");
    colormap = gdk_screen_get_rgb_colormap(screen);
    supports_alpha = false;
  }
  else
  {
    EDN_WARNING("Your screen supports alpha channels!\n");
    supports_alpha = true;
  }

  /* Now we have a colormap appropriate for the screen, use it 
  gtk_widget_set_colormap (widget, colormap);
  */
}


/* This is called when we need to draw the windows contents */
static gboolean expose (GtkWidget *widget, GdkEventExpose *event, gpointer userdata)
{
  gint width;
  gint height;
  cairo_t *cr = NULL;


#	if USE_GTK_VERSION_3_0
	cr = gdk_cairo_create(gtk_widget_get_window(widget));
#	elif USE_GTK_VERSION_2_0
	cr = gdk_cairo_create(widget->window);
#	endif
  //if (supports_alpha)
  {
    // transparent
    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0);
  }
  /*
  else
  {
    // opaque white
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
  }
  */

  /* draw the background */
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);

  /* draw a circle */
  //gtk_window_get_size (GTK_WINDOW (widget), &width, &height);

  //cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
  //cairo_arc (cr, width / 2, height / 2, (width < height ? width : height) / 2 - 8 , 0, 2 * 3.14);
  //cairo_fill (cr);
  //cairo_stroke (cr);
  //cairo_paint (cr);
  
  cairo_destroy (cr);
  return FALSE;
}



// http://zetcode.com/tutorials/cairographicstutorial/transparency/

void MenuContext::Show(int32_t x, int32_t y, bool top)
{
	if(NULL != m_dialog) {
		gtk_widget_destroy(m_dialog);
		m_dialog = NULL;
	}
	m_dialog = gtk_window_new(GTK_WINDOW_POPUP);
	//m_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// Set the dialog on top of the selected windows
	//gtk_window_set_transient_for(GTK_WINDOW(m_dialog), parent);
	// unset the border of the windows
	//gtk_window_set_decorated(GTK_WINDOW(m_dialog), FALSE);
	
	EDN_INFO("Set position X=" << x+2 << " Y=" << y+27);
	
#	if USE_GTK_VERSION_3_0
	gtk_window_move(GTK_WINDOW(m_dialog), x+2, y+27);
#	elif USE_GTK_VERSION_2_0
	gtk_widget_set_uposition(m_dialog, x+2, y+27);
#	endif
	
//#if 1
	
	/* Tell GTK+ that we want to draw the windows background ourself.
	 * If we don't do this then GTK+ will clear the window to the
	 * opaque theme default color, which isn't what we want.
	 */
	gtk_widget_set_app_paintable (m_dialog, TRUE);
	/* We need to handle two events ourself: "expose-event" and "screen-changed".
	 *
	 * The X server sends us an expose event when the window becomes
	 * visible on screen. It means we need to draw the contents.  On a
	 * composited desktop expose is normally only sent when the window
	 * is put on the screen. On a non-composited desktop it can be
	 * sent whenever the window is uncovered by another.
	 *
	 * The screen-changed event means the display to which we are
	 * drawing changed. GTK+ supports migration of running
	 * applications between X servers, which might not support the
	 * same features, so we need to check each time.
	 */
#	ifdef USE_GTK_VERSION_3_0
	g_signal_connect(       G_OBJECT(m_dialog), "draw",					G_CALLBACK(expose),		this);
#	elif defined( USE_GTK_VERSION_2_0 )
	g_signal_connect(       G_OBJECT(m_dialog), "expose_event",			G_CALLBACK(expose),		this);
#	endif
	//g_signal_connect (G_OBJECT (m_dialog), "screen-changed", G_CALLBACK (CB_ScreenChange), NULL);

	
	
	//gtk_window_set_opacity(GTK_WINDOW(m_dialog), 0.25);
	
	//CB_ScreenChange(m_dialog, NULL, NULL);
	
//#else

	//gtk_window_set_opacity(GTK_WINDOW(m_dialog), 0.25);
	


	// enable the close signal of the windows 
	//g_signal_connect(G_OBJECT(m_mainWindow), "delete-event", G_CALLBACK(OnQuit), this);
	//g_signal_connect(G_OBJECT(m_mainWindow), "destroy", G_CALLBACK(OnQuit), this);
/*
	// Create a vertical box for stacking the menu and editor widgets in.
	GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add(GTK_CONTAINER(m_dialog), vbox);
	
	GtkWidget *myLabel =  gtk_label_new("plop 1 ");
	gtk_container_add(GTK_CONTAINER(vbox), myLabel);
	
	myLabel =  gtk_label_new("plop 2 ");
	gtk_container_add(GTK_CONTAINER(vbox), myLabel);
*/
	// Set key Accelerator :
	//AccelKey::getInstance()->LinkCommonAccel(GTK_WINDOW(m_dialog));

	// Create the menu bar.
	//gtk_box_pack_start(	GTK_BOX (vbox), m_MenuBar.GetWidget(), FALSE, FALSE, 0);
	m_widget = gtk_drawing_area_new();
	//gtk_window_set_opacity(GTK_WINDOW(m_widget), 0.8);
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
	g_signal_connect(       G_OBJECT(m_widget), "focus_in_event",		G_CALLBACK(CB_focusGet),         this);
	g_signal_connect(       G_OBJECT(m_widget), "focus_out_event",		G_CALLBACK(CB_focusLost),        this);
	// Keyboard Event
	g_signal_connect_after( G_OBJECT(m_widget), "key_press_event",		G_CALLBACK(CB_keyboardEvent),    this);
	g_signal_connect_after( G_OBJECT(m_widget), "key_release_event",	G_CALLBACK(CB_keyboardEvent),    this);
	// Mouse Event
	g_signal_connect(       G_OBJECT(m_widget), "button_press_event",	G_CALLBACK(CB_mouseButtonEvent), this);
	g_signal_connect(       G_OBJECT(m_widget), "button_release_event",	G_CALLBACK(CB_mouseButtonEvent), this);
	g_signal_connect(       G_OBJECT(m_widget), "motion_notify_event",	G_CALLBACK(CB_mouseMotionEvent), this);
	// Display Event
	g_signal_connect(       G_OBJECT(m_widget), "realize",				G_CALLBACK(CB_displayInit),      this);
#	ifdef USE_GTK_VERSION_3_0
	g_signal_connect(       G_OBJECT(m_widget), "draw",					G_CALLBACK(CB_displayDraw2),		this);
#	elif defined( USE_GTK_VERSION_2_0 )
	g_signal_connect(       G_OBJECT(m_widget), "expose_event",			G_CALLBACK(CB_displayDraw2),		this);
#	endif
	//g_signal_connect (      G_OBJECT(m_widget), "screen-changed",       G_CALLBACK(CB_ScreenChange), NULL);


	gtk_container_add(GTK_CONTAINER(m_dialog), m_widget);

//#endif
	// recursive version of gtk_widget_show
	gtk_widget_show_all(m_dialog); 


}

void MenuContext::Hide(void)
{
	
}


gboolean MenuContext::CB_displayDraw( GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	MenuContext * self = reinterpret_cast<MenuContext*>(data);

#	ifdef USE_GTK_VERSION_3_0
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	if (self->m_shawableSize.x != allocation.width) {
		self->m_shawableSize.x = allocation.width;
	}
	if (self->m_shawableSize.y != allocation.height) {
		self->m_shawableSize.y = allocation.height;
	}
#	elif defined( USE_GTK_VERSION_2_0)
	if (self->m_shawableSize.x != widget->allocation.width) {
		self->m_shawableSize.x = widget->allocation.width;
	}
	if (self->m_shawableSize.y != widget->allocation.height) {
		self->m_shawableSize.y = widget->allocation.height;
	}
#	endif
	EDN_INFO("Request a display of : " << self->m_shawableSize.x << "px * "<<  self->m_shawableSize.y<<"px");

	DrawerManager monDrawer(widget, self->m_shawableSize.x, self->m_shawableSize.y);
	
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
			EDN_INFO("color fg=" << selectFG << " bg="<<  selectBG);
			//monDrawer.Rectangle(self->m_colorManager->Get(selectBG), 0, lineID*fontHeight, self->m_shawableSize.x, Display::GetFontHeight());
			
			monDrawer.Text(self->m_colorManager->Get(selectFG), 2, lineID*fontHeight, name2);
			monDrawer.Flush();
			lineID ++;
		}
	}
	return TRUE;

}




gboolean MenuContext::CB_displayDraw2( GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	MenuContext * self = reinterpret_cast<MenuContext*>(data);

  gint width;
  gint height;
  cairo_t *cr = NULL;


#	if USE_GTK_VERSION_3_0
	cr = gdk_cairo_create(gtk_widget_get_window(widget));
#	elif USE_GTK_VERSION_2_0
	cr = gdk_cairo_create(widget->window);
#	endif
  //if (supports_alpha)
  {
    // transparent
    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0);
  }
  /*
  else
  {
    // opaque white
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
  }
  */

  /* draw the background */
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);

  /* draw a circle */
  gtk_window_get_size (GTK_WINDOW (widget), &width, &height);

  cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
  cairo_arc (cr, width / 2, height / 2, (width < height ? width : height) / 2 - 8 , 0, 2 * 3.14);
  cairo_fill (cr);
  cairo_stroke (cr);
  cairo_paint (cr);
  
  cairo_destroy (cr);
  return FALSE;
}




// sur : émis lors du premier affichage de la GtkDrawingArea
gboolean MenuContext::CB_displayInit( GtkWidget *widget, gpointer data)
{
	MenuContext * self = reinterpret_cast<MenuContext*>(data);
	
#	ifdef USE_GTK_VERSION_3_0
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	int32_t size_x = allocation.width;
	int32_t size_y = allocation.height;
	
	self->m_shawableSize.x = allocation.width;
	self->m_shawableSize.y = allocation.height;
#	elif defined( USE_GTK_VERSION_2_0)
	int32_t size_x = widget->allocation.width;
	int32_t size_y = widget->allocation.height;
	
	self->m_shawableSize.x = widget->allocation.width;
	self->m_shawableSize.y = widget->allocation.height;
#	endif
	EDN_INFO("Request a diplay of : " << size_x << "px * " << size_y << "px");

	//Display::InitDisplayParam(self->m_displayParameters, widget, 700, 1200);

	gtk_widget_queue_draw( widget );
	return TRUE;
}

gint MenuContext::CB_focusGet(	GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	//MenuContext * self = reinterpret_cast<MenuContext*>(data);
	
#	ifdef USE_GTK_VERSION_2_0
	GTK_WIDGET_SET_FLAGS (widget, GTK_HAS_FOCUS);
#	endif
	EDN_INFO("Focus - In");
	gtk_widget_queue_draw( widget );
	return FALSE;
}

gint MenuContext::CB_focusLost(	GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	//MenuContext * self = reinterpret_cast<MenuContext*>(data);
	
#	ifdef USE_GTK_VERSION_2_0
	GTK_WIDGET_UNSET_FLAGS (widget, GTK_HAS_FOCUS);
#	endif
	EDN_INFO("Focus - out");
	return FALSE;
}

gint MenuContext::CB_keyboardEvent(	GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	//MenuContext * self = reinterpret_cast<MenuContext*>(data);
	
	if(event->type == GDK_KEY_PRESS) {
		gtk_widget_queue_draw( widget );
	}
	return true;
}


gint MenuContext::CB_mouseButtonEvent(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	MenuContext * self = reinterpret_cast<MenuContext*>(data);
	
	// get focus on the widget
	gtk_widget_grab_focus(widget);
	if (event->button == 1) {
		/*
		if (event->type == GDK_BUTTON_PRESS) {
			EDN_INFO("mouse-event BT1  ==> One Clicked");
		}else*/ if (event->type == GDK_2BUTTON_PRESS) {
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
	}/* else if (event->button == 2) {
		if (event->type == GDK_BUTTON_PRESS) {
			EDN_INFO("mouse-event BT2 PRESS");
			self->m_menuContext->Show(event->x, event->y, false);
		}
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
			}
		}
	}*/ else {
		EDN_INFO("mouse-event BT? PRESS");
	}
	gtk_widget_queue_draw( widget );
	return true;
}



gint MenuContext::CB_mouseMotionEvent( GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	//MenuContexterView * self = reinterpret_cast<MenuContext*>(data);
	
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
