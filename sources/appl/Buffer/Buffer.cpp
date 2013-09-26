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

bool appl::Buffer::OnEventEntry(const ewol::EventEntry& _event)
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
			APPL_INFO("keyEvent : <suppr>	pos=" << m_cursorPos);
		} else if (_event.GetChar() == etk::UniChar::Delete) {
			APPL_INFO("keyEvent : <del>	pos=" << m_cursorPos);
		} else {
			// normal adding char ...
			char output[5];
			int32_t nbElement = _event.GetChar().GetUtf8(output);
			etk::Vector<int8_t> values;
			for (int32_t iii=0; iii<nbElement; ++iii) {
				values.PushBack(output[iii]);
			}
			m_data.Insert(m_cursorPos, values);
			m_cursorPos += nbElement;
		}
		return true;
	}
	// move events ...
	if (_event.GetStatus() == ewol::keyEvent::statusDown) {
		//m_buffer->CursorMove(_event.GetType());
		
		return true;
	}
	return false;
}


esize_t appl::Buffer::Get(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset) const
{
	_value = '\0';
	if (_charset == unicode::EDN_CHARSET_UTF8) {
		char tmpVal[5];
		tmpVal[0] = m_data[_pos];
		tmpVal[1] = m_data[_pos+1];
		tmpVal[2] = m_data[_pos+2];
		tmpVal[3] = m_data[_pos+3];
		tmpVal[4] = '\0';
		// transform ...
		int32_t nbElement = _value.SetUtf8(tmpVal);
		return nbElement;
	}
	// TODO :: need to trancode iso ==> UNICODE ...
	_value.Set(m_data[_pos]);
	return 1;
}
