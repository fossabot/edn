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
#include <etk/os/FSNode.h>
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

#ifdef APPL_BUFFER_FONT_DISTANCE_FIELD
	#define TEXT_DISPLAY_TYPE OObject2DTextShader
#else
	#define TEXT_DISPLAY_TYPE OObject2DTextColored
#endif

class Buffer {
	public:
		                  Buffer(void);
		                  Buffer(etk::FSNode &newName);
		virtual          ~Buffer(void);
		
		etk::FSNode       GetFileName(void)
		{
			return m_fileName;
		};
		
		void              SetFileName(etk::FSNode &newName)
		{
			m_fileName = newName;
			m_haveName = true;
			NameChange();
		};
		
		void              SetFileName(etk::UString &newName)
		{
			m_fileName.SetName(newName);
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
		virtual int32_t   Display(ewol::TEXT_DISPLAY_TYPE& OOText,
		                          ewol::OObject2DColored& OOColored, int32_t offsetX, int32_t offsetY, int32_t sizeX, int32_t sizeY)
		{
			return ERR_NONE;
		}
		virtual void      AddChar(uniChar_t unicodeData) {};
		virtual void      cursorMove(ewol::eventKbMoveType_te moveTypeEvent) {};
		virtual void      MouseSelectFromCursorTo(etk::Vector2D<float> pos) {};
		virtual void      MouseEvent(etk::Vector2D<float> pos) {};
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
		etk::Vector2D<float>  m_maximumSize;             //!< current maxSize of the buffer
	protected:
		void RequestUpdateOfThePosition(void) { m_updatePositionRequested = true; };
		void SetMaximumSize(etk::Vector2D<float>  maxSize) { m_maximumSize = maxSize; };
	public:
		bool                NeedToUpdateDisplayPosition(void);
		virtual etk::Vector2D<float>   GetBorderSize(void);               // this is to requested the minimum size for the buffer that is not consider as visible ...
		virtual etk::Vector2D<float>   GetPosition(int32_t fontId, bool& centerRequested);
		etk::Vector2D<float>           GetMaxSize(void) { return m_maximumSize; };
	protected:
		bool              m_fileModify;           //!< 
		// naming
		etk::FSNode       m_fileName;             //!< filename of the curent buffer
		bool              m_haveName;             //!< to know if the file have a name or NOT
};


#endif


