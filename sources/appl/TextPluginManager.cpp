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

static std::list<ewol::object::Owner<appl::TextViewerPlugin>>& getList() {
	static std::list<ewol::object::Owner<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<ewol::object::Shared<appl::TextViewerPlugin>>& getListOnEventEntry() {
	static std::vector<ewol::object::Shared<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<ewol::object::Shared<appl::TextViewerPlugin>>& getListOnEventInput() {
	static std::vector<ewol::object::Shared<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<ewol::object::Shared<appl::TextViewerPlugin>>& getListOnWrite() {
	static std::vector<ewol::object::Shared<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<ewol::object::Shared<appl::TextViewerPlugin>>& getListOnReplace() {
	static std::vector<ewol::object::Shared<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<ewol::object::Shared<appl::TextViewerPlugin>>& getListOnRemove() {
	static std::vector<ewol::object::Shared<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<ewol::object::Shared<appl::TextViewerPlugin>>& getListOnReceiveMessage() {
	static std::vector<ewol::object::Shared<appl::TextViewerPlugin>> s_list;
	return s_list;
}
static std::vector<ewol::object::Shared<appl::TextViewerPlugin>>& getListOnCursorMove() {
	static std::vector<ewol::object::Shared<appl::TextViewerPlugin>> s_list;
	return s_list;
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
	getListOnReceiveMessage().clear();
	getListOnCursorMove().clear();
	getList().clear();
}

void appl::textPluginManager::addDefaultPlugin() {
	appl::textPluginManager::addPlugin(ewol::object::makeShared(new appl::TextPluginCopy()));
	appl::textPluginManager::addPlugin(ewol::object::makeShared(new appl::TextPluginMultiLineTab()));
	appl::textPluginManager::addPlugin(ewol::object::makeShared(new appl::TextPluginAutoIndent()));
	appl::textPluginManager::addPlugin(ewol::object::makeShared(new appl::TextPluginHistory()));
	appl::textPluginManager::addPlugin(ewol::object::makeShared(new appl::TextPluginRmLine()));
	appl::textPluginManager::addPlugin(ewol::object::makeShared(new appl::TextPluginSelectAll()));
	appl::textPluginManager::addPlugin(ewol::object::makeShared(new appl::TextPluginCtags()));
}

void appl::textPluginManager::addPlugin(const ewol::object::Shared<appl::TextViewerPlugin>& _plugin) {
	if (_plugin == NULL) {
		return;
	}
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
	if (_plugin->isAvaillableOnReceiveMessage() == true) {
		getListOnReceiveMessage().push_back(_plugin);
	}
	if (_plugin->isAvaillableOnCursorMove() == true) {
		getListOnCursorMove().push_back(_plugin);
	}
}

void appl::textPluginManager::connect(appl::TextViewer& _widget) {
	for (auto &it : getList()) {
		if (it == NULL) {
			continue;
		}
		it->onPluginEnable(_widget);
	}
}

void appl::textPluginManager::disconnect(appl::TextViewer& _widget) {
	for (auto &it : getList()) {
		if (it == NULL) {
			continue;
		}
		it->onPluginDisable(_widget);
	}
}

bool appl::textPluginManager::onEventEntry(appl::TextViewer& _textDrawer,
                                           const ewol::event::Entry& _event) {
	for (auto &it : getListOnEventEntry()) {
		if (it == NULL) {
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
		if (it == NULL) {
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
		if (it == NULL) {
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
		if (it == NULL) {
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
		if (it == NULL) {
			continue;
		}
		if (it->onRemove(_textDrawer, _pos, _posEnd) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onReceiveMessage(appl::TextViewer& _textDrawer,
                                               const ewol::object::Message& _msg) {
	for (auto &it : getListOnReceiveMessage()) {
		if (it == NULL) {
			continue;
		}
		if (it->onReceiveMessage(_textDrawer, _msg) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onCursorMove(appl::TextViewer& _textDrawer,
                                           const appl::Buffer::Iterator& _pos) {
	for (auto &it : getListOnCursorMove()) {
		if (it == NULL) {
			continue;
		}
		if (it->onCursorMove(_textDrawer, _pos) == true ) {
			return true;
		}
	}
	return false;
}

