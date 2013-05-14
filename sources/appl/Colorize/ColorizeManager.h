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


namespace appl
{
	namespace ColorizeManager
	{
		void Init(void);
		void UnInit(void);
		void LoadFile(const etk::UString &_xmlFilename);
		appl::Colorize* Get(const etk::UString &_colorName);
		bool Exist(const etk::UString &_colorName);
		void DisplayListOfColor(void);
	};
};
#endif

