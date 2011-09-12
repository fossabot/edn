/**
 *******************************************************************************
 * @file BufferEmpty.cpp
 * @brief Editeur De N'ours : Text Buffer (edit For No buffer Display)
 * @author Edouard DUPIN
 * @date 19/01/2011
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
#include "BufferEmpty.h"
#include "ColorizeManager.h"
#include "MainWindows.h"

#undef __class__
#define __class__	"BufferEmpty"

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
BufferEmpty::BufferEmpty()
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
BufferEmpty::~BufferEmpty(void)
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
void BufferEmpty::DrawLine(DrawerManager &drawer, bufferAnchor_ts &anchor, position_ts &displayStart, position_ts &displaySize)
{
	EDN_DEBUG("Request draw : " << anchor.m_lineNumber);
	ColorizeManager * myColorManager = ColorizeManager::getInstance();
	// Get color : 
	Colorize	*myColor = NULL;
	if (anchor.m_lineNumber == 0) {
		// Clean Buffer
		drawer.Clean(myColorManager->Get(COLOR_CODE_BASIC_BG) );
		myColor = myColorManager->Get("normal");
		drawer.Text(myColor, 20,20, "edn - Editeur De N'ours, l'Editeur Desoxyribo-Nucleique");
	}
	if (anchor.m_lineNumber == 1) {
		myColor = myColorManager->Get("commentDoxygen");
		drawer.Text(myColor, 20,25 + Display::GetFontHeight(), "No Buffer Availlable to display");
	}
	return;
}




bool BufferEmpty::AnchorGet(int32_t anchorID, bufferAnchor_ts & anchor, position_ts &size, int32_t sizePixelX, int32_t sizePixelY)
{
	int32_t localID = AnchorRealId(anchorID);
	if (localID >=0) {
		EDN_DEBUG("Request anchor");
		size.x = sizePixelX / Display::GetFontWidth();
		size.y = sizePixelY / Display::GetFontHeight();
		anchor.m_nbIterationMax = 2;
		anchor.m_lineNumber = m_AnchorList[localID].m_lineId;
		anchor.m_posStart = -1;
		anchor.m_posStop = -1;
		anchor.m_selectionPosStart = -1;
		anchor.m_selectionPosStop = -1;
		return true;
	} else {
		return false;
	}
}


bool BufferEmpty::AnchorNext(bufferAnchor_ts & anchor)
{
	anchor.m_lineNumber++;
	EDN_DEBUG("Anchor Next : " << anchor.m_lineNumber);
	anchor.m_nbIterationMax--;
	if (anchor.m_nbIterationMax<=0) {
		return false;
	}
	return true;
}

