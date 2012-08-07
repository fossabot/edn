/**
 *******************************************************************************
 * @file EdnBufHistory.cpp
 * @brief Editeur De N'ours : history of buffer modification (sources)
 * @author Edouard DUPIN
 * @date 24/03/2011
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
#include <EdnBufHistory.h>


#undef __class__
#define __class__	"EdnBufHistory"

EdnBufHistory::EdnBufHistory(void)
{
	//APPL_INFO("EdnBufHistory new");
	m_pos = 0;
	m_nInserted = 0;
}

EdnBufHistory::EdnBufHistory(int32_t pos, int32_t nInserted, std::vector<int8_t> &deletedText)
{
	//APPL_INFO("EdnBufHistory new + data");
	m_pos = pos;
	m_nInserted = nInserted;
	m_deletedText = deletedText;
}

void EdnBufHistory::Set(int32_t pos, int32_t nInserted, std::vector<int8_t> &deletedText)
{
	//APPL_INFO("EdnBufHistory new + data");
	m_pos = pos;
	m_nInserted = nInserted;
	m_deletedText = deletedText;
}

EdnBufHistory::~EdnBufHistory(void)
{
	// nothing to do ...
}

int32_t EdnBufHistory::getPos(void)
{
	return m_pos;
}

int32_t EdnBufHistory::getnbDeleted(void)
{
	return m_deletedText.size();
}

int32_t EdnBufHistory::getnbInserted(void)
{
	return m_nInserted;
}

void EdnBufHistory::getData(std::vector<int8_t> &deletedText)
{
	deletedText = m_deletedText;
}

