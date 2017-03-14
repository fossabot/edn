/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <appl/Highlight.hpp>

class HighlightPattern;

#include <appl/GlyphPainting.hpp>
#include <vector>
#include <regex>
#include <etk/RegExp.hpp>
#include <etk/Buffer.hpp>
#include <exml/exml.hpp>

namespace appl {
	class HighlightPattern {
		private:
			ememory::SharedPtr<appl::GlyphPainting> m_glyphPainting;
		public:
			// Constructeur
			HighlightPattern();
			HighlightPattern(const ememory::SharedPtr<appl::GlyphPainting>& _glyphPainting, const exml::Element& _child, int32_t _level);
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
			std::string m_regexValue[2];
			bool m_hasEndRegEx;
			etk::RegExp<etk::Buffer> m_regExp[2]; //!< Start of Regular expression
		public:
			void setPatern(const std::string& _regExp, const std::string& _regExpStop="", bool _hasEndRegEx=false);
			std::pair<std::string,std::string> getPaternString();
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
			 * @return true We find a compleate pattern
			 * @return false Not find the pattern
			 */
			bool find(int32_t _start,
			          int32_t _stop,
			          appl::HighlightInfo& _resultat,
			          const etk::Buffer& _buffer);
			
			void parseRules(const exml::Element& _child, int32_t _level);
	};
}

