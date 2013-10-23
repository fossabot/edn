/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __HIGHLIGHT_MANAGER_H__
#define __HIGHLIGHT_MANAGER_H__


#include <etk/UString.h>
#include <appl/globalMsg.h>
#include <Highlight.h>
#include <ewol/widget/Widget.h>

namespace appl {
	namespace highlightManager {
		/**
		 * @brief Init the Highlight manager
		 */
		void init(void);
		/**
		 * @brief Un-Init the Highlight manager
		 */
		void unInit(void);
		/**
		 * @brief Un-Init the Highlight manager
		 * @param[in] extention of the file
		 * @return type of highlight
		 */
		etk::UString getTypeExtention(const etk::UString& _extention);
		/**
		 * @brief Get the list of extention type
		 * @return the requested list.
		 */
		etk::Vector<etk::UString> getTypeList(void);
	};
};


#endif

