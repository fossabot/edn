/**
 *******************************************************************************
 * @file Highlight.c
 * @brief Editeur De N'ours : Hightlightning Specific
 * @author Edouard DUPIN
 * @date 14/12/2010
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
#include "Highlight.h"
#include "tinyxml.h"


#undef __class__
#define __class__	"Highlight"


void Highlight::ParseRules(TiXmlNode *child, std::vector<HighlightPattern*> &mListPatern, int32_t level)
{
	// Create the patern ...
	HighlightPattern *myPattern = new HighlightPattern();
	// parse under Element
	myPattern->ParseRules(child, level);
	// add element in the list
	mListPatern.push_back(myPattern);
}



Highlight::Highlight(Edn::String &xmlFilename)
{

	TiXmlDocument XmlDocument;
	// open the curent File
	bool loadError = XmlDocument.LoadFile(xmlFilename.c_str());
	if (false == loadError) {
		EDN_ERROR( "can not load Hightlight XML: PARSING error: \"" << xmlFilename.c_str() << "\"");
		return;
	}
	TiXmlElement* root = XmlDocument.FirstChildElement( "EdnLang" );
	if (NULL == root) {
		EDN_ERROR( "can not load Hightlight XML: main node not find: \"EdnLang\"");
		return;
	}

	int32_t level1 = 0;
	int32_t level2 = 0;
	TiXmlNode * child = root->FirstChild();
	while(NULL != child)
	{
		if (child->Type()==TiXmlNode::TINYXML_COMMENT) {
			// nothing to do ...
		} else if (!strcmp(child->Value(), "ext")) {
			const char *myData = child->ToElement()->GetText();
			if (NULL != myData) {
				//EDN_INFO(PFX"(l %d) node fined : %s=\"%s\"", child->Row(), child->Value() , myData);
				Edn::String myEdnData = myData;
				m_listExtentions.push_back(myEdnData);
			}
		} else if (!strcmp(child->Value(), "pass1")) {
			// Get sub Nodes ...
			TiXmlNode *passChild = child->FirstChild();
			while (NULL != passChild) {
				if (passChild->Type()==TiXmlNode::TINYXML_COMMENT) {
					// nothing to do ...
				} else if (!strcmp(passChild->Value(), "rule")) {
					ParseRules(passChild, m_listHighlightPass1, level1++);
				} else {
					EDN_ERROR("(l "<< passChild->Row() << ") node not suported : \""<< passChild->Value() << "\" must be [rule]" );
				}
				// get the next node element : 
				passChild = passChild->NextSibling();
			}
		} else if (!strcmp(child->Value(), "pass2")) {
			// Get sub Nodes ...
			TiXmlNode *passChild = child->FirstChild();
			while (NULL != passChild) {
				if (passChild->Type()==TiXmlNode::TINYXML_COMMENT) {
					// nothing to do ...
				} else if (!strcmp(passChild->Value(), "rule")) {
					ParseRules(passChild, m_listHighlightPass2, level2++);
				} else {
					EDN_ERROR("(l "<< passChild->Row() << ") node not suported : \""<< passChild->Value() << "\" must be [rule]" );
				}
				// get the next node element : 
				passChild = passChild->NextSibling();
			}
		} else {
			EDN_ERROR("(l "<< child->Row() << ") node not suported : \""<< child->Value() << "\" must be [ext,pass1,pass2]" );
		}
		// get the next node element : 
		child = child->NextSibling();
	}
}

Highlight::~Highlight(void)
{
	uint32_t i;
	// clean all Element
	for (i=0; i< m_listHighlightPass1.size(); i++) {
		delete(m_listHighlightPass1[i]);
	}
	// clear the compleate list
	m_listHighlightPass1.clear();
}


bool Highlight::HasExtention(Edn::String &ext)
{
	uint32_t i;
	for (i=0; i<m_listExtentions.size(); i++) {
		if (ext == m_listExtentions[i] ) {
			return true;
		}
	}
	return false;
}

bool Highlight::FileNameCompatible(Edn::String &fileName)
{
	uint32_t i;
	int32_t posCopy = fileName.FindBack('/');
	Edn::String shortFilename;
	if (-1 != posCopy) {
		shortFilename = fileName.Extract(posCopy+1);
	} else {
		shortFilename = fileName;
	}
	posCopy = shortFilename.FindBack('.');
	Edn::String extention;
	if (-1 != posCopy) {
		extention = shortFilename.Extract(posCopy);
	} else {
		extention = shortFilename;
	}
	EDN_DEBUG(" try to find : in \"" << fileName.c_str() << "\"  shortfilename\"" << shortFilename.c_str() << "\"  extention:\"" << extention.c_str() << "\" ");

	for (i=0; i<m_listExtentions.size(); i++) {
		if (extention == m_listExtentions[i] ) {
			return true;
		}
	}
	return false;
}


void Highlight::Display(void)
{
	uint32_t i;
	EDN_INFO("List of ALL Highlight : ");
	for (i=0; i< m_listExtentions.size(); i++) {
		EDN_INFO("        Extention : " << i << " : " << m_listExtentions[i].c_str() );
	}
	// Display all elements
	for (i=0; i< m_listHighlightPass1.size(); i++) {
		EDN_INFO("        " << i << " Pass 1 : " << m_listHighlightPass1[i]->GetName().c_str() );
		//m_listHighlightPass1[i]->Display();
	}
	// Display all elements
	for (i=0; i< m_listHighlightPass2.size(); i++) {
		EDN_INFO("        " << i << " Pass 2 : " << m_listHighlightPass2[i]->GetName().c_str() );
		//m_listHighlightPass2[i]->Display();
	}
}


void Highlight::Parse(int32_t start, int32_t stop, std::vector<colorInformation_ts> &metaData, int32_t &addingPos, EdnVectorBuf &buffer, int32_t elementID)
{
	if (0 > addingPos) {
		addingPos = 0;
	}
	/*int32_t emptyId = -1;
	for (i=0; i< (int32_t)metaData.size(); i++) {
		
	}*/
	//EDN_DEBUG("Parse element " << elementID << " / " << m_listHighlightPass1.size() << " ==> position search: (" << start << "," << stop << ")" );
	if (elementID >= (int32_t)m_listHighlightPass1.size() ){
		//EDN_DEBUG("Return at " << elementID << " / " << m_listHighlightPass1.size() );
		return;
	}
	int32_t elementStart = start;
	int32_t elementStop = stop;
	resultFind_te ret = HLP_FIND_OK;
	colorInformation_ts resultat;
	while (HLP_FIND_ERROR != ret && elementStart<elementStop) {
		ret = m_listHighlightPass1[elementID]->Find(elementStart, elementStop, resultat, buffer);
		if (HLP_FIND_ERROR != ret) {
			//EDN_INFO("Find Pattern in the Buffer : (" << resultat.beginStart << "," << resultat.endStop << ")" );
			// Add curent element in the list ...
			if (HLP_FIND_OK_NO_END == ret) {
				// find if we have a next element with th save Pointer and not higher the this one
				int32_t findNextElement = -1;
				int32_t i;
				int32_t curentLevel = ((HighlightPattern*)resultat.patern)->GetLevel();
				for (i=addingPos; i< (int32_t)metaData.size(); i++) {
					if (curentLevel > ((HighlightPattern*)metaData[i].patern)->GetLevel() ) {
						//EDN_DEBUG("    -> Find upper element at "<< i );
						break;
					} else if (curentLevel < ((HighlightPattern*)metaData[i].patern)->GetLevel() ) {
						findNextElement = i;
						//EDN_DEBUG("    -> Find under element at "<< i );
					}
					if (metaData[i].patern == resultat.patern)
					{
						findNextElement = i;
						//EDN_DEBUG("    -> Find a same element at "<< i );
						break;
					}
				}

				if (-1 != findNextElement) {
					// if not find a end, we need to search the end of this one and parse all data inside...
					int32_t newEnd   = buffer.Size();
					if (findNextElement >= (int32_t)metaData.size()-1) {
						// Remove old element : 
						//EDN_DEBUG("        --> Remove : " << addingPos << "==>" << (int32_t)metaData.size() << " (end)" );
						metaData.erase(metaData.begin()+addingPos,metaData.end());
					} else {
						// Remove old element : 
						//EDN_DEBUG("        --> Remove : " << addingPos << "==>" << findNextElement+1 );
						metaData.erase(metaData.begin()+addingPos,metaData.begin()+findNextElement+1);
						newEnd   = metaData[addingPos].beginStart-1;
					}
					// Regenerate a local parsing : in a higher range of text
					Parse(elementStart, edn_max(newEnd, stop), metaData, addingPos, buffer, elementID);
					// Break the curent process, beacause we reparse the data in all range...
					return;
				} else {
					//EDN_DEBUG("        --> No element removed " );
					metaData.insert(metaData.begin() + addingPos, resultat);
					//EDN_DEBUG("INSERT at "<< addingPos << " S=" << resultat.beginStart << " E=" << resultat.endStop );
				}
			} else {
				metaData.insert(metaData.begin() + addingPos, resultat);
				//EDN_DEBUG("INSERT at "<< addingPos << " S=" << resultat.beginStart << " E=" << resultat.endStop );
			}
			// parse the under element : 
			Parse(elementStart, resultat.beginStart-1, metaData, addingPos, buffer, elementID+1);
			addingPos++;
			elementStart = resultat.endStop;
		}
	}
	// parse the under element : 
	Parse(elementStart, elementStop, metaData, addingPos, buffer, elementID+1);
}

/**
 * @brief second pass of the hightlight
 *
 */
void Highlight::Parse2(int32_t start, int32_t stop, std::vector<colorInformation_ts> &metaData, EdnVectorBuf &buffer, int32_t elementID)
{
	if (elementID >= (int32_t)m_listHighlightPass2.size() ){
		return;
	}
	int32_t elementStart = start;
	int32_t elementStop = stop;
	resultFind_te ret = HLP_FIND_OK;
	colorInformation_ts resultat;
	while (HLP_FIND_ERROR != ret && elementStart<elementStop) {
		if (m_listHighlightPass2[elementID]!=NULL) {
			ret = m_listHighlightPass2[elementID]->Find(elementStart, elementStop, resultat, buffer);
		} else {
			ret = HLP_FIND_ERROR;
		}
		if (HLP_FIND_ERROR != ret) {
			//EDN_INFO("Find Pattern in the Buffer : (" << resultat.beginStart << "," << resultat.endStop << ")" );
			// parse the under element :
			Parse2(elementStart, resultat.beginStart, metaData, buffer, elementID+1);
			// Add curent element in the list ...
			metaData.push_back(resultat);
			elementStart = resultat.endStop;
		}
	}
	// parse the under element :
	Parse2(elementStart, elementStop, metaData, buffer, elementID+1);
}


