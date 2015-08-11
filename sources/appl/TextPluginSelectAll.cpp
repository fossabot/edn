/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginSelectAll.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginSelectAll"


appl::TextPluginSelectAll::TextPluginSelectAll() :
  m_menuIdTitle(-1),
  m_menuIdSelectAll(-1),
  m_menuIdSelectNone(-1) {
	m_activateOnReceiveShortCut = true;
	addObjectType("appl::TextPluginSelectAll");
}

void appl::TextPluginSelectAll::init() {
	appl::TextViewerPlugin::init();
}

void appl::TextPluginSelectAll::onPluginEnable(appl::TextViewer& _textDrawer) {
	std::shared_ptr<ewol::widget::Menu> menu = m_menuInterface.lock();
	if (menu != nullptr) {
		m_menuIdTitle = menu->addTitle("Edit");
		if (m_menuIdTitle != -1) {
			m_menuIdSelectAll = menu->add(m_menuIdTitle, "Select All","", "appl::TextPluginSelectAll::menu:select-all");
			m_menuIdSelectNone = menu->add(m_menuIdTitle, "Un-Select","", "appl::TextPluginSelectAll::menu:select-none");
		}
	}
	// add event :
	_textDrawer.ext_shortCutAdd("ctrl+a", "appl::TextPluginSelectAll::All");
	_textDrawer.ext_shortCutAdd("ctrl+shift+a", "appl::TextPluginSelectAll::None");
}

void appl::TextPluginSelectAll::onPluginDisable(appl::TextViewer& _textDrawer) {
	_textDrawer.ext_shortCutRm("appl::TextPluginSelectAll::All");
	_textDrawer.ext_shortCutRm("appl::TextPluginSelectAll::None");
	std::shared_ptr<ewol::widget::Menu> menu = m_menuInterface.lock();
	if (menu != nullptr) {
		menu->remove(m_menuIdSelectNone);
		menu->remove(m_menuIdSelectAll);
		menu->remove(m_menuIdTitle);
	}
	m_menuIdTitle = -1;
	m_menuIdSelectAll = -1;
	m_menuIdSelectNone = -1;
}


bool appl::TextPluginSelectAll::onReceiveShortCut(appl::TextViewer& _textDrawer,
                                                  const std::string& _shortCutName) {
	if (isEnable() == false) {
		return false;
	}
	if (_shortCutName == "appl::TextPluginSelectAll::All") {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		_textDrawer.select(_textDrawer.begin(), _textDrawer.end());
		return true;
	}
	if (_shortCutName == "appl::TextPluginSelectAll::None") {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		_textDrawer.unSelect();
		return true;
	}
	return false;
}
