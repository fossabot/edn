/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
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
