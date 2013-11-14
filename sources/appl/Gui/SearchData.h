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
#include <appl/debug.h>

namespace SearchData
{
	void setSearch(const std::string &myData);
	void getSearch(std::string &myData);
	bool isSearchEmpty(void);
	void setReplace(const std::string &myData);
	void getReplace(std::string &myData);
	bool isReplaceEmpty(void);
	void setCase(bool value);
	bool getCase(void);
	void setWrap(bool value);
	bool getWrap(void);
	void setRegExp(bool value);
	bool getRegExp(void);
}


#endif
