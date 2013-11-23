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
#include <appl/Highlight.h>
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
		std::string getTypeExtention(const std::string& _extention);
		/**
		 * @brief Get filename with type.
		 * @param[in] _type Type name of the highlight.
		 * @return filename of the highlight.
		 */
		std::string getFileWithTypeType(const std::string& _type);
		/**
		 * @brief Get the list of extention type
		 * @return the requested list.
		 */
		std::vector<std::string> getTypeList(void);
	};
};


#endif

