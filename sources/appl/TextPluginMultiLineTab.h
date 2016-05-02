/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <etk/types.h>
#include <ewol/object/Object.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPlugin.h>

namespace appl {
	class TextPluginMultiLineTab : public appl::TextViewerPlugin {
		protected:
			TextPluginMultiLineTab();
		public:
			DECLARE_FACTORY(TextPluginMultiLineTab);
			virtual ~TextPluginMultiLineTab() {
				// nothing to do ...
			};
		public:
			virtual bool onEventEntry(appl::TextViewer& _textDrawer,
			                          const ewol::event::Entry& _event);
	};
}

