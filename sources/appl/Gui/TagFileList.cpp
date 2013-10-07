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
#define __class__	"TagFileList"

extern const char * const applEventCtagsListSelect     = "appl-event-ctags-list-select";
extern const char * const applEventCtagsListUnSelect   = "appl-event-ctags-list-un-select";
extern const char * const applEventCtagsListValidate   = "appl-event-ctags-list-validate";


appl::TagFileList::TagFileList(void)
{
	m_selectedLine = -1;
	addEventId(applEventCtagsListSelect);
	addEventId(applEventCtagsListValidate);
	setMouseLimit(1);
}


appl::TagFileList::~TagFileList(void)
{
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

bool appl::TagFileList::getTitle(int32_t colomn, etk::UString &myTitle, etk::Color<> &fg, etk::Color<> &bg) {
	myTitle = "title";
	return true;
}

uint32_t appl::TagFileList::getNuberOfRaw(void) {
	return m_list.size();
}

bool appl::TagFileList::getElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, etk::Color<> &fg, etk::Color<> &bg) {
	if (raw >= 0 && raw < m_list.size() && NULL != m_list[raw]) {
		if (0 == colomn) {
			myTextToWrite = etk::UString(m_list[raw]->fileLine);
		} else {
			myTextToWrite = m_list[raw]->filename;
		}
	} else {
		myTextToWrite = "ERROR";
	}
	fg = etk::color::black;
	if (raw % 2) {
		if (colomn%2 == 0) {
			bg = 0xFFFFFF00;
		} else {
			bg = 0xFFFFFF10;
		}
	} else {
		if (colomn%2 == 0) {
			bg = 0xBFBFBFFF;
		} else {
			bg = 0xCFCFCFFF;
		}
	}
	if (m_selectedLine == raw) {
		if (colomn%2 == 0) {
			bg = 0x8F8FFFFF;
		} else {
			bg = 0x7F7FFFFF;
		}
	}
	return true;
};


bool appl::TagFileList::onItemEvent(int32_t IdInput, ewol::keyEvent::status_te typeEvent, int32_t colomn, int32_t raw, float x, float y)
{
	if (typeEvent == ewol::keyEvent::statusSingle) {
		EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		if (1 == IdInput) {
			int32_t previousRaw = m_selectedLine;
			if (raw > m_list.size() ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = raw;
			}
			const char * event = applEventCtagsListValidate;
			if (previousRaw != m_selectedLine) {
				event = applEventCtagsListSelect;
			}
			if(    m_selectedLine  >= 0
			    && m_selectedLine < m_list.size()
			    && NULL != m_list[m_selectedLine] ) {
				generateEventId(event, etk::UString(m_list[raw]->fileLine)+":"+m_list[m_selectedLine]->filename);
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
 * @return ---
 */
void appl::TagFileList::add(etk::UString& file, int32_t line)
{
	appl::TagListElement *tmpFile = new appl::TagListElement(file, line);
	if (NULL != tmpFile) {
		m_list.pushBack(tmpFile);
	}
	markToRedraw();
}


