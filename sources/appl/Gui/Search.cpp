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

Search::Search() :
  ewol::widget::Composer(ewol::widget::Composer::file, "DATA:GUI-Search.xml"),
  m_viewerManager(NULL),
  m_forward(true),
  m_caseSensitive(false),
  m_wrap(true),
  m_searchEntry(NULL),
  m_replaceEntry(NULL) {
	addObjectType("appl::Search");
	// load buffer manager:
	m_viewerManager = appl::ViewerManager::keep();
	// link event
	registerOnEventNameWidget(this, "SEARCH:close",         "pressed", l_eventHideBt);
	registerOnEventNameWidget(this, "SEARCH:search-entry",  "modify",  l_eventSearchEntry);
	registerOnEventNameWidget(this, "SEARCH:search-entry",  "enter",   l_eventSearchEntryEnter);
	registerOnEventNameWidget(this, "SEARCH:search",        "pressed", l_eventSearchBt);
	registerOnEventNameWidget(this, "SEARCH:replace-entry", "modify",  l_eventReplaceEntry);
	registerOnEventNameWidget(this, "SEARCH:replace-entry", "enter",   l_eventReplaceEntryEnter);
	registerOnEventNameWidget(this, "SEARCH:replace",       "pressed", l_eventReplaceBt);
	registerOnEventNameWidget(this, "SEARCH:case",          "value",   l_eventCaseCb);
	registerOnEventNameWidget(this, "SEARCH:wrap",          "value",   l_eventWrapCb);
	registerOnEventNameWidget(this, "SEARCH:up-down",       "value",   l_eventForwardCb);
	// set default properties
	setConfigNamed("SEARCH:case", "value", std::to_string(m_caseSensitive));
	setConfigNamed("SEARCH:wrap", "value", std::to_string(m_wrap));
	setConfigNamed("SEARCH:up-down", "value", std::to_string(m_forward));
	// get widget
	m_searchEntry = ewol::dynamic_pointer_cast<ewol::widget::Entry>(getWidgetNamed("SEARCH:search-entry"));
	m_replaceEntry = ewol::dynamic_pointer_cast<ewol::widget::Entry>(getWidgetNamed("SEARCH:replace-entry"));
	// Display and hide event:
	registerMultiCast(ednMsgGuiSearch);
	// basicly hiden ...
	hide();
}

Search::~Search() {
	
}

void Search::find() {
	if (m_viewerManager == NULL) {
		APPL_WARNING("No viewer manager selected!!!");
		return;
	}
	ewol::object::Shared<appl::TextViewer> viewer = m_viewerManager->getViewerSelected();
	if (viewer == NULL) {
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

void Search::replace() {
	if (m_viewerManager == NULL) {
		APPL_WARNING("No viewer manager selected!!!");
		return;
	}
	ewol::object::Shared<appl::TextViewer> viewer = m_viewerManager->getViewerSelected();
	if (viewer == NULL) {
		APPL_INFO("No viewer selected!!!");
		return;
	}
	if (viewer->hasTextSelected() == false) {
		// nothing to replace ...
		return;
	}
	viewer->replace(m_replaceData);
}


void Search::onReceiveMessage(const ewol::object::Message& _msg) {
	ewol::widget::Composer::onReceiveMessage(_msg);
	APPL_INFO("Search receive message : " << _msg);
	if ( _msg.getMessage() == l_eventSearchEntry) {
		m_searchData = to_u32string(_msg.getData());
	} else if (    _msg.getMessage() == l_eventSearchEntryEnter
	            || _msg.getMessage() == l_eventSearchBt) {
		find();
	} else if ( _msg.getMessage() == l_eventReplaceEntry) {
		m_replaceData = to_u32string(_msg.getData());
	} else if (    _msg.getMessage() == l_eventReplaceEntryEnter
	            || _msg.getMessage() == l_eventReplaceBt) {
		replace();
		find();
	} else if ( _msg.getMessage() == l_eventCaseCb) {
		m_caseSensitive = std::stob(_msg.getData());
	} else if ( _msg.getMessage() == l_eventWrapCb) {
		m_wrap = std::stob(_msg.getData());
	} else if ( _msg.getMessage() == l_eventForwardCb) {
		m_forward = std::stob(_msg.getData());
	} else if ( _msg.getMessage() == l_eventHideBt) {
		hide();
	} else if ( _msg.getMessage() == ednMsgGuiSearch) {
		if (true == isHide()) {
			show();
			if (m_searchEntry!= NULL) {
				m_searchEntry->keepFocus();
			}
		} else {
			if(    (m_searchEntry!=NULL  && true == m_searchEntry->getFocus())
			    || (m_replaceEntry!=NULL && true == m_replaceEntry->getFocus()) ) {
				hide();
			} else if (m_searchEntry!= NULL) {
				m_searchEntry->keepFocus();
			} else {
				hide();
			}
		}
	}
}

void Search::onObjectRemove(const ewol::object::Shared<ewol::Object> _removeObject) {
	ewol::widget::Composer::onObjectRemove(_removeObject);
	if (_removeObject == m_searchEntry) {
		m_searchEntry.reset();
	}
	if (_removeObject == m_replaceEntry) {
		m_replaceEntry.reset();
	}
	if (_removeObject == m_viewerManager) {
		m_viewerManager.reset();
	}
}

