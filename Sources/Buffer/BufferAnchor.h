/**
 *******************************************************************************
 * @file BufferAnchor.h
 * @brief Editeur De N'ours : anchor on a secific buffer (header)
 * @author Edouard DUPIN
 * @date 11/09/2011
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

#ifndef __BUFFER_ANCHOR_H__
#define __BUFFER_ANCHOR_H__

#include "Display.h"
#include "charset.h"
#include "Edn.h"


class BufferAnchor
{
	public :
		BufferAnchor(void);
		~BufferAnchor(void);
		MoveAnchor(int32_t offsetLine);
		//BufModification(...);
		DisplayUpdateFirstLine(void);
		DisplayUpdateNextLine(void);
	private :
		int32_t      m_lineId;
		int32_t      m_bufferPos;
	public:
		int32_t      m_lineNumber;
		int32_t      m_posStart;
		int32_t      m_posStop;
		int32_t      m_selectionPosStart;
		int32_t      m_selectionPosStop;
};


#endif

