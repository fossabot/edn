/**
 *******************************************************************************
 * @file SearchData.h
 * @brief Editeur De N'ours : Search Data element (header)
 * @author Edouard DUPIN
 * @date 02/02/2011
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

#ifndef __SEARCH_DATA_H__
#define __SEARCH_DATA_H__

#include <etk/UString.h>
#include <appl/Debug.h>

namespace SearchData
{
	void SetSearch(etk::UString &myData);
	void GetSearch(etk::UString &myData);
	bool IsSearchEmpty(void);
	void SetReplace(etk::UString &myData);
	void GetReplace(etk::UString &myData);
	bool IsReplaceEmpty(void);
	void SetCase(bool value);
	bool GetCase(void);
	void SetWrap(bool value);
	bool GetWrap(void);
	void SetRegExp(bool value);
	bool GetRegExp(void);
}


#endif
