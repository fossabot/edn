/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/TextPluginManager.hpp>
#include <appl/debug.hpp>
#include <appl/TextPluginCopy.hpp>
#include <appl/TextPluginMultiLineTab.hpp>
#include <appl/TextPluginAutoIndent.hpp>
#include <appl/TextPluginHistory.hpp>
#include <appl/TextPluginRmLine.hpp>
#include <appl/TextPluginSelectAll.hpp>
#include <appl/TextPluginCtags.hpp>

appl::textPluginManager::textPluginManager() {
	
}
void appl::textPluginManager::init(const std::string& _name) {
	gale::Resource::init(_name);
}

void appl::textPluginManager::addDefaultPlugin() {
	appl::textPluginManager::addPlugin(appl::TextPluginCopy::create());
	appl::textPluginManager::addPlugin(appl::TextPluginMultiLineTab::create());
	appl::textPluginManager::addPlugin(appl::TextPluginAutoIndent::create());
	appl::textPluginManager::addPlugin(appl::TextPluginHistory::create());
	appl::textPluginManager::addPlugin(appl::TextPluginRmLine::create());
	appl::textPluginManager::addPlugin(appl::TextPluginSelectAll::create());
	appl::textPluginManager::addPlugin(appl::TextPluginCtags::create());
}

void appl::textPluginManager::addPlugin(ememory::SharedPtr<appl::TextViewerPlugin> _plugin) {
	if (_plugin == nullptr) {
		return;
	}
	APPL_DEBUG("Add plugin : " << _plugin->getObjectType());
	m_list.push_back(_plugin);
	if (_plugin->isAvaillableOnEventEntry() == true) {
		m_listOnEventEntry.push_back(_plugin);
	}
	if (_plugin->isAvaillableOnEventInput() == true) {
		m_listOnEventInput.push_back(_plugin);
	}
	if (_plugin->isAvaillableOnWrite() == true) {
		m_listOnWrite.push_back(_plugin);
	}
	if (_plugin->isAvaillableOnReplace() == true) {
		m_listOnReplace.push_back(_plugin);
	}
	if (_plugin->isAvaillableOnRemove() == true) {
		m_listOnRemove.push_back(_plugin);
	}
	if (_plugin->isAvaillableOnReceiveShortCut() == true) {
		m_listOnReceiveShortCutViewer.push_back(_plugin);
	}
	if (_plugin->isAvaillableOnCursorMove() == true) {
		m_listOnCursorMove.push_back(_plugin);
	}
	ememory::SharedPtr<appl::TextViewer> viewer = m_currentViewer.lock();
	if (viewer != nullptr) {
		_plugin->onPluginEnable(*viewer);
	}
}

void appl::textPluginManager::connect(appl::TextViewer& _widget) {
	m_currentViewer = ememory::dynamicPointerCast<appl::TextViewer>(_widget.sharedFromThis());
	for (auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		it->onPluginEnable(_widget);
	}
}

void appl::textPluginManager::disconnect(appl::TextViewer& _widget) {
	m_currentViewer.reset();
	for (auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		it->onPluginDisable(_widget);
	}
}

bool appl::textPluginManager::onEventEntry(appl::TextViewer& _textDrawer,
                                           const ewol::event::Entry& _event) {
	for (auto &it : m_listOnEventEntry) {
		if (it == nullptr) {
			continue;
		}
		if (it->onEventEntry(_textDrawer, _event) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onEventInput(appl::TextViewer& _textDrawer,
                                           const ewol::event::Input& _event) {
	for (auto &it : m_listOnEventInput) {
		if (it == nullptr) {
			continue;
		}
		if (it->onEventInput(_textDrawer, _event) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onWrite(appl::TextViewer& _textDrawer,
                                      const appl::Buffer::Iterator& _pos,
                                      const std::string& _data) {
	for (auto &it : m_listOnWrite) {
		if (it == nullptr) {
			continue;
		}
		if (it->onWrite(_textDrawer, _pos, _data) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onReplace(appl::TextViewer& _textDrawer,
                                        const appl::Buffer::Iterator& _pos,
                                        const std::string& _data,
                                        const appl::Buffer::Iterator& _posEnd) {
	for (auto &it : m_listOnReplace) {
		if (it == nullptr) {
			continue;
		}
		if (it->onReplace(_textDrawer, _pos, _data, _posEnd) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onRemove(appl::TextViewer& _textDrawer,
                                       const appl::Buffer::Iterator& _pos,
                                       const appl::Buffer::Iterator& _posEnd) {
	for (auto &it : m_listOnRemove) {
		if (it == nullptr) {
			continue;
		}
		if (it->onRemove(_textDrawer, _pos, _posEnd) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onReceiveShortCut(appl::TextViewer& _textDrawer,
                                                const std::string& _shortCutName) {
	for (auto &it : m_listOnReceiveShortCutViewer) {
		if (it == nullptr) {
			continue;
		}
		if (it->onReceiveShortCut(_textDrawer, _shortCutName) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onCursorMove(appl::TextViewer& _textDrawer,
                                           const appl::Buffer::Iterator& _pos) {
	for (auto &it : m_listOnCursorMove) {
		if (it == nullptr) {
			continue;
		}
		if (it->onCursorMove(_textDrawer, _pos) == true ) {
			return true;
		}
	}
	return false;
}

