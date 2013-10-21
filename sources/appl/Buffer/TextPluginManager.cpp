/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Buffer/TextPluginManager.h>
#include <appl/Debug.h>
#include <appl/Buffer/TextPluginCopy.h>
#include <appl/Buffer/TextPluginMultiLineTab.h>
#include <appl/Buffer/TextPluginAutoIndent.h>

static etk::Vector<appl::TextViewerPlugin *>& getList(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static etk::Vector<appl::TextViewerPlugin *>& getListOnEventEntry(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static etk::Vector<appl::TextViewerPlugin *>& getListOnEventInput(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static etk::Vector<appl::TextViewerPlugin *>& getListOnWrite(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static etk::Vector<appl::TextViewerPlugin *>& getListOnReplace(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static etk::Vector<appl::TextViewerPlugin *>& getListOnRemove(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static etk::Vector<appl::TextViewerPlugin *>& getListOnReceiveMessage(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
	return s_list;
}
static etk::Vector<appl::TextViewerPlugin *>& getListOnCursorMove(void) {
	static etk::Vector<appl::TextViewerPlugin *> s_list;
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
	for (esize_t iii=0; iii<getList().size(); ++iii) {
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
}

void appl::textPluginManager::addPlugin(appl::TextViewerPlugin* _plugin) {
	if (_plugin == NULL) {
		return;
	}
	getList().pushBack(_plugin);
	if (_plugin->isAvaillableOnEventEntry() == true) {
		getListOnEventEntry().pushBack(_plugin);
	}
	if (_plugin->isAvaillableOnEventInput() == true) {
		getListOnEventInput().pushBack(_plugin);
	}
	if (_plugin->isAvaillableOnWrite() == true) {
		getListOnWrite().pushBack(_plugin);
	}
	if (_plugin->isAvaillableOnReplace() == true) {
		getListOnReplace().pushBack(_plugin);
	}
	if (_plugin->isAvaillableOnRemove() == true) {
		getListOnRemove().pushBack(_plugin);
	}
	if (_plugin->isAvaillableOnReceiveMessage() == true) {
		getListOnReceiveMessage().pushBack(_plugin);
	}
	if (_plugin->isAvaillableOnCursorMove() == true) {
		getListOnCursorMove().pushBack(_plugin);
	}
}

void appl::textPluginManager::connect(appl::TextViewer& _widget) {
	for (esize_t iii=0; iii<getList().size(); ++iii) {
		if (getList()[iii] == NULL) {
			continue;
		}
		getList()[iii]->onPluginEnable(_widget);
	}
}

void appl::textPluginManager::disconnect(appl::TextViewer& _widget) {
	for (esize_t iii=0; iii<getList().size(); ++iii) {
		if (getList()[iii] == NULL) {
			continue;
		}
		getList()[iii]->onPluginDisable(_widget);
	}
}

bool appl::textPluginManager::onEventEntry(appl::TextViewer& _textDrawer,
                                           const ewol::EventEntry& _event) {
	etk::Vector<appl::TextViewerPlugin *>& list = getListOnEventEntry();
	for (esize_t iii=0; iii<list.size(); ++iii) {
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
                                           const ewol::EventInput& _event) {
	etk::Vector<appl::TextViewerPlugin *>& list = getListOnEventInput();
	for (esize_t iii=0; iii<list.size(); ++iii) {
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
                                      const etk::UString& _data) {
	etk::Vector<appl::TextViewerPlugin *>& list = getListOnWrite();
	for (esize_t iii=0; iii<list.size(); ++iii) {
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
                                        const etk::UString& _data,
                                        const appl::Buffer::Iterator& _posEnd) {
	etk::Vector<appl::TextViewerPlugin *>& list = getListOnReplace();
	for (esize_t iii=0; iii<list.size(); ++iii) {
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
	etk::Vector<appl::TextViewerPlugin *>& list = getListOnRemove();
	for (esize_t iii=0; iii<list.size(); ++iii) {
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
                                               const ewol::EMessage& _msg) {
	etk::Vector<appl::TextViewerPlugin *>& list = getListOnReceiveMessage();
	for (esize_t iii=0; iii<list.size(); ++iii) {
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
	etk::Vector<appl::TextViewerPlugin *>& list = getListOnCursorMove();
	for (esize_t iii=0; iii<list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->onCursorMove(_textDrawer, _pos) == true ) {
			return true;
		}
	}
	return false;
}

