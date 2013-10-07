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
#include <etk/Color.h>
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
	void        init(void);
	void        UnInit(void);
	void        loadFile(const etk::UString& _xmlFilename);
	Colorize *  get(const etk::UString& _colorName);
	etk::Color<>& get(basicColor_te _myColor);
	bool        Exist(const etk::UString& _colorName);
	void        displayListOfColor(void);
};

#endif

