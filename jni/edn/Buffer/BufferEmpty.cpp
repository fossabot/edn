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
 
#include <tools_debug.h>
#include <tools_globals.h>
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
int32_t BufferEmpty::Display(ewol::OObject2DTextColored* OOTextNormal,
                             ewol::OObject2DTextColored* OOTextBold,
                             ewol::OObject2DTextColored* OOTextItalic,
                             ewol::OObject2DTextColored* OOTextBoldItalic,
                             ewol::OObject2DColored* OOColored, int32_t sizeX, int32_t sizeY)
{
	ColorizeManager * myColorManager = ColorizeManager::getInstance();
	// Get color : 
	Colorize	*myColor = NULL;
	//drawer.Flush();
	
	
	if (NULL == OOTextNormal) {
		EDN_ERROR("Input VALUE is NULL");
		return ERR_FAIL;
	}
	if (NULL == OOColored) {
		EDN_ERROR("Input VALUE is NULL");
		return ERR_FAIL;
	}
	int32_t fontId = OOTextNormal->GetFontID();
	int32_t letterHeight = ewol::GetHeight(fontId);
	
	myColor = myColorManager->Get("normal");
	OOTextNormal->SetColor(myColor->GetFG());
	OOTextNormal->TextAdd(20, 20, "edn - Editeur De N'ours, l'Editeur Desoxyribo-Nucleique", sizeX);
	
	myColor = myColorManager->Get("commentDoxygen");
	OOTextNormal->SetColor(myColor->GetFG());
	OOTextNormal->TextAdd(20, 20 + letterHeight*1.30, "No Buffer Availlable to display", sizeX);
	
	
	color_ts bgColor;  //!< Text color
	bgColor.red   = 1.0;
	bgColor.green = 1.0;
	bgColor.blue  = 1.0;
	bgColor.alpha = 1.0;
	OOColored->SetColor(bgColor);
	OOColored->Rectangle( 0, 0, sizeX, sizeY);
	
	return ERR_NONE;
}



