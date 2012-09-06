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

class CharElement
{
	public:
		int16_t m_yOffset;
		int16_t m_xOffset;
		int16_t m_ySize;
		int32_t m_bufferPos;
};

class BufferText : public Buffer {
	public:
		          BufferText(void);
		          BufferText(etk::File &fileName);
		virtual  ~BufferText(void);
		void      Save(void);
		
		void      GetInfo(infoStatBuffer_ts &infoToUpdate);
		void      SetLineDisplay(uint32_t lineNumber);
		
		int32_t   Display(ewol::TEXT_DISPLAY_TYPE& OOTextNormal,
		                  ewol::TEXT_DISPLAY_TYPE& OOTextBold,
		                  ewol::TEXT_DISPLAY_TYPE& OOTextItalic,
		                  ewol::TEXT_DISPLAY_TYPE& OOTextBoldItalic,
		                  ewol::OObject2DColored& OOColored,
		                  int32_t offsetX, int32_t offsetY,
		                  int32_t sizeX, int32_t sizeY);
		void      AddChar(uniChar_t unicodeData);
		void      cursorMove(ewol::eventKbMoveType_te moveTypeEvent);
		void      MouseSelectFromCursorTo(Vector2D<float> pos);
		void      MouseEvent(Vector2D<float> pos);
		void      MouseEventDouble(void);
		void      MouseEventTriple(void);

		void      Copy(ewol::clipBoard::clipboardListe_te clipboardID);
		void      Cut(ewol::clipBoard::clipboardListe_te clipboardID);
		void      Paste(ewol::clipBoard::clipboardListe_te clipboardID);

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
		int32_t   GetLineNumberNumberOfElement(void);
		
		// Direct buffer IO
		EdnBuf                          m_EdnBuf;             //!< buffer associated on this displayer
		Vector2D<float>                 m_displaySize;        //!< number of char displayable in the screan
		// Cursor :
		int32_t                         m_cursorPos;          //!< position in the buffer of the cursor
		int32_t                         m_cursorPreferredCol; //!< colomn of the last up and down ...
		cursorDisplayMode_te            m_cursorMode;         //!< type of cursor Selected
		etk::Vector<CharElement>        m_elmentList;         //!< Elemnt position for every char displayed
		
		// internal function
		void                    BasicInit(void);
	private:
		bool m_centerRequested;
	public:
		virtual Vector2D<float> GetPosition(int32_t fontId, bool& centerRequested);
	private:
		bool     TextDMoveUp(int32_t offset);
		bool     TextDMoveDown(int32_t offset);
		void     SetInsertPosition(int32_t newPosition, bool insertChar = false);
		
		int32_t  GetMousePosition(Vector2D<float> pos);
		
		void     DrawLineNumber(ewol::TEXT_DISPLAY_TYPE* OOText, ewol::OObject2DColored* OOColored, int32_t sizeX, int32_t sizeY, int32_t nbColomn, int32_t lineNumber, int32_t positionY);
		void     DrawCursor(ewol::OObject2DColored* OOColored, int32_t x, int32_t y, int32_t letterHeight, int32_t letterWidth, clipping_ts &clip);

};

#endif


