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
	class textPluginManager : public gale::Resource {
		private:
			ememory::WeakPtr<appl::TextViewer> m_currentViewer;
			std::list<ememory::SharedPtr<appl::TextViewerPlugin>> m_list;
			etk::Vector<ememory::SharedPtr<appl::TextViewerPlugin>> m_listOnEventEntry;
			etk::Vector<ememory::SharedPtr<appl::TextViewerPlugin>> m_listOnEventInput;
			etk::Vector<ememory::SharedPtr<appl::TextViewerPlugin>> m_listOnWrite;
			etk::Vector<ememory::SharedPtr<appl::TextViewerPlugin>> m_listOnReplace;
			etk::Vector<ememory::SharedPtr<appl::TextViewerPlugin>> m_listOnRemove;
			etk::Vector<ememory::SharedPtr<appl::TextViewerPlugin>> m_listOnReceiveShortCutViewer;
			etk::Vector<ememory::SharedPtr<appl::TextViewerPlugin>> m_listOnCursorMove;
		protected:
			textPluginManager();
			void init(const etk::String& _name);
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
			void addPlugin(ememory::SharedPtr<appl::TextViewerPlugin> _plugin);
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
			             const etk::String& _data);
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
			               const etk::String& _data,
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
			                       const etk::String& _shortCutName);
			/**
			 * @brief Called when Cursor move of position.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos New cursor position.
			 * @return true if the event might not propagate anymore
			 */
			bool onCursorMove(appl::TextViewer& _textDrawer,
			                  const appl::Buffer::Iterator& _pos);
	};
}

