/**
 *******************************************************************************
 * @file tools_globals.h
 * @brief Editeur De N'ours : Globals Values (header)
 * @author Edouard DUPIN
 * @date 05/12/2010
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
 
#ifndef __TOOLS_GLOBALS_H__
#define __TOOLS_GLOBALS_H__

#include <tools_debug.h>
#include <types_generique.h>


namespace globals
{
	erreurCode_te   init(void);
	int32_t         getNbColoneBorder(void);
	int32_t         getNbLineBorder(void);

	bool    IsSetDisplayEndOfLine(void);
	void    SetDisplayEndOfLine(bool newVal);
	
	bool    IsSetDisplaySpaceChar(void);
	void    SetDisplaySpaceChar(bool newVal);
	
	bool    IsSetAutoIndent(void);
	void    SetAutoIndent(bool newVal);

	void    init2(void);

}

#endif


