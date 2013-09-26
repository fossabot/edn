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
		} else if (_event.GetChar() == etk::UniChar::Return) {
			m_data.Insert(m_cursorPos, '\n');
		} else if (_event.GetChar() == etk::UniChar::Backspace ) {
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
