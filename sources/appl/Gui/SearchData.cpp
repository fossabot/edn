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

void SearchData::SetSearch(etk::UString &myData)
{
	m_findRequest = myData;
}
void SearchData::GetSearch(etk::UString &myData)
{
	myData = m_findRequest;
}
bool SearchData::IsSearchEmpty(void)
{
	if(m_findRequest.Size() > 0) {
		return false;
	}
	return true;
}

static etk::UString m_replaceRequest = "";
void SearchData::SetReplace(etk::UString &myData)
{
	m_replaceRequest = myData;
}
void SearchData::GetReplace(etk::UString &myData)
{
	myData = m_replaceRequest;
}
bool SearchData::IsReplaceEmpty(void)
{
	if(m_replaceRequest.Size() > 0) {
		return false;
	}
	return true;
}


static bool m_case = false;
void SearchData::SetCase(bool value)
{
	m_case = value;
}
bool SearchData::GetCase(void)
{
	return m_case;
}


static bool m_wrap = true;
void SearchData::SetWrap(bool value)
{
	m_wrap = value;
}
bool SearchData::GetWrap(void)
{
	return m_wrap;
}


static bool m_RegExp = false;
void SearchData::SetRegExp(bool value)
{
	m_RegExp = value;
}
bool SearchData::GetRegExp(void)
{
	return m_RegExp;
}


