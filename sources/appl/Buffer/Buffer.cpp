/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer/Buffer.h>
#include <appl/Debug.h>
#include <ewol/clipBoard.h>

appl::Buffer::Iterator& appl::Buffer::Iterator::operator++ (void) {
	if (    m_data != NULL
	     && m_current < m_data->m_data.size() ) {
		int8_t nbChar = etk::UChar::theoricUTF8Len(m_data->m_data[m_current]);
		if (m_current+nbChar >= m_data->m_data.size()) {
			return *this;
		}
		m_current+=nbChar;
	}
	return *this;
}

appl::Buffer::Iterator& appl::Buffer::Iterator::operator-- (void) {
	if (    m_data != NULL
	     && m_current > 0) {
		int32_t iii = -1;
		while(    etk::UChar::theoricUTF8First(m_data->m_data[m_current+iii]) == false
		       && iii >= -6
		       && m_current-iii>0) {
			++iii;
		};
		m_current += iii;
	}
	return *this;
}

etk::UChar appl::Buffer::Iterator::operator* (void) const {
	etk::UChar retVal = '\0';
	APPL_CHECK_INOUT(m_current < m_data->m_data.size());
	char tmpVal[5];
	memset(tmpVal, 0, sizeof(tmpVal));
	tmpVal[0] = m_data->m_data[m_current];
	int8_t nbChar = etk::UChar::theoricUTF8Len(tmpVal[0]);
	for (int32_t iii=1; iii<nbChar && m_current+iii<m_data->m_data.size(); ++iii) {
		tmpVal[iii] = m_data->m_data[m_current+iii];
	}
	// transform ...
	retVal.setUtf8(tmpVal);
	return retVal;
}


appl::Buffer::Iterator appl::Buffer::position(esize_t _pos) {
	return appl::Buffer::Iterator(this, _pos);
}

appl::Buffer::Iterator appl::Buffer::begin(void) {
	return position(0);
}

appl::Buffer::Iterator appl::Buffer::end(void) {
	// TODO : chek the validity of the char ...
	return position( m_data.size()-1 );
}



appl::Buffer::Buffer(void) :
  m_cursorPos(0),
  m_cursorSelectPos(-1),
  m_cursorPreferredCol(-1) {
	
}

bool appl::Buffer::loadFile(const etk::UString& _name) {
	APPL_DEBUG("Load file : '" << _name << "'");
	m_fileName = _name;
	etk::FSNode file(m_fileName);
	if (file.exist() == false) {
		return false;
	}
	if (true == m_data.dumpFrom(file) ) {
		return true;
	}
	return false;
}

void appl::Buffer::setFileName(const etk::UString& _name) {
	// TODO : ...
}

void appl::Buffer::moveCursorRight(appl::Buffer::moveMode _mode) {
	Iterator it;
	esize_t nbElement;
	switch (_mode) {
		default:
		case moveLetter:
			it = position(m_cursorPos);
			++it;
			moveCursor(it.getPos());
			break;
		case moveWord:
			// TODO : ...
			break;
		case moveEnd:
			// TODO : ...
			nbElement = endLine(m_cursorPos);
			moveCursor(nbElement);
			break;
	}
	
}

void appl::Buffer::moveCursorLeft(appl::Buffer::moveMode _mode) {
	etk::UChar value;
	Iterator it;
	esize_t nbElement;
	switch (_mode) {
		default:
		case moveLetter:
			it = position(m_cursorPos);
			--it;
			moveCursor(it.getPos());
			break;
		case moveWord:
			// TODO : ...
			break;
		case moveEnd:
			nbElement = startLine(m_cursorPos);
			moveCursor(nbElement+1);
			break;
	}
}

void appl::Buffer::moveCursorUp(esize_t _nbLine, ewol::Text& _textDrawer) {
	// find the position of the start of the line.
	esize_t lineStartPos = startLine(m_cursorPos);
	// check if we can go up ...
	if (lineStartPos == 0) {
		return;
	}
	// Decide what column to move to, if there's a preferred column use that
	if (m_cursorPreferredCol < 0) {
		// TODO : Remove this +1 !!!
		m_cursorPreferredCol = getScreenSize(lineStartPos+1, m_cursorPos, _textDrawer);
	}
	EWOL_DEBUG("ploop : " << m_cursorPreferredCol);
	// get the previous line
	esize_t prevLineStartPos = countBackwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move line UP result : prevLineStartPos=" << prevLineStartPos);
	// get the display char position
	esize_t newPos = getPosSize(prevLineStartPos, m_cursorPreferredCol, _textDrawer);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	float posStore = m_cursorPreferredCol;
	moveCursor(newPos);
	m_cursorPreferredCol = posStore;
}

void appl::Buffer::moveCursorDown(esize_t _nbLine, ewol::Text& _textDrawer) {
	// check if we are not at the end of Buffer
	if (m_cursorPos == m_data.size() ) {
		return;
	}
	// find the position of the start of the line.
	esize_t lineStartPos = startLine(m_cursorPos);
	
	if (m_cursorPreferredCol < 0) {
		// TODO : Remove this +1 !!!
		m_cursorPreferredCol = getScreenSize(lineStartPos+1, m_cursorPos, _textDrawer);
	}
	EWOL_DEBUG("ploop : " << m_cursorPreferredCol);
	// get the next line :
	esize_t nextLineStartPos = countForwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move line DOWN result : nextLineStartPos=" << nextLineStartPos);
	// get the display char position
	esize_t newPos = getPosSize(nextLineStartPos, m_cursorPreferredCol, _textDrawer);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	float posStore = m_cursorPreferredCol;
	moveCursor(newPos);
	m_cursorPreferredCol = posStore;
}

esize_t appl::Buffer::startLine(esize_t _pos) {
	esize_t startPos;
	if (false == searchBack(_pos, etk::UChar::Return, startPos)) {
		return 0;
	}
	return startPos;
}

esize_t appl::Buffer::endLine(esize_t _pos) {
	esize_t endPos;
	if (false == search(_pos, etk::UChar::Return, endPos)) {
		endPos = m_data.size();
	}
	return endPos;
}

bool appl::Buffer::search(esize_t _pos, const etk::UChar& _search, esize_t& _result) {
	// move in the string
	esize_t nbElementBuffer = 0;
	etk::UChar value;
	for (Iterator it = position(m_cursorPos);
	     it != end();
	     ++it) {
		if (*it == _search) {
			_result = it;
			return true;
		}
	}
	_result = end();
	return false;
}

bool appl::Buffer::searchBack(esize_t _pos, const etk::UChar& _search, esize_t& _result) {
	// move in the string
	esize_t nbElementBuffer = 0;
	etk::UChar value;
	for (Iterator it = --position(m_cursorPos);
	     it != begin();
	     --it) {
		//APPL_DEBUG("compare : " << *it << " ?= " << _search);
		if (*it == _search) {
			//APPL_DEBUG("find : " << (esize_t)it);
			_result = it;
			return true;
		}
	}
	_result = 0;
	return false;
}


// TODO : vec2 _displaySize
bool appl::Buffer::onEventEntry(const ewol::EventEntry& _event, ewol::Text& _testDrawer) {
	//APPL_DEBUG(" event : " << _event);
	if (_event.getType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.getStatus() != ewol::keyEvent::statusDown) {
			return false;
		}
		etk::UChar localValue = _event.getChar();
		if (localValue == etk::UChar::Tabulation) {
			if (hasTextSelected()) {
				// TODO : Special tabulation multiline indentation ...
				/*
				int32_t nbSelectedLines = m_EdnBuf.CountLines(SelectionStart, SelectionEnd);
				if (1 < nbSelectedLines) {
					if (true == _event.getSpecialKey().isSetShift() ) {
						m_cursorPos = m_EdnBuf.UnIndent();
					} else {
						m_cursorPos = m_EdnBuf.Indent();
					}
				}
				*/
				return true;
			}
		} else if (localValue == etk::UChar::Return) {
			if (true == _event.getSpecialKey().isSetShift()) {
				localValue = etk::UChar::CarrierReturn;
			} else {
				/*
				m_data.insert(m_cursorPos, '\n');
				if (true == globals::isSetAutoIndent() ) {
					int32_t l_lineStart;
					// get the begin of the line or the begin of the line befor selection
					if (false == haveSelectionActive) {
						l_lineStart = m_EdnBuf.StartOfLine(m_cursorPos);
					} else {
						l_lineStart = m_EdnBuf.StartOfLine(SelectionStart);
					}
					// add same characters in the temporar buffer
					for (int32_t kk=l_lineStart; kk<m_cursorPos; kk++) {
						if (' ' == m_EdnBuf[kk]) {
							tmpVect.pushBack(' ');
						} else if('\t' == m_EdnBuf[kk]) {
							tmpVect.pushBack('\t');
						} else {
							break;
						}
					}
				}
				m_selectMode = false;
				moveCursor(m_cursorPos + 1);
				return true;
				*/
			}
		} else if (localValue == etk::UChar::Suppress ) {
			//APPL_INFO("keyEvent : <suppr> pos=" << m_cursorPos);
			if (hasTextSelected()) {
				removeSelection();
			} else {
				int32_t dimention = (esize_t)(++position(m_cursorPos)) - m_cursorPos;
				if (dimention > 0) {
					m_data.remove(m_cursorPos, dimention);
				}
			}
			return true;
		} else if (localValue == etk::UChar::Delete) {
			//APPL_INFO("keyEvent : <del> pos=" << m_cursorPos);
			if (hasTextSelected()) {
				removeSelection();
			} else {
				int32_t dimention = m_cursorPos - (esize_t)(--position(m_cursorPos));
				if (dimention > 0) {
					m_data.remove(m_cursorPos-dimention, dimention);
				}
			}
			return true;
		}
		m_selectMode = false;
		// normal adding char ...
		char output[5];
		int32_t nbElement = localValue.getUtf8(output);
		if (    hasTextSelected() == false
		     && _event.getSpecialKey().isSetInsert() == true) {
			int32_t dimention = (esize_t)(++position(m_cursorPos)) - m_cursorPos;
			m_data.replace(m_cursorPos, dimention, (int8_t*)output, nbElement);
			moveCursor(m_cursorPos+nbElement);
		} else {
			etk::UString myString = output;
			paste(myString);
		}
		return true;
	}
	// move events ...
	if (_event.getStatus() == ewol::keyEvent::statusDown) {
		bool needUpdatePosition = true;
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
				moveCursorUp(1, _testDrawer);
				break;
			case ewol::keyEvent::keyboardDown:
				//APPL_INFO("keyEvent : <DOWN>");
				moveCursorDown(1, _testDrawer);
				break;
			case ewol::keyEvent::keyboardPageUp:
				//APPL_INFO("keyEvent : <PAGE-UP>");
				//TextDMoveUp(m_displaySize.y());
				break;
			case ewol::keyEvent::keyboardPageDown:
				//APPL_INFO("keyEvent : <PAGE-DOWN>");
				//TextDMoveDown(m_displaySize.y());
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
		/*
		if ( true == needUpdatePosition) {
			RequestUpdateOfThePosition();
		}
		*/
		return true;
	}
	return false;
}

bool appl::Buffer::onEventInput(const ewol::EventInput& _event, ewol::Text& _textDrawer, const vec2& _relativePos)
{
	if (_event.getId() == 1) {
		// mouse selection :
		if (_event.getType() == ewol::keyEvent::typeMouse) {
			if (_event.getStatus() == ewol::keyEvent::statusDown) {
				esize_t newPos = getMousePosition(_relativePos, _textDrawer);
				moveCursor(newPos);
				m_selectMode = true;
				return true;
			} else if (_event.getStatus() == ewol::keyEvent::statusUp) {
				esize_t newPos = getMousePosition(_relativePos, _textDrawer);
				moveCursor(newPos);
				m_selectMode = false;
				// TODO : Copy selection :
				//tmpBuffer->Copy(ewol::clipBoard::clipboardSelection);
				return true;
			}
		}
		if (_event.getStatus() == ewol::keyEvent::statusSingle) {
			if (_event.getType() == ewol::keyEvent::typeMouse) {
				esize_t newPos = getMousePosition(_relativePos, _textDrawer);
				moveCursor(newPos);
				return true;
			}
		} else if (_event.getStatus() == ewol::keyEvent::statusDouble) {
			mouseEventDouble();
			return true;
		} else if (_event.getStatus() == ewol::keyEvent::statusTriple) {
			mouseEventTriple();
			return true;
		} else if (_event.getStatus() == ewol::keyEvent::statusMove) {
			if (m_selectMode == true) {
				esize_t newPos = getMousePosition(_relativePos, _textDrawer);
				moveCursor(newPos);
				return true;
			}
		}
	} else if (2 == _event.getId()) {
		if (ewol::keyEvent::statusSingle == _event.getStatus()) {
			esize_t newPos = getMousePosition(_relativePos, _textDrawer);
			moveCursor(newPos);
			ewol::clipBoard::request(ewol::clipBoard::clipboardSelection);
		}
	}
	return false;
}

void appl::Buffer::moveCursor(esize_t _pos) {
	m_cursorPreferredCol = -1;
	// selecting mode ...
	if (m_selectMode == true) {
		if (m_cursorSelectPos == -1) {
			m_cursorSelectPos = m_cursorPos;
		}
		//APPL_DEBUG("Select : " << m_cursorSelectPos << " ==> " << newPos);
		m_cursorPos = _pos;
		if (m_cursorPos == m_cursorSelectPos) {
			m_cursorSelectPos = -1;
		}
		return;
	}
	// move mode
	m_cursorPos = _pos;
	m_cursorSelectPos = -1;
}

bool appl::Buffer::selectAround(int32_t _startPos, int32_t &_beginPos, int32_t &_endPos) {
	etk::UChar currentValue = *position(_startPos);
	_beginPos = 0;
	_endPos = end();
	if (    currentValue == etk::UChar::Tabulation
	     || currentValue == etk::UChar::Space) {
		APPL_DEBUG("select spacer");
		// Search back
		for (Iterator it = --position(_startPos);
		     it != begin();
		     --it) {
			currentValue = *it;
			if (    currentValue != etk::UChar::Tabulation
			     && currentValue != etk::UChar::Space) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = position(_startPos);
		     it != end();
		     ++it) {
			currentValue = *it;
			if (    currentValue != etk::UChar::Tabulation
			     && currentValue != etk::UChar::Space) {
				_endPos = it;
				break;
			}
		}
		return true;
	} else if( false == currentValue.isSpecialChar()){
		APPL_DEBUG("select normal Char");
		// Search back
		for (Iterator it = --position(_startPos);
		     it != begin();
		     --it) {
			currentValue = *it;
			if (    currentValue != '_'
			     && true == currentValue.isSpecialChar()) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = position(_startPos);
		     it != end();
		     ++it) {
			currentValue = *it;
			if (    currentValue != '_'
			     && true == currentValue.isSpecialChar()) {
				_endPos = it;
				break;
			}
		}
		return true;
	} else {
		APPL_DEBUG("select same char");
		etk::UChar comparechar = currentValue;
		// Search back
		for (Iterator it = --position(_startPos);
		     it != begin();
		     --it) {
			currentValue = *it;
			if (comparechar != currentValue) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = position(_startPos);
		     it != end();
		     ++it) {
			currentValue = *it;
			if (comparechar != currentValue) {
				_endPos = it;
				break;
			}
		}
		return true;
	}
	_beginPos = 0;
	_endPos = 0;
	return false;
}

void appl::Buffer::mouseEventDouble(void) {
	//m_selectMode = false;
	esize_t beginPos, endPos;
	if (true == selectAround(m_cursorPos, beginPos, endPos)) {
		moveCursor(endPos);
		m_cursorSelectPos = beginPos;
	}
	// TODO : copy(ewol::clipBoard::clipboardSelection);
}

void appl::Buffer::mouseEventTriple(void) {
	//m_selectMode = false;
	moveCursor(endLine(m_cursorPos));
	m_cursorSelectPos = startLine(m_cursorPos);
	// TODO : copy(ewol::clipBoard::clipboardSelection);
}

// TODO : Rename ...
esize_t appl::Buffer::getPosSize(esize_t _startLinePos, float _distance, ewol::Text& _textDrawer)
{
	esize_t bufferElementSize;
	etk::UChar currentValue;
	esize_t countColomn = 0;
	etk::UString stringToDisplay;
	_textDrawer.clear();
	_textDrawer.forceLineReturn();
	for (Iterator it = position(_startLinePos);
	     it != end();
	     ++it) {
		currentValue = *it;
		expand(countColomn, currentValue, stringToDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.size(); ++kkk) {
			if (stringToDisplay[kkk] == etk::UChar::Return) {
				return it;
			} else {
				_textDrawer.print(stringToDisplay[kkk]);
			}
		}
		if (_textDrawer.getPos().x() >= _distance) {
			return it;
		}
		countColomn += stringToDisplay.size();
	}
	return end();
}

// TODO : Rename ...
float appl::Buffer::getScreenSize(esize_t _startLinePos, esize_t _stopPos, ewol::Text& _textDrawer)
{
	float ret = 0;
	etk::UChar currentValue;
	esize_t countColomn = 0;
	etk::UString stringToDisplay;
	_textDrawer.clear();
	
	for (Iterator it = position(_startLinePos);
	     it != end();
	     ++it) {
		currentValue = *it;
		//APPL_DEBUG("parse : " << currentValue);
		expand(countColomn, currentValue, stringToDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.size(); ++kkk) {
			if (stringToDisplay[kkk] == etk::UChar::Return) {
				return _textDrawer.getPos().x() + 2; // TODO : Add the +2 for the end of line ...
			} else {
				_textDrawer.print(stringToDisplay[kkk]);
			}
		}
		ret = _textDrawer.getPos().x();
		countColomn += stringToDisplay.size();
	}
	return ret;
}

// TODO : Rename && rework ...
esize_t appl::Buffer::getMousePosition(const vec2& _relativePos, ewol::Text& _textDrawer) {
	etk::UChar currentValue;
	vec3 positionCurentDisplay(0,0,0);
	vec3 tmpLetterSize = _textDrawer.calculateSize((etk::UChar)'A');
	esize_t countColomn = 0;
	etk::UString stringToDisplay;
	_textDrawer.clear();
	_textDrawer.forceLineReturn();
	for (Iterator it = begin();
	     it != end();
	     ++it) {
		currentValue = *it;
		expand(countColomn, currentValue, stringToDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.size(); ++kkk) {
			if (stringToDisplay[kkk] == etk::UChar::Return) {
				// TODO : Remove this, use the automatic line manager ...
				_textDrawer.forceLineReturn();
				countColomn = 0;
			} else {
				_textDrawer.print(stringToDisplay[kkk]);
			}
		}
		if (-_relativePos.y() >= positionCurentDisplay.y()) {
			if (-_relativePos.y() < positionCurentDisplay.y()+tmpLetterSize.y()) {
				//APPL_DEBUG("line position : " << _textDrawer.getPos() << " " << positionCurentDisplay );
				if (    _relativePos.x() >= positionCurentDisplay.x()
				     && _relativePos.x() < _textDrawer.getPos().x() ) {
					return it;
				}
			} else {
				return --it;
			}
		}
		positionCurentDisplay = _textDrawer.getPos();
		countColomn += stringToDisplay.size();
	}
	
	return end();
}

static const char *ControlCodeTable[32] = {
	 "NUL", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs",  "ht", "nl",  "vt",  "np", "cr", "so", "si",
	 "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};

void appl::Buffer::expand(esize_t& _indent, const etk::UChar& _value, etk::UString& _out) const {
	_out.clear();
	int32_t tabDist = 4;
	if (_value == etk::UChar::Tabulation) {
		int32_t nSpaces = tabDist - (_indent % tabDist);
		for (int32_t iii=0; iii<nSpaces; iii++) {
			_out.append(etk::UChar::Space);
		}
		return;
	}
	// convert ASCII control codes to readable character sequences
	if (_value == etk::UChar::Null) {
		_out.append(etk::UChar('<'));
		_out.append(etk::UChar('n'));
		_out.append(etk::UChar('u'));
		_out.append(etk::UChar('l'));
		_out.append(etk::UChar('>'));
		return;
	}
	if (_value == etk::UChar::Return) {
		// nothing to display...
		_out.append(etk::UChar::Return);
		return;
	}
	if (_value.get() <= 31) {
		_out.append(etk::UChar('<'));
		const char * tmp = ControlCodeTable[_value.get()];
		while (*tmp!='\0') {
			_out.append(etk::UChar(*tmp));
			tmp++;
		}
		_out.append(etk::UChar('>'));
		return;
	}
	if (_value == etk::UChar::Delete) {
		_out.append(etk::UChar('<'));
		_out.append(etk::UChar('d'));
		_out.append(etk::UChar('e'));
		_out.append(etk::UChar('l'));
		_out.append(etk::UChar('>'));
		return;
	}
	// nothing to do ...
	_out.append(_value);
	//APPL_DEBUG("plop : " << _out);
}

// TODO : No more used !!!
int32_t appl::Buffer::countDispChars(esize_t _startPos, esize_t _posEnd) {
	int32_t charCount = 0;
	etk::UString expanded;
	etk::UChar value;
	for (Iterator it = position(_startPos);
	     it != end();
	     ++it) {
		value = *it;
		expand(charCount, value, expanded);
		charCount += expanded.size();
	}
	return charCount;
}

// TODO : No more used !!!
esize_t appl::Buffer::countForwardDispChars(esize_t _startPos, int32_t _nChars) {
	int32_t charCount = 0;
	etk::UString expanded;
	etk::UChar value;
	for (Iterator it = position(_startPos);
	     it != end();
	     ++it) {
		value = *it;
		if (value == etk::UChar::Return) {
			return it;
		}
		expand(charCount, value, expanded);
		charCount += expanded.size();
	}
	return end();
}

esize_t appl::Buffer::countForwardNLines(esize_t _startPos, int32_t _nLines) {
	if (_nLines <= 0) {
		return _startPos;
	} else if (_startPos > m_data.size() ) {
		return end();
	}
	etk::UChar value;
	int32_t lineCount = 0;
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	for (Iterator it = ++position(_startPos);
	     it != end();
	     ++it) {
		value = *it;
		if (value == etk::UChar::Return) {
			lineCount++;
			if (lineCount == _nLines) {
				//APPL_INFO("    == > (1) at position=" << myPosIt.Position()+1 );
				return (esize_t)it + 1;
			}
		}
	}
	//APPL_INFO("    == > (2) at position=" << myPosIt.Position() );
	return end();
}

esize_t appl::Buffer::countBackwardNLines(esize_t _startPos, int32_t _nLines) {
	if (_startPos <= 0) {
		return 0;
	} else if (_startPos > m_data.size() ) {
		_startPos = m_data.size();
	}
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	etk::UChar value;
	int32_t lineCount = 0;
	for (Iterator it = --position(_startPos);
	     it != begin();
	     --it) {
		value = *it;
		if (value == etk::UChar::Return) {
			lineCount++;
			if (lineCount >= _nLines) {
				//APPL_INFO("    == > (1) at position=" << myPosIt.Position()+1 );
				return ++it;
			}
		}
	}
	//APPL_INFO("    == > (2) at position=0");
	return begin();
}



bool appl::Buffer::copy(etk::UString& _data) {
	_data.clear();
	if (hasTextSelected() == true) {
		esize_t startPos = getStartSelectionPos();
		esize_t endPos = getStopSelectionPos();
		for (Iterator it = position(startPos);
		     it != position(endPos) &&
		     it != end();
		     ++it) {
			_data += *it;
		}
		return true;
	}
	return false;
}

bool appl::Buffer::paste(const etk::UString& _data) {
	etk::Char output = _data.c_str();
	if (hasTextSelected() == true) {
		esize_t startPos = getStartSelectionPos();
		esize_t endPos = getStopSelectionPos();
		m_data.replace(m_cursorPos, endPos-startPos, (int8_t*)((void*)output), output.size());
	} else {
		m_data.insert(m_cursorPos, (int8_t*)((void*)output), output.size());
	}
	m_selectMode = false;
	moveCursor(m_cursorPos+output.size());
	return false;
}

void appl::Buffer::removeSelection(void) {
	if (hasTextSelected() == true) {
		esize_t startPos = getStartSelectionPos();
		esize_t endPos = getStopSelectionPos();
		m_data.remove(startPos, endPos-startPos);
		m_selectMode = false;
		moveCursor(startPos);
	}
}


