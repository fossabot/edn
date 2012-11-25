/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_CTAGS_SELECTION_H__
#define __APPL_CTAGS_SELECTION_H__

#include <appl/Debug.h>
#include <ewol/widget/PopUp.h>
#include <appl/Gui/TagFileList.h>

extern const char * const applEventctagsSelection;
extern const char * const applEventctagsCancel;

namespace appl {
	class TagFileSelection : public widget::PopUp
	{
		private:
			appl::TagFileList*   m_listTag;
			etk::UString         m_eventNamed;
		public:
			TagFileSelection(void);
			virtual ~TagFileSelection(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			const char * const GetObjectType(void) { return "EwolFileChooser"; };
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 * @return ---
			 */
			void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			void OnObjectRemove(ewol::EObject * removeObject);
			/**
			 * @brief Add a Ctags item on the curent list
			 * @param[in] file Compleate file name
			 * @param[in] jump line id
			 * @return ---
			 */
			void AddCtagsNewItem(etk::UString file, int32_t line);
			
	};
};


#endif

