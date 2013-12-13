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

static std::vector<appl::TextViewerPlugin *>& getList(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static std::vector<appl::TextViewerPlugin *>& getListOnEventEntry(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static std::vector<appl::TextViewerPlugin *>& getListOnEventInput(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static std::vector<appl::TextViewerPlugin *>& getListOnWrite(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static std::vector<appl::TextViewerPlugin *>& getListOnReplace(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static std::vector<appl::TextViewerPlugin *>& getListOnRemove(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static std::vector<appl::TextViewerPlugin *>& getListOnReceiveMessage(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static std::vector<appl::TextViewerPlugin *>& getListOnCursorMove(void) {
	static std::vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}

void appl::textPluginManager::init(void) {
	
}

void appl::textPluginManager::unInit(void) {
	// remove all sub plugin class:
	getListOnEventEntry().clear();
	getListOnEventInput().clear();
	getListOnWrite().clear();
	getListOnReplace().clear();
	getListOnRemove().clear();
	getListOnReceiveMessage().clear();
	getListOnCursorMove().clear();
	// remove all plugin:
	for (int32_t iii=0; iii<getList().size(); ++iii) {
		if (getList()[iii] == NULL) {
			continue;
		}
		delete(getList()[iii]);
		getList()[iii] = NULL;
	}
	getList().clear();
}

void appl::textPluginManager::addDefaultPlugin(void) {
	appl::textPluginManager::addPlugin(new appl::TextPluginCopy());
	appl::textPluginManager::addPlugin(new appl::TextPluginMultiLineTab());
	appl::textPluginManager::addPlugin(new appl::TextPluginAutoIndent());
	appl::textPluginManager::addPlugin(new appl::TextPluginHistory());
	appl::textPluginManager::addPlugin(new appl::TextPluginRmLine());
	appl::textPluginManager::addPlugin(new appl::TextPluginSelectAll());
	appl::textPluginManager::addPlugin(new appl::TextPluginCtags());
}

void appl::textPluginManager::addPlugin(appl::TextViewerPlugin* _plugin) {
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
	for (int32_t iii=0; iii<getList().size(); ++iii) {
		if (getList()[iii] == NULL) {
			continue;
		}
		getList()[iii]->onPluginEnable(_widget);
	}
}

void appl::textPluginManager::disconnect(appl::TextViewer& _widget) {
	for (int32_t iii=0; iii<getList().size(); ++iii) {
		if (getList()[iii] == NULL) {
			continue;
		}
		getList()[iii]->onPluginDisable(_widget);
	}
}

bool appl::textPluginManager::onEventEntry(appl::TextViewer& _textDrawer,
                                           const ewol::event::Entry& _event) {
	std::vector<appl::TextViewerPlugin *>& list = getListOnEventEntry();
	for (int32_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onEventEntry(_textDrawer, _event) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onEventInput(appl::TextViewer& _textDrawer,
                                           const ewol::event::Input& _event) {
	std::vector<appl::TextViewerPlugin *>& list = getListOnEventInput();
	for (int32_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onEventInput(_textDrawer, _event) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onWrite(appl::TextViewer& _textDrawer,
                                      const appl::Buffer::Iterator& _pos,
                                      const std::string& _data) {
	std::vector<appl::TextViewerPlugin *>& list = getListOnWrite();
	for (int32_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onWrite(_textDrawer, _pos, _data) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onReplace(appl::TextViewer& _textDrawer,
                                        const appl::Buffer::Iterator& _pos,
                                        const std::string& _data,
                                        const appl::Buffer::Iterator& _posEnd) {
	std::vector<appl::TextViewerPlugin *>& list = getListOnReplace();
	for (int32_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onReplace(_textDrawer, _pos, _data, _posEnd) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onRemove(appl::TextViewer& _textDrawer,
                                       const appl::Buffer::Iterator& _pos,
                                       const appl::Buffer::Iterator& _posEnd) {
	std::vector<appl::TextViewerPlugin *>& list = getListOnRemove();
	for (int32_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onRemove(_textDrawer, _pos, _posEnd) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onReceiveMessage(appl::TextViewer& _textDrawer,
                                               const ewol::object::Message& _msg) {
	std::vector<appl::TextViewerPlugin *>& list = getListOnReceiveMessage();
	for (int32_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onReceiveMessage(_textDrawer, _msg) == true ) {
			return true;
		}
	}
	return false;
}

bool appl::textPluginManager::onCursorMove(appl::TextViewer& _textDrawer,
                                           const appl::Buffer::Iterator& _pos) {
	std::vector<appl::TextViewerPlugin *>& list = getListOnCursorMove();
	for (int32_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onCursorMove(_textDrawer, _pos) == true ) {
			return true;
		}
	}
	return false;
}

