/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/TextPluginHistory.h>
#include <ewol/context/clipBoard.h>
#include <appl/Gui/TextViewer.h>
#include <appl/TextPluginManager.h>

#undef __class__
#define __class__ "TextPluginHistory"

appl::TextPluginHistory::TextPluginHistory() {
	m_activateOnReceiveMessage = true;
	m_activateOnWrite = true;
	m_activateOnReplace = true;
	m_activateOnRemove = true;
	addObjectType("appl::TextPluginHistory");
}

void appl::TextPluginHistory::init() {
	appl::TextViewerPluginData<appl::PluginHistoryData>::init();
}


void appl::TextPluginHistory::onPluginEnable(appl::TextViewer& _textDrawer) {
	// add event :
	/*
	_textDrawer.ext_registerMultiCast(ednMsgGuiRedo);
	_textDrawer.ext_registerMultiCast(ednMsgGuiUndo);
	_textDrawer.ext_shortCutAdd("ctrl+z", ednMsgGuiUndo);
	_textDrawer.ext_shortCutAdd("ctrl+shift+z", ednMsgGuiRedo);
	*/
}

void appl::TextPluginHistory::onPluginDisable(appl::TextViewer& _textDrawer) {
	// TODO : unknow function ...
}

bool appl::TextPluginHistory::onDataReceiveMessageViewer(appl::TextViewer& _textDrawer,
                                                         const ewol::object::Message& _msg,
                                                         appl::PluginHistoryData& _data) {
	if (isEnable() == false) {
		return false;
	}
	/*
	if (_msg.getMessage() == ednMsgGuiRedo) {
		if (_data.m_redo.size() == 0) {
			return true;
		}
		if (_data.m_redo[_data.m_redo.size()-1] == nullptr) {
			_data.m_redo.pop_back();
			return true;
		}
		appl::History *tmpElement = _data.m_redo[_data.m_redo.size()-1];
		_data.m_redo.pop_back();
		_data.m_undo.push_back(tmpElement);
		_textDrawer.replaceDirect(tmpElement->m_addedText,
		                          _textDrawer.position(tmpElement->m_posAdded),
		                          _textDrawer.position(tmpElement->m_endPosRemoved) );
		
		return true;
	} else if (_msg.getMessage() == ednMsgGuiUndo) {
		if (_data.m_undo.size() == 0) {
			return true;
		}
		if (_data.m_undo[_data.m_undo.size()-1] == nullptr) {
			_data.m_undo.pop_back();
			return true;
		}
		appl::History *tmpElement = _data.m_undo[_data.m_undo.size()-1];
		_data.m_undo.pop_back();
		_data.m_redo.push_back(tmpElement);
		_textDrawer.replaceDirect(tmpElement->m_removedText,
		                          _textDrawer.position(tmpElement->m_posAdded),
		                          _textDrawer.position(tmpElement->m_endPosAdded) );
		
		return true;
	}
	*/
	return false;
}

void appl::TextPluginHistory::clearRedo(appl::PluginHistoryData& _data) {
	if (_data.m_redo.size() == 0) {
		return;
	}
	for (size_t iii=0; iii<_data.m_redo.size(); ++iii) {
		if (_data.m_redo[iii] == nullptr) {
			continue;
		}
		delete(_data.m_redo[iii]);
		_data.m_redo[iii] = nullptr;
	}
	_data.m_redo.clear();
}

void appl::TextPluginHistory::clearUndo(appl::PluginHistoryData& _data) {
	if (_data.m_undo.size() == 0) {
		return;
	}
	for (size_t iii=0; iii<_data.m_undo.size(); ++iii) {
		if (_data.m_undo[iii] == nullptr) {
			continue;
		}
		delete(_data.m_undo[iii]);
		_data.m_undo[iii] = nullptr;
	}
	_data.m_undo.clear();
}


bool appl::TextPluginHistory::onDataWrite(appl::TextViewer& _textDrawer,
                                          const appl::Buffer::Iterator& _pos,
                                          const std::string& _strData,
                                          appl::PluginHistoryData& _data) {
	if (isEnable() == false) {
		return false;
	}
	appl::History *tmpElement = new appl::History();
	if (tmpElement != nullptr) {
		tmpElement->m_addedText = _strData;
		tmpElement->m_posAdded = (int64_t)_pos;
		tmpElement->m_endPosRemoved = (int64_t)_pos;
	}
	_textDrawer.writeDirect(_strData, _pos);
	if (tmpElement != nullptr) {
		tmpElement->m_endPosAdded = (int64_t)_textDrawer.cursor();
		clearRedo(_data);
		_data.m_undo.push_back(tmpElement);
	}
	appl::textPluginManager::onCursorMove(_textDrawer, _textDrawer.cursor());
	return true;
}

bool appl::TextPluginHistory::onDataReplace(appl::TextViewer& _textDrawer,
                                            const appl::Buffer::Iterator& _pos,
                                            const std::string& _strData,
                                            const appl::Buffer::Iterator& _posEnd,
                                            appl::PluginHistoryData& _data) {
	if (isEnable() == false) {
		return false;
	}
	appl::History *tmpElement = new appl::History();
	if (tmpElement != nullptr) {
		tmpElement->m_posAdded = (int64_t)_pos;
		tmpElement->m_addedText = _strData;
		tmpElement->m_endPosRemoved = (int64_t)_posEnd;
		_textDrawer.copy(tmpElement->m_removedText, _pos, _posEnd);
	}
	_textDrawer.replaceDirect(_strData, _pos, _posEnd);
	if (tmpElement != nullptr) {
		tmpElement->m_endPosAdded = (int64_t)_textDrawer.cursor();
		clearRedo(_data);
		_data.m_undo.push_back(tmpElement);
	}
	appl::textPluginManager::onCursorMove(_textDrawer, _textDrawer.cursor());
	return true;
}

bool appl::TextPluginHistory::onDataRemove(appl::TextViewer& _textDrawer,
                                           const appl::Buffer::Iterator& _pos,
                                           const appl::Buffer::Iterator& _posEnd,
                                           appl::PluginHistoryData& _data) {
	if (isEnable() == false) {
		return false;
	}
	appl::History *tmpElement = new appl::History();
	if (tmpElement != nullptr) {
		tmpElement->m_addedText = "";
		tmpElement->m_posAdded = (int64_t)_pos;
		tmpElement->m_endPosAdded = tmpElement->m_posAdded;
		tmpElement->m_endPosRemoved = (int64_t)_posEnd;
		_textDrawer.copy(tmpElement->m_removedText, _pos, _posEnd);
		clearRedo(_data);
		_data.m_undo.push_back(tmpElement);
	}
	_textDrawer.removeDirect();
	appl::textPluginManager::onCursorMove(_textDrawer, _textDrawer.cursor());
	return true;
}


