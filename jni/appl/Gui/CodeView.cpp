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

#include <appl/Debug.h>
#include <appl/global.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <ewol/ClipBoard.h>
#include <SearchData.h>

#include <ewol/WidgetManager.h>
#include <ewol/EObject.h>


#undef __class__
#define __class__	"CodeView"


extern const char * const TYPE_EOBJECT_EDN_CODE_VIEW = "CodeView";

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
	
	m_textColorFg = etk::color::color_Black;
	
	m_textColorBg = etk::color::color_Black;
	m_textColorBg.alpha = 0.25;
	SetCanHaveFocus(true);
	RegisterMultiCast(ednMsgBufferId);
	RegisterMultiCast(ednMsgGuiCopy);
	RegisterMultiCast(ednMsgGuiPaste);
	RegisterMultiCast(ednMsgGuiCut);
	RegisterMultiCast(ednMsgGuiRedo);
	RegisterMultiCast(ednMsgGuiUndo);
	RegisterMultiCast(ednMsgGuiRm);
	RegisterMultiCast(ednMsgGuiSelect);
	RegisterMultiCast(ednMsgGuiChangeCharset);
}

CodeView::~CodeView(void)
{
	
}

/**
 * @brief Check if the object has the specific type.
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type of the object we want to check
 * @return true if the object is compatible, otherwise false
 */
bool CodeView::CheckObjectType(const char * const objectType)
{
	if (NULL == objectType) {
		APPL_ERROR("check error : \"" << TYPE_EOBJECT_EDN_CODE_VIEW << "\" != NULL(pointer) ");
		return false;
	}
	if (objectType == TYPE_EOBJECT_EDN_CODE_VIEW) {
		return true;
	} else {
		if(true == ewol::WidgetScrooled::CheckObjectType(objectType)) {
			return true;
		}
		APPL_ERROR("check error : \"" << TYPE_EOBJECT_EDN_CODE_VIEW << "\" != \"" << objectType << "\"");
		return false;
	}
}

/**
 * @brief Get the current Object type of the EObject
 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
 * @param[in] objectType type description
 * @return true if the object is compatible, otherwise false
 */
const char * const CodeView::GetObjectType(void)
{
	return TYPE_EOBJECT_EDN_CODE_VIEW;
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


void CodeView::OnDraw(void)
{
	m_OObjectsColored[      m_currentDrawId].Draw();
	m_OObjectTextNormal[    m_currentDrawId].Draw();
	m_OObjectTextBold[      m_currentDrawId].Draw();
	m_OObjectTextItalic[    m_currentDrawId].Draw();
	m_OObjectTextBoldItalic[m_currentDrawId].Draw();
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
		APPL_DEBUG("Display Code Generation = " << stopTime - startTime << " milli-s");
		
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
		m_needFlipFlop = true;
	}
}


bool CodeView::OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData)
{
	//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
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


/**
 * @brief Event on an input of this Widget
 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
 * @param[in] pos Absolute position of the event
 * @return true the event is used
 * @return false the event is not used
 */
bool CodeView::OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, coord2D_ts pos)
{
	coord2D_ts relativePos = RelativePosition(pos);
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
				BufferManager::Get(m_bufferID)->MouseEvent(m_fontNormal, relativePos.x+m_originScrooled.x, relativePos.y+m_originScrooled.y);
				MarkToReedraw();
			} else if (ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
				m_buttunOneSelected = false;
				BufferManager::Get(m_bufferID)->Copy(ewol::clipBoard::CLIPBOARD_SELECTION);
				MarkToReedraw();
			} else 
		#endif
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			#ifdef __MODE__Touch
				ewol::widgetManager::FocusKeep(this);
				BufferManager::Get(m_bufferID)->MouseEvent(m_fontNormal, relativePos.x+m_originScrooled.x, relativePos.y+m_originScrooled.y);
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
				xxx = relativePos.x;
				yyy = relativePos.y;
				if (xxx<0) {
					xxx = 0;
				}
				if (yyy<0) {
					yyy = 0;
				}
				//APPL_INFO("mouse-motion BT1 %d, %d", xxx, yyy);
				BufferManager::Get(m_bufferID)->MouseSelectFromCursorTo(m_fontNormal, xxx+m_originScrooled.x, yyy+m_originScrooled.y);
				MarkToReedraw();
			}
		}
	} else if (2 == IdInput) {
		if (ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent) {
			BufferManager::Get(m_bufferID)->MouseEvent(m_fontNormal, relativePos.x+m_originScrooled.x, relativePos.y+m_originScrooled.y);
			BufferManager::Get(m_bufferID)->Paste(ewol::clipBoard::CLIPBOARD_SELECTION);
			MarkToReedraw();
			ewol::widgetManager::FocusKeep(this);
		}
	}
	return true;
}



/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void CodeView::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	ewol::WidgetScrooled::OnReceiveMessage(CallerObject, eventId, data);
	APPL_DEBUG("Extern Event : " << CallerObject << "  type : " << eventId << "  data=\"" << data << "\"");
	
	if(eventId == ednMsgBufferId) {
		int32_t bufferID = 0;
		sscanf(data.Utf8Data(), "%d", &bufferID);
		APPL_INFO("Select a new Buffer ... " << bufferID);
		m_bufferID = bufferID;
		// TODO : need to update the state of the file and the filenames ...
	} else if (eventId == ednMsgGuiCopy) {
		BufferManager::Get(m_bufferID)->Copy(ewol::clipBoard::CLIPBOARD_STD);
	} else if (eventId == ednMsgGuiCut) {
		BufferManager::Get(m_bufferID)->Cut(ewol::clipBoard::CLIPBOARD_STD);
	} else if (eventId == ednMsgGuiPaste) {
		BufferManager::Get(m_bufferID)->Paste(ewol::clipBoard::CLIPBOARD_STD);
	} else if (eventId == ednMsgGuiUndo) {
		BufferManager::Get(m_bufferID)->Undo();
	} else if (eventId == ednMsgGuiRedo) {
		BufferManager::Get(m_bufferID)->Redo();
	} else if (eventId == ednMsgGuiRm) {
		// data : "Word" "Line" "Paragraph"
		if (data == "Word") {
			APPL_WARNING(" on event " << eventId << " data=\"" << data << "\" ==> not coded" );
		} else if (data == "Line") {
			BufferManager::Get(m_bufferID)->RemoveLine();
		} else if (data == "Paragraph") {
			APPL_WARNING(" on event " << eventId << " data=\"" << data << "\" ==> not coded" );
		} else {
			APPL_ERROR(" on event " << eventId << " unknow data=\"" << data << "\"" );
		}
	} else if (eventId == ednMsgGuiSelect) {
		// data : "ALL" "NONE"
		if (data == "ALL") {
			BufferManager::Get(m_bufferID)->SelectAll();
		} else if (data == "NONE") {
			BufferManager::Get(m_bufferID)->SelectNone();
		} else {
			APPL_ERROR(" on event " << eventId << " unknow data=\"" << data << "\"" );
		}
	} else if (eventId == ednMsgGuiChangeCharset) {
		// data : "UTF-8" "ISO-8859-1" "ISO-8859-15"
		if (data == "UTF-8") {
			BufferManager::Get(m_bufferID)->SetCharset(unicode::EDN_CHARSET_UTF8);
		} else if (data == "ISO-8859-1") {
			BufferManager::Get(m_bufferID)->SetCharset(unicode::EDN_CHARSET_ISO_8859_1);
		} else if (data == "ISO-8859-15") {
			BufferManager::Get(m_bufferID)->SetCharset(unicode::EDN_CHARSET_ISO_8859_15);
		} else {
			APPL_ERROR(" on event " << eventId << " unknow data=\"" << data << "\"" );
		}
	}
	/*
	switch (id)
	{
		case APPL_MSG__CURRENT_FIND_PREVIOUS:
			{
				etk::UString myDataString;
				SearchData::GetSearch(myDataString);
				BufferManager::Get(m_bufferID)->Search(myDataString, true, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
			break;
		case APPL_MSG__CURRENT_FIND_NEXT:
			{
				etk::UString myDataString;
				SearchData::GetSearch(myDataString);
				BufferManager::Get(m_bufferID)->Search(myDataString, false, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
			break;
		case APPL_MSG__CURRENT_REPLACE:
			{
				etk::UString myDataString;
				SearchData::GetReplace(myDataString);
				BufferManager::Get(m_bufferID)->Replace(myDataString);
			}
			break;
		case APPL_MSG__CURRENT_REPLACE_ALL:
			break;
		case APPL_MSG__CURRENT_GOTO_LINE:
			if (dataID<0) {
				dataID = 0;
			}
			BufferManager::Get(m_bufferID)->JumpAtLine(dataID);
			break;
		case APPL_MSG__CURRENT_SET_CHARSET:
			BufferManager::Get(m_bufferID)->SetCharset((unicode::charset_te)dataID);
			break;
	}
	*/
	// Force redraw of the widget
	MarkToReedraw();
}


void CodeView::OnGetFocus(void)
{
	/*
	ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgBufferId, m_bufferID);
	*/
	ewol::KeyboardShow(ewol::KEYBOARD_MODE_CODE);
	APPL_INFO("Focus - In");
}


void CodeView::OnLostFocus(void)
{
	ewol::KeyboardHide();
	APPL_INFO("Focus - out");
}

void CodeView::SetFontSize(int32_t size)
{
	m_fontSize = size;
	SetScrollingSize(m_fontSize*3.0*1.46); // 1.46 is a magic nmber ...
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
