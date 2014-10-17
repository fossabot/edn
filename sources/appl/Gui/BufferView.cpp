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

// TODO : write it better
static void SortElementList(std::vector<appl::dataBufferStruct>& _list) {
	std::vector<appl::dataBufferStruct> tmpList = _list;
	_list.clear();
	for(size_t iii=0; iii<tmpList.size(); iii++) {
		size_t findPos = 0;
		for(size_t jjj=0; jjj<_list.size(); jjj++) {
			//EWOL_DEBUG("compare : \""<<*tmpList[iii] << "\" and \"" << *m_listDirectory[jjj] << "\"");
			if (tmpList[iii].m_bufferName.getNameFile() > _list[jjj].m_bufferName.getNameFile()) {
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
	m_selectedID = -1;
	m_selectedIdRequested = -1;
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
	// load color properties
	m_paintingProperties = appl::GlyphPainting::create("THEME:COLOR:bufferList.json");
	// get all id properties ...
	m_colorBackground1 = m_paintingProperties->request("backgroung1");
	m_colorBackground2 = m_paintingProperties->request("backgroung2");
	m_colorBackgroundSelect = m_paintingProperties->request("backgroungSelected");
	m_colorTextNormal = m_paintingProperties->request("textNormal");
	m_colorTextModify = m_paintingProperties->request("textModify");
}

void BufferView::init() {
	ewol::widget::List::init();
	if (m_bufferManager != nullptr) {
		m_bufferManager->signalNewBuffer.bind(shared_from_this(), &BufferView::onCallbackNewBuffer);
		m_bufferManager->signalSelectFile.bind(shared_from_this(), &BufferView::onCallbackselectNewFile);
		m_bufferManager->signalRemoveBuffer.bind(shared_from_this(), &BufferView::onCallbackBufferRemoved);
	}
}

BufferView::~BufferView() {
	removeAllElement();
}

void BufferView::removeAllElement() {
	m_list.clear();
}

void BufferView::insertAlphabetic(const appl::dataBufferStruct& _dataStruct, bool _selectNewPosition) {
	// alphabetical order:
	for (size_t iii = 0; iii < m_list.size(); ++iii) {
		if (etk::tolower(m_list[iii].m_bufferName.getNameFile()) > etk::tolower(_dataStruct.m_bufferName.getNameFile())) {
			m_list.insert(m_list.begin() + iii, _dataStruct);
			if (_selectNewPosition == true) {
				m_selectedID = iii;
			}
			return;
		}
	}
	m_list.push_back(_dataStruct);
	if (_selectNewPosition == true) {
		m_selectedID = m_list.size()-1;
	}
}

void BufferView::onCallbackNewBuffer(const std::string& _value) {
	std::shared_ptr<appl::Buffer> buffer = m_bufferManager->get(_value);
	if (buffer == nullptr) {
		APPL_ERROR("event on element nor exist : " << _value);
		return;
	}
	buffer->signalIsSave.bind(shared_from_this(), &BufferView::onCallbackIsSave);
	buffer->signalIsModify.bind(shared_from_this(), &BufferView::onCallbackIsModify);
	buffer->signalChangeName.bind(shared_from_this(), &BufferView::onCallbackChangeName);
	appl::dataBufferStruct tmp(_value, buffer);
	if (m_openOrderMode == true) {
		m_list.push_back(tmp);
	} else {
		insertAlphabetic(tmp);
	}
	markToRedraw();
}

// TODO : Review this callback with the real shared_ptr on the buffer ...
void BufferView::onCallbackselectNewFile(const std::string& _value) {
	m_selectedID = -1;
	for (size_t iii=0; iii<m_list.size(); iii++) {
		if (m_list[iii].m_buffer == nullptr) {
			continue;
		}
		if (m_list[iii].m_buffer->getFileName() != _value) {
			continue;
		}
		m_selectedID = iii;
		break;
	}
	markToRedraw();
}

void BufferView::onCallbackChangeName() {
	for (size_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii].m_bufferName != m_list[iii].m_buffer->getFileName()) {
			m_list[iii].m_bufferName = m_list[iii].m_buffer->getFileName();
			if (m_openOrderMode == false) {
				// re-order the fine in the correct position
				appl::dataBufferStruct tmp = m_list[iii];
				m_list.erase(m_list.begin() + iii);
				insertAlphabetic(tmp, ((int64_t)iii == m_selectedID));
				break;
			}
		}
	}
	markToRedraw();
}

void BufferView::onCallbackBufferRemoved(const std::shared_ptr<appl::Buffer>& _buffer) {
	APPL_ERROR("request remove buffer:");
	auto it = m_list.begin();
	while (it != m_list.end()) {
		if (it->m_buffer == _buffer) {
			it = m_list.erase(it);
			m_selectedID = -1;
		} else {
			++it;
		}
	}
	markToRedraw();
}
void BufferView::onCallbackIsSave() {
	markToRedraw();
}
void BufferView::onCallbackIsModify() {
	markToRedraw();
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
	    && _raw<(int64_t)m_list.size() ) {
		_myTextToWrite = m_list[_raw].m_bufferName.getNameFile();
		
		if (    m_list[_raw].m_buffer != nullptr
		     && m_list[_raw].m_buffer->isModify() == false) {
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

bool BufferView::onItemEvent(int32_t _IdInput, enum ewol::key::status _typeEvent,  int32_t _colomn, int32_t _raw, float _x, float _y) {
	if (1 == _IdInput && _typeEvent == ewol::key::statusSingle) {
		APPL_INFO("Event on List : IdInput=" << _IdInput << " colomn=" << _colomn << " raw=" << _raw );
		if(    _raw >= 0
		    && _raw<(int64_t)m_list.size()) {
			if (m_list[_raw].m_buffer != nullptr) {
				if (m_bufferManager != nullptr) {
					APPL_INFO("Select file :" << m_list[_raw].m_buffer->getFileName() << " in list");
					m_bufferManager->open(m_list[_raw].m_buffer->getFileName());
				}
				return true;
			}
		}
	}
	return false;
}


