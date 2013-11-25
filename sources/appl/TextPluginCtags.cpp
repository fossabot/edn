/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginCtags.h>
#include <ewol/clipBoard.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginCtags"


appl::TextPluginCtags::TextPluginCtags(void) {
	m_activateOnReceiveMessage = true;
}

const char* eventJumpDestination = "event-plugin-ctags-jump-destination";
const char* eventJumpBack = "event-plugin-ctags-jump-back";

void appl::TextPluginCtags::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_registerMultiCast(eventJumpDestination);
	_textDrawer.ext_registerMultiCast(eventJumpBack);
	_textDrawer.ext_shortCutAdd("ctrl+d", eventJumpDestination);
	_textDrawer.ext_shortCutAdd("ctrl+shift+d", eventJumpBack);
}

void appl::TextPluginCtags::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

bool appl::TextPluginCtags::onReceiveMessage(appl::TextViewer& _textDrawer,
                                             const ewol::EMessage& _msg) {
	if (isEnable() == false) {
		return false;
	}
	if (_msg.getMessage() == eventJumpDestination) {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		std::string textToSearch;
		if (_textDrawer.hasTextSelected() == true) {
			_textDrawer.copy(textToSearch, _textDrawer.selectStart(), _textDrawer.selectStop() );
		} else {
			appl::Buffer::Iterator _beginPos;
			appl::Buffer::Iterator _endPos;
			if (_textDrawer.getPosAround(_textDrawer.cursor(), _beginPos, _endPos) == false) {
				APPL_WARNING("Can not get data around...");
				return true;
			}
			_textDrawer.copy(textToSearch, _beginPos, _endPos);
		}
		APPL_ERROR("CTAGS might search : '" << textToSearch << "'");
		return true;
	} else if (_msg.getMessage() == eventJumpBack) {
		if (_textDrawer.hasBuffer() == false) {
			return false;
		}
		return true;
	}
	return false;
}
