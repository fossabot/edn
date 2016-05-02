/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/TextPluginAutoIndent.h>
#include <gale/context/clipBoard.h>
#include <appl/Gui/TextViewer.h>

appl::TextPluginAutoIndent::TextPluginAutoIndent() {
	m_activateOnEventEntry = true;
	addObjectType("appl::TextPluginAutoIndent");
}


bool appl::TextPluginAutoIndent::onEventEntry(appl::TextViewer& _textDrawer,
                                              const ewol::event::Entry& _event) {
	if (isEnable() == false) {
		return false;
	}
	//APPL_DEBUG("KB EVENT : " << _event);
	// just forward event  == > manage directly in the buffer
	if (_event.getType() != gale::key::keyboard::character) {
		return false;
	}
	if (_event.getStatus() != gale::key::status::down) {
		return false;
	}
	if (_event.getChar() != u32char::Return) {
		return false;
	}
	if (_event.getSpecialKey().getShift() == true) {
		return false;
	}
	appl::Buffer::Iterator startLine = _textDrawer.cursor();
	if (_textDrawer.hasTextSelected() == true) {
		startLine = _textDrawer.selectStart();
	}
	startLine = _textDrawer.getStartLine(startLine);
	std::string data = "\n";
	
	
	for (appl::Buffer::Iterator it = startLine;
	     (bool)it == true;
	     ++it) {
		if (*it == u32char::Space) {
			data += u32char::Space;
		} else if(*it == u32char::Tabulation) {
			data += u32char::Tabulation;
		} else {
			break;
		}
	}
	_textDrawer.write(data);
	return true;
}

