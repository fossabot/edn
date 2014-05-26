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
#include <ewol/object/Object.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPluginData.h>

namespace appl {
	class History {
		public:
			History() :
			  m_posAdded(0),
			  m_endPosAdded(0),
			  m_endPosRemoved(0) {
				
			};
			std::string m_addedText;
			std::string m_removedText;
			int64_t m_posAdded;
			int64_t m_endPosAdded;
			int64_t m_endPosRemoved;
	};
	class PluginHistoryData {
		public:
			std::vector<History*> m_undo; //!< History storing data
			std::vector<History*> m_redo; //!< History storing data
	};
	class TextPluginHistory : public appl::TextViewerPluginData<appl::PluginHistoryData> {
		public:
			TextPluginHistory();
			virtual ~TextPluginHistory() { };
		private:
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveMessage(appl::TextViewer& _textDrawer,
			                              const ewol::object::Message& _msg,
			                              appl::PluginHistoryData& _data);
			virtual bool onWrite(appl::TextViewer& _textDrawer,
			                     const appl::Buffer::Iterator& _pos,
			                     const std::string& _strData,
			                     appl::PluginHistoryData& _data);
			virtual bool onReplace(appl::TextViewer& _textDrawer,
			                       const appl::Buffer::Iterator& _pos,
			                       const std::string& _strData,
			                       const appl::Buffer::Iterator& _posEnd,
			                       appl::PluginHistoryData& _data);
			virtual bool onRemove(appl::TextViewer& _textDrawer,
			                      const appl::Buffer::Iterator& _pos,
			                      const appl::Buffer::Iterator& _posEnd,
			                      appl::PluginHistoryData& _data);
			virtual void remove(appl::PluginHistoryData& _data) {
				clearRedo(_data);
				clearUndo(_data);
			}
		private:
			void clearRedo(appl::PluginHistoryData& _data);
			void clearUndo(appl::PluginHistoryData& _data);
		public:
			virtual void onObjectRemove(const ewol::object::Shared<ewol::Object>& _object);
	};
};


#endif
