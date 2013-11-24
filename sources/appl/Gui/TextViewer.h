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
#include <appl/Buffer.h>
#include <appl/globalMsg.h>

#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>
#include <appl/BufferManager.h>
#include <appl/Gui/ViewerManager.h>
#include <tuple>

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
		private:
			appl::GlyphPainting* m_paintingProperties; //!< element painting property
			esize_t m_colorBackground;
			esize_t m_colorSpace;
			esize_t m_colorTabulation;
			esize_t m_colorCursor;
			esize_t m_colorLineNumber;
			esize_t m_colorSelection;
			esize_t m_colorNormal;
		private:
			appl::BufferManager* m_bufferManager; //!< handle on the buffer manager
			appl::ViewerManager* m_viewerManager; //!< handle on the buffer manager
		public:
			TextViewer(const std::string& _fontName="", int32_t _fontSize=-1);
			virtual ~TextViewer(void);
		private:
			appl::Buffer* m_buffer; //!< pointer on the current buffer to display (can be null if the buffer is remover or in state of changing buffer)
			ewol::Text m_displayText; //!< Text display properties.
			ewol::Drawing m_displayDrawing; //!< Other diaplay requested.
			std::vector<std::pair<appl::Buffer*, vec2>> m_drawingRemenber;
		public:
			void setFontSize(int32_t _size);
			void setFontName(const std::string& _fontName);
		protected: // derived function
			virtual void onDraw(void);
		public:  // Derived function
			virtual bool calculateMinSize(void);
			virtual void onRegenerateDisplay(void);
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			virtual void onObjectRemove(ewol::EObject* _removeObject);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual bool onEventEntry(const ewol::EventEntry& _event);
			virtual void onEventClipboard(enum ewol::clipBoard::clipboardListe _clipboardID);
			virtual void onGetFocus(void);
			virtual void onLostFocus(void);
		private:
			float m_lastOffsetDisplay; //!< Line number ofssed in the display
		private:
			bool m_insertMode; //!< the insert mode is enable
		public:
			/**
			 * @brief Update the scrolling position from the cursor position,
			 * it might be be all time in the display screen.
			 */
			void updateScrolling(void);
			// TODO : Doc : write data on buffer
			bool moveCursor(const appl::Buffer::Iterator& _pos);
			bool write(const std::string& _data);
			bool write(const std::string& _data, const appl::Buffer::Iterator& _pos);
			bool replace(const std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd);
			bool replace(const std::string& _data);
			bool replace(const std::u32string& _data) {
				return replace(to_u8string(_data));
			}
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
			void moveCursorUp(uint32_t _nbLine);
			/**
			 * @brief Move the cursor at an other position under.
			 * @param[in] _nbLine number of down line that might be moved
			 */
			void moveCursorDown(uint32_t _nbLine);
			
			appl::Buffer::Iterator getPosSize(const appl::Buffer::Iterator& _startLinePos, float _distance);
			float getScreenSize(const appl::Buffer::Iterator& _startLinePos, const appl::Buffer::Iterator& _stopPos);
		private:
			/**
			 * @brief Set the current buffer selected
			 */
			void setCurrentSelect(void);
			/**
			 * @brief Check if the current buffer is last selected
			 * @return true if selected last
			 */
			bool isSelectedLast(void);
		public:
			/**
			 * @brief Check if the buffer is availlable
			 * @return true if a display buffer is present, false otherwise.
			 */
			virtual bool hasBuffer(void) {
				return m_buffer != NULL;
			}
			/**
			 * @brief Get the status of selection.
			 * @return true if we have a current selection, false otherwise.
			 */
			virtual bool hasTextSelected(void) {
				if (m_buffer==NULL) {
					return false;
				}
				return m_buffer->hasTextSelected();
			}
			/**
			 * @brief Remove Selection of the buffer.
			 */
			virtual void unSelect(void) {
				if (m_buffer==NULL) {
					return;
				}
				m_buffer->unSelect();
			}
			/**
			 * @brief Select a section of text.
			 * @param[in] _start Start position of the selection
			 * @param[in] _stop Stop position of the selection (the curor is set at this position)
			 */
			virtual void select(appl::Buffer::Iterator& _start, appl::Buffer::Iterator& _stop) {
				if (m_buffer==NULL) {
					return;
				}
				moveCursor(_stop);
				m_buffer->setSelectionPos(_start);
			}
			/**
			 * @brief Find a string in the buffer.
			 * @param[in] _pos Position to start the search of the element.
			 * @param[in] _search String to search.
			 * @param[out] _resultStart Find element start position.
			 * @param[out] _resultStop Find element end position.
			 * @param[in] _caseSensitive (optional) Search making attention of the case [default true]
			 * @return true if pos if fined.
			 */
			virtual bool find(const appl::Buffer::Iterator& _pos,
			                  const std::u32string& _search,
			                  appl::Buffer::Iterator& _resultStart,
			                  appl::Buffer::Iterator& _resultStop,
			                  bool _caseSensitive = true) {
				if (m_buffer==NULL) {
					return false;
				}
				bool ret = m_buffer->search(_pos, _search, _resultStart, _caseSensitive);
				if (ret == true) {
					_resultStop = _resultStart + _search.size();
				}
				return ret;
			}
			/**
			 * @brief revers find a string in the buffer.
			 * @param[in] _pos Position to start the search of the element.
			 * @param[in] _search String to search.
			 * @param[out] _resultStart Find element start position.
			 * @param[out] _resultStop Find element end position.
			 * @param[in] _caseSensitive (optional) Search making attention of the case [default true]
			 * @return true if pos if fined.
			 */
			virtual bool rfind(const appl::Buffer::Iterator& _pos,
			                   const std::u32string& _search,
			                   appl::Buffer::Iterator& _resultStart,
			                   appl::Buffer::Iterator& _resultStop,
			                   bool _caseSensitive = true) {
				if (m_buffer==NULL) {
					return false;
				}
				bool ret = m_buffer->searchBack(_pos, _search, _resultStart, _caseSensitive);
				if (ret == true) {
					_resultStop = _resultStart + _search.size();
				}
				return ret;
			}
			/**
			 * @brief Get the cursor position.
			 * @return The iterator on the cursor position
			 */
			appl::Buffer::Iterator cursor(void) {
				if (m_buffer==NULL) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->cursor();
			}
			/**
			 * @brief Get the begin position.
			 * @return The iterator on the begin position
			 */
			appl::Buffer::Iterator begin(void) {
				if (m_buffer==NULL) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->begin();
			}
			/**
			 * @brief Get the end position.
			 * @return The iterator on the end position
			 */
			appl::Buffer::Iterator end(void) {
				if (m_buffer==NULL) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->end();
			}
			/**
			 * @brief Get an Iterator on the start selection.
			 * @return The Iterator
			 */
			appl::Buffer::Iterator selectStart(void) {
				if (m_buffer==NULL) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->selectStart();
			}
			/**
			 * @brief Get an Iterator on the stop selection.
			 * @return The Iterator
			 */
			appl::Buffer::Iterator selectStop(void) {
				if (m_buffer==NULL) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->selectStop();
			}
			/**
			 * @brief get the start of a line with the position in the buffer.
			 * @param[in] _pos position in the buffer.
			 * @return The position in the buffer of the start of the line.
			 */
			appl::Buffer::Iterator getStartLine(const appl::Buffer::Iterator& _pos) {
				if (m_buffer==NULL) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->getStartLine(_pos);
			}
			/**
			 * @brief get the end of a line with the position in the buffer.
			 * @param[in] _pos position in the buffer.
			 * @return The position in the buffer of the end of the line.
			 */
			appl::Buffer::Iterator getEndLine(const appl::Buffer::Iterator& _pos) {
				if (m_buffer==NULL) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->getEndLine(_pos);
			}
			/**
			 * @brief Register of the arrival of a Multicast message
			 * @param[in] _messageId Event Id waiting for...
			 */
			void ext_registerMultiCast(const char* const _messageId) {
				registerMultiCast(_messageId);
			}
			/**
			 * @brief add a specific shortcut with his description
			 * @param[in] _descriptiveString Description string of the shortcut
			 * @param[in] _generateEventId Event generic of the element
			 * @param[in] _data Associate data wit the event
			 */
			virtual void ext_shortCutAdd(const char * _descriptiveString,
			                         const char * _generateEventId,
			                         std::string _data="",
			                         bool _broadcast=false) {
				shortCutAdd(_descriptiveString, _generateEventId, _data, _broadcast);
			}
	};
};

#endif

