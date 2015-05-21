/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <appl/debug.h>
#include <ewol/widget/Composer.h>
#include <ewol/widget/Entry.h>
#include <appl/Buffer.h>
#include <appl/Gui/ViewerManager.h>
namespace appl {
	namespace widget {
		class Search : public ewol::widget::Composer {
			private:
				std::shared_ptr<appl::ViewerManager> m_viewerManager; //!< handle on the buffer manager
				bool m_forward;
				bool m_caseSensitive;
				bool m_wrap;
				std::shared_ptr<ewol::widget::Entry> m_searchEntry;
				std::shared_ptr<ewol::widget::Entry> m_replaceEntry;
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
};
#endif


