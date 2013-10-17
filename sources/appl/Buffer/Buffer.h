/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_BUFFER_H__
#define __APPL_BUFFER_H__

#include <etk/UString.h>
#include <etk/os/FSNode.h>
#include <etk/unicode.h>
#include <ewol/ewol.h>
#include <etk/Buffer.h>
#include <ewol/renderer/EObject.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Text.h>

namespace appl {
	class Buffer : public ewol::EObject {
		public:
			Buffer(void);
			~Buffer(void) { };
		private:
			etk::UString m_fileName; //!< name of the file (with his path)
		public:
			/**
			 * @brief get the curent filename of the Buffer
			 */
			const etk::UString& getFileName(void) {
				return m_fileName;
			}
			bool loadFile(const etk::UString& _name);
			void setFileName(const etk::UString& _name);
			bool m_isModify; //!< true if the file is modify
			etk::Buffer m_data; //!< copy of the file buffer
		public:
			etk::Buffer& getData(void) {
				return m_data;
			};
			/*
			appl::History m_history;
			Highlight m_highlight;
			ejson::Value* m_property;
			appl::Selection m_selection;
			*/
		public:
			esize_t m_cursorPos; //!< cursor position.
			int32_t m_cursorSelectPos; //!< cursor position.
			float m_cursorPreferredCol; //!< position of the cursor when up and down is done.
			bool m_selectMode; //!< when true, the select mode keep the moving selecting
			// note : We need the text drawer interface due to the fact that the move depend on the text display properties.
			bool onEventEntry(const ewol::EventEntry& _event, ewol::Text& _textDrawer);
			bool onEventInput(const ewol::EventInput& _event, ewol::Text& _textDrawer, const vec2& _relativePos);
			void moveCursor(esize_t _pos);
			void mouseEventDouble(void);
			void mouseEventTriple(void);
			bool hasTextSelected(void) {
				return m_cursorSelectPos >= 0;
			}
			bool selectAround(int32_t _startPos, int32_t &_beginPos, int32_t &_endPos);
			/**
			 * @brief Get the position in the buffer of a display distance from the start of the line
			 * @param[in] _startLinePos start of the line
			 * @param[in] _distance Distane from the start of the line
			 * @param[in] _textDrawer Drawer compositing element
			 * @return position in the buffer
			 */
			esize_t getPosSize(esize_t _startLinePos, float _distance, ewol::Text& _textDrawer);
			/**
			 * @brief Get the real distance displayed from the start of the line to the element requested
			 * @param[in] _startLinePos start of the line
			 * @param[in] _stopPos Position that we want to have te distance
			 * @param[in] _textDrawer Drawer compositing element
			 * @return Distance from the start of the line
			 */
			float getScreenSize(esize_t _startLinePos, esize_t _stopPos, ewol::Text& _textDrawer);
			/**
			 * @brief Get the position on the buffer with the position on the mose in the screen
			 * @param[in] _relativePos mouse position( standard GUI position (not ewol generic pos !!!)
			 * @param[in] _textDrawer Drawer compositing element
			 * @return Position in the buffer
			 */
			esize_t getMousePosition(const vec2& _relativePos, ewol::Text& _textDrawer);
			/**
			 * @brief get the next element in the buffer.
			 * @param[in] _pos Position in the buffer
			 * @param[out] _value Unicode value read in the buffer
			 * @param[in] _charset Charset used to parse the current buffer
			 * @return number ofelement read in the buffer (to increment the position)
			 */
			esize_t get(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset = unicode::EDN_CHARSET_UTF8) const;
			/**
			 * @brief get the previous element in the buffer.
			 * @param[in] _pos Position in the buffer (last element of the element)
			 * @param[out] _value Unicode value read in the buffer
			 * @param[in] _charset Charset used to parse the current buffer
			 * @return number of element read in the buffer (to increment the position)
			 */
			esize_t getBack(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset = unicode::EDN_CHARSET_UTF8) const;
			/**
			 * @brief Expand the specify char to have a user frendly display for special char and tabs
			 * @param[in] _indent Curent indentation in the line
			 * @param[in] _value Current value to transform
			 * @param[out] _out String that represent the curent value to display
			 */
			void expand(esize_t& _indent, const etk::UniChar& _value, etk::UString& _out) const;
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
			void moveCursorUp(esize_t _nbLine, ewol::Text& _textDrawer);
			/**
			 * @brief Move the cursor at an other position under.
			 * @param[in] _nbLine number of down line that might be moved
			 */
			void moveCursorDown(esize_t _nbLine, ewol::Text& _textDrawer);
			/**
			 * @brief get the start of a line with the position in the buffer.
			 * @param[in] _pos position in the buffer.
			 * @return The position in the buffer of the start of the line.
			 */
			esize_t startLine(esize_t _pos);
			/**
			 * @brief get the end of a line with the position in the buffer.
			 * @param[in] _pos position in the buffer.
			 * @return The position in the buffer of the end of the line.
			 */
			esize_t endLine(esize_t _pos);
			/**
			 * @brief Search a character in the buffer.
			 * @param[in] _pos Position to start the search of the element.
			 * @param[in] _search Character to search.
			 * @param[out] _result Research position.
			 * @return true if pos if fined.
			 */
			bool search(esize_t _pos, const etk::UniChar& _search, esize_t& _result);
			/**
			 * @brief Search a character in the buffer in back mode.
			 * @param[in] _pos Position to start the search of the element.
			 * @param[in] _search Character to search.
			 * @param[out] _result Research position.
			 * @return true if pos if fined.
			 */
			bool searchBack(esize_t _pos, const etk::UniChar& _search, esize_t& _result);
			/**
			 * @brief Count the number of displayed characters between buffer position
			 * displayed characters are the characters shown on the screen to represent characters in the 
			 * buffer, where tabs and control characters are expanded
			 * @param[in] _posStart start position
			 * @param[in] _posEnd End position
			 * @return the ID in the buffer of the requested char
			 */
			int32_t countDispChars(esize_t _posStart, esize_t _posEnd);
			/**
			 * @brief Return the position of the nth diplaye char
			 * @param[in] _posStart Position of the start
			 * @param[in] _nChars search in the next nChars elements
			 * @return position of the char i the buffer
			 */
			esize_t countForwardDispChars(esize_t _posStart, int32_t _nChars);
			/**
			 * @brief find the first character of the line "nLines" forward
			 * @param[in,out] _startPos Start position.
			 * @param[in,out] _nLines Number of line to count.
			 * @return position of the starting the line.
			 */
			esize_t countForwardNLines(esize_t _startPos, int32_t _nLines);
			/**
			 * @brief find the first character of the line "nLines" backwards
			 * @param[in,out] _startPos Start position to count (this caracter is not counted)
			 * @param[in,out] _nLines Number of line to count (if  == 0 means find the beginning of the line)
			 * @return position of the starting the line
			 */
			esize_t countBackwardNLines(esize_t _startPos, int32_t _nLines);

	};
};


#endif
