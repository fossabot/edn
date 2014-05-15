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
#include <ewol/resource/Resource.h>
#include <appl/GlyphDecoration.h>

namespace appl {
	class GlyphPainting : public ewol::Resource {
		private:
			std::vector<appl::GlyphDecoration> m_list;
		protected:
			GlyphPainting(const std::string& _filename);
			virtual ~GlyphPainting();
		public:
			/**
			 * @brief Load or reload data from config
			 */
			void reload();
			/**
			 * @brief Register a decoration with his name and get the ref id.
			 * @param[in] _name Name of the deco.
			 * @return id of the deco.
			 */
			int32_t request(const std::string& _name);
			/**
			 * @brief Get Decoration handle.
			 * @param[in] _id Id of the decoration.
			 * @return reference on deco.
			 */
			const appl::GlyphDecoration& get(int32_t _id) const {
				return m_list[_id];
			};
			/**
			 * @brief Operator to get decoration handle.
			 * @param[in] _pos Id of the decoration.
			 * @return reference on deco.
			 */
			const appl::GlyphDecoration& operator[] (int32_t _pos) const {
				return m_list[_pos];
			}
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static appl::GlyphPainting* keep(const std::string& _filename);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(appl::GlyphPainting*& _object);
	};
};

#endif

