/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/Highlight.h>
#include <exml/exml.h>
#include <ewol/resources/ResourceManager.h>


#undef __class__
#define __class__ "Highlight"


void appl::Highlight::parseRules(exml::Element* _child,
                           std::vector<HighlightPattern*>& _mListPatern,
                           int32_t _level) {
	// Create the patern ...
	HighlightPattern *myPattern = new HighlightPattern(m_paintingProperties);
	// parse under Element
	myPattern->parseRules(_child, _level);
	// add element in the list
	_mListPatern.push_back(myPattern);
}

appl::Highlight::Highlight(const std::string& _xmlFilename, const std::string& _colorFile) :
  ewol::Resource(_xmlFilename),
  m_typeName("") {
	// keep color propertiy file :
	m_paintingProperties = appl::GlyphPainting::keep(_colorFile);
	
	exml::Document doc;
	if (doc.load(_xmlFilename) == false) {
		APPL_ERROR(" can not load file XML : " << _xmlFilename);
		return;
	}
	exml::Element* root = doc.getNamed("EdnLang");
	if (NULL == root ) {
		APPL_ERROR("(l ?) main node not find: \"EdnLang\" ...");
		return;
	}
	m_typeName = root->getAttribute("lang");
	int32_t level1 = 0;
	int32_t level2 = 0;
	// parse all the elements :
	for(int32_t iii = 0; iii < root->size(); ++iii) {
		exml::Element* child = root->getElement(iii);
		if (child == NULL) {
			// trash here all that is not element ...
			continue;
		}
		if (child->getValue() == "ext") {
			std::string myData = child->getText();
			if (myData.size()!=0) {
				//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", child->Row(), child->Value() , myData);
				m_listExtentions.push_back(myData);
			}
		} else if (child->getValue() == "pass1") {
			// get sub Nodes ...
			for(int32_t jjj=0; jjj< child->size(); jjj++) {
				exml::Element* passChild = child->getElement(jjj);
				if (passChild == NULL) {
					continue;
				}
				if (passChild->getValue() != "rule") {
					APPL_ERROR("(l "<< passChild->getPos() << ") node not suported : \""<< passChild->getValue() << "\" must be [rule]" );
					continue;
				}
				parseRules(passChild, m_listHighlightPass1, level1++);
			}
		} else if (child->getValue() == "pass2") {
			// get sub Nodes ...
			for(int32_t jjj=0; jjj< child->size(); jjj++) {
				exml::Element* passChild = child->getElement(jjj);
				if (passChild == NULL) {
					continue;
				}
				if (passChild->getValue() != "rule") {
					APPL_ERROR("(l "<< passChild->getPos() << ") node not suported : \""<< passChild->getValue() << "\" must be [rule]" );
					continue;
				}
				parseRules(passChild, m_listHighlightPass2, level2++);
			}
		} else {
			APPL_ERROR("(l "<< child->getPos() << ") node not suported : \""<< child->getValue() << "\" must be [ext,pass1,pass2]" );
		}
	}
}

appl::Highlight::~Highlight(void) {
	int32_t i;
	// clean all Element
	for (esize_t iii = 0; iii < m_listHighlightPass1.size(); ++iii) {
		if (m_listHighlightPass1[iii] != NULL) {
			delete(m_listHighlightPass1[iii]);
			m_listHighlightPass1[iii] = NULL;
		}
	}
	// clear the compleate list
	m_listHighlightPass1.clear();
	// clear the compleate list
	m_listExtentions.clear();
}

bool appl::Highlight::hasExtention(const std::string& _ext) {
	for (int32_t iii=0; iii<m_listExtentions.size(); iii++) {
		APPL_VERBOSE("        check : " << m_listExtentions[iii] << "=?=" << _ext);
		if (    m_listExtentions[iii] == "*." + _ext
		     || m_listExtentions[iii] == _ext) {
			return true;
		}
	}
	return false;
}

bool appl::Highlight::fileNameCompatible(const std::string& _fileName) {
	std::string extention;
	etk::FSNode file(_fileName);
	if (true == file.fileHasExtention() ) {
		extention = "*.";
		extention += file.fileGetExtention();
	} else {
		extention = file.getNameFile();
	}
	APPL_DEBUG(" try to find : in \"" << file << "\" extention:\"" << extention << "\" ");

	for (int32_t iii=0; iii<m_listExtentions.size(); iii++) {
		if (extention == m_listExtentions[iii] ) {
			return true;
		}
	}
	return false;
}


void appl::Highlight::display(void) {
	APPL_INFO("List of ALL Highlight : ");
	for (int32_t iii=0; iii< m_listExtentions.size(); iii++) {
		APPL_INFO("        Extention : " << iii << " : " << m_listExtentions[iii] );
	}
	// display all elements
	for (int32_t iii=0; iii< m_listHighlightPass1.size(); iii++) {
		APPL_INFO("        " << iii << " Pass 1 : " << m_listHighlightPass1[iii]->getName() );
		//m_listHighlightPass1[iii]->display();
	}
	// display all elements
	for (int32_t iii=0; iii< m_listHighlightPass2.size(); iii++) {
		APPL_INFO("        " << iii << " Pass 2 : " << m_listHighlightPass2[iii]->getName() );
		//m_listHighlightPass2[iii]->display();
	}
}

/* TODO : Celui qui appelle suprime des element pour rien ... Enfin c'est pas très grave... 
 * Il suffirait juste de suprimer celui d'avant si il n'est pas terminer...
 */
void appl::Highlight::parse(int64_t start,
                      int64_t stop,
                      std::vector<appl::HighlightInfo> &metaData,
                      int64_t addingPos,
                      etk::Buffer &buffer) {
	if (0 > addingPos) {
		addingPos = 0;
	}
	//APPL_DEBUG("Parse element 0 => " << m_listHighlightPass1.size() << "  == > position search: (" << start << "," << stop << ")" );
	int64_t elementStart = start;
	int64_t elementStop = stop;
	appl::HighlightInfo resultat;
	while (elementStart<elementStop) {
		//APPL_DEBUG("Parse element in the buffer id=" << elementStart);
		//try to fond the HL in ALL of we have
		for (int64_t jjj=0; jjj<m_listHighlightPass1.size(); jjj++){
			enum resultFind ret = HLP_FIND_OK;
			//APPL_DEBUG("Parse HL id=" << jjj << " position search: (" << start << "," << buffer.size() << ")" );
			// Stop the search to the end (to get the end of the pattern)
			ret = m_listHighlightPass1[jjj]->find(elementStart, buffer.size(), resultat, buffer);
			if (HLP_FIND_ERROR != ret) {
				//APPL_INFO("Find Pattern in the Buffer : (" << resultat.beginStart << "," << resultat.endStop << ")" );
				// remove element in the current List where the current Element have a end inside the next...
				int64_t kkk=addingPos;
				while(kkk < metaData.size() ) {
					if (metaData[kkk].beginStart <= resultat.endStop) {
						// remove element
						//APPL_INFO("Erase element=" << kkk);
						metaData.erase(metaData.begin()+kkk, metaData.begin()+kkk+1);
						// Increase the end of search
						if (kkk < metaData.size()) {
							// just befor the end of the next element
							elementStop = metaData[kkk].beginStart-1;
						} else {
							// end of the buffer
							elementStop = buffer.size();
						}
					} else {
						// Not find  == > exit the cycle : 
						break;
					}
				}
				// add curent element in the list ...
				metaData.insert(metaData.begin()+addingPos, resultat);
				//APPL_DEBUG("INSERT at "<< addingPos << " S=" << resultat.beginStart << " E=" << resultat.endStop );
				// update the current research starting element: (set position at the end of the current element
				elementStart = resultat.endStop-1;
				// increment the position of insertion:
				addingPos++;
				// We find a pattern  == > Stop search for the current element
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
void appl::Highlight::parse2(int64_t start,
                       int64_t stop,
                       std::vector<appl::HighlightInfo> &metaData,
                       etk::Buffer &buffer) {
	//APPL_DEBUG("Parse element 0 => " << m_listHighlightPass2.size() << "  == > position search: (" << start << "," << stop << ")" );
	int64_t elementStart = start;
	int64_t elementStop = stop;
	appl::HighlightInfo resultat;
	while (elementStart<elementStop) {
		//APPL_DEBUG("Parse element in the buffer id=" << elementStart);
		//try to fond the HL in ALL of we have
		for (int64_t jjj=0; jjj<m_listHighlightPass2.size(); jjj++){
			enum resultFind ret = HLP_FIND_OK;
			//APPL_DEBUG("Parse HL id=" << jjj << " position search: (" << start << "," << buffer.size() << ")" );
			// Stop the search to the end (to get the end of the pattern)
			ret = m_listHighlightPass2[jjj]->find(elementStart, elementStop, resultat, buffer);
			if (HLP_FIND_ERROR != ret) {
				//APPL_INFO("Find Pattern in the Buffer : (" << resultat.beginStart << "," << resultat.endStop << ")" );
				// add curent element in the list ...
				metaData.push_back(resultat);
				elementStart = resultat.endStop-1;
				// Exit current cycle
				break;
			}
		}
		// Go to the next element (and search again ...).
		elementStart++;
	}
}

appl::Highlight* appl::Highlight::keep(const std::string& _filename) {
	//EWOL_INFO("KEEP : appl::Highlight : file : \"" << _filename << "\"");
	appl::Highlight* object = static_cast<appl::Highlight*>(getManager().localKeep(_filename));
	if (NULL != object) {
		return object;
	}
	EWOL_INFO("CREATE : appl::Highlight : file : \"" << _filename << "\"");
	// this element create a new one every time ....
	object = new appl::Highlight(_filename, "THEME:COLOR:textViewer.json");
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??Highlight??");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
	
}

void appl::Highlight::release(appl::Highlight*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}
