/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_CTAGS_SELECTION_H__
#define __APPL_CTAGS_SELECTION_H__

#include <appl/debug.h>
#include <ewol/widget/PopUp.h>
#include <appl/Gui/TagFileList.h>

namespace appl {
	class TagFileSelection : public ewol::widget::PopUp {
		public:
			ewol::object::Signal<std::string> signalSelect;
			ewol::object::Signal<void> signalCancel;
		private:
			std::shared_ptr<appl::TagFileList> m_listTag;
			std::string m_eventNamed;
		public:
			TagFileSelection();
			void init();
		public:
			DECLARE_FACTORY(TagFileSelection);
			virtual ~TagFileSelection();
			/**
			 * @brief add a Ctags item on the curent list
			 * @param[in] file Compleate file name
			 * @param[in] jump line id
			 */
			void addCtagsNewItem(std::string file, int32_t line);
		public: // callback function
			void onCallbackCtagsSelection();
			void onCallbackCtagsCancel();
			void onCallbackCtagsListValidate(const std::string& _value);
			void onCallbackCtagsListSelect(const std::string& _value);
			void onCallbackCtagsListUnSelect();
	};
};


#endif

