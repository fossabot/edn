/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <BufferView.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <MainWindows.h>
#include <ewol/renderer/EObject.h>

#undef __class__
#define __class__	"BufferView"



static void SortElementList(etk::Vector<appl::dataBufferStruct *> &list)
{
	etk::Vector<appl::dataBufferStruct *> tmpList = list;
	list.clear();
	for(int32_t iii=0; iii<tmpList.size(); iii++) {
		if (NULL != tmpList[iii]) {
			int32_t findPos = 0;
			for(int32_t jjj=0; jjj<list.size(); jjj++) {
				//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
				if (list[jjj]!=NULL) {
					if (tmpList[iii]->m_bufferName.getNameFile() > list[jjj]->m_bufferName.GetNameFile()) {
						findPos = jjj+1;
					}
				}
			}
			//EWOL_DEBUG("position="<<findPos);
			list.insert(findPos, tmpList[iii]);
		}
	}
}



BufferView::BufferView(void)
{
	setCanHaveFocus(true);
	RegisterMultiCast(ednMsgBufferListChange);
	RegisterMultiCast(ednMsgBufferState);
	RegisterMultiCast(ednMsgBufferId);
	m_selectedID = -1;
	m_selectedIdRequested = -1;
}

BufferView::~BufferView(void)
{
	removeAllElement();
}

void BufferView::removeAllElement(void)
{
	for(int32_t iii=0; iii<m_list.size(); iii++) {
		if (NULL!=m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.clear();
}

void BufferView::onReceiveMessage(const ewol::EMessage& _msg)
{
	widget::List::onReceiveMessage(_msg);
	if (_msg.getMessage() == ednMsgBufferListChange) {
		// clean The list
		removeAllElement();
		// get all the buffer name and properties:
		int32_t nbBufferOpen = BufferManager::size();
		for (int32_t iii=0; iii<nbBufferOpen; iii++) {
			if (BufferManager::Exist(iii)) {
				BufferText* tmpBuffer = BufferManager::get(iii);
				if (NULL != tmpBuffer) {
					bool isModify  = tmpBuffer->isModify();
					etk::FSNode name = tmpBuffer->getFileName();
					appl::dataBufferStruct* tmpElement = new appl::dataBufferStruct(name, iii, isModify);
					if (NULL != tmpElement) {
						m_list.pushBack(tmpElement);
					} else {
						APPL_ERROR("Allocation error of the tmp buffer list element");
					}
				}
			}
		}
		if (true == globals::OrderTheBufferList() ) {
			SortElementList(m_list);
		}
		markToRedraw();
	}else if (_msg.getMessage() == ednMsgBufferId) {
		m_selectedIdRequested = BufferManager::getSelected();
		markToRedraw();
	}else if (_msg.getMessage() == ednMsgBufferState) {
		// update list of modify section ...
		for (int32_t iii=0; iii<m_list.size(); iii++) {
			if (NULL!=m_list[iii]) {
				m_list[iii]->m_isModify = BufferManager::get(m_list[iii]->m_bufferID)->isModify();
			}
		}
		markToRedraw();
	}
}


etk::Color<> BufferView::getBasicBG(void)
{
	return ColorizeManager::get(COLOR_LIST_BG_1);
}

uint32_t BufferView::getNuberOfColomn(void)
{
	return 1;
}

bool BufferView::getTitle(int32_t colomn, etk::UString &myTitle, etk::Color<> &fg, etk::Color<> &bg)
{
	myTitle = "Buffers : ";
	return true;
}

uint32_t BufferView::getNuberOfRaw(void)
{
	return m_list.size();
}

bool BufferView::getElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, etk::Color<> &fg, etk::Color<> &bg)
{
	bool isModify;
	basicColor_te selectFG = COLOR_LIST_TEXT_NORMAL;
	basicColor_te selectBG = COLOR_LIST_BG_1;
	// when requested a new display selection  == > reset the previous one ...
	if (m_selectedIdRequested != -1) {
		m_selectedID = -1;
	}
	if(    raw >= 0
	    && raw<m_list.size()
	    && NULL != m_list[raw]) {
		myTextToWrite = m_list[raw]->m_bufferName.getNameFile();
		
		if (true == m_list[raw]->m_isModify) {
			selectFG = COLOR_LIST_TEXT_MODIFY;
		} else {
			selectFG = COLOR_LIST_TEXT_NORMAL;
		}
		if (raw%2 == 0) {
			selectBG = COLOR_LIST_BG_1;
		} else {
			selectBG = COLOR_LIST_BG_2;
		}
		// the buffer change of selection ...
		if (m_selectedIdRequested == m_list[raw]->m_bufferID) {
			m_selectedID = raw;
			// stop searching
			m_selectedIdRequested = -1;
			// set the raw visible : 
			setRawVisible(m_selectedID);
		}
		if (m_selectedID == raw) {
			selectBG = COLOR_LIST_BG_SELECTED;
		}
	} else {
		myTextToWrite = "ERROR";
	}
	fg = ColorizeManager::get(selectFG);
	bg = ColorizeManager::get(selectBG);
	return true;
}

bool BufferView::onItemEvent(int32_t IdInput, ewol::keyEvent::status_te typeEvent,  int32_t colomn, int32_t raw, float x, float y)
{
	if (1 == IdInput && typeEvent == ewol::keyEvent::statusSingle) {
		APPL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		if(    raw >= 0
		    && raw<m_list.size()
		    && NULL != m_list[raw]) {
			m_selectedIdRequested = m_list[raw]->m_bufferID;
			SendMultiCast(ednMsgBufferId, m_list[raw]->m_bufferID);
		}
	}
	markToRedraw();
	return false;
}


