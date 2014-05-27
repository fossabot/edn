/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_RM_LINE_H__
#define __APPL_TEXT_RM_LINE_H__

#include <etk/types.h>
#include <ewol/object/Object.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPlugin.h>

namespace appl {
	class TextPluginRmLine : public appl::TextViewerPlugin {
		public:
			TextPluginRmLine();
			virtual ~TextPluginRmLine() {
				// nothing to do ...
			};
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveMessageViewer(appl::TextViewer& _textDrawer, const ewol::object::Message& _msg);
	};
};


#endif
