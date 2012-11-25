/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __COLORIZE_H__
#define __COLORIZE_H__
#include <draw/Color.h>
#include <etk/UString.h>

class Colorize {
	public:
		// Constructeur
		Colorize(void);
		Colorize(etk::UString &newColorName);
		~Colorize(void);
		
		void SetName(etk::UString &newColorName);
		void SetName(const char *newColorName);
		etk::UString GetName(void);
		void SetFgColor(const char *myColor);
		void SetBgColor(const char *myColor);
		
		draw::Color & GetFG(void) { return m_colorFG; };
		draw::Color & GetBG(void) { return m_colorBG; };

		bool HaveBg(void);
		
		void SetItalic(bool enable);
		void SetBold(bool enable);
		bool GetItalic(void);
		bool GetBold(void);
		
		void Display(int32_t i) { APPL_INFO("        " << i << " : \"" <<  ColorName << "\"" << "     fg="<< m_colorFG); };

	private:
		etk::UString ColorName;						//!< curent color Name
		draw::Color m_colorFG;
		draw::Color m_colorBG;
		
		bool italic;
		bool bold;

};

#endif


