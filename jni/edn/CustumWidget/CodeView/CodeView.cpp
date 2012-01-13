/**
 *******************************************************************************
 * @file CodeView.cpp
 * @brief Editeur De N'ours : Code Viewer Widget
 *			This is an abstraction
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

#include <CodeView.h>

#include <tools_debug.h>
#include <tools_globals.h>
#include <Display.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <ClipBoard.h>
#include <SearchData.h>




#undef __class__
#define __class__	"ewol::Button"


CodeView::CodeView(void)
{
	m_label = "CodeView is disable ...";
	m_bufferID = -1;
	m_buttunOneSelected = false;
	
	// Init link with the buffer Manager
	m_bufferManager = BufferManager::getInstance();
	m_colorManager = ColorizeManager::getInstance();
	
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	
	m_textColorBg.red   = 0.0;
	m_textColorBg.green = 0.0;
	m_textColorBg.blue  = 0.0;
	m_textColorBg.alpha = 0.25;
	SetCanHaveFocus(true);
}

CodeView::~CodeView(void)
{
	
}

bool CodeView::CalculateMinSize(void)
{
	m_minSize.x = 50;
	m_minSize.y = 50;
	return true;
}



void CodeView::OnRegenerateDisplay(void)
{
	// create tmp object :
	ewol::OObject2DText*    myOObjectText     = new ewol::OObject2DText("", -1, m_textColorFg);
	ewol::OObject2DColored* myOObjectsColored = new ewol::OObject2DColored();
	
	// generate the objects :
	m_bufferManager->Get(m_bufferID)->Display(myOObjectText, myOObjectsColored, m_size.x, m_size.y);
	
	// clean the object list ...
	ClearOObjectList();
	// add generated element
	AddOObject(myOObjectsColored, "CodeViewBackground");
	AddOObject(myOObjectText,     "CodeViewText");
	
	
/*
	// TODO later : Add this in the basic element of the widget ...
	int32_t borderSize = 2;
	int32_t paddingSize = 3;
	
	int32_t tmpSizeX = m_minSize.x;
	int32_t tmpSizeY = m_minSize.y;
	int32_t tmpOriginX = (m_size.x - tmpSizeX) / 2;
	int32_t tmpOriginY = (m_size.y - tmpSizeY) / 2;
	// no change for the text orogin : 
	int32_t tmpTextOriginX = (m_size.x - tmpSizeX) / 2 + borderSize + 2*paddingSize;
	int32_t tmpTextOriginY = (m_size.y - tmpSizeY) / 2 + borderSize + 2*paddingSize;
	
	if (true==m_userFillX) {
		tmpSizeX = m_size.x;
		tmpOriginX = 0;
	}
	if (true==m_userFillY) {
		tmpSizeY = m_size.y;
		tmpOriginY = 0;
	}
	tmpOriginX += paddingSize;
	tmpOriginY += paddingSize;
	tmpSizeX -= 2*paddingSize;
	tmpSizeY -= 2*paddingSize;

	ewol::OObject2DText * tmpText = new ewol::OObject2DText("", -1, m_textColorFg);
	//int32_t fontId = GetDefaultFontId();
	//int32_t fontHeight = ewol::GetHeight(fontId);
	//int32_t fontWidth = ewol::GetWidth(fontId, m_label.c_str());
	tmpText->Text(tmpTextOriginX, tmpTextOriginY, m_label.c_str(), m_size.x - borderSize - 2*paddingSize);

	ewol::OObject2DColored * tmpOObjects = new ewol::OObject2DColored;
	tmpOObjects->SetColor(m_textColorBg);
	tmpOObjects->Rectangle( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY);
	tmpOObjects->SetColor(m_textColorFg);
	tmpOObjects->RectangleBorder( tmpOriginX, tmpOriginY, tmpSizeX, tmpSizeY, borderSize);
	AddOObject(tmpOObjects, "BouttonDecoration");
	AddOObject(tmpText, "BouttonText");
*/
}

bool CodeView::OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y)
{
/*
	//bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventButtonPressed == generateEventId) {
		EWOL_INFO("BT pressed ... " << m_label);
		//eventIsOK = true;
		ewol::widgetManager::FocusKeep(this);
	} else if(ewolEventButtonEnter == generateEventId) {
		OnRegenerateDisplay();
	}
	//return eventIsOK;
*/
	// in every case this not stop the propagation of the event
	return false;
	// if overwrited... you can ...
}


bool CodeView::OnEventKb(ewol::eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE])
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    UTF8_data != NULL
	    && typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && UTF8_data[0] == '\r') {
		//return OnEventArea(ewolEventButtonPressed, -1, -1);
	}
	return false;
}










#ifdef SDFGSDFGSDFG_FGSDFG_SDF_G___DSFG_SD_FG__SD_F_G_SD_FG


CodeView::CodeView(void) : MsgBroadcast("Code View", EDN_CAT_WORK_AREA)
{
	m_bufferID = -1;
	m_buttunOneSelected = false;
	m_shawableAreaX = 0;
	m_shawableAreaY = 0;

	// Init link with the buffer Manager
	//m_bufferManager = BufferManager::Get();
	//m_colorManager = ColorizeManager::Get();
/*
	m_widget = gtk_drawing_area_new();
	gtk_widget_set_size_request( m_widget, 200, 100);

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
	g_signal_connect(		G_OBJECT(m_widget), "focus_in_event",		G_CALLBACK(CB_focusGet),		 this);
	g_signal_connect(		G_OBJECT(m_widget), "focus_out_event",		G_CALLBACK(CB_focusLost),		 this);
	// Keyboard Event
	g_signal_connect_after(	G_OBJECT(m_widget), "key_press_event",		G_CALLBACK(CB_keyboardEvent),	 this);
	g_signal_connect_after(	G_OBJECT(m_widget), "key_release_event",	G_CALLBACK(CB_keyboardEvent),	 this);
	// Mouse Event
	g_signal_connect(		G_OBJECT(m_widget), "button_press_event",	G_CALLBACK(CB_mouseButtonEvent), this);
	g_signal_connect(		G_OBJECT(m_widget), "button_release_event",	G_CALLBACK(CB_mouseButtonEvent), this);
	g_signal_connect(		G_OBJECT(m_widget), "motion_notify_event",	G_CALLBACK(CB_mouseMotionEvent), this);
	g_signal_connect(		G_OBJECT(m_widget), "scroll-event",			G_CALLBACK(CB_mouseScrollEvent), this);
	// Display Event
	g_signal_connect(		G_OBJECT(m_widget), "realize",				G_CALLBACK(CB_displayInit), 	 this);
#	ifdef USE_GTK_VERSION_3_0
	g_signal_connect(		G_OBJECT(m_widget), "draw",					G_CALLBACK(CB_displayDraw),		this);
#	elif defined( USE_GTK_VERSION_2_0 )
	g_signal_connect(		G_OBJECT(m_widget), "expose_event",			G_CALLBACK(CB_displayDraw),		this);
#	endif
*/
}

CodeView::~CodeView(void)
{

}
/*
GtkWidget * CodeView::GetMainWidget(void)
{
	return m_widget;
}
*/

void CodeView::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		case EDN_MSG__CURRENT_CHANGE_BUFFER_ID:
			//EDN_INFO("Select a new Buffer ... " << dataID);
			m_bufferID = dataID;
			m_bufferManager->Get(m_bufferID)->ForceReDraw(true);
			// request the dispplay of the curent Editor
			SendMessage(EDN_MSG__BUFFER_CHANGE_CURRENT, m_bufferID);
			break;
		case EDN_MSG__CURRENT_SAVE:
			SendMessage(EDN_MSG__BUFF_ID_SAVE, m_bufferID);
			break;
		case EDN_MSG__CURRENT_SAVE_AS:
			SendMessage(EDN_MSG__GUI_SHOW_SAVE_AS, m_bufferID);
			break;
		case EDN_MSG__CURRENT_REMOVE_LINE:
			m_bufferManager->Get(m_bufferID)->RemoveLine();
			break;
		case EDN_MSG__CURRENT_SELECT_ALL:
			m_bufferManager->Get(m_bufferID)->SelectAll();
			break;
		case EDN_MSG__CURRENT_UN_SELECT:
			m_bufferManager->Get(m_bufferID)->SelectNone();
			break;
		case EDN_MSG__CURRENT_COPY:
			if (dataID == -1) {
				dataID = COPY_STD;
			}
			m_bufferManager->Get(m_bufferID)->Copy(dataID);
			break;
		case EDN_MSG__CURRENT_CUT:
			if (dataID == -1) {
				dataID = COPY_STD;
			}
			m_bufferManager->Get(m_bufferID)->Cut(dataID);
			break;
		case EDN_MSG__CURRENT_PASTE:
			if (dataID == -1) {
				dataID = COPY_STD;
			}
			m_bufferManager->Get(m_bufferID)->Paste(dataID);
			break;
		case EDN_MSG__CURRENT_FIND_PREVIOUS:
			{
				etk::String myDataString;
				SearchData::GetSearch(myDataString);
				m_bufferManager->Get(m_bufferID)->Search(myDataString, true, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
			break;
		case EDN_MSG__CURRENT_FIND_NEXT:
			{
				etk::String myDataString;
				SearchData::GetSearch(myDataString);
				m_bufferManager->Get(m_bufferID)->Search(myDataString, false, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
			break;
		case EDN_MSG__CURRENT_REPLACE:
			{
				etk::String myDataString;
				SearchData::GetReplace(myDataString);
				m_bufferManager->Get(m_bufferID)->Replace(myDataString);
			}
			break;
		case EDN_MSG__CURRENT_REPLACE_ALL:
			break;
		case EDN_MSG__CURRENT_CLOSE:
			SendMessage(EDN_MSG__BUFF_ID_CLOSE, m_bufferID);
			break;
		case EDN_MSG__CURRENT_UNDO:
			m_bufferManager->Get(m_bufferID)->Undo();
			break;
		case EDN_MSG__CURRENT_REDO:
			m_bufferManager->Get(m_bufferID)->Redo();
			break;
		case EDN_MSG__CURRENT_GOTO_LINE:
			if (dataID<0) {
				dataID = 0;
			}
			m_bufferManager->Get(m_bufferID)->JumpAtLine(dataID);
			break;
		case EDN_MSG__REFRESH_DISPLAY:
			break;
		case EDN_MSG__CURRENT_SET_CHARSET:
			m_bufferManager->Get(m_bufferID)->SetCharset((charset_te)dataID);
			break;
		case EDN_MSG__USER_DISPLAY_CHANGE:
			// Redraw all the display ... Done under ...
			break;
	}
	// Force redraw of the widget
	//gtk_widget_queue_draw(m_widget);
}


gboolean CodeView::CB_displayDraw( GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	CodeView * self = reinterpret_cast<CodeView*>(data);

	//EDN_INFO("displayDraw_cb");
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
#	endif
	if (true == needRedrawAll) {
		//updateScrollElement();
		self->m_bufferManager->Get(self->m_bufferID)->ForceReDraw(true);
	}
	EDN_INFO("Request a display of : " << self->m_shawableAreaX << "px * "<<  self->m_shawableAreaY<<"px");
	/*
	EDN_INFO("widget width=%d", widget->allocation.width);
	EDN_INFO("widget height=%d", widget->allocation.height);
	*/

	//EDN_INFO("BufferView Display");
	// Get the color Manager :
	ColorizeManager *myColorManager = NULL;
	myColorManager = ColorizeManager::getInstance();
	
	//(void)m_bufferManager->Get(m_bufferID)->Display(m_displayParameters, m_shawableAreaX, m_shawableAreaY);
	DrawerManager monDrawer(widget, self->m_shawableAreaX, self->m_shawableAreaY);
	//EDN_INFO("Display buffer ID = " << m_bufferID);
	(void)self->m_bufferManager->Get(self->m_bufferID)->Display(monDrawer);
//	EDN_WARNING("Must display here ... ");
	return TRUE;
}




// sur : émis lors du premier affichage de la GtkDrawingArea
gboolean CodeView::CB_displayInit( GtkWidget *widget, gpointer data)
{
	CodeView * self = reinterpret_cast<CodeView*>(data);
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
	EDN_INFO("Request a diplay of : "<< size_x <<"px * "<< size_y <<"px");

	gtk_widget_queue_draw( widget );

	return TRUE;
}

gint CodeView::CB_focusGet(	GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	CodeView * self = reinterpret_cast<CodeView*>(data);
#	ifdef USE_GTK_VERSION_2_0
	GTK_WIDGET_SET_FLAGS (widget, GTK_HAS_FOCUS);
#	endif
	self->SendMessage(EDN_MSG__BUFFER_CHANGE_CURRENT, self->m_bufferID);
	EDN_INFO("Focus - In");
	return FALSE;
}

gint CodeView::CB_focusLost(	GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	//CodeView * self = reinterpret_cast<CodeView*>(data);
	
#	ifdef USE_GTK_VERSION_2_0
	GTK_WIDGET_UNSET_FLAGS (widget, GTK_HAS_FOCUS);
#	endif
	EDN_INFO("Focus - out");
	return FALSE;
}

gint CodeView::CB_keyboardEvent(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	CodeView * self = reinterpret_cast<CodeView*>(data);
	
	char Utf8Out[10];
	bool controlKey;
	bool moveKey;
	int32_t key;
	// Convert input key : 
	ConvertInput(event, Utf8Out, controlKey, moveKey, key);
	
	if(event->type == GDK_KEY_PRESS) {
		if(false==controlKey) {
			self->m_bufferManager->Get(self->m_bufferID)->AddChar(Utf8Out);
			gtk_widget_queue_draw( widget );
		} else if (true == moveKey) {
			self->m_bufferManager->Get(self->m_bufferID)->cursorMove(key);
			gtk_widget_queue_draw( widget );
		}
	}
	return true;
}


gint CodeView::CB_mouseButtonEvent(GtkWidget *widget, GdkEventButton *event, gpointer data)
{

	CodeView * self = reinterpret_cast<CodeView*>(data);
	
	// get focus on the widget
	gtk_widget_grab_focus(widget);
	if (event->button == 1) {
		if (event->type == GDK_BUTTON_PRESS) {
			//EDN_INFO("mouse-event BT1  ==> One Clicked %d, %d", (uint32_t)event->x, (uint32_t)event->y);
			self->m_bufferManager->Get(self->m_bufferID)->MouseEvent(event->x, event->y);
			gtk_widget_queue_draw( widget );
			self->m_buttunOneSelected = true;
		}else if (event->type == GDK_2BUTTON_PRESS) {
			//EDN_INFO("mouse-event BT1  ==> Double Clicked %d, %d", (uint32_t)event->x, (uint32_t)event->y);
			self->m_bufferManager->Get(self->m_bufferID)->MouseEventDouble();
			gtk_widget_queue_draw( widget );
			self->m_buttunOneSelected = true;
		}else if (event->type == GDK_3BUTTON_PRESS) {
			//EDN_INFO("mouse-event BT1  ==> Triple Clicked");
			self->m_bufferManager->Get(self->m_bufferID)->MouseEventTriple();
			gtk_widget_queue_draw( widget );
			self->m_buttunOneSelected = true;
		}else if (event->type == GDK_BUTTON_RELEASE) {
			//EDN_INFO("mouse-event BT1  ==> Realease");
			self->m_buttunOneSelected = false;
			self->m_bufferManager->Get(self->m_bufferID)->Copy(COPY_MIDDLE_BUTTON);
		}
	} else if (event->button == 2) {
		if (event->type == GDK_BUTTON_PRESS) {
			EDN_INFO("mouse-event BT2 PRESS");
			self->m_bufferManager->Get(self->m_bufferID)->MouseEvent(event->x, event->y);
			self->m_bufferManager->Get(self->m_bufferID)->Paste(COPY_MIDDLE_BUTTON);
			gtk_widget_queue_draw(widget);
		}
	} else if (event->button == 3) {
		if (event->type == GDK_BUTTON_PRESS) {
			EDN_INFO("mouse-event BT3 PRESS");
		}
	} else {
		EDN_INFO("mouse-event BT" << event->type <<" PRESS");
	}
	return true;
}



gint CodeView::CB_mouseMotionEvent( GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	CodeView * self = reinterpret_cast<CodeView*>(data);
	
	if (true == self->m_buttunOneSelected) {
		int x, y;
		GdkModifierType state;

		if (event->is_hint) {
			gdk_window_get_pointer(event->window, &x, &y, &state);
		} else {
			x = event->x;
			y = event->y;
			state = (GdkModifierType)event->state;
		}
		if (x<0) {
			x = 0;
		}
		if (y<0) {
			y = 0;
		}
		//EDN_INFO("mouse-motion BT1 %d, %d", x, y);
		self->m_bufferManager->Get(self->m_bufferID)->MouseSelectFromCursorTo(x, y);
		gtk_widget_queue_draw( widget );
	}
	return true;
}


gint CodeView::CB_mouseScrollEvent( GtkWidget *widget, GdkEventScroll *event, gpointer data)
{
	CodeView * self = reinterpret_cast<CodeView*>(data);
	
	if (event->direction == GDK_SCROLL_UP)
	{
		// up code
		//EDN_INFO("mouse-event GDK_SCROLL_UP");
		self->m_bufferManager->Get(self->m_bufferID)->ScrollUp();
		gtk_widget_queue_draw( widget );
	}
	else if (event->direction == GDK_SCROLL_DOWN)
	{
		// down code
		//EDN_INFO("mouse-event GDK_SCROLL_DOWN");
		self->m_bufferManager->Get(self->m_bufferID)->ScrollDown();
		gtk_widget_queue_draw( widget );
	} else {
		EDN_INFO("mouse-event SCROLL");
	}

	return true;
}


#endif



