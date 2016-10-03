/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <appl/debug.hpp>
#include <ewol/widget/Composer.hpp>
#include <ewol/widget/Entry.hpp>
#include <appl/Buffer.hpp>
#include <appl/Gui/ViewerManager.hpp>
namespace appl {
	namespace widget {
		class Search;
		using SearchShared = ememory::SharedPtr<appl::widget::Search>;
		using SearchWeak = ememory::WeakPtr<appl::widget::Search>;
		class Search : public ewol::widget::Composer {
			private:
				ememory::SharedPtr<appl::ViewerManager> m_viewerManager; //!< handle on the buffer manager
				bool m_forward;
				bool m_caseSensitive;
				bool m_wrap;
				ememory::SharedPtr<ewol::widget::Entry> m_searchEntry;
				ememory::SharedPtr<ewol::widget::Entry> m_replaceEntry;
				std::u32string m_searchData;
				std::u32string m_replaceData;
			protected:
				// Constructeur
				Search();
				void init();
			public:
				DECLARE_FACTORY(Search);
				virtual ~Search();
			private:
				/**
				 * @brief Find the next element that corespond at the search
				 */
				void find();
				/**
				 * @brief Replace the current selected text.
				 */
				void replace();
			public:
				void selectSearch();
				bool isSelectSearch();
				void selectReplace();
				bool isSelectReplace();
			private: // callback functions
				void OnCallbackHide();
				void OnCallbackSearchValue(const std::string& _value);
				void OnCallbackSearch();
				void OnCallbackSearchEntryValidate(const std::string& _value);
				void OnCallbackReplaceValue(const std::string& _value);
				void OnCallbackReplace();
				void OnCallbackReplaceEntryValidate(const std::string& _value);
				void OnCallbackCase(const bool& _value);
				void OnCallbackWrap(const bool& _value);
				void OnCallbackForward(const bool& _value);
		};
	};
}

