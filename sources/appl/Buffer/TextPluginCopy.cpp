/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer/TextPluginCopy.h>
#include <ewol/clipBoard.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginCopy"


appl::TextPluginCopy::TextPluginCopy(void) {
	m_activateOnReceiveMessage = true;
}

void appl::TextPluginCopy::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.registerMultiCast(ednMsgGuiCopy);
	_textDrawer.registerMultiCast(ednMsgGuiPaste);
	_textDrawer.registerMultiCast(ednMsgGuiCut);
	_textDrawer.shortCutAdd("ctrl+x", ednMsgGuiCut,   "STD");
	_textDrawer.shortCutAdd("ctrl+c", ednMsgGuiCopy,  "STD");
	_textDrawer.shortCutAdd("ctrl+v", ednMsgGuiPaste, "STD");
}

void appl::TextPluginCopy::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

bool appl::TextPluginCopy::onReceiveMessage(appl::TextViewer& _textDrawer,
                                            const ewol::EMessage& _msg) {
	if (isEnable() == false) {
		return false;
	}
	if (    _msg.getMessage() == ednMsgGuiCopy
	     || _msg.getMessage() == ednMsgGuiCut) {
		if (_textDrawer.m_buffer != NULL) {
			etk::UString value;
			_textDrawer.m_buffer->copy(value);
			if (value.size() != 0) {
				ewol::clipBoard::set(ewol::clipBoard::clipboardStd, value);
			}
		}
		if (_msg.getMessage() == ednMsgGuiCut) {
			_textDrawer.m_buffer->removeSelection();
		}
		return true;
	} else if (_msg.getMessage() == ednMsgGuiPaste) {
		if (_textDrawer.m_buffer != NULL) {
			ewol::clipBoard::request(ewol::clipBoard::clipboardStd);
		}
		return true;
	}
	return false;
}
