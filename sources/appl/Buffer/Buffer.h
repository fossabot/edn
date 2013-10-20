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
			class Iterator {
				// Private data :
				private:
					esize_t m_current; //!< curent Id in the Buffer
					appl::Buffer* m_data; //!< Pointer on the curent Buffer
				public:
					/**
					 * @brief Basic itarator constructor with no link.
					 */
					Iterator(void):
					  m_current(0),
					  m_data(NULL) {
						// nothing to do ...
					}
					/**
					 * @brief Recopy constructor.
					 * @param[in] _obj The Iterator that might be copy
					 */
					Iterator(const Iterator & _obj):
					  m_current(_obj.m_current),
					  m_data(_obj.m_data) {
						// nothing to do ...
					}
					/**
					 * @brief Asignation operator.
					 * @param[in] _otherIterator The Iterator that might be copy
					 * @return reference on the curent Iterator
					 */
					Iterator& operator=(const Iterator & _obj) {
						m_current = _obj.m_current;
						m_data = _obj.m_data;
						return *this;
					}
					/**
					 * @brief Basic destructor
					 */
					~Iterator(void) {
						m_current = 0;
						m_data = NULL;
					}
					/**
					 * @brief basic boolean cast
					 * @return true if the element is present in buffer
					 */
					operator bool (void) const {
						if (m_data == NULL) {
							return false;
						}
						return (m_current < m_data->m_data.size());
					}
					/**
					 * @brief basic boolean cast
					 * @return true if the element is present in buffer
					 */
					operator esize_t (void) const {
						if (m_data == NULL) {
							return 0;
						}
						return m_current;
					}
					/**
					 * @brief Incremental operator
					 * @return Reference on the current iterator incremented
					 */
					Iterator& operator++ (void);
					/**
					 * @brief Decremental operator
					 * @return Reference on the current iterator decremented
					 */
					Iterator& operator-- (void);
					/**
					 * @brief Incremental operator
					 * @return Reference on a new iterator and increment the other one
					 */
					Iterator operator++ (int32_t) {
						Iterator it(*this);
						++(*this);
						return it;
					}
					/**
					 * @brief Decremental operator
					 * @return Reference on a new iterator and decrement the other one
					 */
					Iterator operator-- (int32_t) {
						Iterator it(*this);
						--(*this);
						return it;
					}
					/**
					 * @brief egality iterator
					 * @return true if the iterator is identical pos
					 */
					bool operator== (const Iterator& _obj) const {
						if (    m_current == _obj.m_current
						     && m_data == _obj.m_data) {
							return true;
						}
						return false;
					}
					/**
					 * @brief egality iterator
					 * @return true if the iterator is identical pos
					 */
					bool operator!= (const Iterator& _obj) const {
						if (    m_current != _obj.m_current
						     || m_data != _obj.m_data) {
							return true;
						}
						return false;
					}
					/**
					 * @brief Get the value on the current element
					 * @return The request element value
					 */
					etk::UChar operator* (void) const ;
					/**
					 * @brief Get the position in the buffer
					 * @return The requested position.
					 */
					esize_t getPos(void) const {
						return m_current;
					}
					/**
					 * @brief move the element position
					 * @return a new iterator.
					 */
					Iterator operator+ (const int32_t _val) const {
						Iterator tmpp(*this);
						for (int32_t iii=0; iii<_val; ++iii) {
							++tmpp;
						}
						return tmpp;
					}
					/**
					 * @brief move the element position
					 * @return a new iterator.
					 */
					Iterator operator- (const int32_t _val) const {
						Iterator tmpp(*this);
						for (int32_t iii=0; iii<_val; ++iii) {
							--tmpp;
						}
						return tmpp;
					}
				private:
					Iterator(Buffer* _obj, int32_t _pos) :
					  m_current(_pos),
					  m_data(_obj) {
						// nothing to do ...
					}
					friend class Buffer;
			};
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
		public:
			esize_t m_cursorPos; //!< cursor position.
			int32_t m_cursorSelectPos; //!< cursor position.
		public:
			void setSelectionPos(const Iterator& _pos);
			void unSelect(void);
		protected:
			float m_cursorPreferredCol; //!< position of the cursor when up and down is done.
		public:
			void setFavoriteUpDownPos(float _val) {
				m_cursorPreferredCol = _val;
			}
			float getFavoriteUpDownPos(void) {
				return m_cursorPreferredCol;
			}
		private:
			bool m_selectMode; //!< when true, the select mode keep the moving selecting
		public:
			bool getSelectMode(void) {
				return m_selectMode;
			}
			void setSelectMode(bool _status) {
				m_selectMode = _status;
			}
			// note : We need the text drawer interface due to the fact that the move depend on the text display properties.
			bool onEventEntry(const ewol::EventEntry& _event, ewol::Text& _textDrawer);
			//bool onEventInput(const ewol::EventInput& _event, ewol::Text& _textDrawer, const vec2& _relativePos);
			void moveCursor(esize_t _pos);
			/**
			 * @brief Get the status of selection.
			 * @return true if we have a curent selection, false otherwise.
			 */
			bool hasTextSelected(void) {
				return m_cursorSelectPos >= 0;
			}
			/**
			 * @brief Get the Start position of the selection.
			 * @return position of the start selection.
			 */
			esize_t getStartSelectionPos(void) {
				return etk_min(m_cursorPos, m_cursorSelectPos);
			}
			/**
			 * @brief Get the Stop position of the selection.
			 * @return position of the stop selection.
			 */
			esize_t getStopSelectionPos(void) {
				return etk_max(m_cursorPos, m_cursorSelectPos);
			}
			bool getPosAround(const Iterator& _startPos, Iterator &_beginPos, Iterator &_endPos);
			/**
			 * @brief Expand the specify char to have a user frendly display for special char and tabs
			 * @param[in] _indent Curent indentation in the line
			 * @param[in] _value Current value to transform
			 * @param[out] _out String that represent the curent value to display
			 */
			void expand(esize_t& _indent, const etk::UChar& _value, etk::UString& _out) const;
		public:
			/**
			 * @brief get the start of a line with the position in the buffer.
			 * @param[in] _pos position in the buffer.
			 * @return The position in the buffer of the start of the line.
			 */
			Iterator getStartLine(const Iterator& _pos);
			/**
			 * @brief get the end of a line with the position in the buffer.
			 * @param[in] _pos position in the buffer.
			 * @return The position in the buffer of the end of the line.
			 */
			Iterator getEndLine(const Iterator& _pos);
			/**
			 * @brief Search a character in the buffer.
			 * @param[in] _pos Position to start the search of the element.
			 * @param[in] _search Character to search.
			 * @param[out] _result Research position.
			 * @return true if pos if fined.
			 */
			bool search(const Iterator& _pos, const etk::UChar& _search, Iterator& _result);
			/**
			 * @brief Search a character in the buffer in back mode.
			 * @param[in] _pos Position to start the search of the element.
			 * @param[in] _search Character to search.
			 * @param[out] _result Research position.
			 * @return true if pos if fined.
			 */
			bool searchBack(const Iterator& _pos, const etk::UChar& _search, Iterator& _result);
			/**
			 * @brief find the first character of the line "nLines" forward
			 * @param[in,out] _startPos Start position.
			 * @param[in,out] _nLines Number of line to count.
			 * @return position of the starting the line.
			 */
			Iterator countForwardNLines(const Iterator& _startPos, int32_t _nLines);
			/**
			 * @brief find the first character of the line "nLines" backwards
			 * @param[in,out] _startPos Start position to count (this caracter is not counted)
			 * @param[in,out] _nLines Number of line to count (if  == 0 means find the beginning of the line)
			 * @return position of the starting the line
			 */
			Iterator countBackwardNLines(const Iterator& _startPos, int32_t _nLines);
		public:
			/**
			 * @brief copy data in the _data ref value.
			 * @param[out] _data Output stream to copy.
			 * @return true of no error occured.
			 */
			bool copy(etk::UString& _data);
			/**
			 * @brief Remove the selection of the buffer. (do nothing if no secection)
			 */
			void removeSelection(void);
			
			
			bool write(const etk::UString& _data, const appl::Buffer::Iterator& _pos);
			bool replace(const etk::UString& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd);
		public: // iterator section :
			/**
			 * @brief Get an iterator an an specific position
			 * @param[in] _pos Requested position of the iterator.
			 * @return The Iterator
			 */
			Iterator position(esize_t _pos);
			/**
			 * @brief Get an Iterator on the start position.
			 * @return The Iterator
			 */
			Iterator begin(void);
			/**
			 * @brief Get an Iterator on the end position.
			 * @return The Iterator
			 */
			Iterator end(void);
			/**
			 * @brief Get an Iterator on the cursor position.
			 * @return The Iterator
			 */
			Iterator cursor(void);
			/**
			 * @brief Get an Iterator on the start selection.
			 * @return The Iterator
			 */
			Iterator selectStart(void);
			/**
			 * @brief Get an Iterator on the stop selection.
			 * @return The Iterator
			 */
			Iterator selectStop(void);
	};
};


#endif
