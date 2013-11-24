/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginHistory.h>
#include <ewol/clipBoard.h>
#include <appl/Gui/TextViewer.h>
#include <appl/TextPluginManager.h>

#undef __class__
#define __class__ "TextPluginHistory"

appl::TextPluginHistory::TextPluginHistory(void) {
	m_activateOnReceiveMessage = true;
	m_activateOnWrite = true;
	m_activateOnReplace = true;
	m_activateOnRemove = true;
}

appl::TextPluginHistory::~TextPluginHistory(void) {
	clearUndo();
	clearRedo();
};

void appl::TextPluginHistory::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	_textDrawer.ext_registerMultiCast(ednMsgGuiRedo);
	_textDrawer.ext_registerMultiCast(ednMsgGuiUndo);
	_textDrawer.ext_shortCutAdd("ctrl+z", ednMsgGuiUndo);
	_textDrawer.ext_shortCutAdd("ctrl+shift+z", ednMsgGuiRedo);
}

void appl::TextPluginHistory::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

bool appl::TextPluginHistory::onReceiveMessage(appl::TextViewer& _textDrawer,
                                               const ewol::EMessage& _msg) {
	if (isEnable() == false) {
		return false;
	}
	if (_msg.getMessage() == ednMsgGuiRedo) {
		if (m_redo.size() == 0) {
			return true;
		}
		if (m_redo[m_redo.size()-1] == NULL) {
			m_redo.pop_back();
			return true;
		}
		appl::History *tmpElement = m_redo[m_redo.size()-1];
		m_redo.pop_back();
		m_undo.push_back(tmpElement);
		_textDrawer.replaceDirect(tmpElement->m_addedText,
		                          _textDrawer.position(tmpElement->m_posAdded),
		                          _textDrawer.position(tmpElement->m_endPosRemoved) );
		
		return true;
	} else if (_msg.getMessage() == ednMsgGuiUndo) {
		if (m_undo.size() == 0) {
			return true;
		}
		if (m_undo[m_undo.size()-1] == NULL) {
			m_undo.pop_back();
			return true;
		}
		appl::History *tmpElement = m_undo[m_undo.size()-1];
		m_undo.pop_back();
		m_redo.push_back(tmpElement);
		_textDrawer.replaceDirect(tmpElement->m_removedText,
		                          _textDrawer.position(tmpElement->m_posAdded),
		                          _textDrawer.position(tmpElement->m_endPosAdded) );
		
		return true;
	}
	return false;
}

void appl::TextPluginHistory::clearRedo(void) {
	if (m_redo.size() == 0) {
		return;
	}
	for (size_t iii=0; iii<m_redo.size(); ++iii) {
		if (m_redo[iii] == NULL) {
			continue;
		}
		delete(m_redo[iii]);
		m_redo[iii] = NULL;
	}
	m_redo.clear();
}

void appl::TextPluginHistory::clearUndo(void) {
	if (m_undo.size() == 0) {
		return;
	}
	for (size_t iii=0; iii<m_undo.size(); ++iii) {
		if (m_undo[iii] == NULL) {
			continue;
		}
		delete(m_undo[iii]);
		m_undo[iii] = NULL;
	}
	m_undo.clear();
}


bool appl::TextPluginHistory::onWrite(appl::TextViewer& _textDrawer,
                                      const appl::Buffer::Iterator& _pos,
                                      const std::string& _data) {
	if (isEnable() == false) {
		return false;
	}
	appl::History *tmpElement = new appl::History();
	if (tmpElement != NULL) {
		tmpElement->m_addedText = _data;
		tmpElement->m_posAdded = (int64_t)_pos;
		tmpElement->m_endPosRemoved = (int64_t)_pos;
	}
	_textDrawer.writeDirect(_data, _pos);
	if (tmpElement != NULL) {
		tmpElement->m_endPosAdded = (int64_t)_textDrawer.cursor();
		clearRedo();
		m_undo.push_back(tmpElement);
	}
	appl::textPluginManager::onCursorMove(_textDrawer, _textDrawer.cursor());
	return true;
}

bool appl::TextPluginHistory::onReplace(appl::TextViewer& _textDrawer,
                                        const appl::Buffer::Iterator& _pos,
                                        const std::string& _data,
                                        const appl::Buffer::Iterator& _posEnd) {
	if (isEnable() == false) {
		return false;
	}
	appl::History *tmpElement = new appl::History();
	if (tmpElement != NULL) {
		tmpElement->m_posAdded = (int64_t)_pos;
		tmpElement->m_addedText = _data;
		tmpElement->m_endPosRemoved = (int64_t)_posEnd;
		_textDrawer.copy(tmpElement->m_removedText, _pos, _posEnd);
	}
	_textDrawer.replaceDirect(_data, _pos, _posEnd);
	if (tmpElement != NULL) {
		tmpElement->m_endPosAdded = (int64_t)_textDrawer.cursor();
		clearRedo();
		m_undo.push_back(tmpElement);
	}
	appl::textPluginManager::onCursorMove(_textDrawer, _textDrawer.cursor());
	return true;
}

bool appl::TextPluginHistory::onRemove(appl::TextViewer& _textDrawer,
                                       const appl::Buffer::Iterator& _pos,
                                       const appl::Buffer::Iterator& _posEnd) {
	if (isEnable() == false) {
		return false;
	}
	appl::History *tmpElement = new appl::History();
	if (tmpElement != NULL) {
		tmpElement->m_addedText = "";
		tmpElement->m_posAdded = (int64_t)_pos;
		tmpElement->m_endPosAdded = tmpElement->m_posAdded;
		tmpElement->m_endPosRemoved = (int64_t)_posEnd;
		_textDrawer.copy(tmpElement->m_removedText, _pos, _posEnd);
		clearRedo();
		m_undo.push_back(tmpElement);
	}
	_textDrawer.removeDirect();
	appl::textPluginManager::onCursorMove(_textDrawer, _textDrawer.cursor());
	return true;
}


void appl::TextPluginHistory::onObjectRemove(ewol::EObject* _removeObject) {
	// TODO : Dependence with buffer removing ...
}



