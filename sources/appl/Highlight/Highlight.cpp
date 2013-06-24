/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <Highlight.h>
#include <tinyXML/tinyxml.h>


#undef __class__
#define __class__	"Highlight"


void Highlight::ParseRules(exml::Element* child, etk::Vector<HighlightPattern*> &mListPatern, int32_t level)
{
	// Create the patern ...
	HighlightPattern *myPattern = new HighlightPattern();
	// parse under Element
	myPattern->ParseRules(child, level);
	// add element in the list
	mListPatern.PushBack(myPattern);
}



Highlight::Highlight(const etk::UString& _xmlFilename)
{
	exml::Document doc;
	if (doc.Load(_fileName)==false) {
		EWOL_ERROR(" can not load file XML : " << _fileName);
		return false;
	}
	exml::Element* root = (exml::Element*)doc.GetNamed("EdnLang");
	if (NULL == root ) {
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} (l ?) main node not find: \"composer\" ...");
		return false;
	}
	int32_t level1 = 0;
	int32_t level2 = 0;
	// parse all the elements :
	for(int32_t iii=0; iii< _node->Size(); iii++) {
		exml::Element* child = (exml::Element*)_node->Get(iii);
		if (child==NULL) {
			continue;
		}
		if (child->GetValue() == "ext") {
			const char *myData = child->GetText();
			if (NULL != myData) {
				//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", child->Row(), child->Value() , myData);
				etk::UString * myEdnData = new etk::UString(myData);
				m_listExtentions.PushBack(myEdnData);
			}
		} else if (child->GetValue()=="pass1") {
			// Get sub Nodes ...
			for(int32_t jjj=0; jjj< child->Size(); jjj++) {
				exml::Element* passChild = (exml::Element*)child->Get(jjj);
				if (passChild==NULL) {
					continue;
				}
				if (passChild->GetValue() != "rule") {
					APPL_ERROR("(l "<< passChild->Pos() << ") node not suported : \""<< passChild->GetValue() << "\" must be [rule]" );
					continue;
				}
				ParseRules(passChild, m_listHighlightPass1, level1++);
			}
		} else if (child->GetValue() == "pass2") {
			// Get sub Nodes ...
			for(int32_t jjj=0; jjj< child->Size(); jjj++) {
				exml::Element* passChild = (exml::Element*)child->Get(jjj);
				if (passChild==NULL) {
					continue;
				}
				if (passChild->GetValue() != "rule") {
					APPL_ERROR("(l "<< passChild->Pos() << ") node not suported : \""<< passChild->GetValue() << "\" must be [rule]" );
					continue;
				}
				ParseRules(passChild, m_listHighlightPass2, level2++);
			}
		} else {
			APPL_ERROR("(l "<< child->Pos() << ") node not suported : \""<< child->GetValue() << "\" must be [ext,pass1,pass2]" );
		}
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
                      etk::Buffer &buffer)
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
                       etk::Buffer &buffer)
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

