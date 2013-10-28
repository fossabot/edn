/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_GLYPH_PAINTING_H__
#define __APPL_GLYPH_PAINTING_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resources/Resource.h>
#include <appl/GlyphDecoration.h>

namespace appl {
	class GlyphPainting : public ewol::Resource {
		private:
			etk::Vector<appl::GlyphDecoration> m_list;
		protected:
			GlyphPainting(const etk::UString& _filename);
			virtual ~GlyphPainting(void);
		public:
			const char* getType(void) {
				return "appl::GlyphPainting";
			};
			/**
			 * @brief Load or reload data from config
			 */
			void reload(void);
			/**
			 * @brief Register a decoration with his name and get the ref id.
			 * @param[in] _name Name of the deco.
			 * @return id of the deco.
			 */
			esize_t request(const etk::UString& _name);
			/**
			 * @brief Get Decoration handle.
			 * @param[in] _id Id of the decoration.
			 * @return reference on deco.
			 */
			const appl::GlyphDecoration& get(esize_t _id) const {
				return m_list[_id];
			};
			/**
			 * @brief Operator to get decoration handle.
			 * @param[in] _pos Id of the decoration.
			 * @return reference on deco.
			 */
			const appl::GlyphDecoration& operator[] (esize_t _pos) const {
				return m_list[_pos];
			}
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static appl::GlyphPainting* keep(const etk::UString& _filename);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(appl::GlyphPainting*& _object);
	};
};

#endif

