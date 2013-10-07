/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
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

EdnBufHistory::EdnBufHistory(int32_t pos, int32_t nInserted, etk::Vector<int8_t> &deletedText)
{
	//APPL_INFO("EdnBufHistory new + data");
	m_pos = pos;
	m_nInserted = nInserted;
	m_deletedText = deletedText;
}

void EdnBufHistory::set(int32_t pos, int32_t nInserted, etk::Vector<int8_t> &deletedText)
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

void EdnBufHistory::getData(etk::Vector<int8_t> &deletedText)
{
	deletedText = m_deletedText;
}

