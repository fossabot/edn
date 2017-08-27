/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <appl/debug.hpp>
#include <ewol/widget/PopUp.hpp>
#include <appl/Gui/TagFileList.hpp>

namespace appl {
	class TagFileSelection : public ewol::widget::PopUp {
		public:
			esignal::Signal<etk::String> signalSelect;
			esignal::Signal<> signalCancel;
		private:
			ememory::SharedPtr<appl::TagFileList> m_listTag;
			etk::String m_eventNamed;
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
			void addCtagsNewItem(etk::String file, int32_t line);
		public: // callback function
			void onCallbackCtagsSelection();
			void onCallbackCtagsCancel();
			void onCallbackCtagsListValidate(const etk::String& _value);
			void onCallbackCtagsListSelect(const etk::String& _value);
			void onCallbackCtagsListUnSelect();
	};
}

