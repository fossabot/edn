/**
 *******************************************************************************
 * @file EdnBuf_Selection.cpp
 * @brief Editeur De N'ours : Buffer for internal Data - section selection (Sources)
 * @author Edouard DUPIN
 * @date 23/03/2011
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
#include <EdnBuf.h>


#undef __class__
#define __class__	"EdnBuf{Selection}"


/**
 * @brief 
 * 
 * @param[in,out] ---
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
bool EdnBuf::SelectHasSelection(selectionType_te select)
{
	return m_selectionList[select].selected;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::Select(selectionType_te select, int32_t start, int32_t end)
{
	//selection oldSelection = m_selectionList[select];
	m_selectionList[select].selected = start != end;
	m_selectionList[select].zeroWidth = (start == end) ? true : false;
	m_selectionList[select].rectangular = false;
	m_selectionList[select].start = edn_min(start, end);
	m_selectionList[select].end = edn_max(start, end);
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::Unselect(selectionType_te select)
{
	//selection oldSelection = m_selectionList[select];
	m_selectionList[select].selected = false;
	m_selectionList[select].zeroWidth = false;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * @param[in,out] ---
 * @param[in,out] ---
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::RectSelect(selectionType_te select, int32_t start, int32_t end, int32_t rectStart, int32_t rectEnd)
{
	m_selectionList[select].selected = rectStart < rectEnd;
	m_selectionList[select].zeroWidth = (rectStart == rectEnd) ? false : true;
	m_selectionList[select].rectangular = true;
	m_selectionList[select].start = start;
	m_selectionList[select].end = end;
	m_selectionList[select].rectStart = rectStart;
	m_selectionList[select].rectEnd = rectEnd;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
bool EdnBuf::GetSelectionPos(selectionType_te select, int32_t &start, int32_t &end, bool &isRect, int32_t &rectStart, int32_t &rectEnd)
{
	/* Always fill in the parameters (zero-width can be requested too). */
	isRect = m_selectionList[select].rectangular;
	start = m_selectionList[select].start;
	end = m_selectionList[select].end;
	if (m_selectionList[select].rectangular) {
		rectStart = m_selectionList[select].rectStart;
		rectEnd = m_selectionList[select].rectEnd;
	}
	return m_selectionList[select].selected;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::GetSelectionText(selectionType_te select, etk::VectorType<int8_t> &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	// remove output data
	text.Clear();
	
	bool isSelected = GetSelectionPos(select, start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return;
	}
	
	// Rectangular selection
	if (true == isRect) {
		//GetTextInRect(start, end, rectStart, rectEnd, text);
		// TODO : ...
	} else {
		GetRange(start, end, text);
	}
}
void EdnBuf::GetSelectionText(selectionType_te select, etk::UString &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	// remove output data
	text = "";
	
	bool isSelected = GetSelectionPos(select, start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return;
	}
	
	// Rectangular selection
	if (true == isRect) {
		//GetTextInRect(start, end, rectStart, rectEnd, text);
		// TODO : ...
	} else {
		GetRange(start, end, text);
	}
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::RemoveSelected(selectionType_te select)
{
	int32_t start, end;
	int32_t rectStart, rectEnd;
	bool isRect;
	bool isSelected = GetSelectionPos(select, start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return;
	}
	// Rectangular selection
	if (true == isRect) {
		//RemoveRect(start, end, rectStart, rectEnd);
		// TODO : ...
	} else {
		Remove(start, end);
	}
	Unselect(select);
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
int32_t EdnBuf::ReplaceSelected(selectionType_te select, etk::VectorType<int8_t> &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	bool isSelected = GetSelectionPos(select, start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return 0;
	}
	int32_t returnSize = 0;
	// Rectangular selection
	if (true == isRect) {
		//ReplaceRect(start, end, rectStart, rectEnd, text);
		// TODO : ...
	} else {
		returnSize = Replace(start, end, text);
	}
	// Clean selection
	m_selectionList[select].selected = false;
	return returnSize;
}
int32_t EdnBuf::ReplaceSelected(selectionType_te select, etk::UString &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	bool isSelected = GetSelectionPos(select, start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return 0;
	}
	int32_t returnSize = 0;
	// Rectangular selection
	if (true == isRect) {
		//ReplaceRect(start, end, rectStart, rectEnd, text);
		// TODO : ...
	} else {
		returnSize = Replace(start, end, text);
	}
	// Clean selection
	m_selectionList[select].selected = false;
	return returnSize;
}



/*
** Update all of the selections in "buf" for changes in the buffer's text
*/
/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::UpdateSelections(int32_t pos, int32_t nDeleted, int32_t nInserted)
{
	UpdateSelection(SELECTION_PRIMARY   , pos, nDeleted, nInserted);
	UpdateSelection(SELECTION_SECONDARY , pos, nDeleted, nInserted);
	UpdateSelection(SELECTION_HIGHTLIGHT, pos, nDeleted, nInserted);
}


/*
** Update an individual selection for changes in the corresponding text
*/
/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::UpdateSelection(selectionType_te select, int32_t pos, int32_t nDeleted, int32_t nInserted)
{
	if(		(		false == m_selectionList[select].selected
				&&	false == m_selectionList[select].zeroWidth)
		||	pos > m_selectionList[select].end )
	{
		return;
	}
	if (pos+nDeleted <= m_selectionList[select].start) {
		m_selectionList[select].start += nInserted - nDeleted;
		m_selectionList[select].end += nInserted - nDeleted;
	} else if(		pos <= m_selectionList[select].start
				&&	pos+nDeleted >= m_selectionList[select].end)
	{
		m_selectionList[select].start = pos;
		m_selectionList[select].end = pos;
		m_selectionList[select].selected = false;
		m_selectionList[select].zeroWidth = false;
	} else if(		pos <= m_selectionList[select].start
				&&	pos+nDeleted < m_selectionList[select].end)
	{
		m_selectionList[select].start = pos;
		m_selectionList[select].end = nInserted + m_selectionList[select].end - nDeleted;
	} else if(pos < m_selectionList[select].end) {
		m_selectionList[select].end += nInserted - nDeleted;
		if (m_selectionList[select].end <= m_selectionList[select].start) {
			m_selectionList[select].selected = false;
		}
	}
}

