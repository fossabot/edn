/**
 *******************************************************************************
 * @file Display.h
 * @brief Editeur De N'ours : Basic Pixbuf display function (header)
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
 
#ifndef __TOOLS_DISPLAY_H__
#define __TOOLS_DISPLAY_H__

#include <tools_debug.h>


/**
 *  Basic namespace for the font display system
 */
namespace Display
{
	int32_t                 GetFontHeight(void);
	int32_t                 GetFontWidth(void);
};


#endif

