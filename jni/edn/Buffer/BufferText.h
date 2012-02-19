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
		          BufferText(etk::File &fileName);
		virtual  ~BufferText(void);
		void      Save(void);
		
		void      GetInfo(infoStatBuffer_ts &infoToUpdate);
		void      SetLineDisplay(uint32_t lineNumber);
		
		int32_t   Display(ewol::OObject2DTextColored& OOTextNormal,
		                  ewol::OObject2DTextColored& OOTextBold,
		                  ewol::OObject2DTextColored& OOTextItalic,
		                  ewol::OObject2DTextColored& OOTextBoldItalic,
		                  ewol::OObject2DColored& OOColored,
		                  int32_t offsetX, int32_t offsetY,
		                  int32_t sizeX, int32_t sizeY);
		void      ForceReDraw(bool allElement);
		void      AddChar(uniChar_t unicodeData);
		void      cursorMove(ewol::eventKbMoveType_te moveTypeEvent);
		void      MouseSelectFromCursorTo(int32_t width, int32_t height);
		void      MouseEvent(int32_t width, int32_t height);
		void      MouseEventDouble(void);
		void      MouseEventTriple(void);
		void      ScrollDown(void);
		void      ScrollUp(void);

		void      Copy(int8_t clipboardID);
		void      Cut(int8_t clipboardID);
		void      Paste(int8_t clipboardID);

		void      Search(etk::UString &data, bool back, bool caseSensitive, bool wrap, bool regExp);
		void      Replace(etk::UString &data);
		int32_t   FindLine(etk::UString &data);
		void      JumpAtLine(int32_t newLine);
		int32_t   GetCurrentLine(void);
		
		void      RemoveLine(void);
		void      SelectAll(void);
		void      SelectNone(void);
		void      Undo(void);
		void      Redo(void);
		void      SetCharset(unicode::charset_te newCharset);
		int32_t   GetNumberOfLine(void);
	protected:
		void      NameChange(void);

	private:
		// Display
		bool                    NeedToCleanEndPage;         //!< if true, the end of the page need to be clean (arrive after a remove line)
		uint32_t                nbColoneForLineNumber;      //!< number of colome used to display the line Number
		
		// Direct buffer IO
		EdnBuf                  m_EdnBuf;                   //!< buffer associated on this displayer
		int32_t                 m_displayStartLineId;       //!< Id of the line that the display is starting (in nb line)
		int32_t                 m_displayStartPixelX;       //!< Pixel of the display starting (in pixel)
		position_ts             m_displaySize;              //!< number of char displayable in the screan
		int32_t                 m_displayStartBufferPos;    //!< position where the buffer start
		// Cursor :
		int32_t                 m_cursorPos;                //!< position in the buffer of the cursor
		int32_t                 m_cursorPreferredCol;       //!< colomn of the last up and down ...
		cursorDisplayMode_te    m_cursorMode;               //!< type of cursor Selected
		
		// internal function
		void     BasicInit(void);
		void     UpdateWindowsPosition(bool centerPage = false);
		
		void     CleanSelectLine(void);
		
		void     SelectionStart(void);
		void     SelectionEnd(void);
		void     SelectionCheckMode(void);
		
		void     CheckAndUpdateLineForModification(uint32_t lineID);
		bool     TextDMoveUp(int32_t offset);
		bool     TextDMoveDown(int32_t offset);
		void     SetInsertPosition(int32_t newPosition, bool insertChar = false);
		
		int32_t  GetMousePosition(int32_t width, int32_t height);
		void     MoveUpDown(int32_t ofset);
		void     DrawLineNumber(ewol::OObject2DTextColored* OOText, ewol::OObject2DColored* OOColored, int32_t sizeX, int32_t sizeY,char *myPrint,  int32_t lineNumber, int32_t positionY);
		
		void     CursorDisplay(ewol::OObject2DColored* OOColored, int32_t x, int32_t y, int32_t letterHeight, int32_t letterWidth, clipping_ts &clip);

};

#endif


