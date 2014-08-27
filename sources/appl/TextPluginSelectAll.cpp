/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginSelectAll.h>
#include <ewol/context/clipBoard.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginSelectAll"


appl::TextPluginSelectAll::TextPluginSelectAll() {
	m_activateOnReceiveShortCut = true;
	addObjectType("appl::TextPluginSelectAll");
}

void appl::TextPluginSelectAll::init() {
	appl::TextViewerPlugin::init();
}

void appl::TextPluginSelectAll::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_shortCutAdd("ctrl+a", "appl::TextPluginSelectAll::All");
}

void appl::TextPluginSelectAll::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
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
	return false;
}
