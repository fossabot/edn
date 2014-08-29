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
#include <ewol/compositing/TextDF.h>
#include <ewol/compositing/Drawing.h>
#include <appl/BufferManager.h>
#include <appl/Gui/ViewerManager.h>
#include <utility>

namespace appl {
	class TextViewer : public ewol::widget::WidgetScrolled {
		private:
			std::shared_ptr<appl::GlyphPainting> m_paintingProperties; //!< element painting property
			int32_t m_colorBackground;
			int32_t m_colorSpace;
			int32_t m_colorTabulation;
			int32_t m_colorCursor;
			int32_t m_colorLineNumber;
			int32_t m_colorSelection;
			int32_t m_colorNormal;
		private:
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
			std::shared_ptr<appl::ViewerManager> m_viewerManager; //!< handle on the buffer manager
		protected:
			TextViewer();
			void init(const std::string& _fontName="", int32_t _fontSize=-1);
		public:
			DECLARE_FACTORY(TextViewer);
			virtual ~TextViewer();
		private:
			std::shared_ptr<appl::Buffer> m_buffer; //!< pointer on the current buffer to display (can be null if the buffer is remover or in state of changing buffer)
		public:
			/**
			 * @brief Get the buffer property (only for the class : template <typename TYPE> class TextViewerPluginData)
			 * @return pointer on buffer
			 */
			std::shared_ptr<appl::Buffer> internalGetBuffer() {
				return m_buffer;
			}
		private:
			ewol::compositing::Text m_displayText; //!< Text display properties.
			ewol::compositing::Drawing m_displayDrawing; //!< Other diaplay requested.
			std::vector<std::pair<std::shared_ptr<appl::Buffer>, vec2>> m_drawingRemenber;
		public:
			void setFontSize(int32_t _size);
			void setFontName(const std::string& _fontName);
		protected: // derived function
			virtual void onDraw();
		public:  // Derived function
			virtual bool calculateMinSize();
			virtual void onRegenerateDisplay();
			virtual bool onEventInput(const ewol::event::Input& _event);
			virtual bool onEventEntry(const ewol::event::Entry& _event);
			virtual void onEventClipboard(enum ewol::context::clipBoard::clipboardListe _clipboardID);
			virtual void onGetFocus();
			virtual void onLostFocus();
			virtual void changeZoom(float _range);
		private:
			float m_lastOffsetDisplay; //!< Line number ofssed in the display
		private:
			bool m_insertMode; //!< the insert mode is enable
		public:
			/**
			 * @brief Update the scrolling position from the cursor position,
			 * it might be be all time in the display screen.
			 */
			void updateScrolling();
			// TODO : Doc : write data on buffer
			bool moveCursor(const appl::Buffer::Iterator& _pos);
			bool write(const std::string& _data);
			bool write(const std::string& _data, const appl::Buffer::Iterator& _pos);
			bool replace(const std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd);
			bool replace(const std::string& _data);
			bool replace(const std::u32string& _data) {
				return replace(etk::to_string(_data));
			}
			/**
			 * @brief Remove selected data ...
			 */
			void remove();
			/**
			 * @brief Remove selected data ... (No plugin call)
			 */
			void removeDirect() {
				if (m_buffer==nullptr) {
					return;
				}
				m_buffer->removeSelection();
				updateScrolling();
			}
			
			/**
			 * @brief copy data in the _data ref value.
			 * @param[out] _data Output stream to copy.
			 * @return true of no error occured.
			 */
			bool copy(std::string& _data) {
				if (m_buffer==nullptr) {
					return false;
				}
				return m_buffer->copy(_data);
			}
			/**
			 * @brief copy data in the _data ref value.
			 * @param[out] _data Output stream to copy.
			 * @param[in] _pos Position to add the data.
			 * @param[in] _posEnd End position to end replace the data.
			 */
			void copy(std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
				if (m_buffer==nullptr) {
					return;
				}
				m_buffer->copy(_data, _pos, _posEnd);
			}
			/**
			 * @brief Write data at a specific position (No plugin call)
			 * @param[in] _data Data to insert in the buffer
			 * @param[in] _pos Position to add the data.
			 * @return true if the write is done corectly
			 */
			bool writeDirect(const std::string& _data, const appl::Buffer::Iterator& _pos) {
				if (m_buffer==nullptr) {
					return false;
				}
				bool ret = m_buffer->write(_data, _pos);
				updateScrolling();
				return ret;
			}
			/**
			 * @brief Write data at a specific position (No plugin call)
			 * @param[in] _data Data to insert in the buffer
			 * @param[in] _pos Position to add the data.
			 * @param[in] _posEnd End position to end replace the data.
			 * @return true if the write is done corectly
			 */
			bool replaceDirect(const std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
				if (m_buffer==nullptr) {
					return false;
				}
				bool ret = m_buffer->replace(_data, _pos, _posEnd);
				updateScrolling();
				return ret;
			}
			
			appl::Buffer::Iterator getMousePosition(const vec2& _relativePos);
			void mouseEventDouble();
			void mouseEventTriple();
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
			void setCurrentSelect();
			/**
			 * @brief Check if the current buffer is last selected
			 * @return true if selected last
			 */
			bool isSelectedLast();
		public:
			/**
			 * @brief get the path of the current buffer
			 * @return Path of the buffer (remove the ended name)
			 */
			virtual std::string getBufferPath();
			/**
			 * @brief Check if the buffer is availlable
			 * @return true if a display buffer is present, false otherwise.
			 */
			virtual bool hasBuffer() {
				return m_buffer != nullptr;
			}
			/**
			 * @brief Get the status of selection.
			 * @return true if we have a current selection, false otherwise.
			 */
			virtual bool hasTextSelected() {
				if (m_buffer==nullptr) {
					return false;
				}
				return m_buffer->hasTextSelected();
			}
			/**
			 * @brief Remove Selection of the buffer.
			 */
			virtual void unSelect() {
				if (m_buffer==nullptr) {
					return;
				}
				m_buffer->unSelect();
			}
			/**
			 * @brief Select a section of text.
			 * @param[in] _start Start position of the selection
			 * @param[in] _stop Stop position of the selection (the curor is set at this position)
			 */
			virtual void select(const appl::Buffer::Iterator& _start, const appl::Buffer::Iterator& _stop) {
				if (m_buffer==nullptr) {
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
				if (m_buffer==nullptr) {
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
				if (m_buffer==nullptr) {
					return false;
				}
				bool ret = m_buffer->searchBack(_pos, _search, _resultStart, _caseSensitive);
				if (ret == true) {
					_resultStop = _resultStart + _search.size();
				}
				return ret;
			}
			/**
			 * @brief Get the position of selection around (select word).
			 * @param[in] _pos Position to start the selection.
			 * @param[out] _beginPos Position where the element start.
			 * @param[out] _endPos Position where the element stop.
			 * @return true if we find a selection around.
			 */
			bool getPosAround(const appl::Buffer::Iterator& _pos,
			                  appl::Buffer::Iterator &_beginPos,
			                  appl::Buffer::Iterator &_endPos) {
				if (m_buffer==nullptr) {
					return false;
				}
				return m_buffer->getPosAround(_pos, _beginPos, _endPos);
			}
			/**
			 * @brief Get an iterator an an specific position
			 * @param[in] _pos Requested position of the iterator.
			 * @return The Iterator
			 */
			appl::Buffer::Iterator position(int64_t _pos) {
				if (m_buffer==nullptr) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->position(_pos);
			}
			/**
			 * @brief Get the cursor position.
			 * @return The iterator on the cursor position
			 */
			appl::Buffer::Iterator cursor() {
				if (m_buffer==nullptr) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->cursor();
			}
			/**
			 * @brief Get the begin position.
			 * @return The iterator on the begin position
			 */
			appl::Buffer::Iterator begin() {
				if (m_buffer==nullptr) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->begin();
			}
			/**
			 * @brief Get the end position.
			 * @return The iterator on the end position
			 */
			appl::Buffer::Iterator end() {
				if (m_buffer==nullptr) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->end();
			}
			/**
			 * @brief Get an Iterator on the start selection.
			 * @return The Iterator
			 */
			appl::Buffer::Iterator selectStart() {
				if (m_buffer==nullptr) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->selectStart();
			}
			/**
			 * @brief Get an Iterator on the stop selection.
			 * @return The Iterator
			 */
			appl::Buffer::Iterator selectStop() {
				if (m_buffer==nullptr) {
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
				if (m_buffer==nullptr) {
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
				if (m_buffer==nullptr) {
					return appl::Buffer::Iterator();
				}
				return m_buffer->getEndLine(_pos);
			}
			/**
			 * @brief Register of the arrival of a Multicast message
			 * @param[in] _messageId Event Id waiting for...
			 */
			// TODO : Remove
			void ext_registerMultiCast(const char* const _messageId) {
				//registerMultiCast(_messageId);
			}
			/**
			 * @brief add a specific shortcut with his description
			 * @param[in] _descriptiveString Description string of the shortcut
			 * @param[in] _generateEventName Event generic of the element
			 */
			virtual void ext_shortCutAdd(const std::string& _descriptiveString,
			                             const std::string& _generateEventName) {
				shortCutAdd(_descriptiveString, _generateEventName);
			}
			/**
			 * @brief Remove a specific shortcut with his event name
			 * @param[in] _generateEventName Event of the element shortcut
			 */
			virtual void ext_shortCutRm(const std::string& _generateEventName) {
				shortCutRemove(_generateEventName);
			}
		private: // callback fundtions
			void onCallbackIsModify();
			void onCallbackShortCut(const std::string& _value);
			void onCallbackSelectChange();
			void onCallbackselectNewFile(const std::string& _value);
	};
};

#endif

