/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/TextPluginManager.h>
#include <appl/debug.h>
#include <appl/TextPluginCopy.h>
#include <appl/TextPluginMultiLineTab.h>
#include <appl/TextPluginAutoIndent.h>
#include <appl/TextPluginHistory.h>
#include <appl/TextPluginRmLine.h>
#include <appl/TextPluginSelectAll.h>
#include <appl/TextPluginCtags.h>

#undef __class__
#define __class__ "textPluginManager"

static std::list<std::shared_ptr<appl::TextViewerPlugin>>& getList() {
	static std::list<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<std::shared_ptr<appl::TextViewerPlugin>>& getListOnEventEntry() {
	static std::vector<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<std::shared_ptr<appl::TextViewerPlugin>>& getListOnEventInput() {
	static std::vector<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<std::shared_ptr<appl::TextViewerPlugin>>& getListOnWrite() {
	static std::vector<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<std::shared_ptr<appl::TextViewerPlugin>>& getListOnReplace() {
	static std::vector<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<std::shared_ptr<appl::TextViewerPlugin>>& getListOnRemove() {
	static std::vector<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<std::shared_ptr<appl::TextViewerPlugin>>& getListonReceiveShortCutViewer() {
	static std::vector<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<std::shared_ptr<appl::TextViewerPlugin>>& getListOnCursorMove() {
	static std::vector<std::shared_ptr<appl::TextViewerPlugin>> s_list;
	return s_list;
}

static std::weak_ptr<appl::TextViewer>& getViewerConnected() {
	static std::weak_ptr<appl::TextViewer> s_widget;
	return s_widget;
}

void appl::textPluginManager::init() {
	
}

void appl::textPluginManager::unInit() {
	// remove all sub plugin class:
	getListOnEventEntry().clear();
	getListOnEventInput().clear();
	getListOnWrite().clear();
	getListOnReplace().clear();
	getListOnRemove().clear();
	getListonReceiveShortCutViewer().clear();
	getListOnCursorMove().clear();
	getList().clear();
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

void appl::textPluginManager::addPlugin(const std::shared_ptr<appl::TextViewerPlugin>& _plugin) {
	if (_plugin == nullptr) {
		return;
	}
	APPL_DEBUG("Add plugin : " << _plugin->getObjectType());
	getList().push_back(_plugin);
	if (_plugin->isAvaillableOnEventEntry() == true) {
		getListOnEventEntry().push_back(_plugin);
	}
	if (_plugin->isAvaillableOnEventInput() == true) {
		getListOnEventInput().push_back(_plugin);
	}
	if (_plugin->isAvaillableOnWrite() == true) {
		getListOnWrite().push_back(_plugin);
	}
	if (_plugin->isAvaillableOnReplace() == true) {
		getListOnReplace().push_back(_plugin);
	}
	if (_plugin->isAvaillableOnRemove() == true) {
		getListOnRemove().push_back(_plugin);
	}
	if (_plugin->isAvaillableOnReceiveShortCut() == true) {
		getListonReceiveShortCutViewer().push_back(_plugin);
	}
	if (_plugin->isAvaillableOnCursorMove() == true) {
		getListOnCursorMove().push_back(_plugin);
	}
	std::shared_ptr<appl::TextViewer> viewer = getViewerConnected().lock();
	if (viewer != nullptr) {
		_plugin->onPluginEnable(*viewer);
	}
}

void appl::textPluginManager::connect(appl::TextViewer& _widget) {
	getViewerConnected() = std::dynamic_pointer_cast<appl::TextViewer>(_widget.shared_from_this());
	for (auto &it : getList()) {
		if (it == nullptr) {
			continue;
		}
		it->onPluginEnable(_widget);
	}
}

void appl::textPluginManager::disconnect(appl::TextViewer& _widget) {
	getViewerConnected().reset();
	for (auto &it : getList()) {
		if (it == nullptr) {
			continue;
		}
		it->onPluginDisable(_widget);
	}
}

bool appl::textPluginManager::onEventEntry(appl::TextViewer& _textDrawer,
                                           const ewol::event::Entry& _event) {
	for (auto &it : getListOnEventEntry()) {
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
	for (auto &it : getListOnEventInput()) {
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
	for (auto &it : getListOnWrite()) {
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
	for (auto &it : getListOnReplace()) {
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
	for (auto &it : getListOnRemove()) {
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
	for (auto &it : getListonReceiveShortCutViewer()) {
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
	for (auto &it : getListOnCursorMove()) {
		if (it == nullptr) {
			continue;
		}
		if (it->onCursorMove(_textDrawer, _pos) == true ) {
			return true;
		}
	}
	return false;
}

