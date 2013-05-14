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


appl::Colorize::Colorize(const etk::UString &_colorName) :
	m_name(_colorName),
	m_colorFG = draw::color::black;
	m_colorBG = draw::color::none;
	m_italic	= false;
	m_bold	= false;
{
	APPL_VERBOSE("New(Colorise)");
}

void appl::Colorize::SetName(const etk::UString &_newColorName)
{
	APPL_VERBOSE("color change name : \"" << m_name << "\" ==> \"" << _newColorName << "\"");
	m_name = _newColorName;
}

void appl::Colorize::SetFgColor(const etk::UString& _myColor)
{
	m_colorFG = _myColor;
	APPL_VERBOSE(_myColor << " ==> "<< m_colorFG );
}

void appl::Colorize::SetBgColor(const etk::UString& _myColor)
{
	m_colorBG = _myColor;
	APPL_VERBOSE(_myColor << " ==> "<< m_colorBG );
}


void appl::Colorize::SetItalic(bool _enable)
{
	m_italic = _enable;
	if (true == m_italic) {
		APPL_VERBOSE("color : \"" << ColorName << "\" enable italic");
	} else {
		APPL_VERBOSE("color : \"" << ColorName << "\" disable italic");
	}
}

void appl::Colorize::SetBold(bool _enable)
{
	m_bold = _enable;
	if (true == m_bold) {
		APPL_VERBOSE("color : \"" << m_name << "\" enable bold");
	} else {
		APPL_VERBOSE("color : \"" << m_name << "\" disable bold");
	}
}

etk::CCout& appl::operator <<(etk::CCout& _os, const appl::Colorize& _obj)
{
	_os << "{name=\"" << _obj.GetName() << "\"";
	_os << " fg=" << _obj.GetFG();
	if (_obj.HaveBg()==true) {
		_os << " bg=" << _obj.GetBG();
	}
	if (_obj.GetBold()==true) {
		_os << " bold";
	}
	if (_obj.GetItalic()==true) {
		_os << " italic";
	}
		_os << "}";
	return _os;
}


