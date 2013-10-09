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

extern const char * const applEventCtagsListSelect     = "appl-event-ctags-list-select";
extern const char * const applEventCtagsListUnSelect   = "appl-event-ctags-list-un-select";
extern const char * const applEventCtagsListValidate   = "appl-event-ctags-list-validate";

appl::TagFileList::TagFileList(void) {
	m_selectedLine = -1;
	addEventId(applEventCtagsListSelect);
	addEventId(applEventCtagsListValidate);
	setMouseLimit(1);
}


appl::TagFileList::~TagFileList(void) {
	for (int32_t iii=0; iii<m_list.size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
}

etk::Color<> appl::TagFileList::getBasicBG(void) {
	return 0x00000010;
}

uint32_t appl::TagFileList::getNuberOfColomn(void) {
	return 2;
}

bool appl::TagFileList::getTitle(int32_t _colomn, etk::UString& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg) {
	_myTitle = "title";
	return true;
}

uint32_t appl::TagFileList::getNuberOfRaw(void) {
	return m_list.size();
}

bool appl::TagFileList::getElement(int32_t _colomn, int32_t _raw, etk::UString& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg) {
	if (_raw >= 0 && _raw < m_list.size() && NULL != m_list[_raw]) {
		if (0 == _colomn) {
			_myTextToWrite = etk::UString(m_list[_raw]->fileLine);
		} else {
			_myTextToWrite = m_list[_raw]->filename;
		}
	} else {
		_myTextToWrite = "ERROR";
	}
	_fg = etk::color::black;
	if (_raw % 2) {
		if (_colomn%2 == 0) {
			_bg = 0xFFFFFF00;
		} else {
			_bg = 0xFFFFFF10;
		}
	} else {
		if (_colomn%2 == 0) {
			_bg = 0xBFBFBFFF;
		} else {
			_bg = 0xCFCFCFFF;
		}
	}
	if (m_selectedLine == _raw) {
		if (_colomn%2 == 0) {
			_bg = 0x8F8FFFFF;
		} else {
			_bg = 0x7F7FFFFF;
		}
	}
	return true;
};


bool appl::TagFileList::onItemEvent(int32_t _IdInput, ewol::keyEvent::status_te _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y) {
	if (_typeEvent == ewol::keyEvent::statusSingle) {
		EWOL_INFO("Event on List : IdInput=" << _IdInput << " colomn=" << _colomn << " raw=" << _raw );
		if (_IdInput == 1) {
			int32_t previousRaw = m_selectedLine;
			if (_raw > m_list.size() ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = _raw;
			}
			const char * event = applEventCtagsListValidate;
			if (previousRaw != m_selectedLine) {
				event = applEventCtagsListSelect;
			}
			if(    m_selectedLine  >= 0
			    && m_selectedLine < m_list.size()
			    && NULL != m_list[m_selectedLine] ) {
				generateEventId(event, etk::UString(m_list[_raw]->fileLine)+":"+m_list[m_selectedLine]->filename);
			} else {
				generateEventId(applEventCtagsListUnSelect);
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
void appl::TagFileList::add(etk::UString& _file, int32_t _line) {
	appl::TagListElement *tmpFile = new appl::TagListElement(_file, _line);
	if (NULL != tmpFile) {
		m_list.pushBack(tmpFile);
	}
	markToRedraw();
}


