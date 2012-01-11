/**
 *******************************************************************************
 * @file EdnBuf.h
 * @brief Editeur De N'ours : Buffer for internal Data (header)
 * @author Edouard DUPIN
 * @date 23/03/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */
#ifndef __EDN_BUF_H__
#define __EDN_BUF_H__

/* Maximum length in characters of a tab or control character expansion
   of a single buffer character */
#define MAX_EXP_CHAR_LEN 20*4

class EdnBuf;

#include <EdnVectorBuf.h>
#include <EdnBufHistory.h>
#include <HighlightManager.h>
#include <charset.h>

/*
                                                                                                    
               rectStart                       rectStart                                            
start          *************                   *************                                        
      **********           *               xxxx*xxxxxx     *                                        
      *             ********               xxxx*xxxxxxxxxxx*xxxxx                                   
end   ***************                          *************                                        
                    rectEnd                                rectEnd                                  
                                                                                                    
                                                                                                    
                                                                                                    
*/
typedef struct {
	bool selected;          //!< True if the selection is active
	bool rectangular;       //!< True if the selection is rectangular
	bool zeroWidth;         //!< Width 0 selections aren't "real" selections, but they can be useful when creating rectangular selections from the keyboard.
	int32_t start;          //!< Pos. of start of selection, or if rectangular start of line containing it.
	int32_t end;            //!< Pos. of end of selection, or if rectangular end of line containing it.
	int32_t rectStart;      //!< Indent of left edge of rect. selection
	int32_t rectEnd;        //!< Indent of right edge of rect. selection
} selection;

typedef enum{
	SELECTION_PRIMARY,
	SELECTION_SECONDARY,
	SELECTION_HIGHTLIGHT,
	SELECTION_SIZE
}selectionType_te;


typedef struct {
	etk::VectorType<colorInformation_ts>    HLData;
	int32_t                                 idSequence;
	int32_t                                 posHLPass1;
	int32_t                                 posHLPass2;
}displayHLData_ts;



class EdnBuf {
	public:
		// constructer
		EdnBuf(void);
		// destructer
		~EdnBuf(void);
		// public function :
		void    GetAll(                 etk::VectorType<int8_t> &text);
		void    SetAll(                 etk::VectorType<int8_t> &text);
		void    GetRange(               int32_t start, int32_t end, etk::VectorType<int8_t> &output);
		bool    DumpIn(                 FILE *myFile);
		bool    DumpFrom(               FILE *myFile);
		// replace with operator [] ...
		int8_t  operator[] (int32_t);
		void    Insert(                 int32_t pos, etk::VectorType<int8_t> &insertText);
		void    Replace(                int32_t start, int32_t end, etk::VectorType<int8_t> &insertText);
		void    Remove(                 int32_t start, int32_t end);
		int32_t Indent(                 selectionType_te select);
		int32_t UnIndent(               selectionType_te select);
		
		
		void    GetLineText(            int32_t pos, etk::VectorType<int8_t> &text);
		int32_t StartOfLine(            int32_t pos);
		int32_t EndOfLine(              int32_t pos);
		
		int32_t GetExpandedChar(        int32_t &pos, int32_t indent, char outUTF8[MAX_EXP_CHAR_LEN], uint32_t &currentChar);
		int32_t ExpandCharacter(        char c,      int32_t indent, char outUTF8[MAX_EXP_CHAR_LEN]); 	// TODO : rework this
		int32_t CharWidth(              char c,      int32_t indent); 	// TODO : rework this
		int32_t CountDispChars(         int32_t lineStartPos, int32_t targetPos);
		int32_t CountForwardDispChars(  int32_t lineStartPos, int32_t nChars);
		int32_t CountLines(             int32_t startPos, int32_t endPos);
		int32_t CountLines(             void);
		int32_t CountLines(             etk::VectorType<int8_t> &data);
		int32_t CountForwardNLines(     int32_t startPos, int32_t nLines);
		int32_t CountBackwardNLines(    int32_t startPos, int32_t nLines);
		
		bool    SearchForward(          int32_t startPos, etk::VectorType<int8_t> &searchVect, int32_t *foundPos, bool caseSensitive = true);
		bool    SearchBackward(         int32_t startPos, etk::VectorType<int8_t> &searchVect, int32_t *foundPos, bool caseSensitive = true);
		bool    SearchForward(          int32_t startPos, char searchChar, int32_t *foundPos);
		bool    SearchBackward(         int32_t startPos, char searchChar, int32_t *foundPos);
		bool    SelectAround(           int32_t startPos, int32_t &beginPos, int32_t &endPos);

		// Buffer Size system :
		int32_t Size(void)              { return m_data.Size(); };
		int32_t NumberOfLines(void)     {return m_nbLine;};

	// -----------------------------------------
	// selection remember...
	// -----------------------------------------
	public:
		bool        SelectHasSelection(     selectionType_te select);
		void        Select(                 selectionType_te select, int32_t start, int32_t end);
		void        Unselect(               selectionType_te select);
		void        RectSelect(             selectionType_te select, int32_t start, int32_t end, int32_t rectStart, int32_t rectEnd);
		bool        GetSelectionPos(        selectionType_te select, int32_t &start, int32_t &end, bool &isRect, int32_t &rectStart, int32_t &rectEnd);
		void        GetSelectionText(       selectionType_te select, etk::VectorType<int8_t> &text);
		void        RemoveSelected(         selectionType_te select);
		void        ReplaceSelected(        selectionType_te select, etk::VectorType<int8_t> &text);
	private:
		// current selection of the buffer
		selection   m_selectionList[SELECTION_SIZE];    //!< Selection area of the buffer
		void        UpdateSelection(        selectionType_te select, int32_t pos, int32_t nDeleted, int32_t nInserted);
		void        UpdateSelections(       int32_t pos, int32_t nDeleted, int32_t nInserted);

	// -----------------------------------------
	// History section : 
	// -----------------------------------------
	public:
		int32_t                                 Undo(void);
		int32_t                                 Redo(void);
	private:
		bool                                    m_isUndoProcessing;
		bool                                    m_isRedoProcessing;
		etk::VectorType<EdnBufHistory*>         m_historyUndo;
		etk::VectorType<EdnBufHistory*>         m_historyRedo;

	// -----------------------------------------
	// hightlight section : 
	// -----------------------------------------
	private:
		Highlight *                             m_Highlight;         //!< internal link with the Highlight system
		etk::VectorType<colorInformation_ts>    m_HLDataPass1;       //!< colorisation position in the current buffer pass 1
		int32_t                                 m_HLDataSequence;    //!< position of the start of line requested by the screen viewer
		void                                    RegenerateHighLightAt(int32_t pos, int32_t nbDeleted, int32_t nbAdded);
		void                                    GenerateHighLightAt(int32_t pos, int32_t endPos, int32_t addinPos=0);
		void                                    CleanHighLight(void);
		void                                    FindMainHighLightPosition(int32_t startPos, int32_t endPos, int32_t &startId, int32_t &stopId, bool backPreviousNotEnded);
	public:
		void                                    SetHLSystem(  Highlight * newHLSystem);
		void                                    HightlightGenerateLines(displayHLData_ts & MData, int32_t startPos, int32_t nbLines);
		colorInformation_ts *                   GetElementColorAtPosition(displayHLData_ts & MData, int32_t pos);
	private:
		colorInformation_ts *                   GetElementColorAtPosition(int32_t pos, int32_t &starPos);

	private:
		EdnVectorBuf                            m_data;                     //!< buffer of the data in the mode int8_t
		void                                    CountNumberOfLines(void);
		int32_t                                 m_nbLine;                   //!< Number of line in the biffer

	// -----------------------------------------
	// Display property and charset ...
	// -----------------------------------------
	public:
		int32_t     GetTabDistance(void)              { return m_tabDist; } ;
		void        SetTabDistance(int32_t tabDist)   { m_tabDist = tabDist; };
		charset_te  GetCharsetType(void)              { return m_charsetType; };
		void        SetCharsetType(charset_te newOne) { m_charsetType = newOne; if (EDN_CHARSET_UTF8==newOne){m_isUtf8=true;} else {m_isUtf8=false;} };
		bool        GetUTF8Mode(void)                 { return m_isUtf8; };
		void        SetUTF8Mode(bool newOne)          { m_isUtf8 = newOne; m_charsetType=EDN_CHARSET_UTF8; };
	private:
		// Special mode of the buffer :
		bool        m_isUtf8;           //!< true if we are in UTF8 mode ==> if true the size of a char is 0, otherwise .. 1->4 ( TODO : not now)
		charset_te  m_charsetType;      //!< if UTF8 mode is at false : the charset type of the buffer
		// Local Tabulation policies
		int32_t     m_tabDist;          //!< equiv. number of characters in a tab
		bool        m_useTabs;          //!< True if buffer routines are allowed to use tabs for padding in rectangular operations
	
	// -----------------------------------------
	// Local function : 
	// -----------------------------------------
	private:
		void    findRectSelBoundariesForCopy(   int32_t lineStartPos, int32_t rectStart, int32_t rectEnd, int32_t *selStart, int32_t *selEnd);
		char *  getSelectionText(               selection &sel);
		void    removeSelected(                 selection &sel);
		void    replaceSelected(                selection &sel, const char *text);

		void    eventModification(              int32_t pos, int32_t nInserted, etk::VectorType<int8_t> &deletedText);


		int32_t insert(                         int32_t pos, etk::VectorType<int8_t> &insertText);
		
		bool    charMatch(                      char first, char second, bool caseSensitive = true);
};

#endif
