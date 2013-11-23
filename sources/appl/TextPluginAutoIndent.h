/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_PLUGIN_AUTO_INDENT_H__
#define __APPL_TEXT_PLUGIN_AUTO_INDENT_H__

#include <etk/types.h>
#include <ewol/renderer/EObject.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPlugin.h>

namespace appl {
	class TextPluginAutoIndent : public appl::TextViewerPlugin {
		public:
			TextPluginAutoIndent(void);
			~TextPluginAutoIndent(void) {
				// nothing to do ...
			};
		public:
			virtual bool onEventEntry(appl::TextViewer& _textDrawer,
			                          const ewol::EventEntry& _event);
	};
};


#endif