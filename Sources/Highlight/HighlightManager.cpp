/**
 *******************************************************************************
 * @file HighlightManager.cpp
 * @brief Editeur De N'ours : Hightlining Manager
 * @author Edouard DUPIN
 * @date 16/12/2010
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

#include "tools_debug.h"
#include "tools_globals.h"
#include "HighlightManager.h"

#undef __class__
#define __class__	"HighlightManager"

HighlightManager::HighlightManager(void)
{

}

HighlightManager::~HighlightManager(void)
{
	listHighlight.clear();
}


Highlight *HighlightManager::Get(Edn::String &fileName)
{
	uint32_t i;
	for (i=0; i<listHighlight.size(); i++) {
		if (true == listHighlight[i]->FileNameCompatible(fileName) ) {
			return listHighlight[i];
		}
	}
	return NULL;
}

bool HighlightManager::Exist(Edn::String &fileName)
{
	uint32_t i;
	for (i=0; i<listHighlight.size(); i++) {
		if (true == listHighlight[i]->FileNameCompatible(fileName) ) {
			return true;
		}
	}
	return false;
}


void HighlightManager::loadLanguages(void)
{
	Edn::String homedir;
#ifdef NDEBUG
	homedir = getenv("HOME");
	homedir += "/.edn/";
#else
	homedir = "./";
#endif

	Edn::String xmlFilename = homedir;
	xmlFilename += "data/lang_c.xml";
	Highlight *myHightline = new Highlight(xmlFilename);
	listHighlight.push_back(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "data/lang_boo.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.push_back(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "data/lang_Makefile.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.push_back(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "data/lang_asm.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.push_back(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "data/lang_xml.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.push_back(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "data/lang_php.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.push_back(myHightline);
	
	myHightline->Display();
}

