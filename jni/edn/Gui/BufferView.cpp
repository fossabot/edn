/**
 *******************************************************************************
 * @file BufferViewer.cpp
 * @brief Editeur De N'ours : main textViewer diplayer
 * @author Edouard DUPIN
 * @date 04/12/2010
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <tools_debug.h>
#include <tools_globals.h>
#include <Display.h>
#include <BufferView.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <MainWindows.h>
#include <ewol/WidgetMessageMultiCast.h>

#undef __class__
#define __class__	"BufferView"

BufferView::BufferView(void)
{
	SetCanHaveFocus(true);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgBufferListChange);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgBufferState);
	ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgBufferId);
	m_selectedID = -1;

}

BufferView::~BufferView(void)
{

}
/*
GtkWidget * BufferView::GetMainWidget(void)
{
	return m_widget;
}
*/
bool BufferView::OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y)
{
	if (generateEventId == ednMsgBufferListChange) {
		MarkToReedraw();
	}else if (generateEventId == ednMsgBufferId) {
		MarkToReedraw();
	}else if (generateEventId == ednMsgBufferState) {
		MarkToReedraw();
	}
	return false;
}


color_ts BufferView::GetBasicBG(void)
{
	return ColorizeManager::Get(COLOR_LIST_BG_1);
}

uint32_t BufferView::GetNuberOfColomn(void)
{
	return 1;
}

bool BufferView::GetTitle(int32_t colomn, etk::UString &myTitle, color_ts &fg, color_ts &bg)
{
	myTitle = "Buffers : ";
	return true;
}

uint32_t BufferView::GetNuberOfRaw(void)
{
	return BufferManager::SizeOpen();
}

bool BufferView::GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, color_ts &fg, color_ts &bg)
{
	etk::File name;
	bool isModify;
	basicColor_te selectFG = COLOR_LIST_TEXT_NORMAL;
	basicColor_te selectBG = COLOR_LIST_BG_1;
	
	// transforme the ID in the real value ...
	int32_t realID = BufferManager::WitchBuffer(raw+1);
	if (BufferManager::Exist(realID)) {
		isModify = BufferManager::Get(realID)->IsModify();
		name = BufferManager::Get(realID)->GetFileName();
		char *tmpModify = (char*)" ";
		if (true == isModify) {
			tmpModify = (char*)"M";
		}
		myTextToWrite  = "[";
		myTextToWrite += realID;
		myTextToWrite += "](";
		myTextToWrite += tmpModify;
		myTextToWrite += ") ";
		myTextToWrite += name.GetShortFilename();
		
		if (true == isModify) {
			selectFG = COLOR_LIST_TEXT_MODIFY;
		} else {
			selectFG = COLOR_LIST_TEXT_NORMAL;
		}
		if (raw%2==0) {
			selectBG = COLOR_LIST_BG_1;
		} else {
			selectBG = COLOR_LIST_BG_2;
		}
		if (m_selectedID == raw) {
			selectBG = COLOR_LIST_BG_SELECTED;
		}
	} else {
		myTextToWrite = "ERROR";
	}
	fg = ColorizeManager::Get(selectFG);
	bg = ColorizeManager::Get(selectBG);
	return true;
}

bool BufferView::OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent,  int32_t colomn, int32_t raw, etkFloat_t x, etkFloat_t y)
{
	if (1 == IdInput && typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
		EDN_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		int32_t selectBuf = BufferManager::WitchBuffer(raw+1);
		if ( 0 <= selectBuf) {
			m_selectedID = raw;
			ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgBufferId, selectBuf);
		}
	}
	MarkToReedraw();
	return false;
}


