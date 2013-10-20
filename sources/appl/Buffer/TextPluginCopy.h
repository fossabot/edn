/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_PLUGIN_COPY_H__
#define __APPL_TEXT_PLUGIN_COPY_H__

#include <etk/types.h>
#include <ewol/renderer/EObject.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/Buffer/TextPlugin.h>

namespace appl {
	class TextPluginCopy : public appl::TextViewerPlugin {
		public:
			TextPluginCopy(void);
			~TextPluginCopy(void) {
				// nothing to do ...
			};
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveMessage(appl::TextViewer& _textDrawer, const ewol::EMessage& _msg);
	};
};


#endif
