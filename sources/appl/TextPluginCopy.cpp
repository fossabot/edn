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
	m_activateOnReceiveMessage = true;
	addObjectType("appl::TextPluginCopy");
}

void appl::TextPluginCopy::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_registerMultiCast(ednMsgGuiCopy);
	_textDrawer.ext_registerMultiCast(ednMsgGuiPaste);
	_textDrawer.ext_registerMultiCast(ednMsgGuiCut);
	_textDrawer.ext_shortCutAdd("ctrl+x", ednMsgGuiCut,   "STD");
	_textDrawer.ext_shortCutAdd("ctrl+c", ednMsgGuiCopy,  "STD");
	_textDrawer.ext_shortCutAdd("ctrl+v", ednMsgGuiPaste, "STD");
}

void appl::TextPluginCopy::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

bool appl::TextPluginCopy::onReceiveMessage(appl::TextViewer& _textDrawer,
                                            const ewol::object::Message& _msg) {
	if (isEnable() == false) {
		return false;
	}
	if (    _msg.getMessage() == ednMsgGuiCopy
	     || _msg.getMessage() == ednMsgGuiCut) {
		if (_textDrawer.hasBuffer() == true) {
			std::string value;
			_textDrawer.copy(value);
			if (value.size() != 0) {
				ewol::context::clipBoard::set(ewol::context::clipBoard::clipboardStd, value);
			}
		}
		if (_msg.getMessage() == ednMsgGuiCut) {
			_textDrawer.remove();
		}
		return true;
	} else if (_msg.getMessage() == ednMsgGuiPaste) {
		if (_textDrawer.hasBuffer() == true) {
			ewol::context::clipBoard::request(ewol::context::clipBoard::clipboardStd);
		}
		return true;
	}
	return false;
}
