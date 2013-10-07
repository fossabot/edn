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
#include <ewol/renderer/EObject.h>


#undef __class__
#define __class__	"CodeView"

void CodeView::init(void)
{
	m_label = "CodeView is disable ...";
	
	m_bufferID = -1;
	m_buttunOneSelected = false;
	
	m_lineNumberList.clear();
	
	m_textColorFg = etk::color::black;
	
	m_textColorBg = etk::color::black;
	m_textColorBg.setA(0x40);
	setCanHaveFocus(true);
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
	setLimitScrolling(0.2);
	
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
	init();
}

CodeView::CodeView(void)
{
	init();
}

CodeView::~CodeView(void)
{
	
}


/**
 * @brief Check if the number of reference buffer is good or not ...
 * @param[in] bufferID id of the current Buffer that needed to have a reference
 */
void CodeView::updateNumberOfLineReference(int32_t bufferID)
{
	vec2 tmpCoord(0,0);
	if (m_lineNumberList.size() <= bufferID) {
		// update the number of elements : 
		for (int32_t iii=m_lineNumberList.size(); iii <= bufferID; iii++) {
			// add start line at 0 :
			m_lineNumberList.pushBack(tmpCoord);
		}
	}
}


bool CodeView::calculateMinSize(void)
{
	m_minSize.setValue(50,50);
	return true;
}

void CodeView::calculateMaxSize(void)
{
	m_maxSize.setX(2048);
	int32_t letterHeight = m_displayText.calculateSize(etk::UniChar('A')).y();
	BufferText* tmpBuffer = BufferManager::get(m_bufferID);
	if (NULL!=tmpBuffer) {
		m_maxSize.setY(tmpBuffer->getNumberOfLine() * letterHeight);
	} else {
		m_maxSize.setY(50);
	}
}


void CodeView::onDraw(void)
{
	m_displayDrawing.draw();
	m_displayText.draw();
	WidgetScrooled::onDraw();
}

void CodeView::onRegenerateDisplay(void)
{
	if (true == needRedraw()) {
		int64_t startTime = ewol::getTime();
		
		// For the scrooling windows
		calculateMaxSize();
		m_displayDrawing.clear();
		m_displayText.clear();
		
		// reset the background : 
		m_displayDrawing.setPos(vec3(-2048, -2048, 0));
		m_displayDrawing.setColor(ColorizeManager::get(COLOR_CODE_BASIC_BG));
		m_displayDrawing.rectangleWidth(vec3(4096, 4096, 0) );
		
		BufferText* tmpBuffer = BufferManager::get(m_bufferID);
		if(    NULL != tmpBuffer
		    && true == tmpBuffer->needToUpdateDisplayPosition() ) {
			vec2 borderWidth = BufferManager::get(m_bufferID)->getBorderSize();
			bool centerRequested = false;
			// TODO : set it back ...
			vec2  currentPosition = BufferManager::get(m_bufferID)->getPosition(999/*m_OObjectTextNormal.getFontID()*/, centerRequested);
			setScrollingPositionDynamic(borderWidth, currentPosition, centerRequested);
		} // else : nothing to do ...
		
		// generate the objects :
		if (-1 == m_bufferID) {
			m_displayText.setTextAlignement(10, m_size.x()-20, ewol::Text::alignLeft);
			m_displayDrawing.setColor(0x00000022);
			m_displayDrawing.setPos(vec3(10, 0, 0));
			m_displayDrawing.rectangle(vec3((int32_t)m_size.x()-20, 1500, 0) );
			
			m_displayText.setRelPos(vec3(10, 0, 0));
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
			m_displayText.setPos(vec3(0.0f, m_size.y(), 0.0f) );
			m_displayText.forceLineReturn();
			m_displayText.printDecorated(tmpString);
		} else {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->display(m_displayText, m_originScrooled.x(), m_originScrooled.y(), m_size.x(), m_size.y());
			}
			// set the current size of the windows
			setMaxSize(BufferManager::get(m_bufferID)->getMaxSize());
		}
		int64_t stopTime = ewol::getTime();
		APPL_DEBUG("Display Code Generation = " << stopTime - startTime << " micro-s");
		
		// call the herited class...
		WidgetScrooled::onRegenerateDisplay();
	}
}


bool CodeView::onEventEntry(const ewol::EventEntry& _event)
{
	if (_event.getType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.getStatus() == ewol::keyEvent::statusDown) {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->addChar(_event.getChar());
			}
			markToRedraw();
		}
		return true;
	}
	// move events ...
	if (_event.getStatus() == ewol::keyEvent::statusDown) {
		BufferText* tmpBuffer = BufferManager::get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->cursorMove(_event.getType());
		}
		markToRedraw();
	}
	return true;
}

void CodeView::onEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	BufferText* tmpBuffer = BufferManager::get(m_bufferID);
	if (NULL!=tmpBuffer) {
		tmpBuffer->Paste(_clipboardID);
	}
	markToRedraw();
}

bool CodeView::onEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = relativePosition(_event.getPos());
	//APPL_DEBUG("Event at pos : " << _event.getPos() << "  == > " << relativePos );
	// corection for the openGl abstraction
	//relativePos.y = m_size.y - relativePos.y;
	
	vec2 limitedPos = relativePos;
	limitedPos.setValue(etk_avg(1, limitedPos.x(), m_size.x()-1),
	                    etk_avg(1, limitedPos.y(), m_size.y()-1));
	if (true == WidgetScrooled::onEventInput(_event)) {
		keepFocus();
		// nothing to do ... done on upper widget ...
		return true;
	}
	
	if (m_bufferID < 0) {
		return false;
	}
	
	if (1 == _event.getId()) {
		
		if (ewol::keyEvent::typeMouse == _event.getType()) {
			if (ewol::keyEvent::statusDown == _event.getStatus()) {
				m_buttunOneSelected = true;
				keepFocus();
				// TODO : set something good
				BufferText* tmpBuffer = BufferManager::get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->MouseEvent(limitedPos);
				}
				markToRedraw();
			} else if (ewol::keyEvent::statusUp == _event.getStatus()) {
				m_buttunOneSelected = false;
				BufferText* tmpBuffer = BufferManager::get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->Copy(ewol::clipBoard::clipboardSelection);
				}
				markToRedraw();
			}
		}
		if (ewol::keyEvent::statusSingle == _event.getStatus()) {
			if (ewol::keyEvent::typeMouse == _event.getType()) {
				keepFocus();
				BufferText* tmpBuffer = BufferManager::get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->MouseEvent(limitedPos);
				}
				markToRedraw();
			} else {
				// nothing to do ...
			}
		} else if (ewol::keyEvent::statusDouble == _event.getStatus()) {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->MouseEventDouble();
			}
			markToRedraw();
		} else if (ewol::keyEvent::statusTriple == _event.getStatus()) {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->MouseEventTriple();
			}
			markToRedraw();
		} else if (ewol::keyEvent::statusMove == _event.getStatus()) {
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
				// TODO : set something good
				BufferText* tmpBuffer = BufferManager::get(m_bufferID);
				if (NULL!=tmpBuffer) {
					tmpBuffer->MouseSelectFromCursorTo(limitedPos);
				}
				markToRedraw();
			}
		}
	} else if (2 == _event.getId()) {
		if (ewol::keyEvent::statusSingle == _event.getStatus()) {
			// TODO : set something good
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->MouseEvent(limitedPos);
			}
			ewol::clipBoard::Request(ewol::clipBoard::clipboardSelection);
			keepFocus();
		}
	}
	return true;
}

void CodeView::onReceiveMessage(const ewol::EMessage& _msg)
{
	widget::WidgetScrooled::onReceiveMessage(_msg);
	APPL_DEBUG("Extern Event : " << _msg.getCaller() << "  type : " << _msg.GetMessage() << "  data=\"" << _msg.GetData() << "\"");
	
	if(_msg.getMessage() == ednMsgBufferId) {
		//keep the reference of the display offset :
		if(    m_bufferID  >= 0
		    && m_bufferID < m_lineNumberList.size()) {
			m_lineNumberList[m_bufferID] = m_originScrooled;
		}
		int32_t bufferID = 0;
		sscanf(_msg.getData().c_str(), "%d", &bufferID);
		APPL_INFO("Select a new Buffer ... " << bufferID);
		// set the new buffer ID
		m_bufferID = bufferID;
		// update the start display position...
		updateNumberOfLineReference(m_bufferID);
		// set back if needed the display position ...
		if(    m_bufferID  >= 0
		    && m_bufferID < m_lineNumberList.size()) {
			m_originScrooled = m_lineNumberList[m_bufferID];
		}
	} else if (_msg.getMessage() == ednMsgGuiCopy) {
		BufferText* tmpBuffer = BufferManager::get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Copy(ewol::clipBoard::clipboardStd);
		}
	} else if (_msg.getMessage() == ednMsgGuiCut) {
		BufferText* tmpBuffer = BufferManager::get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Cut(ewol::clipBoard::clipboardStd);
		}
	} else if (_msg.getMessage() == ednMsgGuiPaste) {
		ewol::clipBoard::Request(ewol::clipBoard::clipboardStd);
	} else if (_msg.getMessage() == ednMsgGuiUndo) {
		BufferText* tmpBuffer = BufferManager::get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Undo();
		}
	} else if (_msg.getMessage() == ednMsgGuiRedo) {
		BufferText* tmpBuffer = BufferManager::get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->Redo();
		}
	} else if (_msg.getMessage() == ednMsgGuiRm) {
		// data : "Word" "Line" "Paragraph"
		if (_msg.getData() == "Word") {
			APPL_WARNING(" on event " << _msg.getMessage() << " data=\"" << _msg.GetData() << "\"  == > not coded" );
		} else if (_msg.getData() == "Line") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->removeLine();
			}
		} else if (_msg.getData() == "Paragraph") {
			APPL_WARNING(" on event " << _msg.getMessage() << " data=\"" << _msg.GetData() << "\"  == > not coded" );
		} else {
			APPL_ERROR(" on event " << _msg.getMessage() << " unknow data=\"" << _msg.GetData() << "\"" );
		}
	} else if (_msg.getMessage() == ednMsgGuiSelect) {
		// data : "ALL" "NONE"
		if (_msg.getData() == "ALL") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->SelectAll();
			}
		} else if (_msg.getData() == "NONE") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->SelectNone();
			}
		} else {
			APPL_ERROR(" on event " << _msg.getMessage() << " unknow data=\"" << _msg.GetData() << "\"" );
		}
	} else if (_msg.getMessage() == ednMsgGuiChangeCharset) {
		// data : "UTF-8" "ISO-8859-1" "ISO-8859-15"
		if (_msg.getData() == "UTF-8") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->setCharset(unicode::EDN_CHARSET_UTF8);
			}
		} else if (_msg.getData() == "ISO-8859-1") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->setCharset(unicode::EDN_CHARSET_ISO_8859_1);
			}
		} else if (_msg.getData() == "ISO-8859-15") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->setCharset(unicode::EDN_CHARSET_ISO_8859_15);
			}
		} else {
			APPL_ERROR(" on event " << _msg.getMessage() << " unknow data=\"" << _msg.GetData() << "\"" );
		}
	} else if (_msg.getMessage() == ednMsgGuiFind) {
		etk::UString myDataString;
		SearchData::getSearch(myDataString);
		if (_msg.getData() == "Next") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->Search(myDataString, false, SearchData::getCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
		} else if (_msg.getData() == "Previous") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->Search(myDataString, true, SearchData::getCase(), SearchData::GetWrap(), SearchData::GetRegExp() );
			}
		}
	} else if (_msg.getMessage() == ednMsgGuiReplace) {
		etk::UString myDataString;
		SearchData::getReplace(myDataString);
		if (_msg.getData() == "Normal") {
			BufferText* tmpBuffer = BufferManager::get(m_bufferID);
			if (NULL!=tmpBuffer) {
				tmpBuffer->Replace(myDataString);
			}
		} else if (_msg.getData() == "All") {
			
		}
	} else if (_msg.getMessage() == ednMsgGuiGotoLine) {
		int32_t lineID = 0;
		sscanf(_msg.getData().c_str(), "%d", &lineID);
		APPL_INFO("Goto line : " << lineID);
		BufferText* tmpBuffer = BufferManager::get(m_bufferID);
		if (NULL!=tmpBuffer) {
			tmpBuffer->JumpAtLine(lineID);
		}
	}
	// force redraw of the widget
	markToRedraw();
}


void CodeView::onGetFocus(void)
{
	/*
	ewol::widgetMessageMultiCast::Send(getWidgetId(), ednMsgBufferId, m_bufferID);
	*/
	ShowKeyboard();
	APPL_INFO("Focus - In");
}


void CodeView::onLostFocus(void)
{
	HideKeyboard();
	APPL_INFO("Focus - out");
}

void CodeView::setFontSize(int32_t size)
{
	m_displayText.setFontSize(size);
	setScrollingSize(size*3.0*1.46); // 1.46 is a magic nmber ...
}

void CodeView::setFontName(etk::UString fontName)
{
	m_displayText.setFontName(fontName);
}

