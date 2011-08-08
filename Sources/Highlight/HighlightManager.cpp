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
	int32_t i;
	// clean all Element
	for (i=0; i< listHighlight.Size(); i++) {
		if (NULL != listHighlight[i]) {
			delete(listHighlight[i]);
			listHighlight[i] = NULL;
		}
	}
	// clear the compleate list
	listHighlight.Clear();
}


Highlight *HighlightManager::Get(Edn::File &fileName)
{
	int32_t i;
	for (i=0; i<listHighlight.Size(); i++) {
		if (true == listHighlight[i]->FileNameCompatible(fileName) ) {
			return listHighlight[i];
		}
	}
	return NULL;
}

bool HighlightManager::Exist(Edn::File &fileName)
{
	if (NULL != Get(fileName) ) {
		return true;
	}
	return false;
}


void HighlightManager::loadLanguages(void)
{
	Edn::String homedir;
#ifdef NDEBUG
	homedir = "/usr/share/edn/";
#else
	homedir = "./data/";
#endif

	Edn::String xmlFilename = homedir;
	xmlFilename += "lang_c.xml";
	Highlight *myHightline = new Highlight(xmlFilename);
	listHighlight.PushBack(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "lang_boo.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.PushBack(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "lang_Makefile.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.PushBack(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "lang_asm.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.PushBack(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "lang_xml.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.PushBack(myHightline);
	
	xmlFilename = homedir;
	xmlFilename += "lang_php.xml";
	myHightline = new Highlight(xmlFilename);
	listHighlight.PushBack(myHightline);
	
	myHightline->Display();
}

