/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_PLUGIN_MANAGER_H__
#define __APPL_TEXT_PLUGIN_MANAGER_H__

#include <etk/types.h>
#include <ewol/object/Object.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>
#include <appl/TextPlugin.h>

namespace appl {
	class textPluginManager : public ewol::Resource {
		private:
			std::weak_ptr<appl::TextViewer> m_currentViewer;
			std::list<std::shared_ptr<appl::TextViewerPlugin>> m_list;
			std::vector<std::shared_ptr<appl::TextViewerPlugin>> m_listOnEventEntry;
			std::vector<std::shared_ptr<appl::TextViewerPlugin>> m_listOnEventInput;
			std::vector<std::shared_ptr<appl::TextViewerPlugin>> m_listOnWrite;
			std::vector<std::shared_ptr<appl::TextViewerPlugin>> m_listOnReplace;
			std::vector<std::shared_ptr<appl::TextViewerPlugin>> m_listOnRemove;
			std::vector<std::shared_ptr<appl::TextViewerPlugin>> m_listOnReceiveShortCutViewer;
			std::vector<std::shared_ptr<appl::TextViewerPlugin>> m_listOnCursorMove;
		protected:
			textPluginManager();
			void init(const std::string& _name);
		public:
			DECLARE_RESOURCE_SINGLE_FACTORY(textPluginManager, "plugin-Manager");
			virtual ~textPluginManager() {};
			/**
			 * @brief Add default plugin list
			 */
			void addDefaultPlugin();
			/**
			 * @brief Add a plugin.
			 * @param[in] _plugin Plugin pointer to add.
			 */
			void addPlugin(const std::shared_ptr<appl::TextViewerPlugin>& _plugin);
			/**
			 * @brief connect a new widget to the plugin.
			 * @param[in] _widget Reference on the widget caller.
			 */
			void connect(appl::TextViewer& _widget);
			/**
			 * @brief dis-connect a new widget to the plugin.
			 * @param[in] _widget Reference on the widget caller.
			 */
			void disconnect(appl::TextViewer& _widget);
			/**
			 * @brief On entry event call.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _event Generic event.
			 * @return true if the event might not propagate anymore.
			 */
			bool onEventEntry(appl::TextViewer& _widget,
			                  const ewol::event::Entry& _event);
			/**
			 * @brief On Input event call.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _event Generic event.
			 * @return true if the event might not propagate anymore
			 */
			bool onEventInput(appl::TextViewer& _textDrawer,
			                  const ewol::event::Input& _event);
			/**
			 * @brief Called when data is written in the buffer.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos Position in the buffer where data might be witten.
			 * @param[in] _data Input stream written.
			 * @return true if the event might not propagate anymore
			 */
			bool onWrite(appl::TextViewer& _textDrawer,
			             const appl::Buffer::Iterator& _pos,
			             const std::string& _data);
			/**
			 * @brief Called when data is written in the buffer, and some are removed.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos Position in the buffer where data might be witten.
			 * @param[in] _data Input stream written.
			 * @param[in] _posEnd end replace position.
			 * @return true if the event might not propagate anymore
			 */
			bool onReplace(appl::TextViewer& _textDrawer,
			               const appl::Buffer::Iterator& _pos,
			               const std::string& _data,
			               const appl::Buffer::Iterator& _posEnd);
			/**
			 * @brief Called when data is removed.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos Position in the buffer where data might be witten.
			 * @param[in] _posEnd end replace position.
			 * @return true if the event might not propagate anymore
			 */
			bool onRemove(appl::TextViewer& _textDrawer,
			              const appl::Buffer::Iterator& _pos,
			              const appl::Buffer::Iterator& _posEnd);
			/**
			 * @brief Called when a message arrive.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _shortCutName shortcut properties.
			 * @return true if the event might not propagate anymore
			 */
			bool onReceiveShortCut(appl::TextViewer& _textDrawer,
			                       const std::string& _shortCutName);
			/**
			 * @brief Called when Cursor move of position.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos New cursor position.
			 * @return true if the event might not propagate anymore
			 */
			bool onCursorMove(appl::TextViewer& _textDrawer,
			                  const appl::Buffer::Iterator& _pos);
	};
};


#endif
