/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <TextViewer.h>
#include <BufferManager.h>
//#include <ColorizeManager.h>
#include <ewol/clipBoard.h>
#include <SearchData.h>

#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/EObject.h>
#include <appl/TextPluginManager.h>

#undef __class__
#define __class__ "TextViewer"

#define tic() \
	int64_t startTime = ewol::getTime();

#define toc(comment) \
	int64_t endTime = ewol::getTime(); \
	int64_t processTimeLocal = (endTime - startTime); \
	APPL_DEBUG(comment << (float)((float)processTimeLocal / 1000.0) << "ms");

appl::TextViewer::TextViewer(const std::string& _fontName, int32_t _fontSize) :
  m_buffer(NULL),
  m_displayText(_fontName, _fontSize),
  m_insertMode(false) {
	addObjectType("appl::TextViewer");
	setCanHaveFocus(true);
	registerMultiCast(ednMsgBufferId);
	registerMultiCast(ednMsgGuiRm);
	registerMultiCast(ednMsgGuiSelect);
	registerMultiCast(ednMsgGuiFind);
	registerMultiCast(ednMsgGuiReplace);
	registerMultiCast(ednMsgGuiGotoLine);
	registerMultiCast(appl::MsgSelectNewFile);
	setLimitScrolling(0.2);
	
	shortCutAdd("ctrl+w",       ednMsgGuiRm,     "Line");
	shortCutAdd("ctrl+shift+w", ednMsgGuiRm,     "Paragraph");
	shortCutAdd("ctrl+a",       ednMsgGuiSelect, "ALL");
	shortCutAdd("ctrl+shift+a", ednMsgGuiSelect, "NONE");
	
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	// load color properties
	m_paintingProperties = appl::GlyphPainting::keep("THEME:COLOR:textViewer.json");
	// get all id properties ...
	m_colorBackground = m_paintingProperties->request("CODE_basicBackgroung");
	m_colorSpace = m_paintingProperties->request("CODE_space");
	m_colorTabulation = m_paintingProperties->request("CODE_tabulation");
	m_colorCursor = m_paintingProperties->request("CODE_cursor");
	m_colorLineNumber = m_paintingProperties->request("CODE_lineNumber");
	m_colorSelection = m_paintingProperties->request("SelectedText");
	m_colorNormal = m_paintingProperties->request("normal");
	
	// by default we load an example object:
	/*
	m_buffer = new appl::Buffer();
	if (m_buffer == NULL) {
		APPL_ERROR("can not create buffer ... ");
		return;
	}
	m_buffer->loadFile("./example.txt");
	*/
	appl::textPluginManager::connect(*this);
	// last created has focus ...
	setCurrentSelect();
}

appl::TextViewer::~TextViewer(void) {
	appl::textPluginManager::disconnect(*this);
	if (m_paintingProperties != NULL) {
		appl::GlyphPainting::release(m_paintingProperties);
	}
	if (m_bufferManager != NULL) {
		appl::BufferManager::release(m_bufferManager);
	}
}

bool appl::TextViewer::calculateMinSize(void) {
	m_minSize.setValue(50,50);
	return true;
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
	//tic();
	// For the scrooling windows
	m_displayDrawing.clear();
	m_displayText.clear();
	
	// reset the background :
	m_displayDrawing.setPos(vec3(0, 0, 0));
	m_displayDrawing.setColor((*m_paintingProperties)[m_colorBackground].getForeground());
	m_displayDrawing.rectangleWidth(m_size);
	
	if (m_buffer == NULL) {
		m_maxSize.setX(256);
		m_maxSize.setY(256);
		m_displayText.setTextAlignement(10, m_size.x()-20, ewol::Text::alignLeft);
		m_displayText.setRelPos(vec3(10, 0, 0));
		std::string tmpString("<br/>\n"
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
	// the siplay string :
	std::u32string stringToDisplay;
	esize_t bufferElementSize = 0;
	bool isSelect = false;
	appl::Buffer::Iterator selectPosStart = m_buffer->begin();
	appl::Buffer::Iterator selectPosStop = m_buffer->begin();
	if (m_buffer->hasTextSelected() == true) {
		selectPosStart = m_buffer->selectStart();
		selectPosStop = m_buffer->selectStop();
	}
	m_displayText.setPos(vec3(-m_originScrooled.x(), m_size.y()+m_originScrooled.y(), 0));
	m_displayText.forceLineReturn();
	appl::Buffer::Iterator startingIt = m_buffer->begin();
	int32_t startLineId = 0;
	if (m_size.y() < m_displayText.getPos().y()) {
		for (startingIt = m_buffer->begin();
		     (bool)startingIt == true;
		     ++startingIt) {
			if (*startingIt == etk::UChar::Return) {
				++startLineId;
				m_displayText.forceLineReturn();
				if (m_size.y() >= m_displayText.getPos().y()) {
					++startingIt;
					break;
				}
			}
		}
	}
	// Display line number :
	m_lastOffsetDisplay = 0;
	vec3 tmpLetterSize = m_displayText.calculateSize((char32_t)'A');
	{
		esize_t nbLine = m_buffer->getNumberOfLines();
		float nbLineCalc = nbLine;
		int32_t nbChar = 0;
		while (nbLineCalc >= 1.0f) {
			++nbChar;
			nbLineCalc /= 10.0f;
		}
		m_lastOffsetDisplay = tmpLetterSize.x() * (float)nbChar + 1.0f;
		m_displayText.setFontItalic((*m_paintingProperties)[m_colorLineNumber].getItalic());
		m_displayText.setFontBold((*m_paintingProperties)[m_colorLineNumber].getBold());
		m_displayText.setColorBg((*m_paintingProperties)[m_colorLineNumber].getBackground());
		m_displayText.setColor((*m_paintingProperties)[m_colorLineNumber].getForeground());
		m_displayText.setClippingMode(false);
		
		vec3 startWriteRealPosition = m_displayText.getPos();
		m_displayText.setPos(vec3(0.0f, startWriteRealPosition.y(), 0.0f));
		for (int32_t iii=startLineId;
		     iii<nbLine;
		     ++iii) {
			char tmpLineNumber[50];
			sprintf(tmpLineNumber, "%*d", nbChar, iii+1);
			m_displayText.print(tmpLineNumber);
			m_displayText.forceLineReturn();
			if (m_displayText.getPos().y() < -20.0f ) {
				break;
			}
		}
		m_displayText.setPos(vec3(-m_originScrooled.x()+m_lastOffsetDisplay, startWriteRealPosition.y(), 0.0f));
		m_displayText.setClipping(vec2(m_lastOffsetDisplay, 0), m_size);
	}
	appl::DisplayHLData displayLocalSyntax;
	m_buffer->hightlightGenerateLines(displayLocalSyntax, (esize_t)startingIt, m_size.y());
	float maxSizeX = 0;
	appl::HighlightInfo * HLColor = NULL;
	for (appl::Buffer::Iterator it = startingIt;
	     (bool)it == true;
	     ++it) {
		if (it == m_buffer->cursor()) {
			// need to display the cursor :
			tmpCursorPosition = m_displayText.getPos();
		}
		//APPL_DEBUG("display element '" << currentValue << "'at pos : " << m_displayText.getPos() );
		//APPL_DEBUG(" element size : " << iii << " : " << bufferElementSize);
		if (*it == etk::UChar::Return) {
			countNbLine += 1;
			countColomn = 0;
			maxSizeX = etk_max(m_displayText.getPos().x(), maxSizeX);
			// Display the end line position only if we have the focus ...
			if (getFocus() == true) {
				if (it >= selectPosStart && it < selectPosStop) {
					ewol::Drawing& draw = m_displayText.getDrawing();
					draw.setColor(etk::Color<>(0xFF0000FF));
					draw.setPos(m_displayText.getPos() + tmpLetterSize/4.0f);
					draw.rectangle(m_displayText.getPos() + tmpLetterSize*3.0f/4.0f);
				}
			}
			m_displayText.forceLineReturn();
			m_displayText.setPos(vec3(-m_originScrooled.x()+m_lastOffsetDisplay, m_displayText.getPos().y(), 0.0f));
			if (m_displayText.getPos().y() < -20.0f ) {
				break;
			}
			continue;
		}
		HLColor = m_buffer->getElementColorAtPosition(displayLocalSyntax, (esize_t)it);
		bool haveBackground = false;
		if (    HLColor != NULL
		     && HLColor->patern != NULL) {
			m_displayText.setColor(HLColor->patern->getColorGlyph().getForeground());
			m_displayText.setColorBg(HLColor->patern->getColorGlyph().getBackground());
			haveBackground = HLColor->patern->getColorGlyph().haveBackground();
			m_displayText.setFontItalic(HLColor->patern->getColorGlyph().getItalic());
			m_displayText.setFontBold(HLColor->patern->getColorGlyph().getBold());
		} else {
			m_displayText.setFontItalic((*m_paintingProperties)[m_colorNormal].getItalic());
			m_displayText.setFontBold((*m_paintingProperties)[m_colorNormal].getBold());
			m_displayText.setColorBg((*m_paintingProperties)[m_colorNormal].getBackground());
			m_displayText.setColor((*m_paintingProperties)[m_colorNormal].getForeground());
		}
		if (haveBackground == false) {
			if (*it == etk::UChar::Space) {
				m_displayText.setColorBg((*m_paintingProperties)[m_colorSpace].getForeground());
			} else if (*it == etk::UChar::Tabulation) {
				m_displayText.setColorBg((*m_paintingProperties)[m_colorTabulation].getForeground());
			}
		}
		m_buffer->expand(countColomn, *it, stringToDisplay);
		// Display selection only if we have the focus ...
		if (getFocus() == true) {
			if (it >= selectPosStart && it < selectPosStop) {
				m_displayText.setColor((*m_paintingProperties)[m_colorSelection].getForeground());
				m_displayText.setColorBg((*m_paintingProperties)[m_colorSelection].getBackground());
			}
		}
		//APPL_DEBUG("display : '" << currentValue << "'  == > '" << stringToDisplay << "'");
		m_displayText.print(stringToDisplay);
		countColomn += stringToDisplay.size();
	}
	maxSizeX = etk_max(m_displayText.getPos().x(), maxSizeX);
	// Display cursor only if we have the focus ...
	if (    tmpCursorPosition.z() != -1
	     && getFocus() == true) {
		// display the cursor:
		//APPL_DEBUG("display cursor at position : " << tmpCursorPosition);
		m_displayText.setPos(tmpCursorPosition);
		m_displayText.setColorBg((*m_paintingProperties)[m_colorCursor].getForeground());
		m_displayText.printCursor(m_insertMode);
	}
	// set maximum size (X&Y) :
	{
		vec3 tmpLetterSize = m_displayText.calculateSize((char32_t)'A');
		esize_t nbLines = m_buffer->getNumberOfLines();
		m_maxSize.setX(maxSizeX+m_originScrooled.x());
		m_maxSize.setY((float)nbLines*tmpLetterSize.y());
	}
	//toc("Display time : ");
	// call the herited class...
	WidgetScrooled::onRegenerateDisplay();
}

bool appl::TextViewer::onEventEntry(const ewol::EventEntry& _event) {
	if (m_buffer == NULL) {
		return false;
	}
	// First call plugin
	if (appl::textPluginManager::onEventEntry(*this, _event) == true) {
		markToRedraw();
		return true;
	}
	// just forward event  == > manage directly in the buffer
	if (_event.getType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.getStatus() != ewol::keyEvent::statusDown) {
			return false;
		}
		char32_t localValue = _event.getChar();
		if (localValue == etk::UChar::Return) {
			if (true == _event.getSpecialKey().isSetShift()) {
				localValue = etk::UChar::CarrierReturn;
			}
		} else if (localValue == etk::UChar::Suppress ) {
			//APPL_INFO("keyEvent : <suppr> pos=" << m_cursorPos);
			if (m_buffer->hasTextSelected()) {
				remove();
			} else {
				appl::Buffer::Iterator pos = m_buffer->cursor();
				appl::Buffer::Iterator posEnd = pos;
				++posEnd;
				replace("", pos, posEnd);
			}
			return true;
		} else if (localValue == etk::UChar::Delete) {
			//APPL_INFO("keyEvent : <del> pos=" << m_cursorPos);
			if (m_buffer->hasTextSelected()) {
				remove();
			} else {
				appl::Buffer::Iterator pos = m_buffer->cursor();
				appl::Buffer::Iterator posEnd = pos;
				--pos;
				replace("", pos, posEnd);
			}
			return true;
		}
		m_buffer->setSelectMode(false);
		// normal adding char ...
		char output[5];
		int32_t nbElement = etk::getUtf8(localValue, output);
		if (    m_buffer->hasTextSelected() == false
		     && _event.getSpecialKey().isSetInsert() == true) {
			appl::Buffer::Iterator pos = m_buffer->cursor();
			appl::Buffer::Iterator posEnd = pos;
			++posEnd;
			replace(output, pos, posEnd);
			//TODO : choisce UTF  ... replace(localValue, pos, posEnd);
		} else {
			std::string myString = output;
			write(myString);
		}
		return true;
	}
	// move events ...
	if (_event.getStatus() == ewol::keyEvent::statusDown) {
		bool needUpdatePosition = true;
		// selection when shift is set:
		m_buffer->setSelectMode(_event.getSpecialKey().isSetShift());
		// check selection event ...
		switch(_event.getType()) {
			case ewol::keyEvent::keyboardLeft:
				//APPL_INFO("keyEvent : <LEFT>");
				moveCursorLeft();
				break;
			case ewol::keyEvent::keyboardRight:
				//APPL_INFO("keyEvent : <RIGHT>");
				moveCursorRight();
				break;
			case ewol::keyEvent::keyboardUp:
				//APPL_INFO("keyEvent : <UP>");
				moveCursorUp(1);
				break;
			case ewol::keyEvent::keyboardDown:
				//APPL_INFO("keyEvent : <DOWN>");
				moveCursorDown(1);
				break;
			case ewol::keyEvent::keyboardPageUp:
				//APPL_INFO("keyEvent : <PAGE-UP>");
				moveCursorUp(15); // TODO : Set the real number of line ...
				break;
			case ewol::keyEvent::keyboardPageDown:
				//APPL_INFO("keyEvent : <PAGE-DOWN>");
				moveCursorDown(15); // TODO : Set the real number of line ...
				break;
			case ewol::keyEvent::keyboardStart:
				//APPL_INFO("keyEvent : <Start of line>");
				moveCursorLeft(moveEnd);
				break;
			case ewol::keyEvent::keyboardEnd:
				//APPL_INFO("keyEvent : <End of line>");
				moveCursorRight(moveEnd);
				break;
			default:
				break;
		}
		return true;
	}
	return false;
}

bool appl::TextViewer::onEventInput(const ewol::EventInput& _event) {
	if (_event.getId() != 0) {
		keepFocus();
	}
	//tic();
	if (m_buffer == NULL) {
		return false;
	}
	// First call the scrolling widget :
	if (WidgetScrooled::onEventInput(_event) == true) {
		markToRedraw();
		return true;
	}
	// Second call plugin
	if (appl::textPluginManager::onEventInput(*this, _event) == true) {
		markToRedraw();
		return true;
	}
	vec2 relativePos = relativePosition(_event.getPos());
	// offset for the lineNumber:
	relativePos -= vec2(m_lastOffsetDisplay, 0);
	// offset for the scrolling:
	relativePos += vec2(m_originScrooled.x(), -m_originScrooled.y());
	// invert for the buffer event ...
	relativePos.setY(m_size.y()-relativePos.y());
	if (relativePos.x()<0) {
		relativePos.setX(0);
	}
	if (    _event.getId() == 12
	     && _event.getStatus() == ewol::keyEvent::statusSingle) {
		APPL_DEBUG("kjhkjhkjh");
		// Rat5 save event
		sendMultiCast(ednMsgGuiSave, "current");
		return true;
	}
	// just forward event  == > manage directly in the buffer
	if (_event.getId() == 1) {
		// mouse selection :
		if (_event.getType() == ewol::keyEvent::typeMouse) {
			if (_event.getStatus() == ewol::keyEvent::statusDown) {
				//if (_event.getSpecialKey().isSetShift() == false) {
					appl::Buffer::Iterator newPos = getMousePosition(relativePos);
					moveCursor(newPos);
					m_buffer->setSelectMode(true);
					markToRedraw();
					return true;
				//}
			} else if (_event.getStatus() == ewol::keyEvent::statusUp) {
				appl::Buffer::Iterator newPos = getMousePosition(relativePos);
				moveCursor(newPos);
				m_buffer->setSelectMode(false);
				// Copy selection :
				std::string value;
				m_buffer->copy(value);
				if (value.size() != 0) {
					ewol::clipBoard::set(ewol::clipBoard::clipboardSelection, value);
				}
				markToRedraw();
				return true;
			}
		}
		if (_event.getStatus() == ewol::keyEvent::statusSingle) {
			if (_event.getType() == ewol::keyEvent::typeMouse) {
				appl::Buffer::Iterator newPos = getMousePosition(relativePos);
				moveCursor(newPos);
				markToRedraw();
				return true;
			}
		} else if (_event.getStatus() == ewol::keyEvent::statusDouble) {
			mouseEventDouble();
			markToRedraw();
			return true;
		} else if (_event.getStatus() == ewol::keyEvent::statusTriple) {
			mouseEventTriple();
			markToRedraw();
			return true;
		} else if (_event.getStatus() == ewol::keyEvent::statusMove) {
			if (m_buffer->getSelectMode() == true) {
				appl::Buffer::Iterator newPos = getMousePosition(relativePos);
				moveCursor(newPos);
				markToRedraw();
				return true;
			}
		}
	} else if (2 == _event.getId()) {
		if (ewol::keyEvent::statusSingle == _event.getStatus()) {
			appl::Buffer::Iterator newPos = getMousePosition(relativePos);
			moveCursor(newPos);
			ewol::clipBoard::request(ewol::clipBoard::clipboardSelection);
			markToRedraw();
			return true;
		}
	}
	return false;
}


void appl::TextViewer::mouseEventDouble(void) {
	//m_selectMode = false;
	appl::Buffer::Iterator beginPos, endPos;
	if (true == m_buffer->getPosAround(m_buffer->cursor(), beginPos, endPos)) {
		moveCursor(endPos);
		m_buffer->setSelectionPos(beginPos);
	}
	// TODO : copy(ewol::clipBoard::clipboardSelection);
}

void appl::TextViewer::mouseEventTriple(void) {
	//m_selectMode = false;
	moveCursor(m_buffer->getEndLine(m_buffer->cursor()));
	m_buffer->setSelectionPos(m_buffer->getStartLine(m_buffer->cursor()));
	// TODO : copy(ewol::clipBoard::clipboardSelection);
}

appl::Buffer::Iterator appl::TextViewer::getMousePosition(const vec2& _relativePos) {
	char32_t currentValue;
	vec3 positionCurentDisplay(0,0,0);
	vec3 tmpLetterSize = m_displayText.calculateSize((char32_t)'A');
	esize_t countColomn = 0;
	std::u32string stringToDisplay;
	m_displayText.clear();
	m_displayText.forceLineReturn();
	for (appl::Buffer::Iterator it = m_buffer->begin();
	     (bool)it == true;
	     ++it) {
		currentValue = *it;
		m_buffer->expand(countColomn, currentValue, stringToDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.size(); ++kkk) {
			if (stringToDisplay[kkk] == etk::UChar::Return) {
				// TODO : Remove this, use the automatic line manager ...
				m_displayText.forceLineReturn();
				countColomn = 0;
			} else {
				//note : Without this condithion the time od selection change to 0.6 ms to 8ms ...
				if (-_relativePos.y() >= positionCurentDisplay.y()) {
					m_displayText.print(stringToDisplay[kkk]);
				}
			}
		}
		if (-_relativePos.y() >= positionCurentDisplay.y()) {
			if (-_relativePos.y() < positionCurentDisplay.y()+tmpLetterSize.y()) {
				//APPL_DEBUG("line position : " << _textDrawer.getPos() << " " << positionCurentDisplay );
				if (    _relativePos.x() >= positionCurentDisplay.x()
				     && _relativePos.x() < m_displayText.getPos().x() ) {
					return it;
				}
			} else {
				return --it;
			}
		}
		positionCurentDisplay = m_displayText.getPos();
		countColomn += stringToDisplay.size();
	}
	return m_buffer->end();
}

void appl::TextViewer::onEventClipboard(enum ewol::clipBoard::clipboardListe _clipboardID) {
	if (m_buffer != NULL) {
		std::string data = ewol::clipBoard::get(_clipboardID);
		write(data);
	}
	markToRedraw();
}

void appl::TextViewer::onReceiveMessage(const ewol::EMessage& _msg) {
	widget::WidgetScrooled::onReceiveMessage(_msg);
	APPL_VERBOSE("receive msg: " << _msg);
	// First call plugin
	if (appl::textPluginManager::onReceiveMessage(*this, _msg) == true) {
		markToRedraw();
		return;
	}
	// event needed even if selection of buffer is not done ...
	if (_msg.getMessage() == appl::Buffer::eventIsModify) {
		markToRedraw();
		return;
	}
	if (_msg.getMessage() == appl::Buffer::eventSelectChange) {
		markToRedraw();
		return;
	}
	// If not the last buffer selected, then no event parsing ...
	if (isSelectedLast() == false) {
		return;
	}
	if (_msg.getMessage() == appl::MsgSelectNewFile) {
		if (m_buffer != NULL) {
			m_buffer->unRegisterOnEvent(this);
		}
		m_buffer = m_bufferManager->get(_msg.getData());
		if (m_buffer != NULL) {
			m_buffer->registerOnEvent(this, appl::Buffer::eventIsModify);
			m_buffer->registerOnEvent(this, appl::Buffer::eventSelectChange);
		}
		if (m_bufferManager != NULL) {
			m_bufferManager->setBufferSelected(m_buffer);
		}
		markToRedraw();
		return;
	}
}

void appl::TextViewer::onObjectRemove(ewol::EObject* _removeObject) {
	widget::WidgetScrooled::onObjectRemove(_removeObject);
	if (m_buffer == _removeObject) {
		m_buffer = NULL;
		markToRedraw();
	}
}

void appl::TextViewer::onGetFocus(void) {
	showKeyboard();
	APPL_INFO("Focus - In");
	setCurrentSelect();
	markToRedraw();
}

void appl::TextViewer::onLostFocus(void) {
	hideKeyboard();
	APPL_INFO("Focus - out");
	markToRedraw();
}

void appl::TextViewer::setFontSize(int32_t _size) {
	m_displayText.setFontSize(_size);
	setScrollingSize(_size*3.0*1.46); // 1.46 is a magic number ...
}

void appl::TextViewer::setFontName(const std::string& _fontName) {
	m_displayText.setFontName(_fontName);
}

void appl::TextViewer::updateScrolling(void) {
	if (m_buffer == NULL) {
		return;
	}
	vec2 realCursorPosition(0,0);
	uint32_t lineId = m_buffer->getCursorLinesId();
	m_displayText.clear();
	m_displayText.forceLineReturn();
	float lineSize = -m_displayText.getPos().y();
	for (size_t iii=0; iii<lineId; ++iii) {
		m_displayText.forceLineReturn();
	}
	realCursorPosition.setY(-m_displayText.getPos().y());
	realCursorPosition.setX(getScreenSize(m_buffer->getStartLine(m_buffer->cursor())+1, m_buffer->cursor())-10);
	APPL_VERBOSE("position=" << realCursorPosition << " scrool=" << m_originScrooled << " size" << m_size);
	if (realCursorPosition.x() < m_originScrooled.x()-lineSize*2.0f) {
		m_originScrooled.setX(realCursorPosition.x()-lineSize*2.0f);
	} else if (realCursorPosition.x() > m_originScrooled.x()+m_size.x()-lineSize*2.0f-10) {
		m_originScrooled.setX(realCursorPosition.x()-m_size.x()+lineSize*2.0f+10);
	}
	if (realCursorPosition.y() < m_originScrooled.y()+lineSize*2.0f) {
		m_originScrooled.setY(realCursorPosition.y()-lineSize*2.0f);
	} else if (realCursorPosition.y() > m_originScrooled.y()+m_size.y()-lineSize*2.0f) {
		m_originScrooled.setY(realCursorPosition.y()-m_size.y()+lineSize*2.0f);
	}
	realCursorPosition.setMin(vec2(0,0));
	
}

bool appl::TextViewer::moveCursor(const appl::Buffer::Iterator& _pos) {
	if (m_buffer == NULL) {
		return false;
	}
	markToRedraw();
	if (appl::textPluginManager::onCursorMove(*this, _pos) == true) {
		updateScrolling();
		return true;
	}
	m_buffer->moveCursor((esize_t)_pos);
	updateScrolling();
	return true;
}

bool appl::TextViewer::write(const std::string& _data) {
	if (m_buffer == NULL) {
		return false;
	}
	if (m_buffer->hasTextSelected() == true) {
		return replace(_data);
	}
	return write(_data, m_buffer->cursor());
}

bool appl::TextViewer::write(const std::string& _data, const appl::Buffer::Iterator& _pos) {
	if (m_buffer == NULL) {
		return false;
	}
	markToRedraw();
	if (appl::textPluginManager::onWrite(*this, _pos, _data) == true) {
		// no call of the move cursor, because pluging might call theses function to copy and cut data...
		return true;
	}
	bool ret = m_buffer->write(_data, _pos);
	appl::textPluginManager::onCursorMove(*this, m_buffer->cursor());
	return ret;
}

bool appl::TextViewer::replace(const std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
	if (m_buffer == NULL) {
		return false;
	}
	markToRedraw();
	if (appl::textPluginManager::onReplace(*this, _pos, _data, _posEnd) == true) {
		// no call of the move cursor, because pluging might call theses function to copy and cut data...
		return true;
	}
	bool ret = m_buffer->replace(_data, _pos, _posEnd);
	appl::textPluginManager::onCursorMove(*this, m_buffer->cursor());
	return ret;
}

bool appl::TextViewer::replace(const std::string& _data) {
	if (m_buffer == NULL) {
		return false;
	}
	if (m_buffer->hasTextSelected() == false) {
		return write(_data);
	}
	return replace(_data, m_buffer->selectStart(), m_buffer->selectStop());
}

void appl::TextViewer::remove(void) {
	if (m_buffer == NULL) {
		return;
	}
	if (m_buffer->hasTextSelected() == false) {
		// nothing to do ...
		return;
	}
	markToRedraw();
	if (appl::textPluginManager::onRemove(*this, m_buffer->selectStart(), m_buffer->selectStop()) == true) {
		return;
	}
	m_buffer->removeSelection();
	appl::textPluginManager::onCursorMove(*this, m_buffer->cursor());
}



void appl::TextViewer::moveCursorRight(appl::TextViewer::moveMode _mode) {
	if (m_buffer == NULL) {
		return;
	}
	markToRedraw();
	appl::Buffer::Iterator it;
	switch (_mode) {
		default:
		case moveLetter:
			it = m_buffer->cursor();
			++it;
			moveCursor(it);
			break;
		case moveWord:
			// TODO : ...
			break;
		case moveEnd:
			it = m_buffer->getEndLine(m_buffer->cursor());
			moveCursor(it);
			break;
	}
}

void appl::TextViewer::moveCursorLeft(appl::TextViewer::moveMode _mode) {
	if (m_buffer == NULL) {
		return;
	}
	markToRedraw();
	appl::Buffer::Iterator it;
	switch (_mode) {
		default:
		case moveLetter:
			it = m_buffer->cursor();;
			--it;
			moveCursor(it);
			break;
		case moveWord:
			// TODO : ...
			break;
		case moveEnd:
			it = m_buffer->getStartLine(m_buffer->cursor());
			moveCursor(++it);
			break;
	}
}

void appl::TextViewer::moveCursorUp(esize_t _nbLine) {
	if (m_buffer == NULL) {
		return;
	}
	markToRedraw();
	// find the position of the start of the line.
	appl::Buffer::Iterator lineStartPos = m_buffer->getStartLine(m_buffer->cursor());
	// check if we can go up ...
	if (lineStartPos == m_buffer->begin()) {
		return;
	}
	// Decide what column to move to, if there's a preferred column use that
	if (m_buffer->getFavoriteUpDownPos() < 0) {
		// TODO : Remove this +1 !!!
		m_buffer->setFavoriteUpDownPos(getScreenSize(lineStartPos+1, m_buffer->cursor()));
	}
	EWOL_DEBUG("move_up : " << m_buffer->getFavoriteUpDownPos());
	// get the previous line
	appl::Buffer::Iterator prevLineStartPos = m_buffer->countBackwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move line UP result : prevLineStartPos=" << prevLineStartPos);
	// get the display char position
	appl::Buffer::Iterator newPos = getPosSize(prevLineStartPos, m_buffer->getFavoriteUpDownPos());
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	float posStore = m_buffer->getFavoriteUpDownPos();
	moveCursor(newPos);
	m_buffer->setFavoriteUpDownPos(posStore);
}

void appl::TextViewer::moveCursorDown(esize_t _nbLine) {
	if (m_buffer == NULL) {
		return;
	}
	markToRedraw();
	// check if we are not at the end of Buffer
	if (m_buffer->cursor() == m_buffer->end() ) {
		return;
	}
	// find the position of the start of the line.
	appl::Buffer::Iterator lineStartPos = m_buffer->getStartLine(m_buffer->cursor());
	
	if (m_buffer->getFavoriteUpDownPos() < 0) {
		// TODO : Remove this +1 !!!
		m_buffer->setFavoriteUpDownPos(getScreenSize(lineStartPos+1, m_buffer->cursor()));
	}
	EWOL_DEBUG("move down : " << m_buffer->getFavoriteUpDownPos());
	// get the next line :
	appl::Buffer::Iterator nextLineStartPos = m_buffer->countForwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move line DOWN result : nextLineStartPos=" << nextLineStartPos);
	// get the display char position
	appl::Buffer::Iterator newPos = getPosSize(nextLineStartPos, m_buffer->getFavoriteUpDownPos());
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	float posStore = m_buffer->getFavoriteUpDownPos();
	moveCursor(newPos);
	m_buffer->setFavoriteUpDownPos(posStore);
}

// TODO : Rename ...
appl::Buffer::Iterator appl::TextViewer::getPosSize(const appl::Buffer::Iterator& _startLinePos, float _distance) {
	char32_t currentValue;
	esize_t countColomn = 0;
	std::u32string stringToDisplay;
	m_displayText.clear();
	m_displayText.forceLineReturn();
	for (appl::Buffer::Iterator it = _startLinePos;
	     (bool)it == true;
	     ++it) {
		currentValue = *it;
		m_buffer->expand(countColomn, currentValue, stringToDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.size(); ++kkk) {
			if (stringToDisplay[kkk] == etk::UChar::Return) {
				return it;
			} else {
				m_displayText.print(stringToDisplay[kkk]);
			}
		}
		if (m_displayText.getPos().x() >= _distance) {
			return it;
		}
		countColomn += stringToDisplay.size();
	}
	return m_buffer->end();
}

// TODO : Rename ...
float appl::TextViewer::getScreenSize(const appl::Buffer::Iterator& _startLinePos, const appl::Buffer::Iterator& _stopPos) {
	float ret = 0;
	char32_t currentValue;
	esize_t countColomn = 0;
	std::u32string stringToDisplay;
	m_displayText.clear();
	
	for (appl::Buffer::Iterator it = _startLinePos;
	     (bool)it == true && it <= _stopPos;
	     ++it) {
		currentValue = *it;
		//APPL_DEBUG("parse : " << currentValue);
		m_buffer->expand(countColomn, currentValue, stringToDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.size(); ++kkk) {
			if (stringToDisplay[kkk] == etk::UChar::Return) {
				return m_displayText.getPos().x() + 2; // TODO : Add the +2 for the end of line ...
			} else {
				m_displayText.print(stringToDisplay[kkk]);
			}
		}
		ret = m_displayText.getPos().x();
		countColomn += stringToDisplay.size();
	}
	return ret;
}

appl::TextViewer* appl::TextViewer::m_currentBufferSelect = NULL;

void appl::TextViewer::setCurrentSelect(void) {
	if (this == m_currentBufferSelect) {
		return;
	}
	m_currentBufferSelect = this;
	if (m_bufferManager != NULL) {
		m_bufferManager->setBufferSelected(m_buffer);
	}
}

bool appl::TextViewer::isSelectedLast(void) {
	if (this == m_currentBufferSelect) {
		return true;
	}
	return false;
}
