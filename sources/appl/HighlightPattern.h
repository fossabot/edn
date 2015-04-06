/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Highlight.h>

#ifndef __HIGHLIGHT_PATTERN_H__
#define __HIGHLIGHT_PATTERN_H__

class HighlightPattern;


#include <appl/GlyphPainting.h>
#include <vector>
#include <regex>
#include <exml/exml.h>

enum resultFind {
	HLP_FIND_ERROR,
	HLP_FIND_OK,
	HLP_FIND_OK_NO_END,
};

namespace appl {
	class HighlightPattern {
		private:
			std::shared_ptr<appl::GlyphPainting> m_glyphPainting;
		public:
			// Constructeur
			HighlightPattern();
			HighlightPattern(const std::shared_ptr<appl::GlyphPainting>& _glyphPainting, const std::shared_ptr<const exml::Element>& _child, int32_t _level);
			virtual ~HighlightPattern();
		private:
			std::string m_paternName; //!< Current style name (like "c++" or "c" or "script Bash")
		public:
			void setName(const std::string& _name) {
				m_paternName = _name;
			};
			const std::string& getName() {
				return m_paternName;
			};
		private:
			std::string m_paternSubName; //!< Sub patern name if needed
		public:
			void setSubPatternName(const std::string& _name) {
				m_paternSubName = _name;
			};
			const std::string& getSubPatternName() {
				return m_paternSubName;
			};
		private:
			bool m_hasParsingError;
			std::string m_regexValue;
			std::regex m_regExp; //!< Start of Regular expression
		public:
			void setPatern(const std::string& _regExp);
			std::string getPaternString();
		private:
			std::string m_colorName; //!< Current color name
			int32_t m_colorId; //!< Id of the the glyph painting
		public:
			void setColorGlyph(const std::string& _colorName);
			const appl::GlyphDecoration& getColorGlyph() {
				return (*m_glyphPainting)[m_colorId];
			};
		private:
			int32_t m_level; //!< Level of the pattern  == > this is to overwrite next pattern when we create an higher ....
		public:
			void setLevel(int32_t _newLevel) {
				m_level = _newLevel;
			};
			int32_t getLevel() {
				return m_level;
			};
		private:
			
		public:
			void display();
			/**
			 * @brief find Element only in the specify start characters and find the end with the range done
			 * @param[in] _start First character to search data (if recognise it start here)
			 * @param[in] _stop End of the possibility whe search can continue
			 * @param[out] _resultat Position where find data
			 * @param[in] _buffer : Where to search data
			 * @return HLP_FIND_OK We find a compleate pattern
			 * @return HLP_FIND_OK_NO_END Xe find a partial pattern (missing end)
			 * @return HLP_FIND_ERROR Not find the pattern
			 */
			enum resultFind find(int32_t _start,
			                     int32_t _stop,
			                     appl::HighlightInfo& _resultat,
			                     const std::string& _buffer);
			
			void parseRules(const std::shared_ptr<const exml::Element>& _child, int32_t _level);
	};
};

#endif


