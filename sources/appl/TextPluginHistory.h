/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_PLUGIN_HISTORY_H__
#define __APPL_TEXT_PLUGIN_HISTORY_H__

#include <etk/types.h>
#include <ewol/renderer/EObject.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPlugin.h>

namespace appl {
	class History {
		public:
			History(void) :
			  m_posAdded(0),
			  m_endPosAdded(0),
			  m_endPosRemoved(0) {
				
			};
			etk::UString m_addedText;
			etk::UString m_removedText;
			esize_t m_posAdded;
			esize_t m_endPosAdded;
			esize_t m_endPosRemoved;
	};
	class TextPluginHistory : public appl::TextViewerPlugin {
		public:
			TextPluginHistory(void);
			~TextPluginHistory(void);
		private:
			etk::Vector<History*> m_undo; //!< History storing data
			etk::Vector<History*> m_redo; //!< History storing data
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveMessage(appl::TextViewer& _textDrawer,
			                              const ewol::EMessage& _msg);
			virtual bool onWrite(appl::TextViewer& _textDrawer,
			                     const appl::Buffer::Iterator& _pos,
			                     const etk::UString& _data);
			virtual bool onReplace(appl::TextViewer& _textDrawer,
			                       const appl::Buffer::Iterator& _pos,
			                       const etk::UString& _data,
			                       const appl::Buffer::Iterator& _posEnd);
			virtual bool onRemove(appl::TextViewer& _textDrawer,
			                      const appl::Buffer::Iterator& _pos,
			                      const appl::Buffer::Iterator& _posEnd);
		private:
			void clearRedo(void);
			void clearUndo(void);
		public:
			virtual void onObjectRemove(ewol::EObject* _removeObject);
	};
};


#endif
