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
#include <appl/ctags/readtags.h>

// create ctags file : "ctags-exuberant --fields=n -R"
// --fields=n add the line number needed for this software version ..

namespace appl {
	class TextPluginCtags : public appl::TextViewerPlugin {
		private:
			// Global plugin data (not specific on buffer :
			/*
			std::vector<std::pair<std::string, int64_t>> m_historyList;
			*/
			std::string m_tagFolderBase;
			std::string m_tagFilename;
			tagFile* m_ctagFile;
			void loadTagFile();
			void printTag(const tagEntry *_entry);
			void jumpTo(const std::string& _name);
			void jumpFile(const std::string& _filename, int64_t _lineId);
			ememory::SharedPtr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		protected:
			TextPluginCtags();
		public:
			DECLARE_FACTORY(TextPluginCtags);
			virtual ~TextPluginCtags();
		public:
			virtual void onPluginEnable(appl::TextViewer& _textDrawer);
			virtual void onPluginDisable(appl::TextViewer& _textDrawer);
			virtual bool onReceiveShortCut(appl::TextViewer& _textDrawer,
			                               const std::string& _shortCutName);
			// callback function:
			void onCallbackOpenCtagsOpenFileReturn(const std::string& _value);
			void onCallbackOpenCtagsSelectReturn(const std::string& _value);
	};
}

