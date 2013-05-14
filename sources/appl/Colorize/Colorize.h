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

namespace appl
{
	class Colorize {
		public:
			/**
			 * @brief Constructeur
			 * @param[in] _colorName Name of the color...
			 */
			Colorize(etk::UString& _colorName="no_name");
			/**
			 * @brief Desstructeur
			 */
			virtual ~Colorize(void) { };
		private:
			etk::UString m_name; //!< curent color Name
		public:
			void SetName(const etk::UString &_newColorName);
			const etk::UString& GetName(void) { return m_name; };
		
		private:
			draw::Color m_colorFG; //!< Foreground color
			draw::Color m_colorBG; //!< Background color
		public:
			void SetFgColor(const etk::UString& _myColor);
			void SetBgColor(const etk::UString& _myColor);
			const draw::Color& GetFG(void) { return m_colorFG; };
			const draw::Color& GetBG(void) { return m_colorBG; };
			bool HaveBg(void) { return m_colorBG.a!=0; };
		
		private:
			bool m_italic; //!< the color request italic
			bool m_bold; //!< The color request bold
		public:
			void SetItalic(bool _enable);
			void SetBold(bool _enable);
			bool GetItalic(void) { return m_italic; };
			bool GetBold(void) { return m_bold; };
	};
	etk::CCout& operator <<(etk::CCout& _os, const appl::Colorize& _obj);
	
};

#endif


