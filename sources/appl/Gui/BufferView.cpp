/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/debug.hpp>
#include <appl/global.hpp>
#include <appl/Gui/BufferView.hpp>
#include <appl/BufferManager.hpp>
//#include <ColorizeManager.hpp>
#include <appl/Gui/MainWindows.hpp>
#include <ewol/object/Object.hpp>

// TODO : write it better
static void SortElementList(etk::Vector<appl::dataBufferStruct>& _list) {
	etk::Vector<appl::dataBufferStruct> tmpList = _list;
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
	propertyHide.set(true);
	propertyCanFocus.set(true);
	if (m_bufferManager != null) {
		m_bufferManager->signalNewBuffer.connect(sharedFromThis(), &BufferView::onCallbackNewBuffer);
		m_bufferManager->signalSelectFile.connect(sharedFromThis(), &BufferView::onCallbackselectNewFile);
		m_bufferManager->signalRemoveBuffer.connect(sharedFromThis(), &BufferView::onCallbackBufferRemoved);
	}
}

BufferView::~BufferView() {
	removeAllElement();
}


void BufferView::calculateMinMaxSize() {
	/*int32_t fontId = getDefaultFontId();
	int32_t minWidth = ewol::getWidth(fontId, m_label);
	int32_t minHeight = ewol::getHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	*/
	m_minSize.setValue(300, 150);
}

void BufferView::removeAllElement() {
	m_list.clear();
}

void BufferView::insertAlphabetic(const appl::dataBufferStruct& _dataStruct, bool _selectNewPosition) {
	// alphabetical order:
	for (size_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii].m_bufferName.getNameFile().toLower() > _dataStruct.m_bufferName.getNameFile().toLower()) {
			m_list.insert(m_list.begin() + iii, _dataStruct);
			if (_selectNewPosition == true) {
				m_selectedID = iii;
			}
			return;
		}
	}
	m_list.pushBack(_dataStruct);
	if (_selectNewPosition == true) {
		m_selectedID = m_list.size()-1;
	}
}

void BufferView::onCallbackNewBuffer(const etk::String& _value) {
	ememory::SharedPtr<appl::Buffer> buffer = m_bufferManager->get(_value);
	if (buffer == null) {
		APPL_ERROR("event on element nor exist : " << _value);
		return;
	}
	buffer->signalIsSave.connect(sharedFromThis(), &BufferView::onCallbackIsSave);
	buffer->signalIsModify.connect(sharedFromThis(), &BufferView::onCallbackIsModify);
	buffer->signalChangeName.connect(sharedFromThis(), &BufferView::onCallbackChangeName);
	appl::dataBufferStruct tmp(_value, buffer);
	if (m_openOrderMode == true) {
		m_list.pushBack(tmp);
	} else {
		insertAlphabetic(tmp);
	}
	if (m_list.size() <= 1) {
		propertyHide.set(true);
		propertySetOnWidgetNamed("appl-Buffer-viewer-separator", "hide", "true");
	} else {
		propertyHide.set(false);
		propertySetOnWidgetNamed("appl-Buffer-viewer-separator", "hide", "false");
	}
	markToRedraw();
}

// TODO : Review this callback with the real shared_ptr on the buffer ...
void BufferView::onCallbackselectNewFile(const etk::String& _value) {
	m_selectedID = -1;
	for (size_t iii=0; iii<m_list.size(); iii++) {
		if (m_list[iii].m_buffer == null) {
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

void BufferView::onCallbackBufferRemoved(const ememory::SharedPtr<appl::Buffer>& _buffer) {
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
	if (m_list.size() <= 1) {
		propertyHide.set(true);
	} else {
		propertyHide.set(false);
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

ivec2 BufferView::getMatrixSize() const {
	return ivec2(1,m_list.size());
}

fluorine::Variant BufferView::getData(int32_t _role, const ivec2& _pos) {
	switch (_role) {
		case ewol::widget::ListRole::Text:
			return m_list[_pos.y()].m_bufferName.getNameFile();;
		case ewol::widget::ListRole::FgColor:
			if (    m_list[_pos.y()].m_buffer != null
			     && m_list[_pos.y()].m_buffer->isModify() == false) {
				return (*m_paintingProperties)[m_colorTextNormal].getForeground();
			} else {
				return (*m_paintingProperties)[m_colorTextModify].getForeground();
			}
		case ewol::widget::ListRole::BgColor:
			if (m_selectedID == _pos.y()) {
				return (*m_paintingProperties)[m_colorBackgroundSelect].getForeground();
			}
			if (_pos.y() % 2) {
				return (*m_paintingProperties)[m_colorBackground1].getForeground();
			}
			return (*m_paintingProperties)[m_colorBackground2].getForeground();
	}
	return fluorine::Variant();
}


bool BufferView::onItemEvent(const ewol::event::Input& _event, const ivec2& _pos, const vec2& _mousePosition) {
	if (    _event.getId() == 1
	     && _event.getStatus() == gale::key::status::pressSingle) {
		APPL_INFO("Event on List: " << _event << " pos=" << _pos );
		if(    _pos.y() >= 0
		    && _pos.y() < (int64_t)m_list.size()) {
			if (m_list[_pos.y()].m_buffer != null) {
				if (m_bufferManager != null) {
					APPL_INFO("Select file :" << m_list[_pos.y()].m_buffer->getFileName() << " in list");
					m_bufferManager->open(m_list[_pos.y()].m_buffer->getFileName());
				}
				return true;
			}
		}
	}
	return false;
}


