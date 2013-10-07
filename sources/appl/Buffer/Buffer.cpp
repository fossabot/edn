/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer/Buffer.h>
#include <appl/Debug.h>

appl::Buffer::Buffer(void) :
	m_cursorPos(0),
	m_cursorPreferredCol(-1)
{
	
}

bool appl::Buffer::loadFile(const etk::UString& _name)
{
	APPL_DEBUG("Load file : '" << _name << "'");
	m_fileName = _name;
	etk::FSNode file(m_fileName);
	if (file.Exist() == false) {
		return false;
	}
	if (true == m_data.DumpFrom(file) ) {
		return true;
	}
	return false;
}



void appl::Buffer::setFileName(const etk::UString& _name)
{
	// TODO : ...
}


void appl::Buffer::MoveCursorRight(appl::Buffer::moveMode _mode)
{
	m_cursorPreferredCol = -1;
	etk::UniChar value;
	esize_t nbElement;
	switch (_mode) {
		default:
		case moveLetter:
			nbElement = get(m_cursorPos, value);
			if (nbElement>0) {
				m_cursorPos += nbElement;
			}
			break;
		case moveWord:
			// TODO : ...
			break;
		case moveEnd:
			// TODO : ...
			break;
	}
	
}

void appl::Buffer::MoveCursorLeft(appl::Buffer::moveMode _mode)
{
	m_cursorPreferredCol = -1;
	etk::UniChar value;
	esize_t nbElement;
	switch (_mode) {
		default:
		case moveLetter:
			nbElement = getBack(m_cursorPos-1, value);
			if (nbElement>0) {
				m_cursorPos -= nbElement;
			}
			break;
		case moveWord:
			// TODO : ...
			break;
		case moveEnd:
			// TODO : ...
			break;
	}
}

void appl::Buffer::MoveCursorUp(esize_t _nbLine)
{
	// find the position of the start of the line.
	esize_t lineStartPos = StartLine(m_cursorPos);
	// check if we can go up ...
	if (lineStartPos == 0) {
		return;
	}
	// Decide what column to move to, if there's a preferred column use that
	if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = CountDispChars(lineStartPos, m_cursorPos);
	}
	EWOL_DEBUG("ploop : " << m_cursorPreferredCol);
	// get the previous line
	esize_t prevLineStartPos = CountBackwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move line UP result : prevLineStartPos=" << prevLineStartPos);
	// get the display char position
	esize_t newPos = CountForwardDispChars(prevLineStartPos, m_cursorPreferredCol);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	//SetInsertPosition(newPos);
	m_cursorPos = newPos;
}

void appl::Buffer::MoveCursorDown(esize_t _nbLine)
{
	// check if we are not at the end of Buffer
	if (m_cursorPos == m_data.size() ) {
		return;
	}
	// find the position of the start of the line.
	esize_t lineStartPos = StartLine(m_cursorPos);
	
	if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = CountDispChars(lineStartPos, m_cursorPos);
	}
	EWOL_DEBUG("ploop : " << m_cursorPreferredCol);
	// get the next line :
	esize_t nextLineStartPos = CountForwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move line DOWN result : nextLineStartPos=" << nextLineStartPos);
	// get the display char position
	esize_t newPos = CountForwardDispChars(nextLineStartPos, m_cursorPreferredCol);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	//SetInsertPosition(newPos);
	m_cursorPos = newPos;
}

esize_t appl::Buffer::StartLine(esize_t _pos)
{
	esize_t startPos;
	if (false == SearchBack(_pos, etk::UniChar::Return, startPos)) {
		return 0;
	}
	return startPos + 1;
}

esize_t appl::Buffer::EndLine(esize_t _pos)
{
	esize_t endPos;
	if (false == Search(_pos, etk::UniChar::Return, endPos)) {
		endPos = m_data.size();
	}
	return endPos;
}

bool appl::Buffer::Search(esize_t _pos, const etk::UniChar& _search, esize_t& _result)
{
	// move in the string
	esize_t nbElementBuffer = 0;
	etk::UniChar value;
	for(esize_t iii=_pos ; iii<m_data.size() ; iii+=nbElementBuffer ) {
		nbElementBuffer = getBack(iii, value);
		if (value == _search) {
			_result = iii;
			return true;
		}
		if (nbElementBuffer <= 0) {
			nbElementBuffer = 1;
		}
	}
	_result = m_data.size();
	return false;
}

bool appl::Buffer::SearchBack(esize_t _pos, const etk::UniChar& _search, esize_t& _result)
{
	// move in the string
	esize_t nbElementBuffer = 0;
	etk::UniChar value;
	for(esize_t iii=_pos-1 ; iii >= 0 ; iii-=nbElementBuffer ) {
		nbElementBuffer = getBack(iii, value);
		if (value == _search) {
			_result = iii-nbElementBuffer;
			return true;
		}
		if (nbElementBuffer <= 0) {
			nbElementBuffer = 1;
		}
	}
	_result = 0;
	return false;
}


bool appl::Buffer::onEventEntry(const ewol::EventEntry& _event) // TODO : , vec2 _displaySize)
{
	//APPL_DEBUG(" event : " << _event);
	if (_event.getType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.getStatus() != ewol::keyEvent::statusDown) {
			return false;
		}
		if (_event.getChar() == etk::UniChar::Tabulation) {
			m_data.insert(m_cursorPos, '\t');
			m_cursorPos += 1;
		} else if (_event.getChar() == etk::UniChar::Return) {
			m_data.insert(m_cursorPos, '\n');
			m_cursorPos += 1;
		} else if (_event.getChar() == etk::UniChar::Suppress ) {
			//APPL_INFO("keyEvent : <suppr> pos=" << m_cursorPos);
			etk::UniChar value;
			esize_t nbElement = get(m_cursorPos, value);
			if (nbElement>0) {
				m_data.remove(m_cursorPos, nbElement);
			}
		} else if (_event.getChar() == etk::UniChar::Delete) {
			//APPL_INFO("keyEvent : <del> pos=" << m_cursorPos);
			etk::UniChar value;
			esize_t nbElement = getBack(m_cursorPos-1, value);
			if (nbElement>0) {
				m_cursorPos -= nbElement;
				m_data.remove(m_cursorPos, nbElement);
			}
		} else {
			// normal adding char ...
			char output[5];
			int32_t nbElement = _event.getChar().GetUtf8(output);
			if (_event.getSpecialKey().isSetInsert() == false) {
				m_data.insert(m_cursorPos, (int8_t*)output, nbElement);
			} else {
				etk::UniChar value;
				esize_t nbElementRemove = get(m_cursorPos, value);
				m_data.Replace(m_cursorPos, nbElementRemove, (int8_t*)output, nbElement);
			}
			m_cursorPos += nbElement;
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
				MoveCursorLeft();
				break;
			case ewol::keyEvent::keyboardRight:
				//APPL_INFO("keyEvent : <RIGHT>");
				MoveCursorRight();
				break;
			case ewol::keyEvent::keyboardUp:
				//APPL_INFO("keyEvent : <UP>");
				MoveCursorUp(1);
				break;
			case ewol::keyEvent::keyboardDown:
				//APPL_INFO("keyEvent : <DOWN>");
				MoveCursorDown(1);
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
				MoveCursorLeft(moveEnd);
				break;
			case ewol::keyEvent::keyboardEnd:
				//APPL_INFO("keyEvent : <End of line>");
				MoveCursorRight(moveEnd);
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


esize_t appl::Buffer::get(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset) const
{
	_value = '\0';
	if (_pos<0 && _pos<m_data.size()) {
		return 0;
	}
	if (_charset == unicode::EDN_CHARSET_UTF8) {
		char tmpVal[5];
		memset(tmpVal, 0, sizeof(tmpVal));
		tmpVal[0] = m_data[_pos];
		int8_t nbChar = etk::UniChar::TheoricUTF8Len(tmpVal[0]);
		for (int32_t iii=1; iii<nbChar && _pos+iii<m_data.size(); ++iii) {
			tmpVal[iii] = m_data[_pos+iii];
		}
		// transform ...
		int32_t nbElement = _value.setUtf8(tmpVal);
		return nbElement;
	}
	// TODO :: need to trancode iso  == > UNICODE ...
	_value.set(m_data[_pos]);
	return 1;
}

esize_t appl::Buffer::getBack(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset) const
{
	_value = '\0';
	if (_pos<0 && _pos<m_data.size()) {
		return 0;
	}
	if (_charset == unicode::EDN_CHARSET_UTF8) {
		char tmpVal[5];
		char* pointerVal = &tmpVal[4];
		memset(tmpVal, 0, sizeof(tmpVal));
		*pointerVal = m_data[_pos];
		int32_t iii=0;
		while(    etk::UniChar::TheoricUTF8First(*pointerVal) == false
		       && pointerVal > tmpVal
		       && _pos-iii>0) {
			--pointerVal;
			++iii;
			*pointerVal = m_data[_pos - iii];
		};
		int32_t nbElement = _value.setUtf8(pointerVal);
		return nbElement;
	}
	_value.set(m_data[_pos]);
	return 1;
	
}

static const char *ControlCodeTable[32] = {
	 "NUL", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs",  "ht", "nl",  "vt",  "np", "cr", "so", "si",
	 "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};

void appl::Buffer::Expand(esize_t& _indent, const etk::UniChar& _value, etk::UString& _out) const
{
	_out.clear();
	int32_t tabDist = 4;
	if (_value == etk::UniChar::Tabulation) {
		int32_t nSpaces = tabDist - (_indent % tabDist);
		for (int32_t iii=0; iii<nSpaces; iii++) {
			_out.append(etk::UniChar::Space);
		}
		return;
	}
	// convert ASCII control codes to readable character sequences
	if (_value == etk::UniChar::Null) {
		_out.append(etk::UniChar('<'));
		_out.append(etk::UniChar('n'));
		_out.append(etk::UniChar('u'));
		_out.append(etk::UniChar('l'));
		_out.append(etk::UniChar('>'));
		return;
	}
	if (_value == etk::UniChar::Return) {
		// nothing to display...
		return;
	}
	if (_value.get() <= 31) {
		_out.append(etk::UniChar('<'));
		const char * tmp = ControlCodeTable[_value.get()];
		while (*tmp!='\0') {
			_out.append(etk::UniChar(*tmp));
			tmp++;
		}
		_out.append(etk::UniChar('>'));
		return;
	}
	if (_value == etk::UniChar::Delete) {
		_out.append(etk::UniChar('<'));
		_out.append(etk::UniChar('d'));
		_out.append(etk::UniChar('e'));
		_out.append(etk::UniChar('l'));
		_out.append(etk::UniChar('>'));
		return;
	}
	// nothing to do ...
	_out.append(_value);
	//APPL_DEBUG("plop : " << _out);
}

int32_t appl::Buffer::CountDispChars(esize_t _posStart, esize_t _posEnd)
{
	int32_t charCount = 0;
	etk::UString expanded;
	esize_t bufferElementSize;
	etk::UniChar value;
	//APPL_DEBUG("_posStart="<< _posStart << " _posEnd=" << _posEnd);
	for(int32_t iii=_posStart; iii<_posEnd && iii<m_data.size() ; iii+=bufferElementSize ) {
		// get the element value:
		bufferElementSize = get(iii, value);
		//APPL_DEBUG(" get : " << value << " size=" << bufferElementSize);
		Expand(charCount, value, expanded);
		charCount += expanded.size();
		if (bufferElementSize <= 0) {
			bufferElementSize = 1;
		}
	}
	//APPL_DEBUG(" result=" << charCount);
	return charCount;
}

esize_t appl::Buffer::CountForwardDispChars(esize_t _posStart, int32_t _nChars)
{
	int32_t charCount = 0;
	etk::UString expanded;
	esize_t bufferElementSize;
	etk::UniChar value;
	int32_t iii;
	for(iii = _posStart; charCount<_nChars && iii<m_data.size() ; iii+=bufferElementSize ) {
		// get the element value:
		bufferElementSize = get(iii, value);
		if (value == etk::UniChar::Return) {
			return iii;
		}
		Expand(charCount, value, expanded);
		charCount += expanded.size();
		if (bufferElementSize <= 0) {
			bufferElementSize = 1;
		}
	}
	//APPL_DEBUG(" result=" << charCount);
	return iii;
}

esize_t appl::Buffer::CountForwardNLines(esize_t _startPos, int32_t _nLines)
{
	if (_nLines <= 0) {
		return _startPos;
	} else if (_startPos > m_data.size() ) {
		return m_data.size();
	}
	esize_t bufferElementSize;
	etk::UniChar value;
	int32_t lineCount = 0;
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	for(int32_t iii = _startPos+1; iii<m_data.size() ; iii+=bufferElementSize ) {
		// get the element value:
		bufferElementSize = get(iii, value);
		if (value == etk::UniChar::Return) {
			lineCount++;
			if (lineCount == _nLines) {
				//APPL_INFO("    == > (1) at position=" << myPosIt.Position()+1 );
				return iii+1;
			}
		}
		if (bufferElementSize <= 0) {
			bufferElementSize = 1;
		}
	}
	//APPL_INFO("    == > (2) at position=" << myPosIt.Position() );
	return m_data.size();
}

esize_t appl::Buffer::CountBackwardNLines(esize_t _startPos, int32_t _nLines)
{
	if (_startPos <= 0) {
		return 0;
	} else if (_startPos > m_data.size() ) {
		_startPos = m_data.size();
	}
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	esize_t bufferElementSize;
	etk::UniChar value;
	int32_t lineCount = 0;
	for(int32_t iii = _startPos-1; iii >= 0 ; iii-=bufferElementSize ) {
		// get the element value:
		bufferElementSize = getBack(iii, value);
		if (value == etk::UniChar::Return) {
			lineCount++;
			if (lineCount >= _nLines) {
				//APPL_INFO("    == > (1) at position=" << myPosIt.Position()+1 );
				return iii+1;
			}
		}
		if (bufferElementSize <= 0) {
			bufferElementSize = 1;
		}
	}
	//APPL_INFO("    == > (2) at position=0");
	return 0;
}
