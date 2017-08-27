/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/TextPluginMultiLineTab.hpp>
#include <appl/Gui/TextViewer.hpp>

appl::TextPluginMultiLineTab::TextPluginMultiLineTab() {
	m_activateOnEventEntry = true;
	addObjectType("appl::TextPluginMultiLineTab");
}


bool appl::TextPluginMultiLineTab::onEventEntry(appl::TextViewer& _textDrawer,
                                                const ewol::event::Entry& _event) {
	if (isEnable() == false) {
		return false;
	}
	if (_event.getType() != gale::key::keyboard::character) {
		return false;
	}
	//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
	if (_event.getStatus() != gale::key::status::down) {
		return false;
	}
	char32_t localValue = _event.getChar();
	if (localValue != u32char::Tabulation) {
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
	etk::String data;
	_textDrawer.copy(data, itStart, itStop);
	// TODO : Change this ...
	bool m_useTabs = true;
	size_t m_tabDist = 4;
	
	if (true == _event.getSpecialKey().getShift() ) {
		// un-indent
		data.insert(0, 1, u32char::Return);
		for (size_t iii=1; iii<data.size(); ++iii) {
			if ((char32_t)data[iii-1] != u32char::Return) {
				continue;
			}
			if((char32_t)data[iii] == u32char::Tabulation) {
				data.erase(iii, 1);
			} else if((char32_t)data[iii] == u32char::Space) {
				for (size_t jjj=0; jjj<m_tabDist && jjj+iii<data.size() ; jjj++) {
					if((char32_t)data[iii] == u32char::Space) {
						data.erase(iii, 1);
					} else if((char32_t)data[iii] == u32char::Tabulation) {
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
		data.insert(0, 1, u32char::Return);
		for (size_t iii=1; iii<data.size(); iii++) {
			if ((char32_t)data[iii-1] != u32char::Return) {
				continue;
			}
			if (true == _event.getSpecialKey().getCtrl() ) {
				data.insert(iii, 1, u32char::Space);
			} else if (true == m_useTabs) {
				data.insert(iii, 1, u32char::Tabulation);
			} else {
				data.insert(iii, m_tabDist, u32char::Space);
			}
		}
		data.erase(0, 1);
	}
	// Real replace of DATA :
	_textDrawer.replace(data, itStart, itStop);
	_textDrawer.select(itStart, itStart+data.size());
	return true;
}
