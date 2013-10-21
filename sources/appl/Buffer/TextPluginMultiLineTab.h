/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_PLUGIN_MULTI_LINE_TAB_H__
#define __APPL_TEXT_PLUGIN_MULTI_LINE_TAB_H__

#include <etk/types.h>
#include <ewol/renderer/EObject.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/Buffer/TextPlugin.h>

namespace appl {
	class TextPluginMultiLineTab : public appl::TextViewerPlugin {
		public:
			TextPluginMultiLineTab(void);
			~TextPluginMultiLineTab(void) {
				// nothing to do ...
			};
		public:
			virtual bool onEventEntry(appl::TextViewer& _textDrawer,
			                          const ewol::EventEntry& _event);
	};
};


#endif

