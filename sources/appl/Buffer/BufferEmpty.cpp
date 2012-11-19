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
int32_t BufferEmpty::Display(ewol::Text& OOText,
                             ewol::Drawing& OOColored, int32_t offsetX, int32_t offsetY, int32_t sizeX, int32_t sizeY)
{
	// Get color : 
	Colorize	*myColor = NULL;
	
	etk::Vector3D<float> tmpLetterSize = OOText.CalculateSize((uniChar_t)'A');
	int32_t letterHeight = tmpLetterSize.y;
	
	etk::Vector2D<float>  textPos;
	textPos.x = 20;
	textPos.y = sizeY - 20 - letterHeight;
	etk::UString tmpDisplay ;
	
	myColor = ColorizeManager::Get("normal");
	tmpDisplay = "edn - Editeur De N'ours";
	
	OOText.SetColor(myColor->GetFG());
	OOText.SetPos(etk::Vector3D<float>(textPos.x, textPos.y, 0.0f) );
	OOText.SetFontMode(ewol::font::Bold);
	OOText.Print(tmpDisplay);
	
	myColor = ColorizeManager::Get("commentDoxygen");
	textPos.y = (int32_t)(textPos.y - letterHeight*1.30);
	tmpDisplay = "No Buffer Availlable to display";
	
	OOText.SetColor(myColor->GetFG());
	OOText.SetPos(etk::Vector3D<float>(textPos.x, textPos.y, 0.0f) );
	OOText.SetFontMode(ewol::font::Regular);
	OOText.Print(tmpDisplay);
	
	OOColored.SetColor(draw::color::white);
	OOText.SetPos(etk::Vector3D<float>(0.0f, 0.0f, 0.0f) );
	OOColored.RectangleWidth(etk::Vector3D<float>((float)sizeX, (float)sizeY, 0.0f)  );
	
	return ERR_NONE;
}



