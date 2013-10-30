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
#include <appl/Highlight.h>

namespace appl {

	class DisplayHLData {
		public:
			etk::Vector<appl::HighlightInfo> HLData;
			int32_t posHLPass1;
			int32_t posHLPass2;
	};
	class Buffer : public ewol::EObject {
		public:
			class Iterator {
				// Private data :
				private:
					int64_t m_current; //!< curent Id in the Buffer
					appl::Buffer* m_data; //!< Pointer on the curent Buffer
					etk::UChar m_value; //!< store vlue to prevent multiple calcule of getting the data
				public:
					/**
					 * @brief Basic itarator constructor with no link.
					 */
					Iterator(void):
					  m_current(0),
					  m_data(NULL),
					  m_value(etk::UChar::Null) {
						// nothing to do ...
					};
					/**
					 * @brief Recopy constructor.
					 * @param[in] _obj The Iterator that might be copy
					 */
					Iterator(const Iterator & _obj):
					  m_current(_obj.m_current),
					  m_data(_obj.m_data),
					  m_value(etk::UChar::Null) {
						// nothing to do ...
					};
					/**
					 * @brief Asignation operator.
					 * @param[in] _otherIterator The Iterator that might be copy
					 * @return reference on the curent Iterator
					 */
					Iterator& operator=(const Iterator & _obj) {
						m_current = _obj.m_current;
						m_data = _obj.m_data;
						m_value = etk::UChar::Null;
						return *this;
					};
					/**
					 * @brief Basic destructor
					 */
					~Iterator(void) {
						m_current = 0;
						m_data = NULL;
						m_value = etk::UChar::Null;
					};
					/**
					 * @brief basic boolean cast
					 * @return true if the element is present in buffer
					 */
					operator bool (void) const {
						if (m_data == NULL) {
							return false;
						}
						if (m_current >= m_data->m_data.size()) {
							return false;
						}
						if (m_current < 0) {
							return false;
						}
						return true;
					};
					/**
					 * @brief basic boolean cast
					 * @return true if the element is present in buffer
					 */
					operator esize_t (void) const {
						if (m_data == NULL) {
							return 0;
						}
						if (m_current < 0) {
							return 0;
						}
						if (m_current >= m_data->m_data.size()) {
							return m_data->m_data.size()-1;
						}
						return m_current;
					};
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
					};
					/**
					 * @brief Decremental operator
					 * @return Reference on a new iterator and decrement the other one
					 */
					Iterator operator-- (int32_t) {
						Iterator it(*this);
						--(*this);
						return it;
					};
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
					};
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
					};
					/**
					 * @brief <= iterator
					 * @return true if the iterator is identical pos
					 */
					bool operator<= (const Iterator& _obj) const {
						if (m_data != _obj.m_data) {
							return false;
						}
						if (m_current <= _obj.m_current) {
							return true;
						}
						return false;
					};
					/**
					 * @brief >= iterator
					 * @return true if the iterator is identical pos
					 */
					bool operator>= (const Iterator& _obj) const {
						if (m_data != _obj.m_data) {
							return false;
						}
						if (m_current >= _obj.m_current) {
							return true;
						}
						return false;
					};
					/**
					 * @brief < iterator
					 * @return true if the iterator is identical pos
					 */
					bool operator< (const Iterator& _obj) const {
						if (m_data != _obj.m_data) {
							return false;
						}
						if (m_current < _obj.m_current) {
							return true;
						}
						return false;
					};
					/**
					 * @brief > iterator
					 * @return true if the iterator is identical pos
					 */
					bool operator> (const Iterator& _obj) const {
						if (m_data != _obj.m_data) {
							return false;
						}
						if (m_current > _obj.m_current) {
							return true;
						}
						return false;
					};
					/**
					 * @brief Get the value on the current element
					 * @return The request element value
					 */
					etk::UChar operator* (void);
					/**
					 * @brief Get the position in the buffer
					 * @return The requested position.
					 */
					esize_t getPos(void) const {
						if (m_data == NULL) {
							return 0;
						}
						if (m_current < 0) {
							return 0;
						}
						if (m_current >= m_data->m_data.size()) {
							return m_data->m_data.size()-1;
						}
						return m_current;
					};
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
					};
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
					};
				private:
					Iterator(Buffer* _obj, int32_t _pos) :
					  m_current(_pos),
					  m_data(_obj),
					  m_value(etk::UChar::Null) {
						// nothing to do ...
					};
					friend class Buffer;
			};
		public:
			static const char* const eventIsModify;
			static const char* const eventIsSave;
			static const char* const eventSelectChange;
		public:
			Buffer(void);
			~Buffer(void);
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
		protected:
			esize_t m_cursorPos; //!< cursor position.
		public:
			void moveCursor(esize_t _pos);
		protected:
			int32_t m_cursorSelectPos; //!< cursor position.
		public:
			/**
			 * @brief Set the selection position in the buffer.
			 * @param[in] _pos Position of the selection.
			 */
			void setSelectionPos(const Iterator& _pos);
			/**
			 * @brief Un select request.
			 */
			void unSelect(void);
			/**
			 * @brief Remove the selection of the buffer. (do nothing if no secection)
			 */
			void removeSelection(void);
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
		protected:
			float m_cursorPreferredCol; //!< position of the cursor when up and down is done.
		public:
			/**
			 * @brief Set the favorite up and down position (distance from the left of the screen.
			 * @param[in] _val New distance (in pixels).
			 */
			void setFavoriteUpDownPos(float _val) {
				m_cursorPreferredCol = _val;
			}
			/**
			 * @brief Get the favorite distance from the left screen (For up and down moving).
			 * @return The distance in pixels.
			 */
			float getFavoriteUpDownPos(void) {
				return m_cursorPreferredCol;
			}
		protected:
			bool m_selectMode; //!< when true, the select mode keep the moving selecting
		public:
			/**
			 * @brief Set the selection mode (if true, the move event creata a selection)
			 * @param[in] _status New status of the section.
			 */
			void setSelectMode(bool _status) {
				m_selectMode = _status;
			}
			/**
			 * @brief Get the selection mode (if true, the move event creata a selection)
			 * @return The selecting mode.
			 */
			bool getSelectMode(void) {
				return m_selectMode;
			}
		public:
			/**
			 * @brief Get the position of selection around (select word).
			 * @param[in] _startPos Position to start the selection.
			 * @param[out] _beginPos Position where the element start.
			 * @param[out] _endPos Position where the element stop.
			 * @return true if we find a selection around.
			 */
			bool getPosAround(const Iterator& _startPos, Iterator &_beginPos, Iterator &_endPos);
			/**
			 * @brief Expand the specify char to have a user frendly display for special char and tabs
			 * @param[in] _indent Curent indentation in the line
			 * @param[in] _value Current value to transform
			 * @param[out] _out String that represent the curent value to display
			 */
			void expand(esize_t& _indent, const etk::UChar& _value, etk::UString& _out) const;
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
			 * @param[in] _startPos Start position.
			 * @param[in] _nLines Number of line to count.
			 * @return position of the starting the line.
			 */
			Iterator countForwardNLines(const Iterator& _startPos, int32_t _nLines);
			/**
			 * @brief find the first character of the line "nLines" backwards
			 * @param[in] _startPos Start position to count (this caracter is not counted)
			 * @param[in] _nLines Number of line to count (if  == 0 means find the beginning of the line)
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
			 * @brief copy data in the _data ref value.
			 * @param[out] _data Output stream to copy.
			 * @param[in] _pos Position to add the data.
			 * @param[in] _posEnd End position to end replace the data.
			 */
			void copy(etk::UString& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd);
			/**
			 * @brief Write data at a specific position
			 * @param[in] _data Data to insert in the buffer
			 * @param[in] _pos Position to add the data.
			 * @return true if the write is done corectly
			 */
			bool write(const etk::UString& _data, const appl::Buffer::Iterator& _pos);
			/**
			 * @brief Write data at a specific position
			 * @param[in] _data Data to insert in the buffer
			 * @param[in] _pos Position to add the data.
			 * @param[in] _posEnd End position to end replace the data.
			 * @return true if the write is done corectly
			 */
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
		protected:
			esize_t m_nbLines; //!< number of line in the buffer
		public:
			/**
			 * @brief Get the number of line in the buffer.
			 * @return number of line in the Buffer.
			 */
			esize_t getNumberOfLines(void) {
				return m_nbLines;
			}
		protected:
			/**
			 * @brief Count the number of line in the buffer
			 */
			void countNumberofLine(void);
		protected:
			etk::UString m_highlightType; //!< Name of the highlight type
			
			appl::Highlight* m_highlight; //!< internal link with the Highlight system
			etk::Vector<appl::HighlightInfo> m_HLDataPass1; //!< colorisation position in the current buffer pass 1
		public:
			/**
			 * @brief Find the Highligh capability
			 */
			void tryFindHighlightType(void);
			/**
			 * @brief Set type of highlight
			 * @param[in] _type type of the highlight
			 */
			void setHighlightType(const etk::UString& _type);
			/**
			 * @brief Get type of highlight
			 * @return Type of the highlight
			 */
			const etk::UString& setHighlightType(void) {
				return m_highlightType;
			};
			
			void regenerateHighLightAt(int32_t _pos, int32_t _nbDeleted, int32_t _nbAdded);
			void findMainHighLightPosition(int32_t _startPos,
			                               int32_t _endPos,
			                               int32_t& _startId,
			                               int32_t& _stopId,
			                               bool _backPreviousNotEnded);
			void generateHighLightAt(int32_t _pos, int32_t _endPos, int32_t _addingPos=0);
			void cleanHighLight(void);
			appl::HighlightInfo* getElementColorAtPosition(int32_t _pos, int32_t &_starPos);
			void hightlightGenerateLines(appl::DisplayHLData& _MData, int32_t _HLStart, int32_t _nbLines);
			appl::HighlightInfo* getElementColorAtPosition(appl::DisplayHLData& _MData, int32_t _pos);
	};
};


#endif
