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
int32_t	BufferEmpty::Display(DrawerManager &drawer)
{

	ColorizeManager * myColorManager = ColorizeManager::getInstance();
	// Get color : 
	Colorize	*myColor = NULL;
	// Clean Buffer
	drawer.Clean(myColorManager->Get(COLOR_CODE_BASIC_BG) );
	
	myColor = myColorManager->Get("normal");
	// Draw the 2 comments Lines :
	drawer.Text(myColor, 20,20, "edn - Editeur De N'ours, l'Editeur Desoxyribo-Nucleique");
	//drawer.Flush();
	myColor = myColorManager->Get("commentDoxygen");
	drawer.Text(myColor, 20,25 + Display::GetFontHeight(), "No Buffer Availlable to display");
	drawer.Flush();
	/*
	myColor = myColorManager->Get(("SelectedText"));
	drawer.Cursor(20, 50);
	drawer.EndOfLine(20, 70);
	drawer.Tabulation(myColor, 20, 90, 5);
	drawer.UTF8UnknownElement(myColor, 20, 120, 3, false);
	drawer.Flush();
	*/
	return ERR_NONE;
}



