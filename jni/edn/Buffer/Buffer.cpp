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
 
#include <tools_debug.h>
#include <tools_globals.h>
#include <Buffer.h>
#include <BufferManager.h>

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
	static int32_t fileBasicID = 0;
	m_fileModify = true;
	m_haveName = false;
	etk::String mString = "Untitle - ";
	mString += fileBasicID++;
	SetFileName(mString);
	m_haveName = false;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
Buffer::Buffer(etk::File &newName)
{
	m_fileModify = false;
	SetFileName(newName);
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
int32_t	Buffer::Display(DrawerManager &drawer)
{
	return ERR_NONE;
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
void Buffer::ForceReDraw(bool allElement)
{
	// nothing to do
}



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

void Buffer::Search(etk::String &data, bool back, bool caseSensitive, bool wrap, bool regExp)
{
	// nothing to do
}

void Buffer::Replace(etk::String &data)
{
	// nothing to do
}

int32_t Buffer::FindLine(etk::String &data)
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
	// nothing to do
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

