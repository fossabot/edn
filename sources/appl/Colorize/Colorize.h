/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __COLORIZE_H__
#define __COLORIZE_H__
#include <etk/Color.h>
#include <etk/UString.h>

class Colorize {
	public:
		// Constructeur
		Colorize(const etk::UString& _newColorName="no_name");
		~Colorize(void) { };
		
	private:
		etk::UString m_colorName; //!< curent color Name
	public:
		void setName(const etk::UString& _newColorName) { m_colorName = _newColorName; };
		const etk::UString& getName(void) { return m_colorName; };
		
	private:
		etk::Color<> m_colorFG;
	public:
		void setFgColor(const etk::UString& _myColor) { m_colorFG=_myColor; };
		const etk::Color<>& getFG(void) { return m_colorFG; };
		bool haveFg(void) { return m_colorFG.a()!=0; };
	private:
		etk::Color<> m_colorBG;
	public:
		void setBgColor(const etk::UString& _myColor) { m_colorBG=_myColor; };
		const etk::Color<>& getBG(void) { return m_colorBG; };
		bool haveBg(void) { return m_colorBG.a()!=0; };
	private:
		bool m_italic;
	public:
		void setItalic(bool _enable);
		bool getItalic(void) { return m_italic; };
	private:
		bool m_bold;
	public:
		void setBold(bool _enable);
		bool getBold(void) { return m_bold; };
		void display(int32_t _i) { APPL_INFO("        " << _i << " : fg="<< m_colorFG << " bold=" << m_italic << " bold=" << m_italic << "\"" <<  m_colorName << "\""); };
};

#endif


