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
			public: // derived function
				virtual void onReceiveMessage(const ewol::object::Message& _msg);
				virtual void onObjectRemove(const std::shared_ptr<ewol::Object>& _object);
		};
	};
};
#endif


