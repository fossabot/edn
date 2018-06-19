/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */

#include <appl/TextPluginCopy.hpp>
#include <gale/context/clipBoard.hpp>
#include <appl/Gui/TextViewer.hpp>

appl::TextPluginCopy::TextPluginCopy() :
  m_menuIdTitle(-1),
  m_menuIdCopy(-1),
  m_menuIdCut(-1),
  m_menuIdPast(-1),
  m_menuIdRemove(-1) {
	m_activateOnReceiveShortCut = true;
	addObjectType("appl::TextPluginCopy");
}

void appl::TextPluginCopy::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	ememory::SharedPtr<ewol::widget::Menu> menu = m_menuInterface.lock();
	if (menu != null) {
		m_menuIdTitle = menu->addTitle("_T{Edit}");
		if (m_menuIdTitle != -1) {
			m_menuIdCopy = menu->add(m_menuIdTitle, "_T{Copy}",     "", "appl::TextPluginCopy::menu:copy");
			m_menuIdCut = menu->add(m_menuIdTitle, "_T{Cut}",       "", "appl::TextPluginCopy::menu:cut");
			m_menuIdPast = menu->add(m_menuIdTitle, "_T{Paste}",    "", "appl::TextPluginCopy::menu:past");
			m_menuIdRemove = menu->add(m_menuIdTitle, "_T{Remove}", "", "appl::TextPluginCopy::menu:remove");
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
	ememory::SharedPtr<ewol::widget::Menu> menu = m_menuInterface.lock();
	if (menu != null) {
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
                                             const etk::String& _shortCutName) {
	if (isEnable() == false) {
		return false;
	}
	if (    _shortCutName == "appl::TextPluginCopy::copy"
	     || _shortCutName == "appl::TextPluginCopy::cut") {
		if (_textDrawer.hasBuffer() == true) {
			etk::String value;
			_textDrawer.copy(value);
			if (value.size() != 0) {
				gale::context::clipBoard::set(gale::context::clipBoard::clipboardStd, value);
			}
		}
		if (_shortCutName == "appl::TextPluginCopy::cut") {
			_textDrawer.remove();
		}
		return true;
	} else if (_shortCutName == "appl::TextPluginCopy::Paste") {
		if (_textDrawer.hasBuffer() == true) {
			gale::context::clipBoard::request(gale::context::clipBoard::clipboardStd);
		}
		return true;
	}
	return false;
}
