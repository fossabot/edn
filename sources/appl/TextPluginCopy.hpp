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
	class TextPluginCopy : public appl::TextViewerPlugin {
		private:
			int32_t m_menuIdTitle;
			int32_t m_menuIdCopy;
			int32_t m_menuIdCut;
			int32_t m_menuIdPast;
			int32_t m_menuIdRemove;
		protected:
			TextPluginCopy();
		public:
			DECLARE_FACTORY(TextPluginCopy);
			virtual ~TextPluginCopy() {
				// nothing to do ...
			};
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveShortCut(appl::TextViewer& _textDrawer, const std::string& _shortCutName);
	};
}

