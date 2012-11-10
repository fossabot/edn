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

#include <appl/Debug.h>
#include <appl/global.h>
#include <BufferView.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <MainWindows.h>
#include <ewol/eObject/EObject.h>

#undef __class__
#define __class__	"BufferView"



static void SortElementList(etk::Vector<appl::dataBufferStruct *> &list)
{
	etk::Vector<appl::dataBufferStruct *> tmpList = list;
	list.Clear();
	for(int32_t iii=0; iii<tmpList.Size(); iii++) {
		if (NULL != tmpList[iii]) {
			int32_t findPos = 0;
			for(int32_t jjj=0; jjj<list.Size(); jjj++) {
				//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
				if (list[jjj]!=NULL) {
					if (tmpList[iii]->m_bufferName.GetNameFile() > list[jjj]->m_bufferName.GetNameFile()) {
						findPos = jjj+1;
					}
				}
			}
			//EWOL_DEBUG("position="<<findPos);
			list.Insert(findPos, tmpList[iii]);
		}
	}
}



BufferView::BufferView(void)
{
	SetCanHaveFocus(true);
	RegisterMultiCast(ednMsgBufferListChange);
	RegisterMultiCast(ednMsgBufferState);
	RegisterMultiCast(ednMsgBufferId);
	m_selectedID = -1;
	m_selectedIdRequested = -1;
}

BufferView::~BufferView(void)
{
	RemoveAllElement();
}

void BufferView::RemoveAllElement(void)
{
	for(int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL!=m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.Clear();
}

/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void BufferView::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	ewol::List::OnReceiveMessage(CallerObject, eventId, data);
	if (eventId == ednMsgBufferListChange) {
		// clean The list
		RemoveAllElement();
		// Get all the buffer name and properties:
		int32_t nbBufferOpen = BufferManager::Size();
		for (int32_t iii=0; iii<nbBufferOpen; iii++) {
			if (BufferManager::Exist(iii)) {
				bool isModify  = BufferManager::Get(iii)->IsModify();
				etk::FSNode name = BufferManager::Get(iii)->GetFileName();
				appl::dataBufferStruct* tmpElement = new appl::dataBufferStruct(name, iii, isModify);
				if (NULL != tmpElement) {
					m_list.PushBack(tmpElement);
				} else {
					APPL_ERROR("Allocation error of the tmp buffer list element");
				}
			}
		}
		if (true == globals::OrderTheBufferList() ) {
			SortElementList(m_list);
		}
		MarkToRedraw();
	}else if (eventId == ednMsgBufferId) {
		m_selectedIdRequested = BufferManager::GetSelected();
		MarkToRedraw();
	}else if (eventId == ednMsgBufferState) {
		// Update list of modify section ...
		for (int32_t iii=0; iii<m_list.Size(); iii++) {
			if (NULL!=m_list[iii]) {
				m_list[iii]->m_isModify = BufferManager::Get(m_list[iii]->m_bufferID)->IsModify();
			}
		}
		MarkToRedraw();
	}
}


draw::Color BufferView::GetBasicBG(void)
{
	return ColorizeManager::Get(COLOR_LIST_BG_1);
}

uint32_t BufferView::GetNuberOfColomn(void)
{
	return 1;
}

bool BufferView::GetTitle(int32_t colomn, etk::UString &myTitle, draw::Color &fg, draw::Color &bg)
{
	myTitle = "Buffers : ";
	return true;
}

uint32_t BufferView::GetNuberOfRaw(void)
{
	return m_list.Size();
}

bool BufferView::GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, draw::Color &fg, draw::Color &bg)
{
	bool isModify;
	basicColor_te selectFG = COLOR_LIST_TEXT_NORMAL;
	basicColor_te selectBG = COLOR_LIST_BG_1;
	// when requested a new display selection ==> reset the previous one ...
	if (m_selectedIdRequested != -1) {
		m_selectedID = -1;
	}
	if(    raw>=0
	    && raw<m_list.Size()
	    && NULL != m_list[raw]) {
		myTextToWrite = m_list[raw]->m_bufferName.GetNameFile();
		
		if (true == m_list[raw]->m_isModify) {
			selectFG = COLOR_LIST_TEXT_MODIFY;
		} else {
			selectFG = COLOR_LIST_TEXT_NORMAL;
		}
		if (raw%2==0) {
			selectBG = COLOR_LIST_BG_1;
		} else {
			selectBG = COLOR_LIST_BG_2;
		}
		// the buffer change of selection ...
		if (m_selectedIdRequested == m_list[raw]->m_bufferID) {
			m_selectedID = raw;
			// stop searching
			m_selectedIdRequested = -1;
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

bool BufferView::OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent,  int32_t colomn, int32_t raw, float x, float y)
{
	if (1 == IdInput && typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
		APPL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		if(    raw>=0
		    && raw<m_list.Size()
		    && NULL != m_list[raw]) {
			m_selectedID = raw;
			SendMultiCast(ednMsgBufferId, m_list[raw]->m_bufferID);
		}
	}
	MarkToRedraw();
	return false;
}


