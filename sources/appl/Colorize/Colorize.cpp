/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <Colorize.h>


#undef __class__
#define __class__	"Colorize"


Colorize::Colorize(	etk::UString &newColorName)
{

	m_colorFG = draw::color::black;
	m_colorBG = draw::color::none;
	
	italic	= false;
	bold	= false;
	SetName(newColorName);
	APPL_VERBOSE("New(Colorise)");
}

Colorize::Colorize(void)
{
	ColorName = "no_name";
	
	m_colorFG = draw::color::black;
	m_colorBG = draw::color::none;
	
	italic	= false;
	bold	= false;
	
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
	m_colorFG = myColor;
	APPL_VERBOSE(myColor << " ==> "<< m_colorFG );
}

void Colorize::SetBgColor(const char *myColor)
{
	m_colorBG = myColor;
	APPL_VERBOSE(myColor << " ==> "<< m_colorBG );
}

bool Colorize::HaveBg(void)
{
	return m_colorBG.a!=0;
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







