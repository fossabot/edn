/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include "appl/global.h"
#include "Search.h"
#include "SearchData.h"
#include "BufferManager.h"
#include "MainWindows.h"
#include "appl/globalMsg.h"

#include <ewol/widget/Button.h>


#undef __class__
#define __class__	"Search"

const char* const l_eventSearchEntry        = "appl-search-entry";
const char* const l_eventSearchEntryEnter   = "appl-search-entry-enter";
const char* const l_eventReplaceEntry       = "appl-replace-entry";
const char* const l_eventReplaceEntryEnter  = "appl-replace-entry-enter";
const char* const l_eventSearchBt      = "appl-search-button";
const char* const l_eventReplaceBt     = "appl-replace-button";
const char* const l_eventCaseCb        = "appl-case-sensitive-CheckBox";
const char* const l_eventWrapCb        = "appl-wrap-CheckBox";
const char* const l_eventForwardCb     = "appl-forward-CheckBox";
const char* const l_eventHideBt        = "appl-hide-button";

Search::Search(void) :
	widget::Sizer(widget::Sizer::modeHori),
	m_searchEntry(NULL),
	m_replaceEntry(NULL)
{
	m_forward = false;
	
	widget::Button * myButtonImage = NULL;
	myButtonImage = new widget::Button("", "");
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetImage("THEME:GUI:Remove.svg");
		myButtonImage->SetMinSize(32,32);
		myButtonImage->RegisterOnEvent(this, ewolEventButtonPressed, l_eventHideBt);
		SubWidgetAdd(myButtonImage);
	}
	
	m_searchEntry = new widget::Entry();
	if (NULL == m_searchEntry) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		m_searchEntry->RegisterOnEvent(this, ewolEventEntryModify, l_eventSearchEntry);
		m_searchEntry->RegisterOnEvent(this, ewolEventEntryEnter,  l_eventSearchEntryEnter);
		m_searchEntry->SetExpendX(true);
		m_searchEntry->SetFillX(true);
		SubWidgetAdd(m_searchEntry);
	}
	
	myButtonImage = new widget::Button("","");
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetImage("THEME:GUI:Search.svg");
		myButtonImage->SetMinSize(32,32);
		myButtonImage->RegisterOnEvent(this, ewolEventButtonPressed, l_eventSearchBt);
		SubWidgetAdd(myButtonImage);
	}
	
	m_replaceEntry = new widget::Entry();
	if (NULL == m_replaceEntry) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		m_replaceEntry->RegisterOnEvent(this, ewolEventEntryModify, l_eventReplaceEntry);
		m_replaceEntry->RegisterOnEvent(this, ewolEventEntryEnter, l_eventReplaceEntryEnter);
		m_replaceEntry->SetExpendX(true);
		m_replaceEntry->SetFillX(true);
		SubWidgetAdd(m_replaceEntry);
	}
	
	myButtonImage = new widget::Button("","");
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetImage("THEME:GUI:Replace.svg");
		myButtonImage->SetMinSize(32,32);
		myButtonImage->RegisterOnEvent(this, ewolEventButtonPressed, l_eventReplaceBt);
		SubWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button("","");
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetImage("THEME:GUI:CaseSensitive.svg");
		myButtonImage->SetImageToggle("THEME:GUI:CaseSensitive.svg", 0xFFFFFF5F);
		myButtonImage->SetMinSize(32,32);
		myButtonImage->SetToggleMode(true);
		myButtonImage->SetValue(!SearchData::GetCase());
		myButtonImage->RegisterOnEvent(this, ewolEventButtonPressed, l_eventCaseCb);
		SubWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button("","");
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetImage("THEME:GUI:WrapAround.svg");
		myButtonImage->SetImageToggle("THEME:GUI:WrapAround.svg", 0xFFFFFF5F);
		myButtonImage->SetMinSize(32,32);
		myButtonImage->SetToggleMode(true);
		myButtonImage->SetValue(!SearchData::GetWrap());
		myButtonImage->RegisterOnEvent(this, ewolEventButtonPressed, l_eventWrapCb);
		SubWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button("","");
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetImage("THEME:GUI:Up.svg");
		myButtonImage->SetImageToggle("THEME:GUI:Down.svg");
		myButtonImage->SetMinSize(32,32);
		myButtonImage->SetToggleMode(true);
		myButtonImage->SetValue(!m_forward);
		myButtonImage->RegisterOnEvent(this, ewolEventButtonPressed, l_eventForwardCb);
		SubWidgetAdd(myButtonImage);
	}
	
	RegisterMultiCast(ednMsgGuiSearch);
	// basicly hiden ...
	Hide();
}

Search::~Search(void)
{
	
}


void Search::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data)
{
	widget::Sizer::OnReceiveMessage(CallerObject, eventId, data);
	//APPL_INFO("Search receive message : \"" << eventId << "\" data=\"" << data << "\"");
	if ( eventId == l_eventSearchEntry) {
		SearchData::SetSearch(data);
	} else if ( eventId == l_eventSearchEntryEnter) {
		SearchData::SetSearch(data);
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( eventId == l_eventReplaceEntry) {
		SearchData::SetReplace(data);
	} else if ( eventId == l_eventReplaceEntryEnter) {
		SearchData::SetReplace(data);
		SendMultiCast(ednMsgGuiReplace, "Normal");
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( eventId == l_eventSearchBt) {
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( eventId == l_eventReplaceBt) {
		SendMultiCast(ednMsgGuiReplace, "Normal");
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( eventId == l_eventCaseCb) {
		if (data == "1") {
			SearchData::SetCase(false);
		} else {
			SearchData::SetCase(true);
		}
	} else if ( eventId == l_eventWrapCb) {
		if (data == "1") {
			SearchData::SetWrap(false);
		} else {
			SearchData::SetWrap(true);
		}
	} else if ( eventId == l_eventForwardCb) {
		if (data == "1") {
			m_forward = false;
		} else {
			m_forward = true;
		}
	} else if ( eventId == l_eventHideBt) {
		Hide();
	} else if ( eventId == ednMsgGuiSearch) {
		if (true == IsHide()) {
			Show();
			if (m_searchEntry!= NULL) {
				m_searchEntry->KeepFocus();
			}
		} else {
			if(    (m_searchEntry!=NULL  && true==m_searchEntry->GetFocus())
			    || (m_replaceEntry!=NULL && true==m_replaceEntry->GetFocus()) ) {
				Hide();
			} else if (m_searchEntry!= NULL) {
				m_searchEntry->KeepFocus();
			} else {
				Hide();
			}
		}
	}
}

void Search::OnObjectRemove(ewol::EObject * removeObject)
{
	widget::Sizer::OnObjectRemove(removeObject);
	if (removeObject == m_searchEntry) {
		m_searchEntry = NULL;
	}
	if (removeObject == m_replaceEntry) {
		m_replaceEntry = NULL;
	}
}

