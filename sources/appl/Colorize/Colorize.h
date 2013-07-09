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
		void SetName(const etk::UString& _newColorName) { m_colorName = _newColorName; };
		const etk::UString& GetName(void) { return m_colorName; };
		
	private:
		etk::Color<> m_colorFG;
	public:
		void SetFgColor(const etk::UString& _myColor) { m_colorFG=_myColor; };
		const etk::Color<>& GetFG(void) { return m_colorFG; };
		bool HaveFg(void) { return m_colorFG.a()!=0; };
	private:
		etk::Color<> m_colorBG;
	public:
		void SetBgColor(const etk::UString& _myColor) { m_colorBG=_myColor; };
		const etk::Color<>& GetBG(void) { return m_colorBG; };
		bool HaveBg(void) { return m_colorBG.a()!=0; };
	private:
		bool m_italic;
	public:
		void SetItalic(bool _enable);
		bool GetItalic(void) { return m_italic; };
	private:
		bool m_bold;
	public:
		void SetBold(bool _enable);
		bool GetBold(void) { return m_bold; };
		void Display(int32_t _i) { APPL_INFO("        " << _i << " : fg="<< m_colorFG << " bold=" << m_italic << " bold=" << m_italic << "\"" <<  m_colorName << "\""); };
};

#endif


