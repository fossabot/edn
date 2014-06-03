/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_PLUGIN_H__
#define __APPL_TEXT_PLUGIN_H__

#include <etk/types.h>
#include <ewol/object/Object.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/compositing/Text.h>

namespace appl {
	class TextViewerPlugin : public ewol::Object {
		friend class appl::TextViewer;
		public:
			TextViewerPlugin();
			virtual ~TextViewerPlugin();
		private:
			bool m_isEnable; //!< The plugin is enable or not (for all viewer).
		public:
			/**
			 * @brief Set activity status of the plugin
			 * @param[in] _status new activity status
			 */
			void setEnableStatus(bool _status);
			/**
			 * @brief Get the activity status.
			 * @return true if the plugin is active, false otherwise.
			 */
			bool isEnable() {
				return m_isEnable;
			};
		public:
			/**
			 * @brief On plugin global enable.
			 */
			virtual void onPluginEnable() {
				// nothing to do here ...
			};
			/**
			 * @brief On plugin global disable.
			 */
			virtual void onPluginDisable() {
				// nothing to do here ...
			};
			/**
			 * @brief On plugin enable on a specific text viewer.
			 * @param[in] _widget Reference on the widget caller.
			 */
			virtual void onPluginEnable(appl::TextViewer& _textDrawer) {
				// nothing to do here ...
			};
			/**
			 * @brief On plugin disable on a specific text viewer.
			 * @param[in] _widget Reference on the widget caller.
			 */
			virtual void onPluginDisable(appl::TextViewer& _textDrawer) {
				// nothing to do here ...
			};
		protected:
			bool m_activateOnEventEntry; //!< onEventEntry is availlable for this plugin.
		public:
			/**
			 * @brief Get the availlability of a callback
			 * @return true if availlable
			 */
			bool isAvaillableOnEventEntry() {
				return m_activateOnEventEntry;
			}
			/**
			 * @brief On entry event call.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _event Generic event.
			 * @return true if the event might not propagate anymore.
			 */
			virtual bool onEventEntry(appl::TextViewer& _textDrawer,
			                          const ewol::event::Entry& _event) {
				return false;
			};
		protected:
			bool m_activateOnEventInput; //!< onEventInput is availlable for this plugin.
		public:
			/**
			 * @brief Get the availlability of a callback
			 * @return true if availlable
			 */
			bool isAvaillableOnEventInput() {
				return m_activateOnEventInput;
			}
			/**
			 * @brief On Input event call.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _event Generic event.
			 * @return true if the event might not propagate anymore
			 */
			virtual bool onEventInput(appl::TextViewer& _textDrawer,
			                          const ewol::event::Input& _event) {
				return false;
			};
		protected:
			bool m_activateOnWrite; //!< onWrite is availlable for this plugin.
		public:
			/**
			 * @brief Get the availlability of a callback
			 * @return true if availlable
			 */
			bool isAvaillableOnWrite() {
				return m_activateOnWrite;
			}
			/**
			 * @brief Called when data is written in the buffer.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos Position in the buffer where data might be witten.
			 * @param[in] _data Input stream written.
			 * @return true if the event might not propagate anymore
			 */
			virtual bool onWrite(appl::TextViewer& _textDrawer,
			                     const appl::Buffer::Iterator& _pos,
			                     const std::string& _data) {
				return false;
			};
		protected:
			bool m_activateOnReplace; //!< onReplace is availlable for this plugin.
		public:
			/**
			 * @brief Get the availlability of a callback
			 * @return true if availlable
			 */
			bool isAvaillableOnReplace() {
				return m_activateOnReplace;
			}
			/**
			 * @brief Called when data is written in the buffer, and some are removed.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos Position in the buffer where data might be witten.
			 * @param[in] _data Input stream written.
			 * @param[in] _posEnd end replace position.
			 * @return true if the event might not propagate anymore
			 */
			virtual bool onReplace(appl::TextViewer& _textDrawer,
			                       const appl::Buffer::Iterator& _pos,
			                       const std::string& _data,
			                       const appl::Buffer::Iterator& _posEnd) {
				return false;
			};
		protected:
			bool m_activateOnRemove; //!< onRemove is availlable for this plugin.
		public:
			/**
			 * @brief Get the availlability of a callback
			 * @return true if availlable
			 */
			bool isAvaillableOnRemove() {
				return m_activateOnRemove;
			}
			/**
			 * @brief Called when data is removed.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos Position in the buffer where data might be witten.
			 * @param[in] _posEnd end replace position.
			 * @return true if the event might not propagate anymore
			 */
			virtual bool onRemove(appl::TextViewer& _textDrawer,
			                      const appl::Buffer::Iterator& _pos,
			                      const appl::Buffer::Iterator& _posEnd) {
				return false;
			};
		protected:
			bool m_activateOnReceiveMessage; //!< onReceiveMessage is availlable for this plugin.
		public:
			/**
			 * @brief Get the availlability of a callback
			 * @return true if availlable
			 */
			bool isAvaillableOnReceiveMessage() {
				return m_activateOnReceiveMessage;
			}
			/**
			 * @brief Called when a message arrive.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _msg Generic message.
			 * @return true if the event might not propagate anymore
			 */
			virtual bool onReceiveMessageViewer(appl::TextViewer& _textDrawer,
			                                    const ewol::object::Message& _msg) {
				return false;
			}
		protected:
			bool m_activateOnCursorMove; //!< onCursorMove is availlable for this plugin.
		public:
			/**
			 * @brief Get the availlability of a callback
			 * @return true if availlable
			 */
			bool isAvaillableOnCursorMove() {
				return m_activateOnCursorMove;
			}
			/**
			 * @brief Called when Cursor move of position.
			 * @param[in] _widget Reference on the widget caller.
			 * @param[in] _pos New cursor position.
			 * @return true if the event might not propagate anymore
			 */
			virtual bool onCursorMove(appl::TextViewer& _textDrawer,
			                          const appl::Buffer::Iterator& _pos) {
				return false;
			}
	};
};


#endif
