/**
 *******************************************************************************
 * @file Buffer.c
 * @brief Editeur De N'ours : Text Buffer 
 * @author Edouard DUPIN
 * @date 08/12/2010
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
 
#include "tools_debug.h"
#include "tools_globals.h"
#include "Buffer.h"
#include "BufferManager.h"

#undef __class__
#define __class__	"Buffer"

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
Buffer::Buffer()
{
	static int32_t bufID = 0;
	m_uniqueID = bufID++;
	static int32_t fileBasicID = 0;
	m_fileModify = true;
	m_haveName = false;
	Edn::String mString = "Untitle - ";
	mString += fileBasicID++;
	SetFileName(mString);
	m_haveName = false;
	// Set basic anchor
	bufferAnchorReference_ts tmpAnchor;
	tmpAnchor.m_displaySize.x = 0;
	tmpAnchor.m_displaySize.y = 0;
	tmpAnchor.m_displayStart.x = 0;
	tmpAnchor.m_curent = true;
	tmpAnchor.m_idAnchor = -1;
	tmpAnchor.m_lineId = 0;
	tmpAnchor.m_bufferPos = 0;
	for(int32_t iii=0; iii<MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
		tmpAnchor.m_redrawLine[iii] = true;
	}
	tmpAnchor.m_BufferNumberLineOffset = 0;
	m_AnchorList.PushBack(tmpAnchor);
	
	m_lineWidth = 10;
	m_lineHeight = 10;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
Buffer::Buffer(Edn::File &newName)
{
	m_fileModify = false;
	SetFileName(newName);
	// Set basic anchor
	bufferAnchorReference_ts tmpAnchor;
	tmpAnchor.m_displaySize.x = 0;
	tmpAnchor.m_displaySize.y = 0;
	tmpAnchor.m_displayStart.x = 0;
	tmpAnchor.m_curent = true;
	tmpAnchor.m_idAnchor = -1;
	tmpAnchor.m_lineId = 0;
	tmpAnchor.m_bufferPos = 0;
	for(int32_t iii=0; iii<MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
		tmpAnchor.m_redrawLine[iii] = true;
	}
	tmpAnchor.m_BufferNumberLineOffset = 0;
	m_AnchorList .PushBack(tmpAnchor);
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
Buffer::~Buffer(void)
{

}
/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
bool Buffer::IsModify(void)
{
	return m_fileModify;
}

void Buffer::SetModify(bool status)
{
	if (status != m_fileModify) {
		m_fileModify = status;
		GeneralSendMessage(EDN_MSG__BUFFER_CHANGE_MODIFY);
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
 // TODO : ne marche plus ...
void Buffer::Save(void)
{
	// nothing to do
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::GetInfo(infoStatBuffer_ts &infoToUpdate)
{
	// nothing to do
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::SetLineDisplay(uint32_t lineNumber)
{
	// nothing to do
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::DrawLine(DrawerManager &drawer, bufferAnchor_ts &anchor)
{
	return;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 * @todo : Set the move up and DOWN...
 *
 */
void Buffer::MouseSelectFromCursorTo(int32_t width, int32_t height)
{
	// nothing to do
}



/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::MouseEvent(int32_t width, int32_t height)
{
	// nothing to do
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::MouseEventDouble(void)
{
	// nothing to do
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::MouseEventTriple(void)
{
	// nothing to do
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::ScrollDown(void)
{
	// nothing to do
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::ScrollUp(void)
{
	// nothing to do
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::cursorMove(int32_t gtkKey)
{
	// nothing to do
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void Buffer::AddChar(char * UTF8data)
{
	// nothing to do
}

void Buffer::Search(Edn::String &data, bool back, bool caseSensitive, bool wrap, bool regExp)
{
	// nothing to do
}

void Buffer::Replace(Edn::String &data)
{
	// nothing to do
}

int32_t Buffer::FindLine(Edn::String &data)
{
	// nothing to do
	return 0;
}

void Buffer::JumpAtLine(int32_t newLine)
{
	// nothing to do
}


/**
 * @brief Get the current line (to know where to jump)
 *
 * @param ---
 *
 * @return Return the current line number
 *
 */
int32_t Buffer::GetCurrentLine(void)
{
	return 0;
}


/**
 * @brief request a copy of the selection in the named clipBoard ID
 *
 * @param[in] clipboardID		Id of the buffer we want to get data [0..10] (0 copy normal / 10 middle button)
 *
 * @return ---
 *
 */
void Buffer::Copy(int8_t clipboardID)
{
	// nothing to do
}


/**
 * @brief Request a copy and a remove of the curent selection in the named clipBoard ID
 *
 * @param[in] clipboardID		Id of the buffer we want to get data [0..10] (0 copy normal / 10 middle button)
 *
 * @return ---
 *
 */
void Buffer::Cut(int8_t clipboardID)
{
	// nothing to do
}


/**
 * @brief request the past of a specific clipboard on the curent position or selection
 *
 * @param[in] clipboardID		Id of the buffer we want to get data [0..10] (0 copy normal / 10 middle button)
 *
 * @return ---
 *
 */
void Buffer::Paste(int8_t clipboardID)
{
	// nothing to do
}

void Buffer::RemoveLine(void)
{
	// nothing to do;
}

void Buffer::SelectAll(void)
{
	// nothing to do
}

void Buffer::SelectNone(void)
{
	// nothing to do
}

void Buffer::Undo(void)
{
	// nothing to do
}

void Buffer::Redo(void)
{
	// nothing to do
}


void Buffer::AnchorAdd(int32_t anchorID)
{
	int32_t localID = AnchorRealId(anchorID);
	if (localID >=0) {
		EDN_ERROR("[" << m_uniqueID << "] AnchorID="<< anchorID << " already exist !!!");
	} else {
		bufferAnchorReference_ts tmpAnchor = m_AnchorList[0];
		m_AnchorList[0].m_curent = false;
		tmpAnchor.m_idAnchor = anchorID;
		for(int32_t iii=0; iii<MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
			tmpAnchor.m_redrawLine[iii] = true;
		}
		m_AnchorList.PushBack(tmpAnchor);
		EDN_DEBUG("[" << m_uniqueID << "] AnchorID="<< anchorID << " ==> Added");
	}
}


void Buffer::AnchorRm(int32_t anchorID)
{
	if (anchorID == -1) {
		EDN_ERROR("[" << m_uniqueID << "] AnchorID="<< anchorID << " Can not remove this one !!!");
		return;
	}
	int32_t localID = AnchorRealId(anchorID);
	if (localID >=0) {
		if (m_AnchorList.Size() == 2) {
			m_AnchorList[0] = m_AnchorList[1];
			for(int32_t iii=0; iii<MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
				m_AnchorList[0].m_redrawLine[iii] = true;
			}
			m_AnchorList[0].m_BufferNumberLineOffset = 0;
		}
		m_AnchorList.Erase(localID);
		EDN_DEBUG("[" << m_uniqueID << "] AnchorID="<< anchorID << " ==> Remove");
	} else {
		EDN_ERROR("[" << m_uniqueID << "] AnchorID="<< anchorID << " does not exist !!!");
	}
}


bool Buffer::AnchorGet(int32_t anchorID, bufferAnchor_ts & anchor)
{
	EDN_ERROR("[" << m_uniqueID << "] AnchorID="<< anchorID << " Main buffer ==> can not manage Anchor (type buffer specific)");
	return false;
}


bool Buffer::AnchorNext(bufferAnchor_ts & anchor)
{
	EDN_ERROR("[" << m_uniqueID << "] AnchorID=?? Main buffer ==> can not manage Anchor (type buffer specific)");
	return false;
}


void Buffer::AnchorSetSize(int32_t anchorID, int32_t sizePixelX, int32_t sizePixelY)
{
	int32_t localID = AnchorRealId(anchorID);
	if (localID >=0) {
		m_AnchorList[localID].m_displaySize.x = sizePixelX / m_lineWidth;
		m_AnchorList[localID].m_displaySize.y = sizePixelY / m_lineHeight;
	}
}


void Buffer::AnchorSetStartOffset(int32_t anchorID, int32_t offsetX, int32_t offsetY)
{
	int32_t localID = AnchorRealId(anchorID);
	if (localID >=0) {
		m_AnchorList[localID].m_displayStart.x += offsetX;
		if (0<m_AnchorList[localID].m_displayStart.x) {
			m_AnchorList[localID].m_displayStart.x = 0;
		}
		m_AnchorList[localID].m_displayStart.y += offsetY;
		if (0<m_AnchorList[localID].m_displayStart.y) {
			m_AnchorList[localID].m_displayStart.y = 0;
		}
	}
}


int32_t Buffer::AnchorRealId(int32_t anchorID)
{
	//EDN_DEBUG("Get real ID : " << anchorID << " in the anchor list size()=" << m_AnchorList.Size());
	for(int32_t iii=0; iii < m_AnchorList.Size(); iii++) {
		//EDN_DEBUG("check if equal : " << m_AnchorList[iii].m_idAnchor << " id=" << iii);
		if (m_AnchorList[iii].m_idAnchor == anchorID) {
			return iii;
		}
	}
	return -1;
}

int32_t Buffer::AnchorCurrentId(void)
{
	for(int32_t iii=0; iii < m_AnchorList.Size(); iii++) {
		if (m_AnchorList[iii].m_curent == true) {
			return iii;
		}
	}
	return 0;
}

void Buffer::AnchorForceRedrawAll(void)
{
	int32_t localID = AnchorCurrentId();
	if (localID >=0) {
		m_AnchorList[localID].m_BufferNumberLineOffset = 0;
		for(int32_t iii=0; iii < MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
			m_AnchorList[localID].m_redrawLine[iii] = true;
		}
	}
}

void Buffer::AnchorForceRedrawLine(int32_t lineID)
{
	for(int32_t iii=0; iii < m_AnchorList.Size(); iii++) {
		if(    m_AnchorList[iii].m_displayStart.y <= lineID
		    && m_AnchorList[iii].m_displayStart.y + MAX_LINE_DISPLAYABLE_BY_BUFFER > lineID )
		{
			m_AnchorList[iii].m_redrawLine[lineID-m_AnchorList[iii].m_displayStart.y] = true;
		}
	}
}


// TODO : optimiser cette fonction qui met plusieurs fois des variables a true ....

void Buffer::AnchorForceRedrawOffsef(int32_t offset)
{
	//offset *= -1;
	//EDN_DEBUG("** => set ofset : " << offset);
	int32_t localID = AnchorCurrentId();
	if (localID >=0) {
		m_AnchorList[localID].m_BufferNumberLineOffset += offset;
		
		if (offset < 0) {
			if (-1 * offset < MAX_LINE_DISPLAYABLE_BY_BUFFER) {
				for(int32_t iii=-1*offset; iii < MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
					//EDN_DEBUG("move redraw request : " << iii << " <==  " << iii+offset << " val=" << m_AnchorList[localID].m_redrawLine[iii+offset]);
					m_AnchorList[localID].m_redrawLine[iii] = m_AnchorList[localID].m_redrawLine[iii+offset];
				}
				for(int32_t iii=0; iii < -1*offset; iii++) {
					//EDN_DEBUG("move redraw request : " << iii << " <== true");
					m_AnchorList[localID].m_redrawLine[iii] = true;
				}
			} else {
				EDN_WARNING("FORCE a total redraw... 1");
				for(int32_t iii=0; iii < MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
					//EDN_DEBUG("move redraw request : " << iii << " <== true");
					m_AnchorList[localID].m_redrawLine[iii] = true;
				}
			}
		} else {
			if (offset < MAX_LINE_DISPLAYABLE_BY_BUFFER) {
				for(int32_t iii=0; iii < MAX_LINE_DISPLAYABLE_BY_BUFFER-offset ; iii++) {
					//EDN_DEBUG("move redraw request : " << iii << " <== " << iii+offset << " val=" << m_AnchorList[localID].m_redrawLine[iii+offset]);
					m_AnchorList[localID].m_redrawLine[iii] = m_AnchorList[localID].m_redrawLine[iii+offset];
				}
				for(int32_t iii=MAX_LINE_DISPLAYABLE_BY_BUFFER-offset+1; iii < MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
					//EDN_DEBUG("move redraw request : " << iii << " <== true");
					m_AnchorList[localID].m_redrawLine[iii] = true;
				}
			} else {
				EDN_WARNING("FORCE a total redraw... 2");
				for(int32_t iii=0; iii < MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
					//EDN_DEBUG("move redraw request : " << iii << " <== true");
					m_AnchorList[localID].m_redrawLine[iii] = true;
				}
			}
		}
		for(int32_t iii=m_AnchorList[localID].m_displaySize.y; iii < MAX_LINE_DISPLAYABLE_BY_BUFFER; iii++) {
			//EDN_DEBUG("move redraw request : " << iii << " <== true");
			m_AnchorList[localID].m_redrawLine[iii] = true;
		}
	}
}
