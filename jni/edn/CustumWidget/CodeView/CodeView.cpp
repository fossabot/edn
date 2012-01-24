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

#include <ewol/WidgetManager.h>


#undef __class__
#define __class__	"ewol::Button"


CodeView::CodeView(void) : MsgBroadcast("Code View", EDN_CAT_WORK_AREA)
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
	ewol::OObject2DTextColored* myOObjectText     = new ewol::OObject2DTextColored("", -1);
	ewol::OObject2DColored*     myOObjectsColored = new ewol::OObject2DColored();
	
	// generate the objects :
	//m_bufferID = 0;
	m_bufferManager->Get(m_bufferID)->Display(myOObjectText, myOObjectsColored, m_size.x, m_size.y);
	
	// clean the object list ...
	ClearOObjectList();
	// add generated element
	AddOObject(myOObjectsColored, "CodeViewBackground");
	AddOObject(myOObjectText,     "CodeViewText");
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
	//EDN_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
	if (typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		m_bufferManager->Get(m_bufferID)->AddChar(UTF8_data);
		OnRegenerateDisplay();
	}
	return true;
}


bool CodeView::OnEventKbMove(ewol::eventKbType_te typeEvent, ewol::eventKbMoveType_te moveTypeEvent)
{
	if (typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		m_bufferManager->Get(m_bufferID)->cursorMove(moveTypeEvent);
		OnRegenerateDisplay();
	}
	return true;
}



bool CodeView::OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	x -= m_origin.x;
	y -= m_origin.y;
	/*
	etk::String type = (int)typeEvent;
	switch (typeEvent)
	{
		case ewol::EVENT_INPUT_TYPE_DOWN:
			type = "EVENT_INPUT_TYPE_DOWN";
			break;
		case ewol::EVENT_INPUT_TYPE_MOVE:
			type = "EVENT_INPUT_TYPE_MOVE";
			break;
		case ewol::EVENT_INPUT_TYPE_SINGLE:
			type = "EVENT_INPUT_TYPE_SINGLE";
			break;
		case ewol::EVENT_INPUT_TYPE_DOUBLE:
			type = "EVENT_INPUT_TYPE_DOUBLE";
			break;
		case ewol::EVENT_INPUT_TYPE_TRIPLE:
			type = "EVENT_INPUT_TYPE_TRIPLE";
			break;
		case ewol::EVENT_INPUT_TYPE_UP:
			type = "EVENT_INPUT_TYPE_UP";
			break;
		case ewol::EVENT_INPUT_TYPE_ENTER:
			type = "EVENT_INPUT_TYPE_ENTER";
			break;
		case ewol::EVENT_INPUT_TYPE_LEAVE:
			type = "EVENT_INPUT_TYPE_LEAVE";
			break;
		default:
			break;
	};
	
	EDN_DEBUG("Event : " << IdInput << "  type : " << type << "  position(" << x << "," << y << ")");
	*/
	if (1 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
			m_buttunOneSelected = true;
			ewol::widgetManager::FocusKeep(this);
		} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_buttunOneSelected = false;
			m_bufferManager->Get(m_bufferID)->Copy(COPY_MIDDLE_BUTTON);
			OnRegenerateDisplay();
		} else if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			//EDN_INFO("mouse-event BT1  ==> One Clicked %d, %d", (uint32_t)event->x, (uint32_t)event->y);
			m_bufferManager->Get(m_bufferID)->MouseEvent(x, y);
			OnRegenerateDisplay();
		} else if (ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent) {
			//EDN_INFO("mouse-event BT1  ==> Double Clicked %d, %d", (uint32_t)event->x, (uint32_t)event->y);
			m_bufferManager->Get(m_bufferID)->MouseEventDouble();
			OnRegenerateDisplay();
		} else if (ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			//EDN_INFO("mouse-event BT1  ==> Triple Clicked");
			m_bufferManager->Get(m_bufferID)->MouseEventTriple();
			OnRegenerateDisplay();
		} else if (ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
			if (true == m_buttunOneSelected) {
				int xxx, yyy;
				xxx = x;
				yyy = y;
				if (xxx<0) {
					xxx = 0;
				}
				if (yyy<0) {
					yyy = 0;
				}
				//EDN_INFO("mouse-motion BT1 %d, %d", xxx, yyy);
				m_bufferManager->Get(m_bufferID)->MouseSelectFromCursorTo(xxx, yyy);
				OnRegenerateDisplay();
			}
		}
	} else if (2 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			m_bufferManager->Get(m_bufferID)->MouseEvent(x, y);
			m_bufferManager->Get(m_bufferID)->Paste(COPY_MIDDLE_BUTTON);
			OnRegenerateDisplay();
			ewol::widgetManager::FocusKeep(this);
		}
	}
	if (4 == IdInput && ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent)
	{
		//EDN_INFO("mouse-event GDK_SCROLL_UP");
		m_bufferManager->Get(m_bufferID)->ScrollUp();
		OnRegenerateDisplay();
		ewol::widgetManager::FocusKeep(this);
	} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent)
	{
		//EDN_INFO("mouse-event GDK_SCROLL_DOWN");
		m_bufferManager->Get(m_bufferID)->ScrollDown();
		OnRegenerateDisplay();
		ewol::widgetManager::FocusKeep(this);
	}
	return true;
}



void CodeView::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		case EDN_MSG__CURRENT_CHANGE_BUFFER_ID:
			EDN_INFO("Select a new Buffer ... " << dataID);
			m_bufferID = dataID;
			m_bufferManager->Get(m_bufferID)->ForceReDraw(true);
			// request the display of the curent Editor
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
			m_bufferManager->Get(m_bufferID)->SetCharset((unicode::charset_te)dataID);
			break;
		case EDN_MSG__USER_DISPLAY_CHANGE:
			// Redraw all the display ... Done under ...
			break;
	}
	// Force redraw of the widget
	OnRegenerateDisplay();
}


void CodeView::OnGetFocus(void)
{
	SendMessage(EDN_MSG__BUFFER_CHANGE_CURRENT, m_bufferID);
	EDN_INFO("Focus - In");
}


void CodeView::OnLostFocus(void)
{
	EDN_INFO("Focus - out");
}


