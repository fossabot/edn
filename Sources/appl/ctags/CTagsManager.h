/**
 *******************************************************************************
 * @file CTagsManager.h
 * @brief Editeur De N'ours : Ctags manager : acces to the ctags file (header)
 * @author Edouard DUPIN
 * @date 15/07/2011
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
 
#ifndef __C_TAGS_MANAGER_H__
#define __C_TAGS_MANAGER_H__


#include <ewol/widget/Widget.h>
#include "appl/globalMsg.h"
#include "readtags.h"

#define MAX_REG_EXP_SEARCH	(1024)

namespace cTagsManager
{
	void Init(void);
	void UnInit(void);
};


#endif
