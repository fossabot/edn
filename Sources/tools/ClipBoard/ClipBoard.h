/**
 *******************************************************************************
 * @file ClipBoard.h
 * @brief Editeur De N'ours : copy / past main system (header)
 * @author Edouard DUPIN
 * @date 30/12/2010
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
 
#ifndef __CLIP_BOARD_H__
#define __CLIP_BOARD_H__

	#include "tools_debug.h"
	#include "VectorType.h"

	#define TOTAL_OF_CLICKBOARD			(11)
	#define COPY_MIDDLE_BUTTON			(10)
	#define COPY_STD					(0)

	namespace ClipBoard
	{
		void	Init(void);
		void	Set(uint8_t clipboardID, Edn::VectorType<int8_t> &data);
		void	Get(uint8_t clipboardID, Edn::VectorType<int8_t> &data);
	}

#endif


