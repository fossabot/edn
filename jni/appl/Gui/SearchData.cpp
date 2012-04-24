/**
 *******************************************************************************
 * @file SearchData.cpp
 * @brief Editeur De N'ours : Search Data element (Sources)
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


