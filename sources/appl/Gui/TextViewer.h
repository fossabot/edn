/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_VIEWER_H__
#define __APPL_TEXT_VIEWER_H__

#include <appl/debug.h>
#include <appl/Buffer/Buffer.h>
#include <appl/globalMsg.h>

#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>

namespace appl {
	class TextViewerPlugin;
	class TextPluginCopy;
	class TextPluginMultiLineTab;
	class TextPluginAutoIndent;
	class TextPluginHistory;
	class TextViewer : public widget::WidgetScrooled {
		friend class appl::TextViewerPlugin;
		friend class appl::TextPluginCopy;
		friend class appl::TextPluginMultiLineTab;
		friend class appl::TextPluginAutoIndent;
		friend class appl::TextPluginHistory;
		public:
			TextViewer(const etk::UString& _fontName="", int32_t _fontSize=-1);
			virtual ~TextViewer(void);
		private:
			appl::Buffer* m_buffer; //!< pointer on the current buffer to display (can be null if the buffer is remover or in state of changing buffer)
			ewol::Text m_displayText; //!< Text display properties.
			ewol::Drawing m_displayDrawing; //!< Other diaplay requested.
		public:
			void setFontSize(int32_t _size);
			void setFontName(const etk::UString& _fontName);
		protected: // derived function
			virtual void onDraw(void);
		public:  // Derived function
			const char * const getObjectType(void) { return "appl::TextViewer"; };
			virtual bool calculateMinSize(void);
			virtual void onRegenerateDisplay(void);
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual bool onEventEntry(const ewol::EventEntry& _event);
			virtual void onEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID);
			virtual void onGetFocus(void);
			virtual void onLostFocus(void);
		private:
			float m_lastOffsetDisplay; //!< Line number ofssed in the display
		private:
			bool m_insertMode; //!< the insert mode is enable
		public:
			
		public:
			// TODO : Doc : write data on buffer
			bool moveCursor(const appl::Buffer::Iterator& _pos);
			bool write(const etk::UString& _data);
			bool write(const etk::UString& _data, const appl::Buffer::Iterator& _pos);
			bool replace(const etk::UString& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd);
			bool replace(const etk::UString& _data);
			void remove(void);
			
			
			appl::Buffer::Iterator getMousePosition(const vec2& _relativePos);
			void mouseEventDouble(void);
			void mouseEventTriple(void);
		private:
			enum moveMode {
				moveLetter,
				moveWord,
				moveEnd
			};
			/**
			 * Move the cursor right in the line (no stop of a new line)
			 * @param[in] _mode Moving mode char, word, ...
			 */
			void moveCursorRight(moveMode _mode = moveLetter);
			/**
			 * Move the cursor left in the line (no stop of a new line)
			 * @param[in] _mode Moving mode char, word, ...
			 */
			void moveCursorLeft(moveMode _mode = moveLetter);
			/**
			 * @brief Move the cursor at an other position upper.
			 * @param[in] _nbLine number of up line that might be moved
			 */
			void moveCursorUp(esize_t _nbLine);
			/**
			 * @brief Move the cursor at an other position under.
			 * @param[in] _nbLine number of down line that might be moved
			 */
			void moveCursorDown(esize_t _nbLine);
			
			appl::Buffer::Iterator getPosSize(const appl::Buffer::Iterator& _startLinePos, float _distance);
			float getScreenSize(const appl::Buffer::Iterator& _startLinePos, const appl::Buffer::Iterator& _stopPos);
	};
};

#endif

