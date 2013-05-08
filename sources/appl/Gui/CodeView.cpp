/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <CodeView.h>

#include <appl/Debug.h>
#include <appl/global.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <ewol/clipBoard.h>
#include <SearchData.h>

#include <ewol/widget/WidgetManager.h>
#include <ewol/eObject/EObject.h>


#undef __class__
#define __class__	"CodeView"

void CodeView::Init(void)
{
	m_label = "CodeView is disable ...";
	
	m_bufferID = -1;
	m_buttunOneSelected = false;
	
	m_lineNumberList.Clear();
	
	m_textColorFg = draw::color::black;
	
	m_textColorBg = draw::color::black;
	m_textColorBg.a = 0x40;
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
	RegisterMultiCast(ednMsgGuiFind);
	RegisterMultiCast(ednMsgGuiReplace);
	RegisterMultiCast(ednMsgGuiGotoLine);
	SetLimitScrolling(0.2);
	
	ShortCutAdd("ctrl+w",       ednMsgGuiRm,     "Line");
	ShortCutAdd("ctrl+shift+w", ednMsgGuiRm,     "Paragraph");
	ShortCutAdd("ctrl+x",       ednMsgGuiCut,    "STD");
	ShortCutAdd("ctrl+c",       ednMsgGuiCopy,   "STD");
	ShortCutAdd("ctrl+v",       ednMsgGuiPaste,  "STD");
	ShortCutAdd("ctrl+a",       ednMsgGuiSelect, "ALL");
	ShortCutAdd("ctrl+shift+a", ednMsgGuiSelect, "NONE");
}

CodeView::CodeView(etk::UString fontName, int32_t fontSize) :
	m_displayText(fontName, fontSize)
{
	Init();
}

CodeView::CodeView(void)
{
	Init();
}

CodeView::~CodeView(void)
{
	
}


/**
 * @brief Check if the number of reference buffer is good or not ...
 * @param[in] bufferID id of the current Buffer that needed to have a reference
 */
void CodeView::UpdateNumberOfLineReference(int32_t bufferID)
{
	vec2 tmpCoord(0,0);
	if (m_lineNumberList.Size()<=bufferID) {
		// update the number of elements : 
		for (int32_t iii=m_lineNumberList.Size(); iii <= bufferID; iii++) {
			// add start line at 0 :
			m_lineNumberList.PushBack(tmpCoord);
		}
	}
}


bool CodeView::CalculateMinSize(void)
{
	m_minSize.setValue(50,50);
	return true;
}

void CodeView::CalculateMaxSize(void)
{
	m_maxSize.setX(2048);
	int32_t letterHeight = m_displayText.CalculateSize('A').y();
	BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
	if (NULL!=tmpBuffer) {
		m_maxSize.setY(tmpBuffer->GetNumberOfLine() * letterHeight);
	} else {
		m_maxSize.setY(50);
	}
}


void CodeView::OnDraw(ewol::DrawProperty& displayProp)
{
	m_displayDrawing.Draw();
	m_displayText.Draw();
	WidgetScrooled::OnDraw(displayProp);
}

void CodeView::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		int64_t startTime = ewol::GetTime();
		
		// For the scrooling windows
		CalculateMaxSize();
		m_displayDrawing.Clear();
		m_displayText.Clear();
		
		// Reset the background : 
		m_displayDrawing.SetPos(vec3(-2048, -2048, 0));
		m_displayDrawing.SetColor(ColorizeManager::Get(COLOR_CODE_BASIC_BG));
		m_displayDrawing.RectangleWidth(vec3(4096, 4096, 0) );
		
		BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
		if(    NULL != tmpBuffer
		    && true == tmpBuffer->NeedToUpdateDisplayPosition() ) {
			vec2 borderWidth = BufferManager::Get(m_bufferID)->GetBorderSize();
			bool centerRequested = false;
			// TODO : set it back ...
			vec2  currentPosition = BufferManager::Get(m_bufferID)->GetPosition(999/*m_OObjectTextNormal.GetFontID()*/, centerRequested);
			SetScrollingPositionDynamic(borderWidth, currentPosition, centerRequested);
		} // else : nothing to do ...
		
		// generate the objects :
		if (-1 == m_bufferID) {
			m_displayText.SetTextAlignement(10, m_size.x()-20, ewol::Text::alignLeft);
			m_displayDrawing.SetColor(0x00000022);
			m_displayDrawing.SetPos(vec3(10, 0, 0));
			m_displayDrawing.Rectangle(vec3((int32_t)m_size.x()-20, 1500, 0) );
			
			m_displayText.SetRelPos(vec3(10, 0, 0));
			// nothing to display :
			etk::UString tmpString("<br/>\n"
			                       "<font color=\"red\">\n"
			                       "	<b>\n"
			                       "		edn - Editeur De N'ours\n"
			                       "	</b>\n"
			                       "</font>\n"
			                       "<br/>\n"
			                       "<br/>\n"
			                       "<font color=\"indigo\">\n"
			                       "	<i>\n"
			                       "		No Buffer Availlable to display\n"
			                       "	</i>\n"
			                       "</font>\n");
			m_displayText.SetPos(vec3(0.0f, m_size.y(), 0.0f) );
			m_displayText.ForceLineReturn();
			m_displayText.PrintDecorated(tmpString);
		} else {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->Display(m_displayText, m_originScrooled.x(), m_originScrooled.y(), m_size.x(), m_size.y());
			}
			// set the current size of the windows
			SetMaxSize(BufferManager::Get(m_bufferID)->GetMaxSize());
		}
		int64_t stopTime = ewol::GetTime();
		APPL_DEBUG("Display Code Generation = " << stopTime - startTime << " micro-s");
		
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
	}
}


bool CodeView::OnEventEntry(const ewol::EventEntry& _event)
{
	if (_event.GetType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.GetStatus() == ewol::keyEvent::statusDown) {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->AddChar(_event.GetChar());
			}
			MarkToRedraw();
		}
		return true;
	}
	// move events ...
	if (_event.GetStatus() == ewol::keyEvent::statusDown) {
		BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->cursorMove(_event.GetType());
		}
		MarkToRedraw();
	}
	return true;
}

void CodeView::OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID)
{
	BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
	if (NULL!=tmpBuffer) {
		tmpBuffer->Paste(clipboardID);
	}
	MarkToRedraw();
}

bool CodeView::OnEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = RelativePosition(_event.GetPos());
	//APPL_DEBUG("Event at pos : " << _event.GetPos() << " ==> " << relativePos );
	// corection for the openGl abstraction
	//relativePos.y = m_size.y - relativePos.y;
	
	vec2 limitedPos = relativePos;
	limitedPos.setValue(etk_avg(1, limitedPos.x(), m_size.x()-1),
	                    etk_avg(1, limitedPos.y(), m_size.y()-1));
	if (true == WidgetScrooled::OnEventInput(_event)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widget ...
		return true;
	}
	
	if (m_bufferID < 0) {
		return false;
	}
	
	if (1 == _event.GetId()) {
		
		if (ewol::keyEvent::typeMouse == _event.GetType()) {
			if (ewol::keyEvent::statusDown == _event.GetStatus()) {
				m_buttunOneSelected = true;
				ewol::widgetManager::FocusKeep(this);
				// TODO : Set something good
				BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->MouseEvent(limitedPos);
				}
				MarkToRedraw();
			} else if (ewol::keyEvent::statusUp == _event.GetStatus()) {
				m_buttunOneSelected = false;
				BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->Copy(ewol::clipBoard::clipboardSelection);
				}
				MarkToRedraw();
			}
		}
		if (ewol::keyEvent::statusSingle == _event.GetStatus()) {
			if (ewol::keyEvent::typeMouse == _event.GetType()) {
				ewol::widgetManager::FocusKeep(this);
				BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->MouseEvent(limitedPos);
				}
				MarkToRedraw();
			} else {
				// nothing to do ...
			}
		} else if (ewol::keyEvent::statusDouble == _event.GetStatus()) {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->MouseEventDouble();
			}
			MarkToRedraw();
		} else if (ewol::keyEvent::statusTriple == _event.GetStatus()) {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->MouseEventTriple();
			}
			MarkToRedraw();
		} else if (ewol::keyEvent::statusMove == _event.GetStatus()) {
			if (true == m_buttunOneSelected) {
				int xxx, yyy;
				xxx = relativePos.x();
				yyy = relativePos.y();
				if (xxx<0) {
					xxx = 0;
				}
				if (yyy<0) {
					yyy = 0;
				}
				//APPL_INFO("mouse-motion BT1 %d, %d", xxx, yyy);
				// TODO : Set something good
				BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->MouseSelectFromCursorTo(limitedPos);
				}
				MarkToRedraw();
			}
		}
	} else if (2 == _event.GetId()) {
		if (ewol::keyEvent::statusSingle == _event.GetStatus()) {
			// TODO : Set something good
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->MouseEvent(limitedPos);
			}
			ewol::clipBoard::Request(ewol::clipBoard::clipboardSelection);
			ewol::widgetManager::FocusKeep(this);
		}
	}
	return true;
}

void CodeView::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data)
{
	widget::WidgetScrooled::OnReceiveMessage(CallerObject, eventId, data);
	APPL_DEBUG("Extern Event : " << CallerObject << "  type : " << eventId << "  data=\"" << data << "\"");
	
	if(eventId == ednMsgBufferId) {
		//keep the reference of the display offset :
		if(    m_bufferID >=0
		    && m_bufferID < m_lineNumberList.Size()) {
			m_lineNumberList[m_bufferID] = m_originScrooled;
		}
		int32_t bufferID = 0;
		sscanf(data.c_str(), "%d", &bufferID);
		APPL_INFO("Select a new Buffer ... " << bufferID);
		// set the new buffer ID
		m_bufferID = bufferID;
		// update the start display position...
		UpdateNumberOfLineReference(m_bufferID);
		// set back if needed the display position ...
		if(    m_bufferID >=0
		    && m_bufferID < m_lineNumberList.Size()) {
			m_originScrooled = m_lineNumberList[m_bufferID];
		}
	} else if (eventId == ednMsgGuiCopy) {
		BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Copy(ewol::clipBoard::clipboardStd);
		}
	} else if (eventId == ednMsgGuiCut) {
		BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Cut(ewol::clipBoard::clipboardStd);
		}
	} else if (eventId == ednMsgGuiPaste) {
		ewol::clipBoard::Request(ewol::clipBoard::clipboardStd);
	} else if (eventId == ednMsgGuiUndo) {
		BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Undo();
		}
	} else if (eventId == ednMsgGuiRedo) {
		BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Redo();
		}
	} else if (eventId == ednMsgGuiRm) {
		// data : "Word" "Line" "Paragraph"
		if (data == "Word") {
			APPL_WARNING(" on event " << eventId << " data=\"" << data << "\" ==> not coded" );
		} else if (data == "Line") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->RemoveLine();
			}
		} else if (data == "Paragraph") {
			APPL_WARNING(" on event " << eventId << " data=\"" << data << "\" ==> not coded" );
		} else {
			APPL_ERROR(" on event " << eventId << " unknow data=\"" << data << "\"" );
		}
	} else if (eventId == ednMsgGuiSelect) {
		// data : "ALL" "NONE"
		if (data == "ALL") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->SelectAll();
			}
		} else if (data == "NONE") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->SelectNone();
			}
		} else {
			APPL_ERROR(" on event " << eventId << " unknow data=\"" << data << "\"" );
		}
	} else if (eventId == ednMsgGuiChangeCharset) {
		// data : "UTF-8" "ISO-8859-1" "ISO-8859-15"
		if (data == "UTF-8") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->SetCharset(unicode::EDN_CHARSET_UTF8);
			}
		} else if (data == "ISO-8859-1") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->SetCharset(unicode::EDN_CHARSET_ISO_8859_1);
			}
		} else if (data == "ISO-8859-15") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->SetCharset(unicode::EDN_CHARSET_ISO_8859_15);
			}
		} else {
			APPL_ERROR(" on event " << eventId << " unknow data=\"" << data << "\"" );
		}
	} else if (eventId == ednMsgGuiFind) {
		etk::UString myDataString;
		SearchData::GetSearch(myDataString);
		if (data == "Next") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->Search(myDataString, false, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
		} else if (data == "Previous") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->Search(myDataString, true, SearchData::GetCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
		}
	} else if (eventId == ednMsgGuiReplace) {
		etk::UString myDataString;
		SearchData::GetReplace(myDataString);
		if (data == "Normal") {
			BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->Replace(myDataString);
			}
		} else if (data == "All") {
			
		}
	} else if (eventId == ednMsgGuiGotoLine) {
		int32_t lineID = 0;
		sscanf(data.c_str(), "%d", &lineID);
		APPL_INFO("Goto line : " << lineID);
		BufferText* tmpBuffer = BufferManager::Get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->JumpAtLine(lineID);
		}
	}
	// Force redraw of the widget
	MarkToRedraw();
}


void CodeView::OnGetFocus(void)
{
	/*
	ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgBufferId, m_bufferID);
	*/
	ewol::Keyboard(true);
	APPL_INFO("Focus - In");
}


void CodeView::OnLostFocus(void)
{
	ewol::Keyboard(false);
	APPL_INFO("Focus - out");
}

void CodeView::SetFontSize(int32_t size)
{
	m_displayText.SetFontSize(size);
	SetScrollingSize(size*3.0*1.46); // 1.46 is a magic nmber ...
}

void CodeView::SetFontName(etk::UString fontName)
{
	m_displayText.SetFontName(fontName);
}

