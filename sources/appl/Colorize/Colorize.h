/**
 *******************************************************************************
 * @file Colorize.h
 * @brief Editeur De N'ours : Colirizing system (header)
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
 
#ifndef __COLORIZE_H__
#define __COLORIZE_H__
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


