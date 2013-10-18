/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __EDN_BUF_H__
#define __EDN_BUF_H__

/* Maximum length in characters of a tab or control character expansion
   of a single buffer character */
#define MAX_EXP_CHAR_LEN 20*4

class EdnBuf;

#include <etk/Buffer.h>
#include <EdnBufHistory.h>
#include <HighlightManager.h>
#include <etk/unicode.h>

/*
                                                                                                    
               rectStart                       rectStart                                            
start          *************                   *************                                        
      **********           *               xxxx*xxxxxx     *                                        
      *             ********               xxxx*xxxxxxxxxxx*xxxxx                                   
end   ***************                          *************                                        
                    rectEnd                                rectEnd                                  
                                                                                                    
                                                                                                    
                                                                                                    
*/
typedef struct {
	bool selected; //!< True if the selection is active
	bool rectangular; //!< True if the selection is rectangular
	bool zeroWidth; //!< Width 0 selections aren't "real" selections, but they can be useful when creating rectangular selections from the keyboard.
	int32_t start; //!< Pos. of start of selection, or if rectangular start of line containing it.
	int32_t end; //!< Pos. of end of selection, or if rectangular end of line containing it.
	int32_t rectStart; //!< Indent of left edge of rect. selection
	int32_t rectEnd; //!< Indent of right edge of rect. selection
} selection;

typedef struct {
	etk::Vector<colorInformation_ts> HLData;
	int32_t posHLPass1;
	int32_t posHLPass2;
}displayHLData_ts;

class EdnBuf {
	// TODO : set an iterator to acces at every data without knowin the system ...
	public:
		// constructer
		EdnBuf(void);
		// destructer
		~EdnBuf(void);
		// public function :
		void getAll(etk::Vector<int8_t>& _text);
		void setAll(etk::Vector<int8_t>& _text);
		void getRange(int32_t _start, int32_t _end, etk::Vector<int8_t>& _output);
		void getRange(int32_t _start, int32_t _end, etk::UString& _output);
		bool DumpIn(etk::FSNode& _file);
		bool DumpFrom(etk::FSNode& _file);
		// replace with operator [] ...
		int8_t operator[] (int32_t) const;
		int32_t insert(int32_t _pos, etk::Vector<int8_t>& _insertText);
		int32_t insert(int32_t _pos, etk::UString& _insertText);
		int32_t Replace(int32_t _start, int32_t _end, etk::Vector<int8_t>& _insertText);
		int32_t Replace(int32_t _start, int32_t _end, etk::UString& _insertText);
		void remove(int32_t _start, int32_t _end);
		int32_t Indent(void);
		int32_t UnIndent(void);
		
		
		void getLineText(int32_t _pos, etk::Vector<int8_t>& _text);
		int32_t StartOfLine(int32_t _pos);
		int32_t EndOfLine(int32_t _pos);
		
		int32_t getExpandedChar(int32_t& _pos, int32_t _indent, etk::UChar _outUnicode[MAX_EXP_CHAR_LEN], uint32_t& _currentChar);
		int32_t getExpandedChar(int32_t& _pos, int32_t _indent, char _outUTF8[MAX_EXP_CHAR_LEN], uint32_t& _currentChar);
		int32_t ExpandCharacter(char _c, int32_t _indent, char _outUTF8[MAX_EXP_CHAR_LEN]); // TODO : remove
		int32_t CharWidth(char _c, int32_t _indent); // TODO : rework this
		int32_t CountDispChars(int32_t _lineStartPos, int32_t _targetPos);
		int32_t CountForwardDispChars(int32_t _lineStartPos, int32_t _nChars);
		int32_t CountLines(int32_t _startPos, int32_t _endPos);
		int32_t CountLines(void);
		int32_t CountLines(etk::Vector<int8_t>& _data);
		int32_t CountForwardNLines(int32_t _startPos, int32_t _nLines);
		int32_t CountBackwardNLines(int32_t _startPos, int32_t _nLines);
		
		bool SearchForward(int32_t _startPos, etk::UString& _search, int32_t* _foundPos, int32_t* _foundPosEnd, bool _caseSensitive = true);
		bool SearchBackward(int32_t _startPos, etk::UString& _search, int32_t* _foundPos, int32_t* _foundPosEnd, bool _caseSensitive = true);
		bool SearchForward(int32_t _startPos, char _searchChar, int32_t* _foundPos);
		bool SearchBackward(int32_t _startPos, char _searchChar, int32_t* _foundPos);
		bool SelectAround(int32_t _startPos, int32_t& _beginPos, int32_t& _endPos);

		// Buffer size system :
		int32_t size(void) { return m_data.size(); };
		int32_t NumberOfLines(void) { return m_nbLine; };

	// -----------------------------------------
	// selection remember...
	// -----------------------------------------
	public:
		bool SelectHasSelection(void);
		void Select(int32_t _start, int32_t _end);
		void Unselect(void);
		void RectSelect(int32_t _start, int32_t _end, int32_t _rectStart, int32_t _rectEnd);
		bool getSelectionPos(int32_t& _start, int32_t& _end, bool& _isRect, int32_t& _rectStart, int32_t& _rectEnd);
		void getSelectionText(etk::Vector<int8_t>& _text);
		void getSelectionText(etk::UString& _text);
		void removeSelected(void);
		int32_t ReplaceSelected(etk::Vector<int8_t>& _text);
		int32_t ReplaceSelected(etk::UString& _text);
	private:
		// current selection of the buffer
		selection m_selectionList; //!< Selection area of the buffer
		void updateSelection(int32_t _pos, int32_t _nDeleted, int32_t _nInserted);

	// -----------------------------------------
	// History section : 
	// -----------------------------------------
	public:
		int32_t Undo(void);
		int32_t Redo(void);
	private:
		bool m_isUndoProcessing;
		bool m_isRedoProcessing;
		etk::Vector<EdnBufHistory*> m_historyUndo;
		etk::Vector<EdnBufHistory*> m_historyRedo;

	// -----------------------------------------
	// hightlight section : 
	// -----------------------------------------
	private:
		Highlight * m_Highlight; //!< internal link with the Highlight system
		etk::Vector<colorInformation_ts> m_HLDataPass1; //!< colorisation position in the current buffer pass 1
		void RegenerateHighLightAt(int32_t _pos, int32_t _nbDeleted, int32_t _nbAdded);
		void generateHighLightAt(int32_t _pos, int32_t _endPos, int32_t _addinPos=0);
		void CleanHighLight(void);
		void findMainHighLightPosition(int32_t _startPos, int32_t _endPos, int32_t &_startId, int32_t &_stopId, bool _backPreviousNotEnded);
	public:
		void setHLSystem(Highlight* _newHLSystem);
		void HightlightGenerateLines(displayHLData_ts& _MData, int32_t _startPos, int32_t _nbLines);
		colorInformation_ts* getElementColorAtPosition(displayHLData_ts& _MData, int32_t _pos);
	private:
		colorInformation_ts* getElementColorAtPosition(int32_t _pos, int32_t &_starPos);

	private:
		etk::Buffer m_data; //!< buffer of the data in the mode int8_t
		void CountNumberOfLines(void);
		int32_t m_nbLine; //!< Number of line in the biffer

	// -----------------------------------------
	// display property and charset ...
	// -----------------------------------------
	public:
		int32_t getTabDistance(void) { return m_tabDist; } ;
		void setTabDistance(int32_t _tabDist) { m_tabDist = _tabDist; };
		unicode::charset_te getCharsetType(void) { return m_charsetType; };
		void setCharsetType(unicode::charset_te _newOne) { m_charsetType = _newOne; if (unicode::EDN_CHARSET_UTF8 == _newOne){m_isUtf8=true;} else {m_isUtf8=false;} };
		bool getUTF8Mode(void) { return m_isUtf8; };
		void setUTF8Mode(bool _newOne) { m_isUtf8 = _newOne; m_charsetType=unicode::EDN_CHARSET_UTF8; };
	private:
		// Special mode of the buffer :
		bool m_isUtf8; //!< true if we are in UTF8 mode  == > if true the size of a char is 0, otherwise .. 1->4 ( TODO : not now)
		unicode::charset_te m_charsetType; //!< if UTF8 mode is at false : the charset type of the buffer
		// Local Tabulation policies
		int32_t m_tabDist; //!< equiv. number of characters in a tab
		bool m_useTabs; //!< True if buffer routines are allowed to use tabs for padding in rectangular operations
	
	// -----------------------------------------
	// Local function : 
	// -----------------------------------------
	private:
		void findRectSelBoundariesForCopy(int32_t _lineStartPos, int32_t _rectStart, int32_t _rectEnd, int32_t *_selStart, int32_t *_selEnd);
		char* getSelectionText(selection& _sel);
		void removeSelected(selection& _sel);
		void replaceSelected(selection& _sel, const char* _text);
		void eventModification(int32_t _pos, int32_t _nInserted, etk::Vector<int8_t>& _deletedText);
		int32_t LocalInsert(int32_t _pos, etk::Vector<int8_t> &_insertText);
		int32_t LocalInsert(int32_t _pos, etk::UString &_insertText);
		bool charMatch(char _first, char _second, bool _caseSensitive = true);
};

#endif
