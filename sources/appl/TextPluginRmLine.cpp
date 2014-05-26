/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginRmLine.h>
#include <ewol/context/clipBoard.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginRmLine"


appl::TextPluginRmLine::TextPluginRmLine() {
	m_activateOnReceiveMessage = true;
	addObjectType("appl::TextPluginRmLine");
}

void appl::TextPluginRmLine::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_registerMultiCast(ednMsgGuiRm);
	_textDrawer.ext_shortCutAdd("ctrl+w", ednMsgGuiRm);
}

void appl::TextPluginRmLine::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

bool appl::TextPluginRmLine::onReceiveMessage(appl::TextViewer& _textDrawer,
                                              const ewol::object::Message& _msg) {
	if (isEnable() == false) {
		return false;
	}
	if (_msg.getMessage() == ednMsgGuiRm) {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		if (_textDrawer.hasTextSelected() == true) {
			appl::Buffer::Iterator start = _textDrawer.getStartLine(_textDrawer.selectStart());
			appl::Buffer::Iterator end = _textDrawer.getEndLine(_textDrawer.selectStop())+1;
			_textDrawer.replace("", start, end);
		} else {
			appl::Buffer::Iterator start = _textDrawer.getStartLine(_textDrawer.cursor());
			appl::Buffer::Iterator end = _textDrawer.getEndLine(_textDrawer.cursor())+1;
			_textDrawer.replace("", start, end);
		}
		return true;
	}
	return false;
}
