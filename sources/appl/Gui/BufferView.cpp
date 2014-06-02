/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/Gui/BufferView.h>
#include <appl/BufferManager.h>
//#include <ColorizeManager.h>
#include <appl/Gui/MainWindows.h>
#include <ewol/object/Object.h>

#undef __class__
#define __class__ "BufferView"

static void SortElementList(std::vector<appl::dataBufferStruct*>& _list) {
	std::vector<appl::dataBufferStruct *> tmpList = _list;
	_list.clear();
	for(size_t iii=0; iii<tmpList.size(); iii++) {
		if (NULL == tmpList[iii]) {
			continue;
		}
		size_t findPos = 0;
		for(size_t jjj=0; jjj<_list.size(); jjj++) {
			//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
			if (_list[jjj] == NULL) {
				continue;
			}
			if (tmpList[iii]->m_bufferName.getNameFile() > _list[jjj]->m_bufferName.getNameFile()) {
				findPos = jjj+1;
			}
		}
		//EWOL_DEBUG("position="<<findPos);
		_list.insert(_list.begin()+findPos, tmpList[iii]);
	}
}

BufferView::BufferView() :
  m_openOrderMode(false) {
	addObjectType("appl::BufferView");
	setCanHaveFocus(true);
	registerMultiCast(ednMsgBufferListChange);
	registerMultiCast(ednMsgBufferState);
	registerMultiCast(ednMsgBufferId);
	registerMultiCast(appl::MsgSelectNewFile);
	registerMultiCast(appl::MsgSelectChange);
	registerMultiCast(appl::MsgNameChange);
	m_selectedID = -1;
	m_selectedIdRequested = -1;
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	// load color properties
	m_paintingProperties = appl::GlyphPainting::keep("THEME:COLOR:bufferList.json");
	// get all id properties ...
	m_colorBackground1 = m_paintingProperties->request("backgroung1");
	m_colorBackground2 = m_paintingProperties->request("backgroung2");
	m_colorBackgroundSelect = m_paintingProperties->request("backgroungSelected");
	m_colorTextNormal = m_paintingProperties->request("textNormal");
	m_colorTextModify = m_paintingProperties->request("textModify");
}

BufferView::~BufferView() {
	removeAllElement();
}

void BufferView::removeAllElement() {
	for(auto &it : m_list) {
		delete(it);
		it = NULL;
	}
	m_list.clear();
}

void BufferView::insertAlphabetic(appl::dataBufferStruct* _dataStruct, bool _selectNewPosition) {
	if (_dataStruct == NULL) {
		return;
	}
	// alphabetical order:
	for (size_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii] == NULL) {
			continue;
		}
		if (std::tolower(m_list[iii]->m_bufferName.getNameFile()) > std::tolower(_dataStruct->m_bufferName.getNameFile())) {
			m_list.insert(m_list.begin() + iii, _dataStruct);
			_dataStruct = NULL;
			if (_selectNewPosition == true) {
				m_selectedID = iii;
			}
			break;
		}
	}
	if (_dataStruct != NULL) {
		m_list.push_back(_dataStruct);
		if (_selectNewPosition == true) {
			m_selectedID = m_list.size()-1;
		}
	}
}


void BufferView::onReceiveMessage(const ewol::object::Message& _msg) {
	ewol::widget::List::onReceiveMessage(_msg);
	if (_msg.getMessage() == appl::MsgSelectNewFile) {
		ewol::object::Shared<appl::Buffer> buffer = m_bufferManager->get(_msg.getData());
		if (buffer == NULL) {
			APPL_ERROR("event on element nor exist : " << _msg.getData());
			return;
		}
		buffer->registerOnEvent(this, appl::Buffer::eventIsSave);
		buffer->registerOnEvent(this, appl::Buffer::eventIsModify);
		buffer->registerOnEvent(this, appl::Buffer::eventChangeName);
		appl::dataBufferStruct* tmp = new appl::dataBufferStruct(_msg.getData(), buffer);
		if (tmp == NULL) {
			APPL_ERROR("Allocation error of the tmp buffer list element");
			return;
		}
		if (m_openOrderMode == true) {
			m_list.push_back(tmp);
		} else {
			insertAlphabetic(tmp);
		}
		markToRedraw();
		return;
	}
	if (_msg.getMessage() == appl::Buffer::eventChangeName) {
		for (size_t iii = 0; iii < m_list.size(); ++iii) {
			if (m_list[iii] == NULL) {
				continue;
			}
			if (m_list[iii]->m_bufferName != m_list[iii]->m_buffer->getFileName()) {
				m_list[iii]->m_bufferName = m_list[iii]->m_buffer->getFileName();
				if (m_openOrderMode == false) {
					// re-order the fine in the correct position
					appl::dataBufferStruct* tmp = m_list[iii];
					m_list[iii] = NULL;
					m_list.erase(m_list.begin() + iii);
					insertAlphabetic(tmp, ((int64_t)iii == m_selectedID));
					break;
				}
			}
		}
		markToRedraw();
		return;
	}
	if (_msg.getMessage() == appl::Buffer::eventIsSave) {
		markToRedraw();
		return;
	}
	if (_msg.getMessage() == appl::Buffer::eventIsModify) {
		markToRedraw();
		return;
	}
	APPL_DEBUG("message : " << _msg);
	if (_msg.getMessage() == appl::MsgSelectChange) {
		m_selectedID = -1;
		ewol::object::Shared<appl::Buffer> tmpBuffer;
		if (m_bufferManager != NULL) {
			tmpBuffer = m_bufferManager->getBufferSelected();
		}
		if (tmpBuffer != NULL) {
			for (size_t iii=0; iii<m_list.size(); iii++) {
				if (m_list[iii] == NULL) {
					continue;
				}
				if (m_list[iii]->m_buffer != tmpBuffer) {
					continue;
				}
				m_selectedID = iii;
				break;
			}
		}
		markToRedraw();
		return;
	}
	if (_msg.getMessage() == ednMsgBufferListChange) {
		// clean The list
		removeAllElement();
		// get all the buffer name and properties:
		size_t nbBufferOpen = 0; // BufferManager::size();
		for (size_t iii=0; iii<nbBufferOpen; iii++) {
			/*
			if (BufferManager::exist(iii)) {
				BufferText* tmpBuffer = BufferManager::get(iii);
				if (NULL != tmpBuffer) {
					bool isModify  = tmpBuffer->isModify();
					etk::FSNode name = tmpBuffer->getFileName();
					appl::dataBufferStruct* tmpElement = new appl::dataBufferStruct(name, iii, isModify);
					if (NULL != tmpElement) {
						m_list.push_back(tmpElement);
					} else {
						APPL_ERROR("Allocation error of the tmp buffer list element");
					}
				}
			}
			*/
		}
		if (true == globals::OrderTheBufferList() ) {
			SortElementList(m_list);
		}
		markToRedraw();
	}else if (_msg.getMessage() == ednMsgBufferId) {
		m_selectedIdRequested = 0; //BufferManager::getSelected();
		markToRedraw();
	}else if (_msg.getMessage() == ednMsgBufferState) {
		// update list of modify section ...
		for (auto &it : m_list) {
			if (it != nullptr) {
				//it->m_isModify = BufferManager::get(it->m_bufferID)->isModify();
			}
		}
		markToRedraw();
	}
}

void BufferView::onObjectRemove(const ewol::object::Shared<ewol::Object>& _object) {
	ewol::widget::List::onObjectRemove(_object);
	auto it(m_list.begin());
	while (it != m_list.end()) {
		if (    *it != nullptr
		     && (*it)->m_buffer == _object) {
			m_list.erase(it);
			markToRedraw();
			it = m_list.begin();
		} else {
			++it;
		}
	}
	if (m_bufferManager == _object) {
		m_bufferManager.reset();
	}
}


etk::Color<> BufferView::getBasicBG() {
	return (*m_paintingProperties)[m_colorBackground1].getForeground();
}

uint32_t BufferView::getNuberOfColomn() {
	return 1;
}

bool BufferView::getTitle(int32_t _colomn, std::string &_myTitle, etk::Color<> &_fg, etk::Color<> &_bg) {
	_myTitle = "Buffers : ";
	return true;
}

uint32_t BufferView::getNuberOfRaw() {
	return m_list.size();
}

bool BufferView::getElement(int32_t _colomn, int32_t _raw, std::string& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg) {
	if(    _raw >= 0
	    && _raw<(int64_t)m_list.size()
	    && m_list[_raw] != nullptr) {
		_myTextToWrite = m_list[_raw]->m_bufferName.getNameFile();
		
		if (    m_list[_raw]->m_buffer != NULL
		     && m_list[_raw]->m_buffer->isModify() == false) {
			_fg = (*m_paintingProperties)[m_colorTextNormal].getForeground();
		} else {
			_fg = (*m_paintingProperties)[m_colorTextModify].getForeground();
		}
		if (_raw%2 == 0) {
			_bg = (*m_paintingProperties)[m_colorBackground1].getForeground();
		} else {
			_bg = (*m_paintingProperties)[m_colorBackground2].getForeground();
		}
		// the buffer change of selection ...
		if (m_selectedID == _raw) {
			_bg = (*m_paintingProperties)[m_colorBackgroundSelect].getForeground();
		}
	} else {
		_myTextToWrite = "ERROR";
	}
	return true;
}

bool BufferView::onItemEvent(int32_t _IdInput, enum ewol::key::status _typeEvent,  int32_t _colomn, int32_t _raw, float _x, float _y)
{
	if (1 == _IdInput && _typeEvent == ewol::key::statusSingle) {
		APPL_INFO("Event on List : IdInput=" << _IdInput << " colomn=" << _colomn << " raw=" << _raw );
		if(    _raw >= 0
		    && _raw<(int64_t)m_list.size()
		    && NULL != m_list[_raw]) {
			if (m_list[_raw]->m_buffer != NULL) {
				sendMultiCast(appl::MsgSelectNewFile, m_list[_raw]->m_buffer->getFileName());
				m_selectedID = _raw;
				markToRedraw();
				return true;
			}
		}
	}
	return false;
}


