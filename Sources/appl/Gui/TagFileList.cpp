/**
 *******************************************************************************
 * @file TagFileList.cpp
 * @brief Editeur De N'ours : Tags list display to jump (sources)
 * @author Edouard DUPIN
 * @date 16/10/2012
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
	AddEventId(applEventCtagsListSelect);
	AddEventId(applEventCtagsListValidate);
	SetMouseLimit(1);
}


appl::TagFileList::~TagFileList(void)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
}

draw::Color appl::TagFileList::GetBasicBG(void) {
	draw::Color bg(0x00000010);
	return bg;
}

uint32_t appl::TagFileList::GetNuberOfColomn(void) {
	return 2;
}

bool appl::TagFileList::GetTitle(int32_t colomn, etk::UString &myTitle, draw::Color &fg, draw::Color &bg) {
	myTitle = "title";
	return true;
}

uint32_t appl::TagFileList::GetNuberOfRaw(void) {
	return m_list.Size();
}

bool appl::TagFileList::GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, draw::Color &fg, draw::Color &bg) {
	if (raw >= 0 && raw < m_list.Size() && NULL != m_list[raw]) {
		if (0==colomn) {
			// note : tmp while the list support multiple colomn
			myTextToWrite = m_list[raw]->filename;
			myTextToWrite = m_list[raw]->filename + ":" + etk::UString(m_list[raw]->fileLine);
		} else {
			myTextToWrite = etk::UString(m_list[raw]->fileLine);
		}
	} else {
		myTextToWrite = "ERROR";
	}
	fg = draw::color::black;
	if (raw % 2) {
		bg = 0xFFFFFF00;
	} else {
		bg = 0xBFBFBFFF;
	}
	if (m_selectedLine == raw) {
		bg = 0x8F8FFFFF;
	}
	return true;
};


bool appl::TagFileList::OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, float x, float y)
{
	if (typeEvent == ewol::EVENT_INPUT_TYPE_SINGLE) {
		EWOL_INFO("Event on List : IdInput=" << IdInput << " colomn=" << colomn << " raw=" << raw );
		if (1 == IdInput) {
			int32_t previousRaw = m_selectedLine;
			if (raw > m_list.Size() ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = raw;
			}
			const char * event = applEventCtagsListValidate;
			if (previousRaw != m_selectedLine) {
				event = applEventCtagsListSelect;
			}
			if(    m_selectedLine >=0
			    && m_selectedLine < m_list.Size()
			    && NULL != m_list[m_selectedLine] ) {
				GenerateEventId(event, etk::UString(m_list[raw]->fileLine)+":"+m_list[m_selectedLine]->filename);
			} else {
				GenerateEventId(applEventCtagsListUnSelect);
			}
			// need to regenerate the display of the list : 
			MarkToRedraw();
			return true;
		}
	}
	return false;
}


/**
 * @brief Add a Ctags item on the curent list
 * @param[in] file Compleate file name
 * @param[in] jump line id
 * @return ---
 */
void appl::TagFileList::Add(etk::UString& file, int32_t line)
{
	appl::TagListElement *tmpFile = new appl::TagListElement(file, line);
	if (NULL != tmpFile) {
		m_list.PushBack(tmpFile);
	}
	MarkToRedraw();
}


