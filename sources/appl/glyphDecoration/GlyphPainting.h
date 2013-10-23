/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __GLYPH_DECORATION_MANAGER_H__
#define __GLYPH_DECORATION_MANAGER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resources/Resource.h>
#include <appl/glyphDecoration/GlyphDecoration.h>

namespace appl {
	class GlyphPainting : public ewol::Resource {
		private:
			etk::Vector<appl::GlyphDecoration*> m_list;
		protected:
			GlyphPainting(const etk::UString& _filename);
			virtual ~GlyphPainting(void);
		public:
			const char* getType(void) {
				return "appl::GlyphPainting";
			};
			void reload(void);
			
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static appl::GlyphPainting* keep(const etk::UString& _filename = "GlyphPainting::default");
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(appl::GlyphPainting*& _object);
	};
};

#endif
*/
