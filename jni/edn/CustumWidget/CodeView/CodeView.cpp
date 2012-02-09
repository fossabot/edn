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
#include <ewol/WidgetMessageMultiCast.h>


#undef __class__
#define __class__	"ewol::CodeView"


CodeView::CodeView(void)
{
	m_label = "CodeView is disable ...";
	m_fontNormal = -1;
	m_fontBold = -1;
	m_fontItalic = -1;
	m_fontBoldItalic = -1;
	m_fontSize = 15;
	
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
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgBufferId);
	
	
	//old
	/*
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentChangeBufferId);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentSave);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentSaveAs);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentSelectAll);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentRemoveLine);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentUnSelect);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentCopy);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentCut);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentPaste);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentFindPrevious);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentFindNext);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentFindOldNext);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentReplace);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentReplaceAll);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentClose);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentUndo);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentRedo);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentGotoLine);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgCodeViewCurrentSetCharset);
	*/
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

void CodeView::CalculateMaxSize(void)
{
	m_maxSize.x = 4096;
	int32_t letterHeight = ewol::GetHeight(m_fontNormal);
	m_maxSize.y = m_bufferManager->Get(m_bufferID)->GetNumberOfLine() * letterHeight;
}



void CodeView::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		// For the scrooling windows
		CalculateMaxSize();
		
		// create tmp object :
		ewol::OObject2DTextColored* myOObjectTextNormal     = new ewol::OObject2DTextColored(m_fontNormal);
		ewol::OObject2DTextColored* myOObjectTextBold       = new ewol::OObject2DTextColored(m_fontBold);
		ewol::OObject2DTextColored* myOObjectTextItalic     = new ewol::OObject2DTextColored(m_fontItalic);
		ewol::OObject2DTextColored* myOObjectTextBoldItalic = new ewol::OObject2DTextColored(m_fontBoldItalic);
		ewol::OObject2DColored*     myOObjectsColored = new ewol::OObject2DColored();
		
		// generate the objects :
		//m_bufferID = 0;
		m_bufferManager->Get(m_bufferID)->Display(myOObjectTextNormal, myOObjectTextBold, myOObjectTextItalic, myOObjectTextBoldItalic, myOObjectsColored, 
		                                          m_originScrooled.x, m_originScrooled.y, m_size.x, m_size.y);
		
		// clean the object list ...
		ClearOObjectList();
		// add generated element
		AddOObject(myOObjectsColored,       "CodeViewBackground");
		AddOObject(myOObjectTextNormal,     "CodeViewTextNormal");
		AddOObject(myOObjectTextBold,       "CodeViewTextBold");
		AddOObject(myOObjectTextItalic,     "CodeViewTextItalic");
		AddOObject(myOObjectTextBoldItalic, "CodeViewTextBoldItalic");
		
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
	}
}


bool CodeView::OnEventKb(ewol::eventKbType_te typeEvent, char UTF8_data[UTF8_MAX_SIZE])
{
	//EDN_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
	if (typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		m_bufferManager->Get(m_bufferID)->AddChar(UTF8_data);
		MarkToReedraw();
	}
	return true;
}


bool CodeView::OnEventKbMove(ewol::eventKbType_te typeEvent, ewol::eventKbMoveType_te moveTypeEvent)
{
	if (typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		m_bufferManager->Get(m_bufferID)->cursorMove(moveTypeEvent);
		MarkToReedraw();
	}
	return true;
}



bool CodeView::OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y)
{
	if (true == WidgetScrooled::OnEventInput(IdInput, typeEvent, x, y)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widet ...
		return true;
	}
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
			//EDN_INFO("mouse-event BT1  ==> One Clicked %d, %d", (uint32_t)event->x, (uint32_t)event->y);
			m_bufferManager->Get(m_bufferID)->MouseEvent(x, y);
			MarkToReedraw();
		} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			m_buttunOneSelected = false;
			m_bufferManager->Get(m_bufferID)->Copy(COPY_MIDDLE_BUTTON);
			MarkToReedraw();
		} else if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			// nothing to do ...
		} else if (ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent) {
			//EDN_INFO("mouse-event BT1  ==> Double Clicked %d, %d", (uint32_t)event->x, (uint32_t)event->y);
			m_bufferManager->Get(m_bufferID)->MouseEventDouble();
			MarkToReedraw();
		} else if (ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			//EDN_INFO("mouse-event BT1  ==> Triple Clicked");
			m_bufferManager->Get(m_bufferID)->MouseEventTriple();
			MarkToReedraw();
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
				MarkToReedraw();
			}
		}
	} else if (2 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			m_bufferManager->Get(m_bufferID)->MouseEvent(x, y);
			m_bufferManager->Get(m_bufferID)->Paste(COPY_MIDDLE_BUTTON);
			MarkToReedraw();
			ewol::widgetManager::FocusKeep(this);
		}
	}
	// TODO : No mere used here ... ==> set in the scrooled windows ...
	/* 
	if (4 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent)
	{
		//EDN_INFO("mouse-event GDK_SCROLL_UP");
		m_bufferManager->Get(m_bufferID)->ScrollUp();
		MarkToReedraw();
		ewol::widgetManager::FocusKeep(this);
	} else if (5 == IdInput && ewol::EVENT_INPUT_TYPE_UP == typeEvent)
	{
		//EDN_INFO("mouse-event GDK_SCROLL_DOWN");
		m_bufferManager->Get(m_bufferID)->ScrollDown();
		MarkToReedraw();
		ewol::widgetManager::FocusKeep(this);
	}
	*/
	return true;
}



bool CodeView::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y)
{
	EDN_DEBUG("Extern Event : " << widgetID << "  type : " << generateEventId << "  position(" << x << "," << y << ")");
	
	
	
	if( ednMsgBufferId == generateEventId) {
		int32_t bufferID = 0;
		sscanf(data, "%d", &bufferID);
		EDN_INFO("Select a new Buffer ... " << bufferID);
		m_bufferID = bufferID;
		m_bufferManager->Get(m_bufferID)->ForceReDraw(true);
		// TODO : need to update the state of the file and the filenames ...
	}
	// old
	/*
	else if( ednMsgCodeViewCurrentChangeBufferId == generateEventId) {
		int32_t bufferID = 0;
		sscanf(data, "%d", &bufferID);
		EDN_INFO("Select a new Buffer ... " << bufferID);
		m_bufferID = bufferID;
		m_bufferManager->Get(m_bufferID)->ForceReDraw(true);
		// request the display of the curent Editor
		ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgBufferChangeCurrent, (char*)data);
		
	}
	*/
	/*
	 else if (ednMsgCodeViewCurrentSave == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentSaveAs == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentSelectAll == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentRemoveLine == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentUnSelect == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentCopy == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentCut == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentPaste == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentFindPrevious == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentFindNext == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentFindOldNext == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentReplace == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentReplaceAll == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentClose == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentUndo == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentRedo == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentGotoLine == generateEventId) {
	
	} else if (ednMsgCodeViewCurrentSetCharset == generateEventId) {
	
	}
	*/
	else {
	
	}
	
	/*
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
	*/
	// Force redraw of the widget
	MarkToReedraw();
	return true;
}


void CodeView::OnGetFocus(void)
{
	ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgBufferId, m_bufferID);
	ewol::KeyboardShow(ewol::KEYBOARD_MODE_CODE);
	EDN_INFO("Focus - In");
}


void CodeView::OnLostFocus(void)
{
	ewol::KeyboardHide();
	EDN_INFO("Focus - out");
}

void CodeView::SetFontSize(int32_t size)
{
	m_fontSize = size;
}

void CodeView::SetFontNameNormal(etk::String fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontNormal = fontID;
	}
}

void CodeView::SetFontNameBold(etk::String fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontBold = fontID;
	}
}

void CodeView::SetFontNameItalic(etk::String fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontItalic = fontID;
	}
}

void CodeView::SetFontNameBoldItalic(etk::String fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontBoldItalic = fontID;
	}
}
