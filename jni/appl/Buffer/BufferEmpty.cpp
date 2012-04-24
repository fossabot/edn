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
 
#include <appl/Debug.h>
#include <appl/global.h>
#include <BufferEmpty.h>
#include <ColorizeManager.h>
#include <MainWindows.h>

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
int32_t BufferEmpty::Display(ewol::OObject2DTextColored& OOTextNormal,
                             ewol::OObject2DTextColored& OOTextBold,
                             ewol::OObject2DTextColored& OOTextItalic,
                             ewol::OObject2DTextColored& OOTextBoldItalic,
                             ewol::OObject2DColored& OOColored, int32_t offsetX, int32_t offsetY, int32_t sizeX, int32_t sizeY)
{
	// Get color : 
	Colorize	*myColor = NULL;
	
	int32_t fontId = OOTextNormal.GetFontID();
	int32_t letterHeight = ewol::GetHeight(fontId);
	
	coord2D_ts textPos;
	textPos.x = 20;
	textPos.y = 20;
	
	myColor = ColorizeManager::Get("normal");
	OOTextBold.SetColor(myColor->GetFG());
	etk::UString tmpDisplay = "edn - Editeur De N'ours";
	OOTextBold.Text(textPos, tmpDisplay);
	
	myColor = ColorizeManager::Get("commentDoxygen");
	OOTextNormal.SetColor(myColor->GetFG());
	textPos.y = (int32_t)(textPos.y + letterHeight*1.30);
	tmpDisplay = "No Buffer Availlable to display";
	OOTextNormal.Text(textPos, tmpDisplay);
	
	OOColored.SetColor(etk::color::color_White);
	OOColored.Rectangle( 0, 0, sizeX, sizeY);
	
	return ERR_NONE;
}



