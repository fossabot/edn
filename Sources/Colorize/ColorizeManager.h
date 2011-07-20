/**
 *******************************************************************************
 * @file ColoriseManager.h
 * @brief Editeur De N'ours : Colorising Manager (header)
 * @author Edouard DUPIN
 * @date 14/12/2010
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
 
#ifndef __COLORIZE_MANAGER_H__
#define __COLORIZE_MANAGER_H__

#include "Singleton.h"
#include <vector>
#include <string>
#include "Colorize.h"

typedef enum {
	// BASIC color for codeViewer
	COLOR_CODE_SPACE,
	COLOR_CODE_TAB,
	COLOR_CODE_BASIC_BG,
	COLOR_CODE_CURSOR,
	COLOR_CODE_LINE_NUMBER,
	// Basic color for list viewer
	COLOR_LIST_BG_1,
	COLOR_LIST_BG_2,
	COLOR_LIST_BG_SELECTED,
	COLOR_LIST_TEXT_NORMAL,
	COLOR_LIST_TEXT_MODIFY,
	// KNOW the number of BASIC color
	COLOR_NUMBER_MAX,
}basicColor_te;



class ColorizeManager: public Singleton<ColorizeManager>
{
	friend class Singleton<ColorizeManager>;
	// specific for sigleton system...
	private:
		// Constructeur
		ColorizeManager(void);
		~ColorizeManager(void);
	public:
		void		  LoadFile(Edn::String &xmlFilename);
		void		  LoadFile(const char * xmlFilename);
		Colorize	* Get(const char *colorName);
		Colorize	* Get(Edn::String &colorName);
		color_ts	& Get(basicColor_te myColor);
		bool		  Exist(Edn::String &colorName);
		bool		  Exist(const char *colorName);
		void		  DisplayListOfColor(void);

	private:
		std::vector<Colorize*>	listMyColor;		//!< List of ALL Color
		Colorize*				errorColor;
		color_ts				basicColors[COLOR_NUMBER_MAX];
};

#endif

