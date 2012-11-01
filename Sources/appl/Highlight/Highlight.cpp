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

#include <appl/Debug.h>
#include <appl/global.h>
#include <Highlight.h>
#include <tinyXML/tinyxml.h>


#undef __class__
#define __class__	"Highlight"


void Highlight::ParseRules(TiXmlNode *child, etk::Vector<HighlightPattern*> &mListPatern, int32_t level)
{
	// Create the patern ...
	HighlightPattern *myPattern = new HighlightPattern();
	// parse under Element
	myPattern->ParseRules(child, level);
	// add element in the list
	mListPatern.PushBack(myPattern);
}



Highlight::Highlight(etk::UString &xmlFilename)
{
	TiXmlDocument XmlDocument;

	etk::FSNode fileName(etk::UString("DATA:") + xmlFilename);
	if (false == fileName.Exist()) {
		APPL_ERROR("File Does not exist : " << fileName);
		return;
	}
	int32_t fileSize = fileName.FileSize();
	if (0==fileSize) {
		APPL_ERROR("This file is empty : " << fileName);
		return;
	}
	if (false == fileName.FileOpenRead()) {
		APPL_ERROR("Can not open the file : " << fileName);
		return;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		APPL_ERROR("Error Memory allocation size=" << fileSize);
		return;
	}
	memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	fileName.FileRead(fileBuffer, 1, fileSize);
	// close the file:
	fileName.FileClose();
	// load the XML from the memory
	bool loadError = XmlDocument.Parse((const char*)fileBuffer, 0, TIXML_ENCODING_UTF8);
	if (false == loadError) {
		APPL_ERROR( "can not load Hightlight XML: PARSING error: \"" << xmlFilename << "\"");
		return;
	}


	TiXmlElement* root = XmlDocument.FirstChildElement( "EdnLang" );
	if (NULL == root) {
		APPL_ERROR( "can not load Hightlight XML: main node not find: \"EdnLang\"");
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
				//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", child->Row(), child->Value() , myData);
				etk::UString * myEdnData = new etk::UString(myData);
				m_listExtentions.PushBack(myEdnData);
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
					APPL_ERROR("(l "<< passChild->Row() << ") node not suported : \""<< passChild->Value() << "\" must be [rule]" );
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
					APPL_ERROR("(l "<< passChild->Row() << ") node not suported : \""<< passChild->Value() << "\" must be [rule]" );
				}
				// get the next node element : 
				passChild = passChild->NextSibling();
			}
		} else {
			APPL_ERROR("(l "<< child->Row() << ") node not suported : \""<< child->Value() << "\" must be [ext,pass1,pass2]" );
		}
		// get the next node element : 
		child = child->NextSibling();
	}
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
}

Highlight::~Highlight(void)
{
	int32_t i;
	// clean all Element
	for (i=0; i< m_listHighlightPass1.Size(); i++) {
		if (NULL != m_listHighlightPass1[i]) {
			delete(m_listHighlightPass1[i]);
			m_listHighlightPass1[i] = NULL;
		}
	}
	// clear the compleate list
	m_listHighlightPass1.Clear();

	// clean all Element
	for (i=0; i< m_listExtentions.Size(); i++) {
		if (NULL != m_listExtentions[i]) {
			delete(m_listExtentions[i]);
			m_listExtentions[i] = NULL;
		}
	}
	// clear the compleate list
	m_listExtentions.Clear();
}

void Highlight::ReloadColor(void)
{
	int32_t i;
	for (i=0; i< m_listHighlightPass1.Size(); i++) {
		if (NULL != m_listHighlightPass1[i]) {
			m_listHighlightPass1[i]->ReloadColor();
		}
	}
	for (i=0; i< m_listHighlightPass2.Size(); i++) {
		if (NULL != m_listHighlightPass2[i]) {
			m_listHighlightPass2[i]->ReloadColor();
		}
	}
}

bool Highlight::HasExtention(etk::UString &ext)
{
	int32_t i;
	for (i=0; i<m_listExtentions.Size(); i++) {
		if (ext == *m_listExtentions[i] ) {
			return true;
		}
	}
	return false;
}

bool Highlight::FileNameCompatible(etk::FSNode &fileName)
{
	int32_t i;
	etk::UString extention;
	if (true == fileName.FileHasExtention() ) {
		extention = "*.";
		extention += fileName.FileGetExtention();
	} else {
		extention = fileName.GetNameFile();
	}
	APPL_DEBUG(" try to find : in \"" << fileName << "\" extention:\"" << extention << "\" ");

	for (i=0; i<m_listExtentions.Size(); i++) {
		if (extention == *m_listExtentions[i] ) {
			return true;
		}
	}
	return false;
}


void Highlight::Display(void)
{
	int32_t i;
	APPL_INFO("List of ALL Highlight : ");
	for (i=0; i< m_listExtentions.Size(); i++) {
		APPL_INFO("        Extention : " << i << " : " << *m_listExtentions[i] );
	}
	// Display all elements
	for (i=0; i< m_listHighlightPass1.Size(); i++) {
		APPL_INFO("        " << i << " Pass 1 : " << m_listHighlightPass1[i]->GetName() );
		//m_listHighlightPass1[i]->Display();
	}
	// Display all elements
	for (i=0; i< m_listHighlightPass2.Size(); i++) {
		APPL_INFO("        " << i << " Pass 2 : " << m_listHighlightPass2[i]->GetName() );
		//m_listHighlightPass2[i]->Display();
	}
}
// 13h 46min 22s | (l=  214) Highlight::Parse                     | [II] Find Pattern in the Buffer : (2457,2479)


// TODO : Celui qui appelle suprime des element pour rien ... Enfin c'est pas trègrave... Il suffirait juste de suprimer celuis d'avant si il n'est pas terminer...
void Highlight::Parse(int32_t start,
                      int32_t stop,
                      etk::Vector<colorInformation_ts> &metaData,
                      int32_t addingPos,
                      EdnVectorBuf &buffer)
{
	if (0 > addingPos) {
		addingPos = 0;
	}
	//APPL_DEBUG("Parse element 0 => " << m_listHighlightPass1.Size() << " ==> position search: (" << start << "," << stop << ")" );
	int32_t elementStart = start;
	int32_t elementStop = stop;
	colorInformation_ts resultat;
	while (elementStart<elementStop) {
		//APPL_DEBUG("Parse element in the buffer id=" << elementStart);
		//try to fond the HL in ALL of we have
		for (int32_t jjj=0; jjj<m_listHighlightPass1.Size(); jjj++){
			resultFind_te ret = HLP_FIND_OK;
			//APPL_DEBUG("Parse HL id=" << jjj << " position search: (" << start << "," << buffer.Size() << ")" );
			// Stop the search to the end (to get the end of the pattern)
			ret = m_listHighlightPass1[jjj]->Find(elementStart, buffer.Size(), resultat, buffer);
			if (HLP_FIND_ERROR != ret) {
				//APPL_INFO("Find Pattern in the Buffer : (" << resultat.beginStart << "," << resultat.endStop << ")" );
				// Remove element in the current List where the current Element have a end inside the next...
				int32_t kkk=addingPos;
				while(kkk < metaData.Size() ) {
					if (metaData[kkk].beginStart <= resultat.endStop) {
						// Remove element
						//APPL_INFO("Erase element=" << kkk);
						metaData.EraseLen(kkk, kkk+1);
						// Increase the end of search
						if (kkk < metaData.Size()) {
							// just befor the end of the next element
							elementStop = metaData[kkk].beginStart-1;
						} else {
							// end of the buffer
							elementStop = buffer.Size();
						}
					} else {
						// Not find ==> exit the cycle : 
						break;
					}
				}
				// Add curent element in the list ...
				metaData.Insert(addingPos, resultat);
				//APPL_DEBUG("INSERT at "<< addingPos << " S=" << resultat.beginStart << " E=" << resultat.endStop );
				// Update the current research starting element: (Set position at the end of the current element
				elementStart = resultat.endStop-1;
				// increment the position of insertion:
				addingPos++;
				// We find a pattern ==> Stop search for the current element
				break;
			}
		}
		// Go to the next element (and search again ...).
		elementStart++;
	}
}


/**
 * @brief second pass of the hightlight
 *
 */
void Highlight::Parse2(int32_t start,
                       int32_t stop,
                       etk::Vector<colorInformation_ts> &metaData,
                       EdnVectorBuf &buffer)
{
	//APPL_DEBUG("Parse element 0 => " << m_listHighlightPass2.size() << " ==> position search: (" << start << "," << stop << ")" );
	int32_t elementStart = start;
	int32_t elementStop = stop;
	colorInformation_ts resultat;
	while (elementStart<elementStop) {
		//APPL_DEBUG("Parse element in the buffer id=" << elementStart);
		//try to fond the HL in ALL of we have
		int32_t jjj;
		for (jjj=0; jjj<m_listHighlightPass2.Size(); jjj++){
			resultFind_te ret = HLP_FIND_OK;
			//APPL_DEBUG("Parse HL id=" << jjj << " position search: (" << start << "," << buffer.Size() << ")" );
			// Stop the search to the end (to get the end of the pattern)
			ret = m_listHighlightPass2[jjj]->Find(elementStart, elementStop, resultat, buffer);
			if (HLP_FIND_ERROR != ret) {
				//APPL_INFO("Find Pattern in the Buffer : (" << resultat.beginStart << "," << resultat.endStop << ")" );
				// Add curent element in the list ...
				metaData.PushBack(resultat);
				elementStart = resultat.endStop-1;
				// Exit current cycle
				break;
			}
		}
		// Go to the next element (and search again ...).
		elementStart++;
	}
}

