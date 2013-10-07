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
#include <exml/exml.h>
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
		
		void            setName(etk::UString &name) { m_paternName = name;};
		etk::UString    getName(void) { return m_paternName;};
		
		void            setPaternStart(etk::UString &regExp);
		void            setPaternStop(etk::UString &regExp);
		void            setColor(etk::UString &colorName);
		void            setEscapeChar(etk::UString &EscapeChar);
		void            setMultiline(bool enable) { m_multiline = enable; };
		
		void            setLevel(int32_t newLevel) { m_level = newLevel; };
		int32_t         getLevel(void) { return m_level; };
		
		bool            isEnable(void);
		void            display(void);
		resultFind_te   find(int32_t start, int32_t stop, colorInformation_ts &resultat, etk::Buffer &buffer);
		Colorize *      getColor(void) { return m_color; };
		void            ParseRules(exml::Element *child, int32_t level);
		
		void            ReloadColor(void);

	private:
		int32_t                             m_level;                    //!< Level of the pattern  == > this is to overwrite next pattern when we create an higher ....
		etk::UString                        m_paternName;               //!< Current style name (like "c++" or "c" or "script Bash")
		etk::UString                        m_colorName;                //!< Current color name
		Colorize *                          m_color;                    //!< Link to the color manager
		etk::RegExp<etk::Buffer> *          m_regExpStart;              //!< Start of Regular expression
		etk::RegExp<etk::Buffer> *          m_regExpStop;               //!< Stop of Regular Expression
		bool                                m_haveStopPatern;           //!< Stop patern presence
		bool                                m_multiline;                //!< The patern is multiline
		etk::UniChar                        m_escapeChar;               //!< Escape char to prevent exeit of patern ....
		etk::Vector<HighlightPattern *>     m_subPatern;                //!< Under patern of this one
//		etk::Vector<HighlightPattern *>     m_subColor;                 //!< Under Color in the start RegExp ...
};

#endif


