/**
 *******************************************************************************
 * @file BufferManager.h
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

#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__

#include <BufferText.h>
#include <appl/globalMsg.h>
#include <ewol/widget/Widget.h>

namespace BufferManager
{
	void        Init(void);
	void        UnInit(void);
	int32_t     GetSelected(void);
	BufferText* Get(int32_t BufferID);
	bool        Exist(int32_t BufferID);
	bool        Exist(etk::FSNode &myFile);
	int32_t     GetId(etk::FSNode &myFile);
	// return the number of buffer (open in the past) if 5 buffer open and 4 close ==> return 5
	uint32_t    Size(void);
	uint32_t    SizeOpen(void);
	int32_t     WitchBuffer(int32_t iEmeElement);
};

#endif

