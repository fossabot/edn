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
#include <ewol/EObject.h>


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
	
	m_textColorFg.red   = 0.0;
	m_textColorFg.green = 0.0;
	m_textColorFg.blue  = 0.0;
	m_textColorFg.alpha = 1.0;
	
	m_textColorBg.red   = 0.0;
	m_textColorBg.green = 0.0;
	m_textColorBg.blue  = 0.0;
	m_textColorBg.alpha = 0.25;
	SetCanHaveFocus(true);
	/*
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgBufferId);
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
	m_maxSize.x = 2048;
	int32_t letterHeight = ewol::GetHeight(m_fontNormal);
	m_maxSize.y = BufferManager::Get(m_bufferID)->GetNumberOfLine() * letterHeight;
}


bool CodeView::OnDraw(void)
{
	m_OObjectsColored[      m_currentDrawId].Draw();
	m_OObjectTextNormal[    m_currentDrawId].Draw();
	m_OObjectTextBold[      m_currentDrawId].Draw();
	m_OObjectTextItalic[    m_currentDrawId].Draw();
	m_OObjectTextBoldItalic[m_currentDrawId].Draw();
	return true;
}

void CodeView::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		int64_t startTime = GetCurrentTime();
		
		// For the scrooling windows
		CalculateMaxSize();
		
		// clean internal elements ...
		m_OObjectTextNormal[    m_currentCreateId].SetFontID(m_fontNormal);
		m_OObjectTextBold[      m_currentCreateId].SetFontID(m_fontBold);
		m_OObjectTextItalic[    m_currentCreateId].SetFontID(m_fontItalic);
		m_OObjectTextBoldItalic[m_currentCreateId].SetFontID(m_fontBoldItalic);
		
		m_OObjectTextNormal[    m_currentCreateId].Clear();
		m_OObjectTextBold[      m_currentCreateId].Clear();
		m_OObjectTextItalic[    m_currentCreateId].Clear();
		m_OObjectTextBoldItalic[m_currentCreateId].Clear();
		m_OObjectsColored[      m_currentCreateId].Clear();
		
		
		// generate the objects :
		BufferManager::Get(m_bufferID)->Display(m_OObjectTextNormal[m_currentCreateId],
		                                          m_OObjectTextBold[m_currentCreateId],
		                                          m_OObjectTextItalic[m_currentCreateId],
		                                          m_OObjectTextBoldItalic[m_currentCreateId],
		                                          m_OObjectsColored[m_currentCreateId],
		                                          m_originScrooled.x, m_originScrooled.y, m_size.x, m_size.y);
		
		int64_t stopTime = GetCurrentTime();
		EDN_DEBUG("Display Code Generation = " << stopTime - startTime << " milli-s");
		
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
		m_needFlipFlop = true;
	}
}


bool CodeView::OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData)
{
	//EDN_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
	if (typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		BufferManager::Get(m_bufferID)->AddChar(unicodeData);
		MarkToReedraw();
	}
	return true;
}


bool CodeView::OnEventKbMove(ewol::eventKbType_te typeEvent, ewol::eventKbMoveType_te moveTypeEvent)
{
	if (typeEvent == ewol::EVENT_KB_TYPE_DOWN) {
		BufferManager::Get(m_bufferID)->cursorMove(moveTypeEvent);
		MarkToReedraw();
	}
	return true;
}



bool CodeView::OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, ewol::eventPosition_ts pos)
{
	if (m_bufferID < 0) {
		return false;
	}
	if (true == WidgetScrooled::OnEventInput(IdInput, typeEvent, pos)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widet ...
		return true;
	}
	if (1 == IdInput) {
		#ifndef __MODE__Touch
			if (ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
				m_buttunOneSelected = true;
				ewol::widgetManager::FocusKeep(this);
				BufferManager::Get(m_bufferID)->MouseEvent(pos.local.x, pos.local.y);
				MarkToReedraw();
			} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_buttunOneSelected = false;
				BufferManager::Get(m_bufferID)->Copy(COPY_MIDDLE_BUTTON);
				MarkToReedraw();
			} else 
		#endif
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			#ifdef __MODE__Touch
				ewol::widgetManager::FocusKeep(this);
				BufferManager::Get(m_bufferID)->MouseEvent(pos.local.x, pos.local.y);
				MarkToReedraw();
			#else
				// nothing to do ...
			#endif
		} else if (ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent) {
			BufferManager::Get(m_bufferID)->MouseEventDouble();
			MarkToReedraw();
		} else if (ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			BufferManager::Get(m_bufferID)->MouseEventTriple();
			MarkToReedraw();
		} else if (ewol::EVENT_INPUT_TYPE_MOVE == typeEvent) {
			if (true == m_buttunOneSelected) {
				int xxx, yyy;
				xxx = pos.local.x;
				yyy = pos.local.y;
				if (xxx<0) {
					xxx = 0;
				}
				if (yyy<0) {
					yyy = 0;
				}
				//EDN_INFO("mouse-motion BT1 %d, %d", xxx, yyy);
				BufferManager::Get(m_bufferID)->MouseSelectFromCursorTo(xxx, yyy);
				MarkToReedraw();
			}
		}
	} else if (2 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			BufferManager::Get(m_bufferID)->MouseEvent(pos.local.x, pos.local.y);
			BufferManager::Get(m_bufferID)->Paste(COPY_MIDDLE_BUTTON);
			MarkToReedraw();
			ewol::widgetManager::FocusKeep(this);
		}
	}
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
		BufferManager::Get(m_bufferID)->ForceReDraw(true);
		// TODO : need to update the state of the file and the filenames ...
	}
	// old
	/*
	else if( ednMsgCodeViewCurrentChangeBufferId == generateEventId) {
		int32_t bufferID = 0;
		sscanf(data, "%d", &bufferID);
		EDN_INFO("Select a new Buffer ... " << bufferID);
		m_bufferID = bufferID;
		BufferManager::Get(m_bufferID)->ForceReDraw(true);
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
			BufferManager::Get(m_bufferID)->ForceReDraw(true);
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
			BufferManager::Get(m_bufferID)->RemoveLine();
			break;
		case EDN_MSG__CURRENT_SELECT_ALL:
			BufferManager::Get(m_bufferID)->SelectAll();
			break;
		case EDN_MSG__CURRENT_UN_SELECT:
			BufferManager::Get(m_bufferID)->SelectNone();
			break;
		case EDN_MSG__CURRENT_COPY:
			if (dataID == -1) {
				dataID = COPY_STD;
			}
			BufferManager::Get(m_bufferID)->Copy(dataID);
			break;
		case EDN_MSG__CURRENT_CUT:
			if (dataID == -1) {
				dataID = COPY_STD;
			}
			BufferManager::Get(m_bufferID)->Cut(dataID);
			break;
		case EDN_MSG__CURRENT_PASTE:
			if (dataID == -1) {
				dataID = COPY_STD;
			}
			BufferManager::Get(m_bufferID)->Paste(dataID);
			break;
		case EDN_MSG__CURRENT_FIND_PREVIOUS:
			{
				etk::UString myDataString;
				SearchData::GetSearch(myDataString);
				BufferManager::Get(m_bufferID)->Search(myDataString, true, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
			break;
		case EDN_MSG__CURRENT_FIND_NEXT:
			{
				etk::UString myDataString;
				SearchData::GetSearch(myDataString);
				BufferManager::Get(m_bufferID)->Search(myDataString, false, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
			break;
		case EDN_MSG__CURRENT_REPLACE:
			{
				etk::UString myDataString;
				SearchData::GetReplace(myDataString);
				BufferManager::Get(m_bufferID)->Replace(myDataString);
			}
			break;
		case EDN_MSG__CURRENT_REPLACE_ALL:
			break;
		case EDN_MSG__CURRENT_CLOSE:
			SendMessage(EDN_MSG__BUFF_ID_CLOSE, m_bufferID);
			break;
		case EDN_MSG__CURRENT_UNDO:
			BufferManager::Get(m_bufferID)->Undo();
			break;
		case EDN_MSG__CURRENT_REDO:
			BufferManager::Get(m_bufferID)->Redo();
			break;
		case EDN_MSG__CURRENT_GOTO_LINE:
			if (dataID<0) {
				dataID = 0;
			}
			BufferManager::Get(m_bufferID)->JumpAtLine(dataID);
			break;
		case EDN_MSG__REFRESH_DISPLAY:
			break;
		case EDN_MSG__CURRENT_SET_CHARSET:
			BufferManager::Get(m_bufferID)->SetCharset((unicode::charset_te)dataID);
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
	/*
	ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgBufferId, m_bufferID);
	*/
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

void CodeView::SetFontNameNormal(etk::UString fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontNormal = fontID;
	}
}

void CodeView::SetFontNameBold(etk::UString fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontBold = fontID;
	}
}

void CodeView::SetFontNameItalic(etk::UString fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontItalic = fontID;
	}
}

void CodeView::SetFontNameBoldItalic(etk::UString fontName)
{
	int32_t fontID = ewol::LoadFont(fontName, m_fontSize);
	if (fontID >= 0) {
		m_fontBoldItalic = fontID;
	}
}
