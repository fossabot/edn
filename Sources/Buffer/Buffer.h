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

#include <string>
#include "Display.h"
#include "charset.h"
#include "Edn.h"
//#include "BufferAnchor.h"

extern "C"
{
	typedef struct{
		uint32_t nbTotalLine;				//!< Number of line in the buffer
		uint32_t nbTotalColomn;				//!< Number of line in the buffer
		uint32_t startLineDisplay;			//!< First line display.
		uint32_t startColomnDisplay;		//!< First Colomn displayed
		uint32_t diplayableColomn;			//!< NB colomn that can be displayed
		uint32_t diplayableLine;			//!< NB Line that can be displayed
	}infoStatBuffer_ts;
	
	typedef struct {
		int32_t      m_idAnchor;
		int32_t      m_lineId;
		int32_t      m_bufferPos;
	} bufferAnchorReference_ts;
	
	typedef struct {
		int32_t      m_lineNumber;
		int32_t      m_nbIterationMax;
		int32_t      m_posStart;
		int32_t      m_posStop;
		int32_t      m_selectionPosStart;
		int32_t      m_selectionPosStop;
	} bufferAnchor_ts;
}


class Buffer {
	public:
		                  Buffer(void);
		                  Buffer(Edn::File &newName);
		virtual          ~Buffer(void);
		
		Edn::File         GetFileName(void)
		{
			return m_fileName;
		};
		
		void              SetFileName(Edn::File &newName)
		{
			m_fileName = newName;
			m_haveName = true;
			NameChange();
		};
		
		void              SetFileName(Edn::String &newName)
		{
			m_fileName.SetCompleateName(newName);
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
		
		virtual void        DrawLine(DrawerManager &drawer, bufferAnchor_ts &anchor, position_ts &displayStart, position_ts &displaySize);
		// return the new cursor position ...
		virtual position_ts AddChar(char * UTF8data);
		virtual position_ts cursorMove(int32_t gtkKey);
		virtual position_ts MouseSelectFromCursorTo(int32_t width, int32_t height);
		virtual position_ts MouseEvent(int32_t width, int32_t height);
		virtual position_ts MouseEventDouble(void);
		virtual position_ts MouseEventTriple(void);
		virtual position_ts RemoveLine(void);
		virtual position_ts SelectAll(void);
		virtual position_ts SelectNone(void);
		virtual position_ts Undo(void);
		virtual position_ts Redo(void);
		
		virtual void        SetCharset(charset_te newCharset) {};
		
		virtual void        ScrollDown(void); // must be deprecated
		virtual void        ScrollUp(void);   // must be deprecated

		//virtual void	SelectAll(void);
		virtual void        Copy(int8_t clipboardID);
		virtual position_ts Cut(int8_t clipboardID);
		virtual position_ts Paste(int8_t clipboardID);
		virtual position_ts Search(Edn::String &data, bool back, bool caseSensitive, bool wrap, bool regExp);
		virtual position_ts Replace(Edn::String &data);
		virtual int32_t   FindLine(Edn::String &data);
		virtual position_ts JumpAtLine(int32_t newLine);
		virtual int32_t   GetCurrentLine(void);
		
	protected:
		bool              m_fileModify;           //!< 
		// naming
		Edn::File         m_fileName;             //!< filename of the curent buffer
		bool              m_haveName;             //!< to know if the file have a name or NOT

	// anchor section
	public:
		void              AnchorAdd(int32_t anchorID);
		void              AnchorRm(int32_t anchorID);
		virtual bool      AnchorGet(int32_t anchorID, bufferAnchor_ts & anchor, position_ts &size, int32_t sizePixelX, int32_t sizePixelY);
		virtual bool      AnchorNext(bufferAnchor_ts & anchor);
		
	protected:
		int32_t           AnchorRealId(int32_t anchorID);
		Edn::VectorType<bufferAnchorReference_ts> m_AnchorList;              //!< list of all line anchor in the current buffer

};


#endif


