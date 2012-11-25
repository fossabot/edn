/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __COLORIZE_MANAGER_H__
#define __COLORIZE_MANAGER_H__

#include <Colorize.h>
#include <appl/globalMsg.h>
#include <draw/Color.h>
#include <ewol/widget/Widget.h>

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



namespace ColorizeManager
{
	void        Init(void);
	void        UnInit(void);
	void        LoadFile(etk::UString &xmlFilename);
	void        LoadFile(const char * xmlFilename);
	Colorize *  Get(const char *colorName);
	Colorize *  Get(etk::UString &colorName);
	draw::Color& Get(basicColor_te myColor);
	bool        Exist(etk::UString &colorName);
	bool        Exist(const char *colorName);
	void        DisplayListOfColor(void);
};

#endif

