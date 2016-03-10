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
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>


#undef __class__
#define __class__ "Search"

appl::widget::Search::Search() :
  m_forward(true),
  m_caseSensitive(false),
  m_wrap(true) {
	addObjectType("appl::widget::Search");
}
void appl::widget::Search::init() {
	ewol::widget::Composer::init();
	loadFromFile("DATA:GUI-Search.xml");
	m_viewerManager = appl::ViewerManager::create();
	// link event
	subBind(ewol::widget::Button, "SEARCH:close",         signalPressed, shared_from_this(), &appl::widget::Search::OnCallbackHide);
	subBind(ewol::widget::Entry,  "SEARCH:search-entry",  signalModify,  shared_from_this(), &appl::widget::Search::OnCallbackSearchValue);
	subBind(ewol::widget::Entry,  "SEARCH:search-entry",  signalEnter,   shared_from_this(), &appl::widget::Search::OnCallbackSearchEntryValidate);
	subBind(ewol::widget::Button, "SEARCH:search",        signalPressed, shared_from_this(), &appl::widget::Search::OnCallbackSearch);
	subBind(ewol::widget::Entry,  "SEARCH:replace-entry", signalModify,  shared_from_this(), &appl::widget::Search::OnCallbackReplaceValue);
	subBind(ewol::widget::Entry,  "SEARCH:replace-entry", signalEnter,   shared_from_this(), &appl::widget::Search::OnCallbackReplaceEntryValidate);
	subBind(ewol::widget::Button, "SEARCH:replace",       signalPressed, shared_from_this(), &appl::widget::Search::OnCallbackReplace);
	subBind(ewol::widget::Button, "SEARCH:case",          signalValue,   shared_from_this(), &appl::widget::Search::OnCallbackCase);
	subBind(ewol::widget::Button, "SEARCH:wrap",          signalValue,   shared_from_this(), &appl::widget::Search::OnCallbackWrap);
	subBind(ewol::widget::Button, "SEARCH:up-down",       signalValue,   shared_from_this(), &appl::widget::Search::OnCallbackForward);
	// set default properties
	propertySetOnWidgetNamed("SEARCH:case", "value", etk::to_string(m_caseSensitive));
	propertySetOnWidgetNamed("SEARCH:wrap", "value", etk::to_string(m_wrap));
	propertySetOnWidgetNamed("SEARCH:up-down", "value", etk::to_string(m_forward));
	// get widget
	m_searchEntry = std::dynamic_pointer_cast<ewol::widget::Entry>(getSubObjectNamed("SEARCH:search-entry"));
	m_replaceEntry = std::dynamic_pointer_cast<ewol::widget::Entry>(getSubObjectNamed("SEARCH:replace-entry"));
	// basicly hiden ...
	propertyHide.set(true);
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

void appl::widget::Search::OnCallbackHide() {
	propertyHide.set(true);
}
void appl::widget::Search::OnCallbackSearchValue(const std::string& _value) {
	m_searchData = etk::to_u32string(_value);
}
void appl::widget::Search::OnCallbackSearch() {
	find();
}
void appl::widget::Search::OnCallbackSearchEntryValidate(const std::string& _value) {
	m_searchData = etk::to_u32string(_value);
	find();
}
void appl::widget::Search::OnCallbackReplaceValue(const std::string& _value) {
	m_replaceData = etk::to_u32string(_value);
}
void appl::widget::Search::OnCallbackReplace() {
	replace();
	find();
}
void appl::widget::Search::OnCallbackReplaceEntryValidate(const std::string& _value) {
	m_replaceData = etk::to_u32string(_value);
	replace();
	find();
}
void appl::widget::Search::OnCallbackCase(const bool& _value) {
	m_caseSensitive = _value;
}
void appl::widget::Search::OnCallbackWrap(const bool& _value) {
	m_wrap = _value;
}
void appl::widget::Search::OnCallbackForward(const bool& _value) {
	m_forward = _value;
}

void appl::widget::Search::selectSearch() {
	if (m_searchEntry != nullptr) {
		m_searchEntry->keepFocus();
	}
}

bool appl::widget::Search::isSelectSearch() {
	if (m_searchEntry != nullptr) {
		return m_searchEntry->getFocus();
	}
	return false;
}

void appl::widget::Search::selectReplace() {
	if (m_replaceEntry != nullptr) {
		m_replaceEntry->keepFocus();
	}
}

bool appl::widget::Search::isSelectReplace() {
	if (m_replaceEntry != nullptr) {
		return m_replaceEntry->getFocus();
	}
	return false;
}

