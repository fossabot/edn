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
			// herited function
			const char * const GetObjectType(void) { return "EwolFileChooser"; };
			// herited function
			void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data);
			// herited function
			void OnObjectRemove(ewol::EObject * removeObject);
			/**
			 * @brief Add a Ctags item on the curent list
			 * @param[in] file Compleate file name
			 * @param[in] jump line id
			 */
			void AddCtagsNewItem(etk::UString file, int32_t line);
			
	};
};


#endif

