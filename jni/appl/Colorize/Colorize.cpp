/**
 *******************************************************************************
 * @file Colorise.cpp
 * @brief Editeur De N'ours : Colirising system
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

#include <appl/Debug.h>
#include <appl/global.h>
#include <Colorize.h>


#undef __class__
#define __class__	"Colorize"


Colorize::Colorize(	etk::UString &newColorName)
{

	m_colorFG = etk::color::color_Black;
	m_colorBG = etk::color::color_Black;
	
	italic	= false;
	bold	= false;
	haveFG	= false;
	haveBG	= false;
	SetName(newColorName);
	APPL_VERBOSE("New(Colorise)");
}

Colorize::Colorize(void)
{
	ColorName = "no_name";
	
	m_colorFG = etk::color::color_Black;
	m_colorBG = etk::color::color_Black;
	
	italic	= false;
	bold	= false;
	haveFG	= false;
	haveBG	= false;
	
	APPL_VERBOSE("New(Colorise)");
}

Colorize::~Colorize(void)
{
	// nothing to do ...
}


void Colorize::SetName(const char *newColorName)
{
	APPL_VERBOSE("color change name : \"" << ColorName << "\" ==> \"" << newColorName << "\"");
	ColorName = newColorName;
}

void Colorize::SetName(etk::UString &newColorName)
{
	APPL_VERBOSE("color change name : \"" << ColorName << "\" ==> \"" << newColorName << "\"");
	ColorName = newColorName;
}


etk::UString Colorize::GetName(void)
{
	return ColorName;
}


void Colorize::SetFgColor(const char *myColor)
{
	haveFG = true;
	m_colorFG = etk::color::Parse(myColor);
	APPL_VERBOSE(myColor << " ==> "<< m_colorFG );
}

void Colorize::SetBgColor(const char *myColor)
{
	haveBG = true;
	m_colorBG = etk::color::Parse(myColor);
	APPL_VERBOSE(myColor << " ==> "<< m_colorBG );
}

bool Colorize::HaveBg(void)
{
	return haveBG;
}


void Colorize::SetItalic(bool enable)
{
	italic = enable;
	if (true == enable) {
		APPL_VERBOSE("color : \"" << ColorName << "\" enable italic");
	} else {
		APPL_VERBOSE("color : \"" << ColorName << "\" disable italic");
	}
}

bool Colorize::GetItalic(void)
{
	return italic;
}


void Colorize::SetBold(bool enable)
{
	bold = enable;
	if (true == enable) {
		APPL_VERBOSE("color : \"" << ColorName << "\" enable bold");
	} else {
		APPL_VERBOSE("color : \"" << ColorName << "\" disable bold");
	}
}

bool Colorize::GetBold(void)
{
	return bold;
}







