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
#ifdef __VIDEO__OPENGL_ES_2
int32_t BufferEmpty::Display(ewol::TEXT_DISPLAY_TYPE& OOText,
                             ewol::OObject2DColored& OOColored, int32_t offsetX, int32_t offsetY, int32_t sizeX, int32_t sizeY)
#else
int32_t BufferEmpty::Display(ewol::TEXT_DISPLAY_TYPE& OOTextNormal,
                             ewol::TEXT_DISPLAY_TYPE& OOTextBold,
                             ewol::TEXT_DISPLAY_TYPE& OOTextItalic,
                             ewol::TEXT_DISPLAY_TYPE& OOTextBoldItalic,
                             ewol::OObject2DColored& OOColored, int32_t offsetX, int32_t offsetY, int32_t sizeX, int32_t sizeY)
#endif
{
	// Get color : 
	Colorize	*myColor = NULL;
	
	#ifdef __VIDEO__OPENGL_ES_2
		int32_t letterHeight = OOText.GetHeight();
	#else
		int32_t letterHeight = OOTextNormal.GetHeight();
	#endif
	
	Vector2D<float>  textPos;
	textPos.x = 20;
	textPos.y = sizeY - 20 - letterHeight;
	etk::UString tmpDisplay ;
	
	myColor = ColorizeManager::Get("normal");
	tmpDisplay = "edn - Editeur De N'ours";
	#ifdef __VIDEO__OPENGL_ES_2
		OOText.SetColor(myColor->GetFG());
		OOText.SetBold(true);
		OOText.SetItalic(false);
		OOText.Text(textPos, tmpDisplay);
	#else
		OOTextBold.SetColor(myColor->GetFG());
		OOTextBold.Text(textPos, tmpDisplay);
	#endif
	
	myColor = ColorizeManager::Get("commentDoxygen");
	textPos.y = (int32_t)(textPos.y - letterHeight*1.30);
	tmpDisplay = "No Buffer Availlable to display";
	#ifdef __VIDEO__OPENGL_ES_2
		OOText.SetBold(false);
		OOText.SetItalic(false);
		OOText.SetColor(myColor->GetFG());
		OOText.Text(textPos, tmpDisplay);
	#else
		OOTextNormal.SetColor(myColor->GetFG());
		OOTextNormal.Text(textPos, tmpDisplay);
	#endif
	
	OOColored.SetColor(draw::color::white);
	OOColored.Rectangle( 0, 0, sizeX, sizeY);
	
	return ERR_NONE;
}



