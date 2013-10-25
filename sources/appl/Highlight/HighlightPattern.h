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
#include <glyphDecoration/GlyphPainting.h>
#include <etk/Vector.h>
#include <exml/exml.h>
#include <etk/Buffer.h>

typedef enum {
	HLP_FIND_ERROR,
	HLP_FIND_OK,
	HLP_FIND_OK_NO_END,
}resultFind_te;

namespace appl {
	class HighlightPattern {
		private:
			appl::GlyphPainting*& m_glyphPainting;
		public:
			// Constructeur
			HighlightPattern(appl::GlyphPainting*& _glyphPainting);
			~HighlightPattern(void);
		private:
			etk::UString m_paternName; //!< Current style name (like "c++" or "c" or "script Bash")
		public:
			void setName(etk::UString& _name) {
				m_paternName = _name;
			};
			etk::UString getName(void) {
				return m_paternName;
			};
		private:
			etk::RegExp<etk::Buffer>* m_regExpStart; //!< Start of Regular expression
		public:
			void setPaternStart(etk::UString& _regExp);
		private:
			etk::RegExp<etk::Buffer>* m_regExpStop; //!< Stop of Regular Expression
		public:
			void setPaternStop(etk::UString& _regExp);
		private:
			etk::UString m_colorName; //!< Current color name
			esize_t m_colorId; //!< Id of the the glyph painting
		public:
			void setColorGlyph(etk::UString& _colorName);
			const appl::GlyphDecoration& getColorGlyph(void) {
				return (*m_glyphPainting)[m_colorId];
			};
		private:
			etk::UChar m_escapeChar; //!< Escape char to prevent exeit of patern ....
		public:
			void setEscapeChar(const etk::UChar& _EscapeChar);
		private:
			bool m_multiline; //!< The patern is multiline
		public:
			void setMultiline(bool _enable) {
				m_multiline = _enable;
			};
		private:
			int32_t m_level; //!< Level of the pattern  == > this is to overwrite next pattern when we create an higher ....
		public:
			void setLevel(int32_t _newLevel) {
				m_level = _newLevel;
			};
			int32_t getLevel(void) {
				return m_level;
			};
		private:
			
		public:
			void display(void);
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
			resultFind_te find(int32_t _start,
			                   int32_t _stop,
			                   appl::ColorInfo& _resultat,
			                   etk::Buffer& _buffer);
			
			void parseRules(exml::Element* _child, int32_t _level);
	};
};

#endif


