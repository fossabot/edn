/**
 *******************************************************************************
 * @file BufferText.h
 * @brief Editeur De N'ours : Text Buffer (edit only ASCII text File) (header)
 * @author Edouard DUPIN
 * @date 19/01/2011
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
 
#ifndef __BUFFER_TEXT_H__
#define __BUFFER_TEXT_H__

#include "ColorizeManager.h"
#include "Buffer.h"
#include "EdnBuf.h"

typedef enum {
	CURSOR_DISPLAY_MODE_NORMAL = 259,
	CURSOR_DISPLAY_MODE_INSERT,
	CURSOR_DISPLAY_MODE_NOT_FOCUS,
} cursorDisplayMode_te;

class BufferText : public Buffer {
	public:
		          BufferText(void);
		          BufferText(Edn::File &fileName);
		virtual  ~BufferText(void);
		void      Save(void);
		
		void      GetInfo(infoStatBuffer_ts &infoToUpdate);
		void      SetLineDisplay(uint32_t lineNumber);
		
		int32_t   Display(DrawerManager &drawer);
		void      ForceReDraw(bool allElement);
		void      AddChar(char * UTF8data);
		void      cursorMove(int32_t gtkKey);
		void      MouseSelectFromCursorTo(int32_t width, int32_t height);
		void      MouseEvent(int32_t width, int32_t height);
		void      MouseEventDouble(void);
		void      MouseEventTriple(void);
		void      ScrollDown(void);
		void      ScrollUp(void);

		void      Copy(int8_t clipboardID);
		void      Cut(int8_t clipboardID);
		void      Paste(int8_t clipboardID);

		void      Search(Edn::String &data, bool back, bool caseSensitive, bool wrap, bool regExp);
		void      Replace(Edn::String &data);
		int32_t   FindLine(Edn::String &data);
		void      JumpAtLine(int32_t newLine);
		void      RemoveLine(void);
		void      SelectAll(void);
		void      SelectNone(void);
		void      Undo(void);
		void      Redo(void);
		void      SetCharset(charset_te newCharset);
	protected:
		void      NameChange(void);

	private:
		// Display
		bool                    NeedToCleanEndPage;         //!< if true, the end of the page need to be clean (arrive after a remove line)
		uint32_t                nbColoneForLineNumber;      //!< number of colome used to display the line Number
		ColorizeManager *       myColorManager;             //!< for the background color : 
		
		
		// Direct buffer IO
		EdnBuf                  m_EdnBuf;                   //!< buffer associated on this displayer
		position_ts             m_displayStart;             //!< position where the display is starting
		position_ts             m_displaySize;              //!< number of char displayable in the screan
		int32_t                 m_displayStartBufferPos;    //!< position where the buffer start
		// Cursor :
		int32_t                 m_cursorPos;                //!< position in the buffer of the cursor
		int32_t                 m_cursorPreferredCol;       //!< colomn of the last up and down ...
		bool                    m_cursorOn;                 //!< the blink of the cursor ...
		cursorDisplayMode_te    m_cursorMode;               //!< type of cursor Selected
		
		displayHLData_ts        m_displayLocalSyntax;       //!< for the display of the local elements (display HL mode)
		
		// internal function
		void     BasicInit(void);
		void     UpdateWindowsPosition(bool centerPage = false);
		void     displayLineNumber(DrawerManager &drawer);
		
		void     CleanSelectLine(void);
		
		void     SelectionStart(void);
		void     SelectionEnd(void);
		void     SelectionCheckMode(void);
		
		void     CheckAndUpdateLineForModification(uint32_t lineID);
		bool     TextDMoveUp(int32_t offset);
		bool     TextDMoveDown(int32_t offset);
		void     SetInsertPosition(int32_t newPosition, bool insertChar = false);
		
		void     GetMousePosition(int32_t width, int32_t height, int32_t &x, int32_t &y);
		void     MoveUpDown(int32_t ofset);
		void     DrawLineNumber(DrawerManager &drawer,char *myPrint,  int32_t lineNumber, int32_t positionY);

};

#endif


