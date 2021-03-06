/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <appl/debug.h>
#include <ewol/widget/PopUp.h>
#include <appl/Gui/TagFileList.h>

namespace appl {
	class TagFileSelection : public ewol::widget::PopUp {
		public:
			esignal::Signal<std::string> signalSelect;
			esignal::Signal<> signalCancel;
		private:
			ememory::SharedPtr<appl::TagFileList> m_listTag;
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
}

