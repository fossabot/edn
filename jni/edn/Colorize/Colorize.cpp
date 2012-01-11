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

#include <tools_debug.h>
#include <tools_globals.h>
#include <Colorize.h>


#undef __class__
#define __class__	"Colorize"


Colorize::Colorize(	etk::String &newColorName)
{

	m_colorFG.red=0;
	m_colorFG.green=0;
	m_colorFG.blue=0;
	m_colorBG.red=0;
	m_colorBG.green=0;
	m_colorBG.blue=0;
	
	italic	= false;
	bold	= false;
	haveFG	= false;
	haveBG	= false;
	SetName(newColorName);
	//EDN_INFO("New(Colorise)");
}

Colorize::Colorize(void)
{
	ColorName = "no_name";
	
	m_colorFG.red=0;
	m_colorFG.green=0;
	m_colorFG.blue=0;
	m_colorBG.red=0;
	m_colorBG.green=0;
	m_colorBG.blue=0;
	
	italic	= false;
	bold	= false;
	haveFG	= false;
	haveBG	= false;
	
	//EDN_INFO("New(Colorise)");
}

Colorize::~Colorize(void)
{
	// nothing to do ...
}


void Colorize::SetName(const char *newColorName)
{
	//EDN_INFO("color change name : \"%s\" ==> \"%s\"",ColorName.c_str(), newColorName);
	ColorName = newColorName;
}

void Colorize::SetName(etk::String &newColorName)
{
	//EDN_INFO("color change name : \"%s\" ==> \"%s\"",ColorName.c_str(), newColorName.c_str());
	ColorName = newColorName;
}


etk::String Colorize::GetName(void)
{
	return ColorName;
}


void Colorize::SetFgColor(const char *myColor)
{
	haveFG = true;
	unsigned int r=0;
	unsigned int v=0;
	unsigned int b=0;
	sscanf(myColor, "#%02x%02x%02x", &r,&v,&b);
	m_colorFG.red = (float)r/255.0;
	m_colorFG.green = (float)v/255.0;
	m_colorFG.blue = (float)b/255.0;
	//EDN_INFO(myColor << " ==> r="<< r <<" v="<< v <<" b="<< b );
}

void Colorize::SetBgColor(const char *myColor)
{
	haveBG = true;
	unsigned int r,v,b;
	sscanf(myColor, "#%02x%02x%02x", &r,&v,&b);
	m_colorBG.red = (float)r/255.0;
	m_colorBG.green = (float)v/255.0;
	m_colorBG.blue = (float)b/255.0;
}

bool Colorize::HaveBg(void)
{
	return haveBG;
}


void Colorize::SetItalic(bool enable)
{
	italic = enable;
	/*
	if (true == enable) {
		EDN_INFO("color : \"%s\" enable italic", ColorName.c_str());
	} else {
		EDN_INFO("color : \"%s\" disable italic", ColorName.c_str());
	}
	*/
}

bool Colorize::GetItalic(void)
{
	return italic;
}


void Colorize::SetBold(bool enable)
{
	bold = enable;
	/*
	if (true == enable) {
		EDN_INFO("color : \"%s\" enable bold", ColorName.c_str());
	} else {
		EDN_INFO("color : \"%s\" disable bold", ColorName.c_str());
	}
	*/
}

bool Colorize::GetBold(void)
{
	return bold;
}







