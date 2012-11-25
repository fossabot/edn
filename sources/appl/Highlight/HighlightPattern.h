/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <Highlight.h>

#ifndef __HIGHLIGHT_PATTERN_H__
#define __HIGHLIGHT_PATTERN_H__

class HighlightPattern;


#include <etk/RegExp.h>
#include <Colorize.h>
#include <etk/Vector.h>
#include <tinyXML/tinyxml.h>
#include <etk/Buffer.h>

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
		resultFind_te   Find(int32_t start, int32_t stop, colorInformation_ts &resultat, etk::Buffer &buffer);
		Colorize *      GetColor(void) { return m_color; };
		void            ParseRules(TiXmlNode *child, int32_t level);
		
		void            ReloadColor(void);

	private:
		int32_t                             m_level;                    //!< Level of the pattern ==> this is to overwrite next pattern when we create an higher ....
		etk::UString                        m_paternName;               //!< Current style name (like "c++" or "c" or "script Bash")
		etk::UString                        m_colorName;                //!< Current color name
		Colorize *                          m_color;                    //!< Link to the color manager
		etk::RegExp<etk::Buffer> *          m_regExpStart;              //!< Start of Regular expression
		etk::RegExp<etk::Buffer> *          m_regExpStop;               //!< Stop of Regular Expression
		bool                                m_haveStopPatern;           //!< Stop patern presence
		bool                                m_multiline;                //!< The patern is multiline
		uniChar_t                           m_escapeChar;               //!< Escape char to prevent exeit of patern ....
		etk::Vector<HighlightPattern *>     m_subPatern;                //!< Under patern of this one
//		etk::Vector<HighlightPattern *>     m_subColor;                 //!< Under Color in the start RegExp ...
};

#endif


