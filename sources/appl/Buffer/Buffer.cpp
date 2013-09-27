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
	m_cursorPos(3)
{
	
}

bool appl::Buffer::LoadFile(const etk::UString& _name)
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



void appl::Buffer::SetFileName(const etk::UString& _name)
{
	// TODO : ...
}


void appl::Buffer::MoveCursorRight(appl::Buffer::moveMode _mode)
{
	etk::UniChar value;
	esize_t nbElement;
	switch (_mode) {
		default:
		case moveLetter:
			nbElement = Get(m_cursorPos, value);
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
	etk::UniChar value;
	esize_t nbElement;
	switch (_mode) {
		default:
		case moveLetter:
			nbElement = GetBack(m_cursorPos-1, value);
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
	/*
	// Find the position of the start of the line.
	esize_t lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
	// check if we can go up ...
	if (lineStartPos == 0) {
		return;
	}
	// Decide what column to move to, if there's a preferred column use that
	int32_t column = m_cursorPreferredCol;
	if (m_cursorPreferredCol < 0) {
		column = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
	}
	// Get the previous line
	esize_t prevLineStartPos = m_EdnBuf.CountBackwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move Line UP result : prevLineStartPos=" << prevLineStartPos);
	// Get the display char position
	esize_t newPos = m_EdnBuf.CountForwardDispChars(prevLineStartPos, column);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	// move the cursor
	SetInsertPosition(newPos);
	// if a preferred column wasn't aleady established, establish it
	if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = column;
	}
	return;
	*/
}

void appl::Buffer::MoveCursorDown(esize_t _nbLine)
{
	/*
	// check if we are not at the end of Buffer
	if (m_cursorPos == m_EdnBuf.Size() ) {
		return;
	}
	// Find the position of the start of the line.
	esize_t lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
	
	int32_t column = m_cursorPreferredCol;
	if (m_cursorPreferredCol < 0) {
		column = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
	}
	// get the next line :
	esize_t nextLineStartPos = m_EdnBuf.CountForwardNLines(lineStartPos, _nbLine);
	//APPL_INFO("Move Line DOWN result : nextLineStartPos=" << nextLineStartPos);
	// Get the display char position
	esize_t newPos = m_EdnBuf.CountForwardDispChars(nextLineStartPos, column);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	
	SetInsertPosition(newPos);
	// if a preferred column wasn't aleady established, establish it
	if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = column;
	}
	return;
	*/
}

esize_t appl::Buffer::StartLine(esize_t _pos)
{
	esize_t startPos;
	if (false == SearchBackward(pos, etk::UniChar::Return, &startPos)) {
		return 0;
	}
	return startPos + 1;
}

esize_t appl::Buffer::EndLine(esize_t _pos)
{
	esize_t endPos;
	if (false == SearchForward(pos, etk::UniChar::Return, &endPos)) {
		endPos = m_data.Size();
	}
	return endPos;
}

bool appl::Buffer::SearchForward(esize_t _pos, const etk::UniChar& _search, esize_t& _result)
{
	// move in the string
	esize_t nbElementBuffer = 0;
	etk::UniChar value;
	for(esize_t iii=_pos ; iii<m_data.Size() ; iii+=nbElementBuffer ) {
		nbElementBuffer = GetBack(iii, value);
		if (value == _search) {
			_result = iii;
			return true;
		}
	}
	_result = m_data.Size();
	return false;
}

bool appl::Buffer::SearchBack(esize_t _pos, const etk::UniChar& _search, esize_t& _result)
{
	// move in the string
	esize_t nbElementBuffer = 0;
	etk::UniChar value;
	for(esize_t iii=_pos-1 ; iii>=0 ; iii-=nbElementBuffer ) {
		nbElementBuffer = GetBack(iii, value);
		if (value == _search) {
			_result = iii-nbElementBuffer;
			return true;
		}
	}
	_result = 0;
	return false;
}


bool appl::Buffer::OnEventEntry(const ewol::EventEntry& _event) // TODO : , vec2 _displaySize)
{
	APPL_DEBUG(" event : " << _event);
	if (_event.GetType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.GetStatus() != ewol::keyEvent::statusDown) {
			return false;
		}
		if (_event.GetChar() == etk::UniChar::Tabulation) {
			m_data.Insert(m_cursorPos, '\t');
			m_cursorPos += 1;
		} else if (_event.GetChar() == etk::UniChar::Return) {
			m_data.Insert(m_cursorPos, '\n');
			m_cursorPos += 1;
		} else if (_event.GetChar() == etk::UniChar::Suppress ) {
			//APPL_INFO("keyEvent : <suppr> pos=" << m_cursorPos);
			etk::UniChar value;
			esize_t nbElement = Get(m_cursorPos, value);
			if (nbElement>0) {
				m_data.Remove(m_cursorPos, nbElement);
			}
		} else if (_event.GetChar() == etk::UniChar::Delete) {
			//APPL_INFO("keyEvent : <del> pos=" << m_cursorPos);
			etk::UniChar value;
			esize_t nbElement = GetBack(m_cursorPos-1, value);
			if (nbElement>0) {
				m_cursorPos -= nbElement;
				m_data.Remove(m_cursorPos, nbElement);
			}
		} else {
			// normal adding char ...
			char output[5];
			int32_t nbElement = _event.GetChar().GetUtf8(output);
			m_data.Insert(m_cursorPos, (int8_t*)output, nbElement);
			m_cursorPos += nbElement;
		}
		return true;
	}
	// move events ...
	if (_event.GetStatus() == ewol::keyEvent::statusDown) {
		bool needUpdatePosition = true;
		// check selection event ...
		switch(_event.GetType()) {
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


esize_t appl::Buffer::Get(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset) const
{
	_value = '\0';
	if (_pos<0 && _pos<m_data.Size()) {
		return 0;
	}
	if (_charset == unicode::EDN_CHARSET_UTF8) {
		char tmpVal[5];
		memset(tmpVal, 0, sizeof(tmpVal));
		tmpVal[0] = m_data[_pos];
		int8_t nbChar = etk::UniChar::TheoricUTF8Len(tmpVal[0]);
		for (int32_t iii=1; iii<nbChar && _pos+iii<m_data.Size(); ++iii) {
			tmpVal[iii] = m_data[_pos+iii];
		}
		// transform ...
		int32_t nbElement = _value.SetUtf8(tmpVal);
		return nbElement;
	}
	// TODO :: need to trancode iso ==> UNICODE ...
	_value.Set(m_data[_pos]);
	return 1;
}

esize_t appl::Buffer::GetBack(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset) const
{
	_value = '\0';
	if (_pos<0 && _pos<m_data.Size()) {
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
		int32_t nbElement = _value.SetUtf8(pointerVal);
		return nbElement;
	}
	_value.Set(m_data[_pos]);
	return 1;
	
}



