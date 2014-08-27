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


appl::TextPluginCopy::TextPluginCopy() {
	m_activateOnReceiveShortCut = true;
	addObjectType("appl::TextPluginCopy");
}

void appl::TextPluginCopy::init() {
	appl::TextViewerPlugin::init();
}

void appl::TextPluginCopy::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_shortCutAdd("ctrl+x", "appl::TextPluginCopy::cut");
	_textDrawer.ext_shortCutAdd("ctrl+c", "appl::TextPluginCopy::copy");
	_textDrawer.ext_shortCutAdd("ctrl+v", "appl::TextPluginCopy::Paste");
}

void appl::TextPluginCopy::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
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
