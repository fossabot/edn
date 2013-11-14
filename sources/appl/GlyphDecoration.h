/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __GLYPH_DECORATION_H__
#define __GLYPH_DECORATION_H__

#include <etk/Color.h>
#include <etk/UString.h>

namespace appl {
	class GlyphDecoration {
		public:
			// Constructeur
			GlyphDecoration(const std::string& _newColorName = "no_name");
			~GlyphDecoration(void) {
				// nothing to do ...
			};
		private:
			std::string m_colorName; //!< curent color Name
		public:
			/**
			 * @brief Set color name of the element.
			 * @param[in] _newColorName new color name.
			 */
			void setName(const std::string& _newColorName) {
				m_colorName = _newColorName;
			};
			/**
			 * @brief Get the color name.
			 * @return The name of the color.
			 */
			const std::string& getName(void) const {
				return m_colorName;
			};
		private:
			etk::Color<> m_colorFG; //!< Foreground color
		public:
			/**
			 * @brief Set foreground color.
			 * @param[in] _myColor new color description.
			 */
			void setForeground(const std::string& _myColor) {
				m_colorFG = _myColor;
			};
			/**
			 * @brief Get the foreground color.
			 * @return The color.
			 */
			const etk::Color<>& getForeground(void) const {
				return m_colorFG;
			};
			/**
			 * @brief Get the foreground color status.
			 * @return true if the color is visible.
			 */
			bool haveFg(void) const {
				return m_colorFG.a() != 0;
			};
		private:
			etk::Color<> m_colorBG; //!< Background color
		public:
			/**
			 * @brief Set background color.
			 * @param[in] _myColor new color description.
			 */
			void setBackground(const std::string& _myColor) {
				m_colorBG = _myColor;
			};
			/**
			 * @brief Get the background color.
			 * @return The color.
			 */
			const etk::Color<>& getBackground(void) const {
				return m_colorBG;
			};
			/**
			 * @brief Get the background color status.
			 * @return true if the color is visible.
			 */
			bool haveBackground(void) const {
				return m_colorBG.a()!=0;
			};
		private:
			bool m_italic; //!< the gryph might be italic.
		public:
			/**
			 * @brief Set the italic status.
			 * @param[in] _enable new status of italic request.
			 */
			void setItalic(bool _enable);
			/**
			 * @brief Get the italic status.
			 * @return true if the glyph might be display in italic.
			 */
			bool getItalic(void) const {
				return m_italic;
			};
		private:
			bool m_bold; //!< the gryph might be bold.
		public:
			/**
			 * @brief Set the bold status.
			 * @param[in] _enable new status of bold request.
			 */
			void setBold(bool _enable);
			/**
			 * @brief Get the bold status.
			 * @return true if the glyph might be display in bold.
			 */
			bool getBold(void) const {
				return m_bold;
			};
	};
	etk::CCout& operator <<(etk::CCout& _os, const appl::GlyphDecoration& _obj);
}

#endif


