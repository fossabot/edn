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
#include "appl/BufferManager.h"
#include "appl/Gui/MainWindows.h"
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
  m_replaceEntry(NULL) {
	m_forward = false;
	// TODO : change the mode of creating interface : 
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
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Remove.svg");
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidget(tmpImage);
		myButtonImage->registerOnEvent(this, widget::Button::eventPressed, l_eventHideBt);
		subWidgetAdd(myButtonImage);
	}
	
	m_searchEntry = new widget::Entry();
	if (NULL == m_searchEntry) {
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		m_searchEntry->registerOnEvent(this, widget::Entry::eventModify, l_eventSearchEntry);
		m_searchEntry->registerOnEvent(this, widget::Entry::eventEnter,  l_eventSearchEntryEnter);
		m_searchEntry->setExpand(bvec2(true,false));
		m_searchEntry->setFill(bvec2(true,false));
		subWidgetAdd(m_searchEntry);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Search.svg");
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidget(tmpImage);
		myButtonImage->registerOnEvent(this, widget::Button::eventPressed, l_eventSearchBt);
		subWidgetAdd(myButtonImage);
	}
	
	m_replaceEntry = new widget::Entry();
	if (NULL == m_replaceEntry) {
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		m_replaceEntry->registerOnEvent(this, widget::Entry::eventModify, l_eventReplaceEntry);
		m_replaceEntry->registerOnEvent(this, widget::Entry::eventEnter, l_eventReplaceEntryEnter);
		m_replaceEntry->setExpand(bvec2(true,false));
		m_replaceEntry->setFill(bvec2(true,false));
		subWidgetAdd(m_replaceEntry);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Replace.svg");
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidget(tmpImage);
		myButtonImage->registerOnEvent(this, widget::Button::eventPressed, l_eventReplaceBt);
		subWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		myButtonImage->setToggleMode(true);
		
		widget::Image* tmpImage = new widget::Image("THEME:GUI:CaseSensitive.svg");
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidget(tmpImage);
		
		tmpImage = new widget::Image("THEME:GUI:CaseSensitive.svg"); // TODO : set color on Image .... 0xFFFFFF5F
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidgetToggle(tmpImage);
		
		myButtonImage->setValue(!SearchData::getCase());
		myButtonImage->registerOnEvent(this, widget::Button::eventPressed, l_eventCaseCb);
		subWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		myButtonImage->setToggleMode(true);
		
		widget::Image* tmpImage = new widget::Image("THEME:GUI:WrapAround.svg");
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidget(tmpImage);
		
		tmpImage = new widget::Image("THEME:GUI:WrapAround.svg"); // TODO : set color on Image .... 0xFFFFFF5F
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidgetToggle(tmpImage);
		
		myButtonImage->setValue(!SearchData::getWrap());
		myButtonImage->registerOnEvent(this, widget::Button::eventPressed, l_eventWrapCb);
		subWidgetAdd(myButtonImage);
	}
	
	myButtonImage = new widget::Button();
	if (NULL == myButtonImage) {
		APPL_ERROR("Widget allocation error  == > it will missing in the display");
	} else {
		myButtonImage->setToggleMode(true);
		
		widget::Image* tmpImage = new widget::Image("THEME:GUI:Up.svg");
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidget(tmpImage);
		
		tmpImage = new widget::Image("THEME:GUI:Down.svg");
		tmpImage->setImageSize(ewol::Dimension(vec2(8,8), ewol::Dimension::Millimeter));
		myButtonImage->setSubWidgetToggle(tmpImage);
		
		myButtonImage->setValue(!m_forward);
		myButtonImage->registerOnEvent(this, widget::Button::eventPressed, l_eventForwardCb);
		subWidgetAdd(myButtonImage);
	}
	
	registerMultiCast(ednMsgGuiSearch);
	// basicly hiden ...
	hide();
}

Search::~Search(void) {
	
}


void Search::onReceiveMessage(const ewol::EMessage& _msg) {
	widget::Sizer::onReceiveMessage(_msg);
	//APPL_INFO("Search receive message : \"" << eventId << "\" data=\"" << data << "\"");
	if ( _msg.getMessage() == l_eventSearchEntry) {
		SearchData::setSearch(_msg.getData());
	} else if ( _msg.getMessage() == l_eventSearchEntryEnter) {
		SearchData::setSearch(_msg.getData());
		if (true == m_forward) {
			sendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			sendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.getMessage() == l_eventReplaceEntry) {
		SearchData::setReplace(_msg.getData());
	} else if ( _msg.getMessage() == l_eventReplaceEntryEnter) {
		SearchData::setReplace(_msg.getData());
		sendMultiCast(ednMsgGuiReplace, "Normal");
		if (true == m_forward) {
			sendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			sendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.getMessage() == l_eventSearchBt) {
		if (true == m_forward) {
			sendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			sendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.getMessage() == l_eventReplaceBt) {
		sendMultiCast(ednMsgGuiReplace, "Normal");
		if (true == m_forward) {
			sendMultiCast(ednMsgGuiFind, "Previous");
		} else {
			sendMultiCast(ednMsgGuiFind, "Next");
		}
	} else if ( _msg.getMessage() == l_eventCaseCb) {
		if (_msg.getData() == "true") {
			SearchData::setCase(false);
		} else {
			SearchData::setCase(true);
		}
	} else if ( _msg.getMessage() == l_eventWrapCb) {
		if (_msg.getData() == "true") {
			SearchData::setWrap(false);
		} else {
			SearchData::setWrap(true);
		}
	} else if ( _msg.getMessage() == l_eventForwardCb) {
		if (_msg.getData() == "true") {
			m_forward = false;
		} else {
			m_forward = true;
		}
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

void Search::onObjectRemove(ewol::EObject * _removeObject) {
	widget::Sizer::onObjectRemove(_removeObject);
	if (_removeObject == m_searchEntry) {
		m_searchEntry = NULL;
	}
	if (_removeObject == m_replaceEntry) {
		m_replaceEntry = NULL;
	}
}

