/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginRmLine.h>
#include <appl/Gui/TextViewer.h>

#undef __class__
#define __class__ "TextPluginRmLine"


appl::TextPluginRmLine::TextPluginRmLine() {
	m_activateOnReceiveShortCut = true;
	addObjectType("appl::TextPluginRmLine");
}


void appl::TextPluginRmLine::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_shortCutAdd("ctrl+w", "appl::TextPluginRmLine::Rm");
}

void appl::TextPluginRmLine::onPluginDisable(appl::TextViewer& _textDrawer) {
	_textDrawer.ext_shortCutRm("appl::TextPluginRmLine::Rm");
}

bool appl::TextPluginRmLine::onReceiveShortCut(appl::TextViewer& _textDrawer,
                                               const std::string& _shortCutName) {
	if (isEnable() == false) {
		return false;
	}
	if (_shortCutName == "appl::TextPluginRmLine::Rm") {
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
