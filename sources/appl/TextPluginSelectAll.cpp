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


appl::TextPluginSelectAll::TextPluginSelectAll(void) {
	m_activateOnReceiveMessage = true;
}

static const char* eventSelectAll = "plugin-select-all";

void appl::TextPluginSelectAll::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_registerMultiCast(eventSelectAll);
	_textDrawer.ext_shortCutAdd("ctrl+a", eventSelectAll);
}

void appl::TextPluginSelectAll::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

bool appl::TextPluginSelectAll::onReceiveMessage(appl::TextViewer& _textDrawer,
                                                 const ewol::object::Message& _msg) {
	if (isEnable() == false) {
		return false;
	}
	if (_msg.getMessage() == eventSelectAll) {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		_textDrawer.select(_textDrawer.begin(), _textDrawer.end());
		return true;
	}
	return false;
}
