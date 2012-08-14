/**
 *******************************************************************************
 * @file HighlightPattern.h
 * @brief Editeur De N'ours : Hightlight Patern (header)
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

#include <Highlight.h>

#ifndef __HIGHLIGHT_PATTERN_H__
#define __HIGHLIGHT_PATTERN_H__

class HighlightPattern;


#include <etk/RegExp.h>
#include <Colorize.h>
#include <etk/Vector.h>
#include <tinyXML/tinyxml.h>
#include <EdnVectorBuf.h>

typedef enum {
	HLP_FIND_ERROR,
	HLP_FIND_OK,
	HLP_FIND_OK_NO_END,
}resultFind_te;

class HighlightPattern;

class HighlightPattern {
	public:
		// Constructeur
		HighlightPattern(void);
		~HighlightPattern(void);
		
		void            SetName(etk::UString &name) { m_paternName = name;};
		etk::UString    GetName(void) { return m_paternName;};
		
		void            SetPaternStart(etk::UString &regExp);
		void            SetPaternStop(etk::UString &regExp);
		void            SetColor(etk::UString &colorName);
		void            SetEscapeChar(etk::UString &EscapeChar);
		void            SetMultiline(bool enable) { m_multiline = enable; };
		
		void            SetLevel(int32_t newLevel) { m_level = newLevel; };
		int32_t         GetLevel(void) { return m_level; };
		
		bool            IsEnable(void);
		void            Display(void);
		resultFind_te   Find(int32_t start, int32_t stop, colorInformation_ts &resultat, EdnVectorBuf &buffer);
		Colorize *      GetColor(void) { return m_color; };
		void            ParseRules(TiXmlNode *child, int32_t level);
		
		void            ReloadColor(void);

	private:
		int32_t                             m_level;                    //!< Level of the pattern ==> this is to overwrite next pattern when we create an higher ....
		etk::UString                        m_paternName;               //!< Current style name (like "c++" or "c" or "script Bash")
		etk::UString                        m_colorName;                //!< Current color name
		Colorize *                          m_color;                    //!< Link to the color manager
		etk::RegExp<EdnVectorBuf> *         m_regExpStart;              //!< Start of Regular expression
		etk::RegExp<EdnVectorBuf> *         m_regExpStop;               //!< Stop of Regular Expression
		bool                                m_haveStopPatern;           //!< Stop patern presence
		bool                                m_multiline;                //!< The patern is multiline
		uniChar_t                           m_escapeChar;               //!< Escape char to prevent exeit of patern ....
		etk::Vector<HighlightPattern *> m_subPatern;                //!< Under patern of this one
//		etk::Vector<HighlightPattern *> m_subColor;                 //!< Under Color in the start RegExp ...
};

#endif


