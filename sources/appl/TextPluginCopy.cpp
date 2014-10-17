/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginCopy.h>
#include <ewol/context/clipBoard.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginCopy"


appl::TextPluginCopy::TextPluginCopy() :
  m_menuIdTitle(-1),
  m_menuIdCopy(-1),
  m_menuIdCut(-1),
  m_menuIdPast(-1),
  m_menuIdRemove(-1) {
	m_activateOnReceiveShortCut = true;
	addObjectType("appl::TextPluginCopy");
}

void appl::TextPluginCopy::init() {
	appl::TextViewerPlugin::init();
}

void appl::TextPluginCopy::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	std::shared_ptr<ewol::widget::Menu> menu = m_menuInterface.lock();
	if (menu != nullptr) {
		m_menuIdTitle = menu->addTitle("Edit");
		if (m_menuIdTitle != -1) {
			m_menuIdCopy = menu->add(m_menuIdTitle, "Copy",   "", "appl::TextPluginCopy::menu:copy");
			m_menuIdCut = menu->add(m_menuIdTitle, "Cut",    "", "appl::TextPluginCopy::menu:cut");
			m_menuIdPast = menu->add(m_menuIdTitle, "Paste",  "", "appl::TextPluginCopy::menu:past");
			m_menuIdRemove = menu->add(m_menuIdTitle, "Remove", "", "appl::TextPluginCopy::menu:remove");
		}
	}
	_textDrawer.ext_shortCutAdd("ctrl+x", "appl::TextPluginCopy::cut");
	_textDrawer.ext_shortCutAdd("ctrl+c", "appl::TextPluginCopy::copy");
	_textDrawer.ext_shortCutAdd("ctrl+v", "appl::TextPluginCopy::Paste");
}

void appl::TextPluginCopy::onPluginDisable(appl::TextViewer& _textDrawer) {
	_textDrawer.ext_shortCutRm("appl::TextPluginCopy::cut");
	_textDrawer.ext_shortCutRm("appl::TextPluginCopy::copy");
	_textDrawer.ext_shortCutRm("appl::TextPluginCopy::Paste");
	std::shared_ptr<ewol::widget::Menu> menu = m_menuInterface.lock();
	if (menu != nullptr) {
		menu->remove(m_menuIdRemove);
		menu->remove(m_menuIdPast);
		menu->remove(m_menuIdCut);
		menu->remove(m_menuIdCopy);
		menu->remove(m_menuIdTitle);
	}
	m_menuIdTitle = -1;
	m_menuIdCopy = -1;
	m_menuIdCut = -1;
	m_menuIdPast = -1;
	m_menuIdRemove = -1;
}

bool appl::TextPluginCopy::onReceiveShortCut(appl::TextViewer& _textDrawer,
                                             const std::string& _shortCutName) {
	if (isEnable() == false) {
		return false;
	}
	if (    _shortCutName == "appl::TextPluginCopy::copy"
	     || _shortCutName == "appl::TextPluginCopy::cut") {
		if (_textDrawer.hasBuffer() == true) {
			std::string value;
			_textDrawer.copy(value);
			if (value.size() != 0) {
				ewol::context::clipBoard::set(ewol::context::clipBoard::clipboardStd, value);
			}
		}
		if (_shortCutName == "appl::TextPluginCopy::cut") {
			_textDrawer.remove();
		}
		return true;
	} else if (_shortCutName == "appl::TextPluginCopy::Paste") {
		if (_textDrawer.hasBuffer() == true) {
			ewol::context::clipBoard::request(ewol::context::clipBoard::clipboardStd);
		}
		return true;
	}
	return false;
}
