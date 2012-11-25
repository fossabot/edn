/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __BUFFER_TEXT_H__
#define __BUFFER_TEXT_H__

#include <etk/UString.h>
#include <etk/os/FSNode.h>
#include <etk/unicode.h>
#include <ewol/ewol.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Text.h>

#include "ColorizeManager.h"
#include "EdnBuf.h"

typedef enum {
	CURSOR_DISPLAY_MODE_NORMAL = 259,
	CURSOR_DISPLAY_MODE_INSERT,
	CURSOR_DISPLAY_MODE_NOT_FOCUS,
} cursorDisplayMode_te;

class CharElement
{
	public:
		float    m_LineOffset; // TODO : DEPRECATED ...
		int16_t  m_yOffset;
		int16_t  m_xOffset;
		int16_t  m_ySize;
		int32_t  m_bufferPos;
};


typedef struct{
	uint32_t nbTotalLine;				//!< Number of line in the buffer
	uint32_t nbTotalColomn;				//!< Number of line in the buffer
	uint32_t startLineDisplay;			//!< First line display.
	uint32_t startColomnDisplay;		//!< First Colomn displayed
	uint32_t diplayableColomn;			//!< NB colomn that can be displayed
	uint32_t diplayableLine;			//!< NB Line that can be displayed
}infoStatBuffer_ts;


class BufferText
{
	private:
		bool                  m_fileModify;              //!< 
		// naming
		etk::FSNode           m_fileName;                //!< filename of the curent buffer
		bool                  m_haveName;                //!< to know if the file have a name or NOT
		bool                  m_updatePositionRequested; //!< if a position xhange in the windows ...
		etk::Vector2D<float>  m_maximumSize;             //!< current maxSize of the buffer
	public:
				void      SetModify(bool status);
		virtual etk::Vector2D<float>   GetBorderSize(void);
		void RequestUpdateOfThePosition(void) { m_updatePositionRequested = true; };
		void SetMaximumSize(etk::Vector2D<float>  maxSize) { m_maximumSize = maxSize; };
		bool                NeedToUpdateDisplayPosition(void);
		etk::Vector2D<float>           GetMaxSize(void) { return m_maximumSize; };
				bool      IsModify(void);
	public:
		etk::FSNode       GetFileName(void)
		{
			return m_fileName;
		};
		
		void SetFileName(etk::FSNode &newName)
		{
			m_fileName = newName;
			m_haveName = true;
			NameChange();
		};
		
		void SetFileName(etk::UString &newName)
		{
			m_fileName.SetName(newName);
			m_haveName = true;
			NameChange();
		};
		
		bool HaveName(void)
		{
			return m_haveName;
		}
	public:
		          BufferText(void);
		          BufferText(etk::FSNode &fileName);
		virtual  ~BufferText(void);
		void      Save(void);
		
		void      GetInfo(infoStatBuffer_ts &infoToUpdate);
		void      SetLineDisplay(uint32_t lineNumber);
		int32_t   Display(ewol::Text& OOText,
		                  int32_t offsetX, int32_t offsetY,
		                  int32_t sizeX, int32_t sizeY);
		void      AddChar(uniChar_t unicodeData);
		void      cursorMove(ewol::keyEvent::keyboard_te  moveTypeEvent);
		void      MouseSelectFromCursorTo(etk::Vector2D<float> pos);
		void      MouseEvent(etk::Vector2D<float> pos);
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
		etk::Vector2D<float>            m_displaySize;        //!< number of char displayable in the screan
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
		virtual etk::Vector2D<float> GetPosition(int32_t fontId, bool& centerRequested);
	private:
		bool     TextDMoveUp(int32_t offset);
		bool     TextDMoveDown(int32_t offset);
		void     SetInsertPosition(int32_t newPosition, bool insertChar = false);
		
		int32_t  GetMousePosition(etk::Vector2D<float> pos);
		
		void     DrawLineNumber(ewol::Text* OOText, int32_t sizeX, int32_t sizeY, int32_t nbColomn, int32_t lineNumber, int32_t positionY);

};

#endif


