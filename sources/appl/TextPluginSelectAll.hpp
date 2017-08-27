/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <etk/types.hpp>
#include <ewol/object/Object.hpp>
#include <appl/Gui/TextViewer.hpp>
#include <ewol/compositing/Text.hpp>
#include <appl/TextPlugin.hpp>

namespace appl {
	class TextPluginSelectAll : public appl::TextViewerPlugin {
		protected:
			TextPluginSelectAll();
			int32_t m_menuIdTitle;
			int32_t m_menuIdSelectAll;
			int32_t m_menuIdSelectNone;
		public:
			DECLARE_FACTORY(TextPluginSelectAll);
			virtual ~TextPluginSelectAll() {
				// nothing to do ...
			};
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveShortCut(appl::TextViewer& _textDrawer, const etk::String& _shortCutName);
	};
}

