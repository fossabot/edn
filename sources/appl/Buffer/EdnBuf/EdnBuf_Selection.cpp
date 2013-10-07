/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
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
bool EdnBuf::SelectHasSelection(void)
{
	return m_selectionList.selected;
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
void EdnBuf::Select(int32_t start, int32_t end)
{
	//selection oldSelection = m_selectionList[select];
	m_selectionList.selected = start != end;
	m_selectionList.zeroWidth = (start == end) ? true : false;
	m_selectionList.rectangular = false;
	m_selectionList.start = etk_min(start, end);
	m_selectionList.end = etk_max(start, end);
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::Unselect(void)
{
	//selection oldSelection = m_selectionList[select];
	m_selectionList.selected = false;
	m_selectionList.zeroWidth = false;
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
void EdnBuf::RectSelect(int32_t start, int32_t end, int32_t rectStart, int32_t rectEnd)
{
	m_selectionList.selected = rectStart < rectEnd;
	m_selectionList.zeroWidth = (rectStart == rectEnd) ? false : true;
	m_selectionList.rectangular = true;
	m_selectionList.start = start;
	m_selectionList.end = end;
	m_selectionList.rectStart = rectStart;
	m_selectionList.rectEnd = rectEnd;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
bool EdnBuf::getSelectionPos(int32_t &start, int32_t &end, bool &isRect, int32_t &rectStart, int32_t &rectEnd)
{
	/* Always fill in the parameters (zero-width can be requested too). */
	isRect = m_selectionList.rectangular;
	start = m_selectionList.start;
	end = m_selectionList.end;
	if (m_selectionList.rectangular) {
		rectStart = m_selectionList.rectStart;
		rectEnd = m_selectionList.rectEnd;
	}
	return m_selectionList.selected;
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::getSelectionText(etk::Vector<int8_t> &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	// remove output data
	text.clear();
	
	bool isSelected = getSelectionPos(start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return;
	}
	
	// Rectangular selection
	if (true == isRect) {
		//GetTextInRect(start, end, rectStart, rectEnd, text);
		// TODO : ...
	} else {
		getRange(start, end, text);
	}
}
void EdnBuf::getSelectionText(etk::UString &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	// remove output data
	text = "";
	
	bool isSelected = getSelectionPos(start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return;
	}
	
	// Rectangular selection
	if (true == isRect) {
		//GetTextInRect(start, end, rectStart, rectEnd, text);
		// TODO : ...
	} else {
		getRange(start, end, text);
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
void EdnBuf::removeSelected(void)
{
	int32_t start, end;
	int32_t rectStart, rectEnd;
	bool isRect;
	bool isSelected = getSelectionPos(start, end, isRect, rectStart, rectEnd);
	
	// No data selected ...
	if (false == isSelected) {
		return;
	}
	// Rectangular selection
	if (true == isRect) {
		//RemoveRect(start, end, rectStart, rectEnd);
		// TODO : ...
	} else {
		remove(start, end);
	}
	Unselect();
}


/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
int32_t EdnBuf::ReplaceSelected(etk::Vector<int8_t> &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	bool isSelected = getSelectionPos(start, end, isRect, rectStart, rectEnd);
	
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
	m_selectionList.selected = false;
	return returnSize;
}
int32_t EdnBuf::ReplaceSelected(etk::UString &text)
{
	int32_t start, end, rectStart, rectEnd;
	bool isRect;
	bool isSelected = getSelectionPos(start, end, isRect, rectStart, rectEnd);
	
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
	m_selectionList.selected = false;
	return returnSize;
}


/*
** update an individual selection for changes in the corresponding text
*/
/**
 * @brief 
 * 
 * @param[in,out] ---
 * 
 * @return ---
 * 
 */
void EdnBuf::updateSelection(int32_t pos, int32_t nDeleted, int32_t nInserted)
{
	if(		(		false == m_selectionList.selected
				&&	false == m_selectionList.zeroWidth)
		||	pos > m_selectionList.end )
	{
		return;
	}
	if (pos+nDeleted <= m_selectionList.start) {
		m_selectionList.start += nInserted - nDeleted;
		m_selectionList.end += nInserted - nDeleted;
	} else if(		pos <= m_selectionList.start
				&&	pos+nDeleted >= m_selectionList.end)
	{
		m_selectionList.start = pos;
		m_selectionList.end = pos;
		m_selectionList.selected = false;
		m_selectionList.zeroWidth = false;
	} else if(		pos <= m_selectionList.start
				&&	pos+nDeleted < m_selectionList.end)
	{
		m_selectionList.start = pos;
		m_selectionList.end = nInserted + m_selectionList.end - nDeleted;
	} else if(pos < m_selectionList.end) {
		m_selectionList.end += nInserted - nDeleted;
		if (m_selectionList.end <= m_selectionList.start) {
			m_selectionList.selected = false;
		}
	}
}

