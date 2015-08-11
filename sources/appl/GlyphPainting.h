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
#include <gale/resource/Resource.h>
#include <appl/GlyphDecoration.h>

namespace appl {
	class GlyphPainting : public gale::Resource {
		private:
			std::vector<appl::GlyphDecoration> m_list;
		protected:
			GlyphPainting();
			void init(const std::string& _filename);
		public:
			DECLARE_RESOURCE_NAMED_FACTORY(GlyphPainting);
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
	};
};

#endif

