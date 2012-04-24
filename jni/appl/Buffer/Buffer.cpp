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
 
#include <appl/Debug.h>
#include <appl/global.h>
#include <Buffer.h>
#include <BufferManager.h>
#include <ewol/EObject.h>

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
	m_updatePositionRequested = false;
	m_fileModify = true;
	m_haveName = false;
	etk::UString mString = "Untitle - ";
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
		// TODO : Remove from here
		etk::UString data = "Modify";
		ewol::EObjectMessageMultiCast::AnonymousSend(ednMsgBufferState, data);
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
bool Buffer::NeedToUpdateDisplayPosition(void)
{
	bool tmpVal = m_updatePositionRequested;
	m_updatePositionRequested = false;
	return tmpVal;
}

coord2D_ts Buffer::GetBorderSize(void)
{
	coord2D_ts tmpVal;
	tmpVal.x = 30;
	tmpVal.y = 30;
	return tmpVal;
}


coord2D_ts Buffer::GetPosition(int32_t fontId,bool& centerRequested)
{
	centerRequested = false;
	coord2D_ts tmpVal;
	tmpVal.x = 0;
	tmpVal.y = 0;
	return tmpVal;
}
