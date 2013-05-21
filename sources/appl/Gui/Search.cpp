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
#include <ewol/widget/Image.h>


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

Search::Search(void) :
	widget::Sizer(widget::Sizer::modeHori),
	m_searchEntry(NULL),
	m_replaceEntry(NULL)
{
	m_forward = false;
	// TODO : Change the mode of creating interface : 
	/* 
	<composer>
		<sizer mode="hori" expand="true,false" fill="true" lock="true">
			<button name="SEARCH:close">
				<image src="THEME:GUI:Remove.svg" fill="true" size="70,70mm"/>
			</button>
			<entry name="SEARCH:search-entry" expand="true" fill="true"/>
			<button name="SEARCH:search">
				<image src="THEME:GUI:Search.svg" fill="true" size="70,70mm"/>
			</button>
			<entry name="SEARCH:replace-entry" expand="true" fill="true"/>
			<button name="SEARCH:replace">
				<image src="THEME:GUI:Replace.svg" fill="true" size="70,70mm"/>
			</button>
			<button name="SEARCH:case">
				<image src="THEME:GUI:CaseSensitive.svg" fill="true" size="70,70mm" hover="Close search bar"/>
				<image src="THEME:GUI:CaseSensitive.svg" fill="true" size="70,70mm" hover="Close search bar"/>
			</button>
			<button name="SEARCH:wrap">
				<image src="THEME:GUI:WrapAround.svg" fill="true" size="70,70mm" hover="Close search bar"/>
				<image src="THEME:GUI:WrapAround.svg" fill="true" size="70,70mm" hover="Close search bar"/>
			</button>
			<button name="SEARCH:up-down">
				<image src="THEME:GUI:Up.svg" fill="true" size="70,70mm" hover="Close search bar"/>
				<image src="THEME:GUI:Down.svg" fill="true" size="70,70mm" hover="Close search bar"/>
			</button>
		</size>
	</composer>
	*/
	widget::Button * myButtonImage = NULL;
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Remove.svg");
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidget(tmpImage);
		myButtonImage->RegisterOnEvent(this, widget::Button::eventPressed, l_eventHideBt);
		SubWidgetAdd(myButtonImage);
	}
	
	m_searchEntry = new widget::Entry();
	if (NULL == m_searchEntry) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		m_searchEntry->RegisterOnEvent(this, widget::Entry::eventModify, l_eventSearchEntry);
		m_searchEntry->RegisterOnEvent(this, widget::Entry::eventEnter,  l_eventSearchEntryEnter);
		m_searchEntry->SetExpand(bvec2(true,false));
		m_searchEntry->SetFill(bvec2(true,false));
		SubWidgetAdd(m_searchEntry);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Search.svg");
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidget(tmpImage);
		myButtonImage->RegisterOnEvent(this, widget::Button::eventPressed, l_eventSearchBt);
		SubWidgetAdd(myButtonImage);
	}
	
	m_replaceEntry = new widget::Entry();
	if (NULL == m_replaceEntry) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		m_replaceEntry->RegisterOnEvent(this, widget::Entry::eventModify, l_eventReplaceEntry);
		m_replaceEntry->RegisterOnEvent(this, widget::Entry::eventEnter, l_eventReplaceEntryEnter);
		m_replaceEntry->SetExpand(bvec2(true,false));
		m_replaceEntry->SetFill(bvec2(true,false));
		SubWidgetAdd(m_replaceEntry);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Replace.svg");
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidget(tmpImage);
		myButtonImage->RegisterOnEvent(this, widget::Button::eventPressed, l_eventReplaceBt);
		SubWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetToggleMode(true);
		
		widget::Image* tmpImage = new widget::Image("THEME:GUI:CaseSensitive.svg");
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidget(tmpImage);
		
		tmpImage = new widget::Image("THEME:GUI:CaseSensitive.svg"); // TODO : Set color on Image .... 0xFFFFFF5F
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidgetToggle(tmpImage);
		
		myButtonImage->SetValue(!SearchData::GetCase());
		myButtonImage->RegisterOnEvent(this, widget::Button::eventPressed, l_eventCaseCb);
		SubWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetToggleMode(true);
		
		widget::Image* tmpImage = new widget::Image("THEME:GUI:WrapAround.svg");
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidget(tmpImage);
		
		tmpImage = new widget::Image("THEME:GUI:WrapAround.svg"); // TODO : Set color on Image .... 0xFFFFFF5F
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidgetToggle(tmpImage);
		
		myButtonImage->SetValue(!SearchData::GetWrap());
		myButtonImage->RegisterOnEvent(this, widget::Button::eventPressed, l_eventWrapCb);
		SubWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error ==> it will missing in the display");
	} else {
		myButtonImage->SetToggleMode(true);
		
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Up.svg");
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidget(tmpImage);
		
		tmpImage = new widget::Image("THEME:GUI:Down.svg");
		tmpImage->SetImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->SetSubWidgetToggle(tmpImage);
		
		myButtonImage->SetValue(!m_forward);
		myButtonImage->RegisterOnEvent(this, widget::Button::eventPressed, l_eventForwardCb);
		SubWidgetAdd(myButtonImage);
	}
	
	RegisterMultiCast(ednMsgGuiSearch);
	// basicly hiden ...
	Hide();
}

Search::~Search(void)
{
	
}


void Search::OnReceiveMessage(const ewol::EMessage& _msg)
{
	widget::Sizer::OnReceiveMessage(_msg);
	//APPL_INFO("Search receive message : \"" << eventId << "\" data=\"" << data << "\"");
	if ( _msg.GetMessage() == l_eventSearchEntry) {
		SearchData::SetSearch(_msg.GetData());
	} else if ( _msg.GetMessage() == l_eventSearchEntryEnter) {
		SearchData::SetSearch(_msg.GetData());
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.GetMessage() == l_eventReplaceEntry) {
		SearchData::SetReplace(_msg.GetData());
	} else if ( _msg.GetMessage() == l_eventReplaceEntryEnter) {
		SearchData::SetReplace(_msg.GetData());
		SendMultiCast(ednMsgGuiReplace, "Normal");
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.GetMessage() == l_eventSearchBt) {
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.GetMessage() == l_eventReplaceBt) {
		SendMultiCast(ednMsgGuiReplace, "Normal");
		if (true==m_forward) {
			SendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			SendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.GetMessage() == l_eventCaseCb) {
		if (_msg.GetData() == "1") {
			SearchData::SetCase(false);
		} else {
			SearchData::SetCase(true);
		}
	} else if ( _msg.GetMessage() == l_eventWrapCb) {
		if (_msg.GetData() == "1") {
			SearchData::SetWrap(false);
		} else {
			SearchData::SetWrap(true);
		}
	} else if ( _msg.GetMessage() == l_eventForwardCb) {
		if (_msg.GetData() == "1") {
			m_forward = false;
		} else {
			m_forward = true;
		}
	} else if ( _msg.GetMessage() == l_eventHideBt) {
		Hide();
	} else if ( _msg.GetMessage() == ednMsgGuiSearch) {
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

void Search::OnObjectRemove(ewol::EObject * _removeObject)
{
	widget::Sizer::OnObjectRemove(_removeObject);
	if (_removeObject == m_searchEntry) {
		m_searchEntry = NULL;
	}
	if (_removeObject == m_replaceEntry) {
		m_replaceEntry = NULL;
	}
}

