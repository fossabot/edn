/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <etk/tool.h>
#include <appl/Gui/TagFileList.h>

#undef __class__
#define __class__ "TagFileList"

appl::TagFileList::TagFileList() :
  signalSelect(*this, "select"),
  signalValidate(*this, "validate"),
  signalUnSelect(*this, "unselect") {
	addObjectType("appl::TagFileList");
	m_selectedLine = -1;
	setMouseLimit(1);
	// Load color properties: (use file list to be generic ...)
	m_colorProperty = ewol::resource::ColorFile::create("THEME:COLOR:ListFileSystem.json");
	if (m_colorProperty != nullptr) {
		m_colorIdText = m_colorProperty->request("text");
		m_colorIdBackground1 = m_colorProperty->request("background1");
		m_colorIdBackground2 = m_colorProperty->request("background2");
		m_colorIdBackgroundSelected = m_colorProperty->request("selected");
	}
}
void appl::TagFileList::init() {
	ewol::widget::List::init();
}

appl::TagFileList::~TagFileList() {
	for (auto &it : m_list) {
		delete(it);
		it = nullptr;
	}
}

etk::Color<> appl::TagFileList::getBasicBG() {
	return 0x00000010;
}

uint32_t appl::TagFileList::getNuberOfColomn() {
	return 2;
}

bool appl::TagFileList::getTitle(int32_t _colomn, std::string& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg) {
	_myTitle = "title";
	return true;
}

uint32_t appl::TagFileList::getNuberOfRaw() {
	return m_list.size();
}

bool appl::TagFileList::getElement(int32_t _colomn, int32_t _raw, std::string& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg) {
	if (_raw >= 0 && (size_t)_raw < m_list.size() && nullptr != m_list[_raw]) {
		if (0 == _colomn) {
			_myTextToWrite = etk::to_string(m_list[_raw]->fileLine);
		} else {
			_myTextToWrite = m_list[_raw]->filename;
		}
	} else {
		_myTextToWrite = "ERROR";
	}
	_fg = m_colorProperty->get(m_colorIdText);
	if (_raw % 2) {
		_bg = m_colorProperty->get(m_colorIdBackground1);
	} else {
		_bg = m_colorProperty->get(m_colorIdBackground2);
	}
	if (m_selectedLine == _raw) {
		_bg = m_colorProperty->get(m_colorIdBackgroundSelected);
	}
	return true;
};


bool appl::TagFileList::onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y) {
	if (_typeEvent == gale::key::status_single) {
		EWOL_INFO("Event on List : IdInput=" << _IdInput << " colomn=" << _colomn << " raw=" << _raw );
		if (_IdInput == 1) {
			int32_t previousRaw = m_selectedLine;
			if (_raw > (int64_t)m_list.size() ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = _raw;
			}
			if(    m_selectedLine  >= 0
			    && m_selectedLine < (int64_t)m_list.size()
			    && nullptr != m_list[m_selectedLine] ) {
				if (previousRaw != m_selectedLine) {
					signalSelect.emit(etk::to_string(m_list[_raw]->fileLine)+":"+m_list[m_selectedLine]->filename);
				} else {
					signalValidate.emit(etk::to_string(m_list[_raw]->fileLine)+":"+m_list[m_selectedLine]->filename);
				}
			} else {
				signalUnSelect.emit();
			}
			// need to regenerate the display of the list : 
			markToRedraw();
			return true;
		}
	}
	return false;
}


/**
 * @brief add a Ctags item on the curent list
 * @param[in] file Compleate file name
 * @param[in] jump line id
 */
void appl::TagFileList::add(std::string& _file, int32_t _line) {
	appl::TagListElement *tmpFile = new appl::TagListElement(_file, _line);
	if (nullptr != tmpFile) {
		m_list.push_back(tmpFile);
	}
	markToRedraw();
}


