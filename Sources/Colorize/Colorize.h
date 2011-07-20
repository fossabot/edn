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

#include "Edn.h"

extern "C" {
	typedef struct {
		float red;
		float green;
		float blue;
	} color_ts;
}


class Colorize {
	public:
		// Constructeur
		Colorize(void);
		Colorize(Edn::String &newColorName);
		~Colorize(void);
		
		void SetName(Edn::String &newColorName);
		void SetName(const char *newColorName);
		Edn::String GetName(void);
		void SetFgColor(const char *myColor);
		void SetBgColor(const char *myColor);
		void ApplyFG(cairo_t * cr) { cairo_set_source_rgb(cr, m_colorFG.red, m_colorFG.green, m_colorFG.blue); };
		void ApplyBG(cairo_t * cr) { cairo_set_source_rgb(cr, m_colorBG.red, m_colorBG.green, m_colorBG.blue); };
		
		color_ts & GetFG(void) { return m_colorFG; };
		color_ts & GetBG(void) { return m_colorBG; };

		bool HaveBg(void);
		
		void SetItalic(bool enable);
		void SetBold(bool enable);
		bool GetItalic(void);
		bool GetBold(void);
		
		void Display(int32_t i) { EDN_INFO("        " << i << " : \"" <<  ColorName.c_str() << "\"" /*<< "     fg="<< m_colorFG.red <<","<< m_colorFG.green <<","<< m_colorFG.blue <<"     bg="<< m_colorBG.red <<","<< m_colorBG.green <<","<< m_colorBG.blue*/ ); };

	private:
		Edn::String ColorName;						//!< curent color Name
		color_ts m_colorFG;
		color_ts m_colorBG;
		
		bool italic;
		bool bold;
		bool haveFG;
		bool haveBG;

};

#endif


