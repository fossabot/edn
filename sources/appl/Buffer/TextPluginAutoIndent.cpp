/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer/TextPluginAutoIndent.h>
#include <ewol/clipBoard.h>
#include <appl/Gui/TextViewer.h>


appl::TextPluginAutoIndent::TextPluginAutoIndent(void) {
	m_activateOnEventEntry = true;
}

bool appl::TextPluginAutoIndent::onEventEntry(appl::TextViewer& _textDrawer,
                                              const ewol::EventEntry& _event) {
	/*
	if (enable == false) {
		return false;
	}
	*/
	// just forward event  == > manage directly in the buffer
	if (_event.getType() != ewol::keyEvent::keyboardChar) {
		return false;
	}
	//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
	if (_event.getStatus() != ewol::keyEvent::statusDown) {
		return false;
	}
	if (_event.getChar() != etk::UChar::Return) {
		return false;
	}
	if (_event.getSpecialKey().isSetShift() == false) {
		return false;
	}
	appl::Buffer::Iterator startLine = _textDrawer.m_buffer->cursor();
	if (_textDrawer.m_buffer->hasTextSelected() == true) {
		startLine = _textDrawer.m_buffer->selectStart();
	}
	etk::UString data = etk::UChar::Return;
	
	for (appl::Buffer::Iterator it = startLine;
	     it != _textDrawer.m_buffer->end();
	     ++it) {
		if (*it == etk::UChar::Space) {
			data.append(etk::UChar::Space);
		} else if(*it == etk::UChar::Tabulation) {
			data.append(etk::UChar::Tabulation);
		} else {
			break;
		}
	}
	APPL_DEBUG("kjhkjhkjhkjh : '" << data << "'");
	_textDrawer.write(data);
	return true;
}

