/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <TextViewer.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <ewol/clipBoard.h>
#include <SearchData.h>

#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/EObject.h>

#undef __class__
#define __class__ "TextViewer"

appl::TextViewer::TextViewer(const etk::UString& _fontName, int32_t _fontSize) :
  m_buffer(NULL),
  m_displayText(_fontName, _fontSize),
  m_insertMode(false) {
	setCanHaveFocus(true);
	registerMultiCast(ednMsgBufferId);
	registerMultiCast(ednMsgGuiCopy);
	registerMultiCast(ednMsgGuiPaste);
	registerMultiCast(ednMsgGuiCut);
	registerMultiCast(ednMsgGuiRedo);
	registerMultiCast(ednMsgGuiUndo);
	registerMultiCast(ednMsgGuiRm);
	registerMultiCast(ednMsgGuiSelect);
	registerMultiCast(ednMsgGuiChangeCharset);
	registerMultiCast(ednMsgGuiFind);
	registerMultiCast(ednMsgGuiReplace);
	registerMultiCast(ednMsgGuiGotoLine);
	setLimitScrolling(0.2);
	
	shortCutAdd("ctrl+w",       ednMsgGuiRm,     "Line");
	shortCutAdd("ctrl+shift+w", ednMsgGuiRm,     "Paragraph");
	shortCutAdd("ctrl+x",       ednMsgGuiCut,    "STD");
	shortCutAdd("ctrl+c",       ednMsgGuiCopy,   "STD");
	shortCutAdd("ctrl+v",       ednMsgGuiPaste,  "STD");
	shortCutAdd("ctrl+a",       ednMsgGuiSelect, "ALL");
	shortCutAdd("ctrl+shift+a", ednMsgGuiSelect, "NONE");
	
	// by default we load an example object:
	
	m_buffer = new appl::Buffer();
	if (m_buffer == NULL) {
		APPL_ERROR("can not create buffer ... ");
		return;
	}
	m_buffer->loadFile("./example.txt");
	
}

appl::TextViewer::~TextViewer(void) {
	
}

bool appl::TextViewer::calculateMinSize(void) {
	m_minSize.setValue(50,50);
	return true;
}

void appl::TextViewer::calculateMaxSize(void) {
	m_maxSize.setX(256);
	m_maxSize.setY(256);
}

void appl::TextViewer::onDraw(void) {
	m_displayDrawing.draw();
	m_displayText.draw();
	WidgetScrooled::onDraw();
}

void appl::TextViewer::onRegenerateDisplay(void) {
	if (false == needRedraw()) {
		return;
	}
	// For the scrooling windows
	calculateMaxSize();
	m_displayDrawing.clear();
	m_displayText.clear();
	
	// reset the background :
	m_displayDrawing.setPos(vec3(0, 0, 0));
	m_displayDrawing.setColor(etk::Color<>(220, 220, 220, 256));
	m_displayDrawing.rectangleWidth(m_size);
	
	if (m_buffer == NULL) {
		m_displayText.setTextAlignement(10, m_size.x()-20, ewol::Text::alignLeft);
		m_displayText.setRelPos(vec3(10, 0, 0));
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
		// call the herited class...
		WidgetScrooled::onRegenerateDisplay();
		return;
	}
	// normal displa of the buffer :
	vec3 tmpCursorPosition(0, 0, -1);
	// real display ...
	etk::Buffer& buf = m_buffer->getData();
	m_displayText.setColor(etk::Color<>(0, 0, 0, 256));
	float countNbLine = 1;
	esize_t countColomn = 0;
	vec3 tmpLetterSize = m_displayText.calculateSize((etk::UChar)'A');
	vec3 positionCurentDisplay(0.0f, m_size.y()-tmpLetterSize.y(), 0.0f);
	m_displayText.setPos(positionCurentDisplay);
	// the siplay string :
	etk::UString stringToDisplay;
	esize_t bufferElementSize = 0;
	etk::UChar currentValue;
	bool isSelect = false;
	int32_t selectPosStart = etk_min(m_buffer->m_cursorPos, m_buffer->m_cursorSelectPos);
	int32_t selectPosStop = etk_max(m_buffer->m_cursorPos, m_buffer->m_cursorSelectPos);
	if (m_buffer->m_cursorSelectPos<0) {
		selectPosStart = -1;
		selectPosStop = -1;
	}
	for (int32_t iii=0; iii<buf.size(); iii+=bufferElementSize) {
		if (iii == m_buffer->m_cursorPos) {
			// need to display the cursor :
			tmpCursorPosition = positionCurentDisplay;
		}
		if (iii >= selectPosStart && iii < selectPosStop) {
			isSelect = true;
		} else {
			isSelect = false;
		}
		bufferElementSize = m_buffer->get(iii, currentValue);
		//APPL_DEBUG(" element size : " << iii << " : " << bufferElementSize);
		if (currentValue == etk::UChar::Return) {
			countNbLine += 1;
			countColomn = 0;
			if (bufferElementSize  == 0) {
				bufferElementSize = 1;
			}
			positionCurentDisplay.setX(0);
			positionCurentDisplay.setY(positionCurentDisplay.y()-tmpLetterSize.y());
			m_displayText.setPos(positionCurentDisplay);
			continue;
		}
		m_buffer->expand(countColomn, currentValue, stringToDisplay);
		if (isSelect == true) {
			m_displayText.setColorBg(etk::Color<>(0x00FF00FF));
		} else {
			m_displayText.setColorBg(etk::Color<>(0x00000000));
		}
		//APPL_DEBUG("display : '" << currentValue << "'  == > '" << stringToDisplay << "'");
		//m_displayText.setPos(positionCurentDisplay);
		m_displayText.print(stringToDisplay);
		positionCurentDisplay = m_displayText.getPos();
		countColomn += stringToDisplay.size();
		
		if (bufferElementSize  == 0) {
			bufferElementSize = 1;
		}
	}
	if (tmpCursorPosition.z()!=-1) {
		// display the cursor:
		//APPL_DEBUG("display cursor at position : " << tmpCursorPosition);
		m_displayText.setPos(tmpCursorPosition);
		m_displayText.setColor(etk::Color<>(0xFF0000FF));
		m_displayText.setColorBg(etk::Color<>(0xFF0000FF));
		m_displayText.printCursor(m_insertMode);
	}
	// call the herited class...
	WidgetScrooled::onRegenerateDisplay();
}

bool appl::TextViewer::onEventEntry(const ewol::EventEntry& _event) {
	if (m_buffer == NULL) {
		return false;
	}
	// just forward event  == > manage directly in the buffer
	if (m_buffer->onEventEntry(_event, m_displayText) == true) {
		markToRedraw();
		return true;
	}
	return false;
}

bool appl::TextViewer::onEventInput(const ewol::EventInput& _event) {
	if (m_buffer == NULL) {
		return false;
	}
	keepFocus();
	vec2 relativePos = relativePosition(_event.getPos());
	// invert for the buffer event ...
	relativePos.setY(m_size.y()-relativePos.y());
	// just forward event  == > manage directly in the buffer
	if (m_buffer->onEventInput(_event, m_displayText, relativePos) == true) {
		markToRedraw();
		return true;
	}
	return true;
}

void appl::TextViewer::onEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID) {
	if (m_buffer != NULL) {
		etk::UString data = ewol::clipBoard::get(_clipboardID);
		m_buffer->paste(data);
	}
	markToRedraw();
}

void appl::TextViewer::onReceiveMessage(const ewol::EMessage& _msg) {
	// force redraw of the widget
	if (    _msg.getMessage() == ednMsgGuiCopy
	     || _msg.getMessage() == ednMsgGuiCut) {
		if (m_buffer != NULL) {
			etk::UString value;
			m_buffer->copy(value);
			if (value.size() != 0) {
				ewol::clipBoard::set(ewol::clipBoard::clipboardStd, value);
			}
		}
		if (_msg.getMessage() == ednMsgGuiCut) {
			m_buffer->removeSelection();
		}
	} else if (_msg.getMessage() == ednMsgGuiPaste) {
		if (m_buffer != NULL) {
			ewol::clipBoard::request(ewol::clipBoard::clipboardStd);
		}
	} else if (_msg.getMessage() == ednMsgGuiUndo) {
		if (m_buffer != NULL) {
			//m_buffer->undo();
		}
	} else if (_msg.getMessage() == ednMsgGuiRedo) {
		if (m_buffer != NULL) {
			//m_buffer->redo();
		}
	}
	markToRedraw();
}

void appl::TextViewer::onGetFocus(void) {
	showKeyboard();
	APPL_INFO("Focus - In");
}

void appl::TextViewer::onLostFocus(void) {
	hideKeyboard();
	APPL_INFO("Focus - out");
}

void appl::TextViewer::setFontSize(int32_t _size) {
	m_displayText.setFontSize(_size);
	setScrollingSize(_size*3.0*1.46); // 1.46 is a magic number ...
}

void appl::TextViewer::setFontName(const etk::UString& _fontName) {
	m_displayText.setFontName(_fontName);
}

