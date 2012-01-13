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

#include <etk/String.h>
#include <etk/File.h>
#include <Display.h>
#include <charset.h>
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
		
		void              SetFileName(etk::String &newName)
		{
			m_fileName.SetCompleateName(newName, etk::FILE_TYPE_DIRECT);
			m_haveName = true;
			NameChange();
		};
		
		bool              HaveName(void)
		{
			return m_haveName;
		}
		
		virtual void      Save(void);
				bool      IsModify(void);
	protected:
				void      SetModify(bool status);
		virtual void      NameChange(void) { /*EDN_DEBUG("check name change ==> no HL change possible");*/};
	public:
		virtual void      GetInfo(infoStatBuffer_ts &infoToUpdate);
		virtual void      SetLineDisplay(uint32_t lineNumber);
		
		virtual int32_t   Display(ewol::OObject2DText* OOText, ewol::OObject2DColored* OOColored, int32_t sizeX, int32_t sizeY);
		virtual void      ForceReDraw(bool allElement);
		virtual void      AddChar(char * UTF8data);
		virtual void      cursorMove(int32_t gtkKey);
		virtual void      MouseSelectFromCursorTo(int32_t width, int32_t height);
		virtual void      MouseEvent(int32_t width, int32_t height);
		virtual void      MouseEventDouble(void);
		virtual void      MouseEventTriple(void);
		virtual void      ScrollDown(void);
		virtual void      ScrollUp(void);
		virtual void      RemoveLine(void);
		virtual void      SelectAll(void);
		virtual void      SelectNone(void);
		virtual void      Undo(void);
		virtual void      Redo(void);
		virtual void      SetCharset(charset_te newCharset) {};

		//virtual void	SelectAll(void);
		virtual void      Copy(int8_t clipboardID);
		virtual void      Cut(int8_t clipboardID);
		virtual void      Paste(int8_t clipboardID);
		virtual void      Search(etk::String &data, bool back, bool caseSensitive, bool wrap, bool regExp);
		virtual void      Replace(etk::String &data);
		virtual int32_t   FindLine(etk::String &data);
		virtual void      JumpAtLine(int32_t newLine);
		virtual int32_t   GetCurrentLine(void);
		
	protected:
		bool              m_fileModify;           //!< 
		// naming
		etk::File         m_fileName;             //!< filename of the curent buffer
		bool              m_haveName;             //!< to know if the file have a name or NOT
};


#endif


