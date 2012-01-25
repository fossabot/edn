/**
 *******************************************************************************
 * @file Display.cpp
 * @brief Editeur De N'ours : Basic Pixbuf display function
 * @author Edouard DUPIN
 * @date 21/01/2010
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
#include <Display.h>

#undef __class__
#define __class__	"Display"

int32_t Display::GetFontHeight(void)
{
	int32_t fontId = ewol::GetDefaultFontId();
	return ewol::GetHeight(fontId);
}

int32_t Display::GetFontWidth(void)
{
	int32_t fontId = ewol::GetDefaultFontId();
	return ewol::GetWidth(fontId, "A");
}

