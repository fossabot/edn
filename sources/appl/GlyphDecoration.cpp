/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/debug.hpp>
#include <appl/global.hpp>
#include <appl/GlyphDecoration.hpp>

appl::GlyphDecoration::GlyphDecoration(const std::string &_newColorName) :
  m_colorName(_newColorName),
  m_colorFG(etk::color::black),
  m_colorBG(etk::color::none),
  m_italic(false),
  m_bold(false) {
	APPL_VERBOSE("create");
}

void appl::GlyphDecoration::setItalic(bool _enable) {
	m_italic = _enable;
	if (_enable == true) {
		APPL_VERBOSE("color : \"" << m_colorName << "\" enable italic");
	} else {
		APPL_VERBOSE("color : \"" << m_colorName << "\" disable italic");
	}
}

void appl::GlyphDecoration::setBold(bool _enable) {
	m_bold = _enable;
	if (_enable == true) {
		APPL_VERBOSE("color : \"" << m_colorName << "\" enable bold");
	} else {
		APPL_VERBOSE("color : \"" << m_colorName << "\" disable bold");
	}
}

std::ostream& appl::operator <<(std::ostream& _os, const appl::GlyphDecoration& _obj) {
	_os << "{fg=" << _obj.getForeground();
	_os << ",bg=" << _obj.getBackground();
	_os << ",italic=" << _obj.getItalic();
	_os << ",bold=" << _obj.getBold();
	_os << "name='" << _obj.getName() << "'}";
	return _os;
}
