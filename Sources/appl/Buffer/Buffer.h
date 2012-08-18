/**
 *******************************************************************************
 * @file Buffer.h
 * @brief Editeur De N'ours : Text Buffer (header)
 * @author Edouard DUPIN
 * @date 08/12/2010
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
 
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <etk/UString.h>
#include <etk/File.h>
#include <etk/unicode.h>
#include <ewol/ewol.h>


typedef struct{
	uint32_t nbTotalLine;				//!< Number of line in the buffer
	uint32_t nbTotalColomn;				//!< Number of line in the buffer
	uint32_t startLineDisplay;			//!< First line display.
	uint32_t startColomnDisplay;		//!< First Colomn displayed
	uint32_t diplayableColomn;			//!< NB colomn that can be displayed
	uint32_t diplayableLine;			//!< NB Line that can be displayed
}infoStatBuffer_ts;


class Buffer {
	public:
		                  Buffer(void);
		                  Buffer(etk::File &newName);
		virtual          ~Buffer(void);
		
		etk::File         GetFileName(void)
		{
			return m_fileName;
		};
		
		void              SetFileName(etk::File &newName)
		{
			m_fileName = newName;
			m_haveName = true;
			NameChange();
		};
		
		void              SetFileName(etk::UString &newName)
		{
			m_fileName.SetCompleateName(newName, etk::FILE_TYPE_DIRECT);
			m_haveName = true;
			NameChange();
		};
		
		bool              HaveName(void)
		{
			return m_haveName;
		}
		
		virtual void      Save(void) {};
				bool      IsModify(void);
	protected:
				void      SetModify(bool status);
		virtual void      NameChange(void) { /*APPL_DEBUG("check name change ==> no HL change possible");*/};
	public:
		virtual void      GetInfo(infoStatBuffer_ts &infoToUpdate) {};
		virtual void      SetLineDisplay(uint32_t lineNumber) {};
		
		virtual int32_t   Display(ewol::OObject2DTextColored& OOTextNormal,
		                          ewol::OObject2DTextColored& OOTextBold,
		                          ewol::OObject2DTextColored& OOTextItalic,
		                          ewol::OObject2DTextColored& OOTextBoldItalic,
		                          ewol::OObject2DColored& OOColored, int32_t offsetX, int32_t offsetY, int32_t sizeX, int32_t sizeY)
		{
			return ERR_NONE;
		}
		virtual void      AddChar(uniChar_t unicodeData) {};
		virtual void      cursorMove(ewol::eventKbMoveType_te moveTypeEvent) {};
		virtual void      MouseSelectFromCursorTo(int32_t fontId, int32_t width, int32_t height) {};
		virtual void      MouseEvent(int32_t fontId, int32_t width, int32_t height) {};
		virtual void      MouseEventDouble(void) {};
		virtual void      MouseEventTriple(void) {};
		virtual void      RemoveLine(void) {};
		virtual void      SelectAll(void) {};
		virtual void      SelectNone(void) {};
		virtual void      Undo(void) {};
		virtual void      Redo(void) {};
		virtual void      SetCharset(unicode::charset_te newCharset) {};

		//virtual void	SelectAll(void);
		virtual void      Copy(ewol::clipBoard::clipboardListe_te clipboardID) {};
		virtual void      Cut(ewol::clipBoard::clipboardListe_te clipboardID) {};
		virtual void      Paste(ewol::clipBoard::clipboardListe_te clipboardID) {};
		virtual void      Search(etk::UString &data, bool back, bool caseSensitive, bool wrap, bool regExp) {};
		virtual void      Replace(etk::UString &data) {};
		virtual int32_t   FindLine(etk::UString &data) { return 0; };
		virtual void      JumpAtLine(int32_t newLine) {};
		virtual int32_t   GetCurrentLine(void) { return 0; };
		
		virtual int32_t   GetNumberOfLine(void) { return 1; };
		
		// moving with cursor change position:
	private:
		bool       m_updatePositionRequested; //!< if a position xhange in the windows ...
		Vector2D<float>  m_maximumSize;             //!< current maxSize of the buffer
	protected:
		void RequestUpdateOfThePosition(void) { m_updatePositionRequested = true; };
		void SetMaximumSize(Vector2D<float>  maxSize) { m_maximumSize = maxSize; };
	public:
		bool                NeedToUpdateDisplayPosition(void);
		virtual Vector2D<float>   GetBorderSize(void);               // this is to requested the minimum size for the buffer that is not consider as visible ...
		virtual Vector2D<float>   GetPosition(int32_t fontId, bool& centerRequested);
		Vector2D<float>           GetMaxSize(void) { return m_maximumSize; };
	protected:
		bool              m_fileModify;           //!< 
		// naming
		etk::File         m_fileName;             //!< filename of the curent buffer
		bool              m_haveName;             //!< to know if the file have a name or NOT
};


#endif


