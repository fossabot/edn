/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer/TextPluginMultiLineTab.h>
#include <ewol/clipBoard.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginMultiLineTab"

appl::TextPluginMultiLineTab::TextPluginMultiLineTab(void) {
	m_activateOnEventEntry = true;
}

bool appl::TextPluginMultiLineTab::onEventEntry(appl::TextViewer& _textDrawer,
                                                const ewol::EventEntry& _event) {
	if (isEnable() == false) {
		return false;
	}
	if (_event.getType() != ewol::keyEvent::keyboardChar) {
		return false;
	}
	//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
	if (_event.getStatus() != ewol::keyEvent::statusDown) {
		return false;
	}
	etk::UChar localValue = _event.getChar();
	if (localValue != etk::UChar::Tabulation) {
		return false;
	}
	if (_textDrawer.m_buffer->hasTextSelected() == false) {
		return false;
	}
	appl::Buffer::Iterator itStart = _textDrawer.m_buffer->selectStart();
	appl::Buffer::Iterator itStop = _textDrawer.m_buffer->selectStop();
	// get the compleate section of the buffer :
	itStart = _textDrawer.m_buffer->getStartLine(itStart);
	itStop = _textDrawer.m_buffer->getEndLine(itStop);
	// copy the curent data in a classicle string:
	etk::UString data;
	_textDrawer.m_buffer->copy(data, itStart, itStop);
	// TODO : Change this ...
	bool m_useTabs = true;
	int32_t m_tabDist = 4;
	
	if (true == _event.getSpecialKey().isSetShift() ) {
		// un-indent
		data.add(0, etk::UChar::Return);
		for (esize_t iii=1; iii<data.size(); ++iii) {
			if (data[iii-1] == etk::UChar::Return) {
				if(data[iii] == etk::UChar::Tabulation) {
					data.remove(iii);
				} else if(data[iii] == etk::UChar::Space) {
					for (esize_t jjj=0; jjj<m_tabDist && jjj+iii<data.size() ; jjj++) {
						if(data[iii] == etk::UChar::Space) {
							data.remove(iii);
						} else if(data[iii] == etk::UChar::Tabulation) {
							data.remove(iii);
							break;
						} else {
							break;
						}
					}
				}
			}
		}
		data.remove(0);
	} else {
		// indent
		data.add(0, etk::UChar::Return);
		for (esize_t iii=1; iii<data.size(); iii++) {
			if (data[iii-1] == etk::UChar::Return) {
				if (true == m_useTabs) {
					data.add(iii, etk::UChar::Tabulation);
				} else {
					for (int32_t jjj=0; jjj<m_tabDist; jjj++) {
						data.add(iii, etk::UChar::Space);
					}
				}
			}
		}
		data.remove(0);
	}
	// Real replace of DATA :
	_textDrawer.replace(data, itStart, itStop);
	_textDrawer.m_buffer->setSelectionPos(itStart+1);
	return true;
}