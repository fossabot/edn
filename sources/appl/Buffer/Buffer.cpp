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
			--iii;
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

appl::Buffer::Iterator appl::Buffer::cursor(void) {
	return position( m_cursorPos );
}

appl::Buffer::Iterator appl::Buffer::selectStart(void) {
	return position( getStartSelectionPos() );
}

appl::Buffer::Iterator appl::Buffer::selectStop(void) {
	return position( getStopSelectionPos() );
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


appl::Buffer::Iterator appl::Buffer::getStartLine(const appl::Buffer::Iterator& _pos) {
	appl::Buffer::Iterator startPos;
	if (false == searchBack(_pos, etk::UChar::Return, startPos)) {
		return begin();
	}
	return startPos;
}

appl::Buffer::Iterator appl::Buffer::getEndLine(const appl::Buffer::Iterator& _pos) {
	appl::Buffer::Iterator endPos;
	if (false == search(_pos, etk::UChar::Return, endPos)) {
		endPos = end();
	}
	return endPos;
}


bool appl::Buffer::search(const appl::Buffer::Iterator& _pos, const etk::UChar& _search, appl::Buffer::Iterator& _result) {
	// move in the string
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

bool appl::Buffer::searchBack(const appl::Buffer::Iterator& _pos, const etk::UChar& _search, appl::Buffer::Iterator& _result) {
	// move in the string
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
	_result = begin();
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

bool appl::Buffer::getPosAround(const appl::Buffer::Iterator& _startPos,
                                appl::Buffer::Iterator &_beginPos,
                                appl::Buffer::Iterator &_endPos) {
	etk::UChar currentValue = *position(_startPos);
	_beginPos = begin();
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
	_beginPos = begin();
	_endPos = begin();
	return false;
}

void appl::Buffer::setSelectionPos(const appl::Buffer::Iterator& _pos) {
	m_cursorSelectPos = _pos;
}

void appl::Buffer::unSelect(void) {
	m_cursorSelectPos = -1;
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

appl::Buffer::Iterator appl::Buffer::countForwardNLines(const appl::Buffer::Iterator& _startPos, int32_t _nLines) {
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
				return ++it;
			}
		}
	}
	//APPL_INFO("    == > (2) at position=" << myPosIt.Position() );
	return end();
}

appl::Buffer::Iterator appl::Buffer::countBackwardNLines(const appl::Buffer::Iterator& _startPos, int32_t _nLines) {
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

bool appl::Buffer::write(const etk::UString& _data, const appl::Buffer::Iterator& _pos) {
	etk::Char output = _data.c_str();
	m_data.insert(_pos, (int8_t*)((void*)output), output.size());
	m_selectMode = false;
	moveCursor((esize_t)_pos+output.size());
	return true;
}

bool appl::Buffer::replace(const etk::UString& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
	etk::Char output = _data.c_str();
	m_data.replace(_pos, (esize_t)_posEnd-(esize_t)_pos, (int8_t*)((void*)output), output.size());
	m_selectMode = false;
	moveCursor((esize_t)_pos+output.size());
	return true;
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


