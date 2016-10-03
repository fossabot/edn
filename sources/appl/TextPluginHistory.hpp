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
#include <appl/TextPluginData.hpp>

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
		private:
			int32_t m_menuIdTitle;
			int32_t m_menuIdUndo;
			int32_t m_menuIdRedo;
		protected:
			TextPluginHistory();
		public:
			DECLARE_FACTORY(TextPluginHistory);
			virtual ~TextPluginHistory() { };
		private:
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onDataReceiveShortCut(appl::TextViewer& _textDrawer,
			                                   const std::string& _shortCutName,
			                                   appl::PluginHistoryData& _data);
			virtual bool onDataWrite(appl::TextViewer& _textDrawer,
			                         const appl::Buffer::Iterator& _pos,
			                         const std::string& _strData,
			                         appl::PluginHistoryData& _data);
			virtual bool onDataReplace(appl::TextViewer& _textDrawer,
			                           const appl::Buffer::Iterator& _pos,
			                           const std::string& _strData,
			                           const appl::Buffer::Iterator& _posEnd,
			                           appl::PluginHistoryData& _data);
			virtual bool onDataRemove(appl::TextViewer& _textDrawer,
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
	};
}

