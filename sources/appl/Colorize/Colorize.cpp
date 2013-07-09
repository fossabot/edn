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


Colorize::Colorize(const etk::UString &_newColorName) :
	m_colorName(_newColorName),
	m_colorFG(etk::color::black),
	m_colorBG(etk::color::none),
	m_italic(false),
	m_bold(false)
{
	APPL_VERBOSE("New(Colorise)");
}

void Colorize::SetItalic(bool _enable)
{
	m_italic = _enable;
	if (true == _enable) {
		APPL_VERBOSE("color : \"" << m_colorName << "\" enable italic");
	} else {
		APPL_VERBOSE("color : \"" << m_colorName << "\" disable italic");
	}
}

void Colorize::SetBold(bool _enable)
{
	m_bold = _enable;
	if (true == _enable) {
		APPL_VERBOSE("color : \"" << m_colorName << "\" enable bold");
	} else {
		APPL_VERBOSE("color : \"" << m_colorName << "\" disable bold");
	}
}



