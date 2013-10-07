/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/global.h>
#include <SearchData.h>


#undef __class__
#define __class__	"SearchData"


static etk::UString m_findRequest = "";

void SearchData::setSearch(const etk::UString &myData)
{
	m_findRequest = myData;
}
void SearchData::getSearch(etk::UString &myData)
{
	myData = m_findRequest;
}
bool SearchData::isSearchEmpty(void)
{
	if(m_findRequest.size() > 0) {
		return false;
	}
	return true;
}

static etk::UString m_replaceRequest = "";
void SearchData::setReplace(const etk::UString &myData)
{
	m_replaceRequest = myData;
}
void SearchData::getReplace(etk::UString &myData)
{
	myData = m_replaceRequest;
}
bool SearchData::isReplaceEmpty(void)
{
	if(m_replaceRequest.size() > 0) {
		return false;
	}
	return true;
}


static bool m_case = false;
void SearchData::setCase(bool value)
{
	m_case = value;
}
bool SearchData::getCase(void)
{
	return m_case;
}


static bool m_wrap = true;
void SearchData::setWrap(bool value)
{
	m_wrap = value;
}
bool SearchData::getWrap(void)
{
	return m_wrap;
}


static bool m_RegExp = false;
void SearchData::setRegExp(bool value)
{
	m_RegExp = value;
}
bool SearchData::getRegExp(void)
{
	return m_RegExp;
}


