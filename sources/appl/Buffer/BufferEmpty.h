/**
 *******************************************************************************
 * @file BufferEmpty.h
 * @brief Editeur De N'ours : Text Buffer (edit For No buffer Display) (header)
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
 
#ifndef __BUFFER_EMPTY_H__
#define __BUFFER_EMPTY_H__

#include <Buffer.h>

class BufferEmpty : public Buffer {
	public:
		BufferEmpty(void);
		virtual ~BufferEmpty(void);
		int32_t Display(ewol::Text& OOText,
		                ewol::Drawing& OOColored, int32_t offsetX, int32_t offsetY, int32_t sizeX, int32_t sizeY);
};


#endif


