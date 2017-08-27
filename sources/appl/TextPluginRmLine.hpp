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
	class TextPluginRmLine : public appl::TextViewerPlugin {
		protected:
			TextPluginRmLine();
		public:
			DECLARE_FACTORY(TextPluginRmLine);
			virtual ~TextPluginRmLine() {
				// nothing to do ...
			};
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveShortCut(appl::TextViewer& _textDrawer, const etk::String& _shortCutName);
	};
}

