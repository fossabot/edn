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

#undef __class__
#define __class__ "TextPluginAutoIndent"


appl::TextPluginAutoIndent::TextPluginAutoIndent(void) {
	m_activateOnEventEntry = true;
}

bool appl::TextPluginAutoIndent::onEventEntry(appl::TextViewer& _textDrawer,
                                              const ewol::EventEntry& _event) {
	if (isEnable() == false) {
		return false;
	}
	//APPL_DEBUG("KB EVENT : " << _event);
	// just forward event  == > manage directly in the buffer
	if (_event.getType() != ewol::keyEvent::keyboardChar) {
		return false;
	}
	if (_event.getStatus() != ewol::keyEvent::statusDown) {
		return false;
	}
	if (_event.getChar() != etk::UChar::Return) {
		return false;
	}
	if (_event.getSpecialKey().isSetShift() == true) {
		return false;
	}
	appl::Buffer::Iterator startLine = _textDrawer.m_buffer->cursor();
	if (_textDrawer.m_buffer->hasTextSelected() == true) {
		startLine = _textDrawer.m_buffer->selectStart();
	}
	startLine = _textDrawer.m_buffer->getStartLine(startLine);
	etk::UString data = etk::UChar::Return;
	
	
	for (appl::Buffer::Iterator it = startLine+1;
	     (bool)it == true;
	     ++it) {
		if (*it == etk::UChar::Space) {
			data.append(etk::UChar::Space);
		} else if(*it == etk::UChar::Tabulation) {
			data.append(etk::UChar::Tabulation);
		} else {
			break;
		}
	}
	_textDrawer.write(data);
	return true;
}

