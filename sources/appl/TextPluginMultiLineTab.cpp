/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginMultiLineTab.h>
#include <ewol/context/clipBoard.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginMultiLineTab"

appl::TextPluginMultiLineTab::TextPluginMultiLineTab(void) {
	m_activateOnEventEntry = true;
}

bool appl::TextPluginMultiLineTab::onEventEntry(appl::TextViewer& _textDrawer,
                                                const ewol::event::Entry& _event) {
	if (isEnable() == false) {
		return false;
	}
	if (_event.getType() != ewol::key::keyboardChar) {
		return false;
	}
	//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
	if (_event.getStatus() != ewol::key::statusDown) {
		return false;
	}
	char32_t localValue = _event.getChar();
	if (localValue != etk::UChar::Tabulation) {
		return false;
	}
	if (_textDrawer.hasTextSelected() == false) {
		return false;
	}
	appl::Buffer::Iterator itStart = _textDrawer.selectStart();
	appl::Buffer::Iterator itStop = _textDrawer.selectStop();
	// get the compleate section of the buffer :
	itStart = _textDrawer.getStartLine(itStart);
	itStop = _textDrawer.getEndLine(itStop);
	// copy the curent data in a classicle string:
	std::string data;
	_textDrawer.copy(data, itStart, itStop);
	// TODO : Change this ...
	bool m_useTabs = true;
	int32_t m_tabDist = 4;
	
	if (true == _event.getSpecialKey().getShift() ) {
		// un-indent
		data.insert(0, 1, etk::UChar::Return);
		for (int32_t iii=1; iii<data.size(); ++iii) {
			if (data[iii-1] != etk::UChar::Return) {
				continue;
			}
			if(data[iii] == etk::UChar::Tabulation) {
				data.erase(iii, 1);
			} else if(data[iii] == etk::UChar::Space) {
				for (int32_t jjj=0; jjj<m_tabDist && jjj+iii<data.size() ; jjj++) {
					if(data[iii] == etk::UChar::Space) {
						data.erase(iii, 1);
					} else if(data[iii] == etk::UChar::Tabulation) {
						data.erase(iii, 1);
						break;
					} else {
						break;
					}
				}
			}
		}
		data.erase(0, 1);
	} else {
		// indent
		data.insert(0, 1, etk::UChar::Return);
		for (int32_t iii=1; iii<data.size(); iii++) {
			if (data[iii-1] != etk::UChar::Return) {
				continue;
			}
			if (true == _event.getSpecialKey().getCtrl() ) {
				data.insert(iii, 1, etk::UChar::Space);
			} else if (true == m_useTabs) {
				data.insert(iii, 1, etk::UChar::Tabulation);
			} else {
				data.insert(iii, m_tabDist, etk::UChar::Space);
			}
		}
		data.erase(0, 1);
	}
	// Real replace of DATA :
	_textDrawer.replace(data, itStart, itStop);
	_textDrawer.select(itStart, itStart+data.size());
	return true;
}