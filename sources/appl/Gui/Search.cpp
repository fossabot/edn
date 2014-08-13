/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include "appl/global.h"
#include "appl/Gui/Search.h"
#include "appl/BufferManager.h"
#include "appl/Gui/TextViewer.h"
#include "appl/Gui/MainWindows.h"
#include "appl/globalMsg.h"


#undef __class__
#define __class__ "Search"

const char* const l_eventSearchEntry = "appl-search-entry";
const char* const l_eventSearchEntryEnter = "appl-search-entry-enter";
const char* const l_eventReplaceEntry = "appl-replace-entry";
const char* const l_eventReplaceEntryEnter = "appl-replace-entry-enter";
const char* const l_eventSearchBt = "appl-search-button";
const char* const l_eventReplaceBt = "appl-replace-button";
const char* const l_eventCaseCb = "appl-case-sensitive-CheckBox";
const char* const l_eventWrapCb = "appl-wrap-CheckBox";
const char* const l_eventForwardCb = "appl-forward-CheckBox";
const char* const l_eventHideBt = "appl-hide-button";

appl::widget::Search::Search() :
  m_forward(true),
  m_caseSensitive(false),
  m_wrap(true) {
	addObjectType("appl::widget::Search");
}
void appl::widget::Search::init() {
	ewol::widget::Composer::init(ewol::widget::Composer::file, "DATA:GUI-Search.xml");
	// load buffer manager:onObjectRemove
	m_viewerManager = appl::ViewerManager::create();
	// link event
	registerOnEventNameWidget(shared_from_this(), "SEARCH:close",         "pressed", l_eventHideBt);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:search-entry",  "modify",  l_eventSearchEntry);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:search-entry",  "enter",   l_eventSearchEntryEnter);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:search",        "pressed", l_eventSearchBt);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:replace-entry", "modify",  l_eventReplaceEntry);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:replace-entry", "enter",   l_eventReplaceEntryEnter);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:replace",       "pressed", l_eventReplaceBt);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:case",          "value",   l_eventCaseCb);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:wrap",          "value",   l_eventWrapCb);
	registerOnEventNameWidget(shared_from_this(), "SEARCH:up-down",       "value",   l_eventForwardCb);
	// set default properties
	setConfigNamed("SEARCH:case", "value", etk::to_string(m_caseSensitive));
	setConfigNamed("SEARCH:wrap", "value", etk::to_string(m_wrap));
	setConfigNamed("SEARCH:up-down", "value", etk::to_string(m_forward));
	// get widget
	m_searchEntry = std::dynamic_pointer_cast<ewol::widget::Entry>(getWidgetNamed("SEARCH:search-entry"));
	m_replaceEntry = std::dynamic_pointer_cast<ewol::widget::Entry>(getWidgetNamed("SEARCH:replace-entry"));
	// Display and hide event:
	registerMultiCast(ednMsgGuiSearch);
	// basicly hiden ...
	hide();
}

appl::widget::Search::~Search() {
	
}

void appl::widget::Search::find() {
	if (m_viewerManager == nullptr) {
		APPL_WARNING("No viewer manager selected!!!");
		return;
	}
	std::shared_ptr<appl::TextViewer> viewer = m_viewerManager->getViewerSelected();
	if (viewer == nullptr) {
		APPL_INFO("No viewer selected!!!");
		return;
	}
	viewer->unSelect();
	appl::Buffer::Iterator resultStart;
	appl::Buffer::Iterator resultStop;
	if (m_forward == true) {
		if (viewer->find(viewer->cursor(), m_searchData, resultStart, resultStop, m_caseSensitive) == false) {
			if (    m_wrap == false 
			     || viewer->find(viewer->begin(), m_searchData, resultStart, resultStop, m_caseSensitive) == false) {
				// TODO : Display an IHM pop-up
				APPL_WARNING("No element find ...");
				return;
			}
		}
		viewer->select(resultStart, resultStop);
	} else {
		if (viewer->rfind(viewer->cursor(), m_searchData, resultStart, resultStop, m_caseSensitive) == false) {
			if (    m_wrap == false 
			     || viewer->rfind(viewer->end(), m_searchData, resultStart, resultStop, m_caseSensitive) == false) {
				// TODO : Display an IHM pop-up
				APPL_WARNING("No element find ...");
				return;
			}
		}
		viewer->select(resultStop, resultStart);
	}
}

void appl::widget::Search::replace() {
	if (m_viewerManager == nullptr) {
		APPL_WARNING("No viewer manager selected!!!");
		return;
	}
	std::shared_ptr<appl::TextViewer> viewer = m_viewerManager->getViewerSelected();
	if (viewer == nullptr) {
		APPL_INFO("No viewer selected!!!");
		return;
	}
	if (viewer->hasTextSelected() == false) {
		// nothing to replace ...
		return;
	}
	viewer->replace(m_replaceData);
}


void appl::widget::Search::onReceiveMessage(const ewol::object::Message& _msg) {
	ewol::widget::Composer::onReceiveMessage(_msg);
	APPL_INFO("Search receive message : " << _msg);
	if ( _msg.getMessage() == l_eventSearchEntry) {
		m_searchData = etk::to_u32string(_msg.getData());
	} else if (    _msg.getMessage() == l_eventSearchEntryEnter
	            || _msg.getMessage() == l_eventSearchBt) {
		find();
	} else if ( _msg.getMessage() == l_eventReplaceEntry) {
		m_replaceData = etk::to_u32string(_msg.getData());
	} else if (    _msg.getMessage() == l_eventReplaceEntryEnter
	            || _msg.getMessage() == l_eventReplaceBt) {
		replace();
		find();
	} else if ( _msg.getMessage() == l_eventCaseCb) {
		m_caseSensitive = etk::string_to_bool(_msg.getData());
	} else if ( _msg.getMessage() == l_eventWrapCb) {
		m_wrap = etk::string_to_bool(_msg.getData());
	} else if ( _msg.getMessage() == l_eventForwardCb) {
		m_forward = etk::string_to_bool(_msg.getData());
	} else if ( _msg.getMessage() == l_eventHideBt) {
		hide();
	} else if ( _msg.getMessage() == ednMsgGuiSearch) {
		if (true == isHide()) {
			show();
			if (m_searchEntry!= nullptr) {
				m_searchEntry->keepFocus();
			}
		} else {
			if(    (m_searchEntry!=nullptr  && true == m_searchEntry->getFocus())
			    || (m_replaceEntry!=nullptr && true == m_replaceEntry->getFocus()) ) {
				hide();
			} else if (m_searchEntry!= nullptr) {
				m_searchEntry->keepFocus();
			} else {
				hide();
			}
		}
	}
}

void appl::widget::Search::onObjectRemove(const std::shared_ptr<ewol::Object>& _object) {
	ewol::widget::Composer::onObjectRemove(_object);
	if (_object == m_searchEntry) {
		m_searchEntry.reset();
	}
	if (_object == m_replaceEntry) {
		m_replaceEntry.reset();
	}
	if (_object == m_viewerManager) {
		m_viewerManager.reset();
	}
}

