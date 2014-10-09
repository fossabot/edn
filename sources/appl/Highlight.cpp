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
#include <ewol/ewol.h>
#include <ewol/resource/Manager.h>


#undef __class__
#define __class__ "Highlight"

// first pass
//#define HL_DEBUG APPL_INFO
#define HL_DEBUG APPL_VERBOSE

// second pass
//#define HL2_DEBUG APPL_INFO
#define HL2_DEBUG APPL_VERBOSE

void appl::Highlight::parseRules(exml::Element* _child,
                                 std::vector<std::unique_ptr<HighlightPattern>>& _mListPatern,
                                 int32_t _level,
                                 bool forceMaximize) {
	// Create the patern ...
	HighlightPattern *myPattern = new HighlightPattern(m_paintingProperties);
	// parse under Element
	myPattern->parseRules(_child, _level, forceMaximize);
	// add element in the list
	_mListPatern.push_back(std::unique_ptr<HighlightPattern>(myPattern));
}

appl::Highlight::Highlight() {
	addObjectType("appl::Highlight");
}

void appl::Highlight::init(const std::string& _xmlFilename, const std::string& _colorFile) {
	ewol::Resource::init(_xmlFilename);
	
	// keep color propertiy file :
	m_paintingProperties = appl::GlyphPainting::create(_colorFile);
	
	exml::Document doc;
	if (doc.load(_xmlFilename) == false) {
		APPL_ERROR(" can not load file XML : " << _xmlFilename);
		return;
	}
	exml::Element* root = doc.getNamed("EdnLang");
	if (nullptr == root ) {
		APPL_ERROR("(l ?) main node not find: \"EdnLang\" ...");
		return;
	}
	m_typeName = root->getAttribute("lang");
	int32_t level1 = 0;
	int32_t level2 = 0;
	// parse all the elements :
	for(size_t iii = 0; iii < root->size(); ++iii) {
		exml::Element* child = root->getElement(iii);
		if (child == nullptr) {
			// trash here all that is not element ...
			continue;
		}
		if (child->getValue() == "ext") {
			std::string myData = child->getText();
			if (myData.size()!=0) {
				//HL_DEBUG("(l %d) node fined : %s=\"%s\"", child->Row(), child->Value() , myData);
				m_listExtentions.push_back(myData);
			}
		} else if (child->getValue() == "pass1") {
			// get sub Nodes ...
			for(size_t jjj=0; jjj< child->size(); jjj++) {
				exml::Element* passChild = child->getElement(jjj);
				if (passChild == nullptr) {
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
			for(size_t jjj=0; jjj< child->size(); jjj++) {
				exml::Element* passChild = child->getElement(jjj);
				if (passChild == nullptr) {
					continue;
				}
				if (passChild->getValue() != "rule") {
					APPL_ERROR("(l "<< passChild->getPos() << ") node not suported : \""<< passChild->getValue() << "\" must be [rule]" );
					continue;
				}
				parseRules(passChild, m_listHighlightPass2, level2++, true);
			}
		} else {
			APPL_ERROR("(l "<< child->getPos() << ") node not suported : \""<< child->getValue() << "\" must be [ext,pass1,pass2]" );
		}
	}
}

appl::Highlight::~Highlight() {
	// clear the compleate list
	m_listHighlightPass1.clear();
	// clear the compleate list
	m_listHighlightPass2.clear();
	// clear the compleate list
	m_listExtentions.clear();
}

bool appl::Highlight::isCompatible(const std::string& _name) {
	for (auto &it : m_listExtentions) {
		APPL_VERBOSE("        check : " << it << "=?=" << _name);
		std::regex expression;
		try {
			expression.assign(it, std::regex_constants::optimize | std::regex_constants::ECMAScript);
		} catch (std::regex_error e) {
			APPL_ERROR("can not parse regex : '" << e.what() << "' for : " << it);
			continue;
		}
		std::smatch resultMatch;
		std::regex_search(_name.begin(), _name.end(), resultMatch, expression, std::regex_constants::match_continuous);
		if (resultMatch.size() <= 0) {
			continue;
		}
		if (resultMatch[0].first != _name.begin()) {
			continue;
		}
		if (resultMatch[0].second != _name.end()) {
			continue;
		}
		return true;
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

	for (auto &it : m_listExtentions) {
		if (extention == it ) {
			return true;
		}
	}
	return false;
}


void appl::Highlight::display() {
	APPL_INFO("List of ALL Highlight : ");
	for (auto &it : m_listExtentions) {
		APPL_INFO("        Extention : " << it );
	}
	// display all elements
	for (auto &it : m_listHighlightPass1) {
		APPL_INFO("        Pass 1 : " << it->getName() );
		//m_listHighlightPass1[iii]->display();
	}
	// display all elements
	for (auto &it : m_listHighlightPass2) {
		APPL_INFO("        pass 2 : " << it->getName() );
		//m_listHighlightPass2[iii]->display();
	}
}

/* TODO : Celui qui appelle suprime des element pour rien ... Enfin c'est pas très grave... 
 * Il suffirait juste de suprimer celui d'avant si il n'est pas terminer...
 */
void appl::Highlight::parse(int64_t _start,
                            int64_t _stop,
                            std::vector<appl::HighlightInfo> & _metaData,
                            int64_t _addingPos,
                            std::string& _buffer) {
	if (0 > _addingPos) {
		_addingPos = 0;
	}
	HL_DEBUG("Parse element 0 => " << m_listHighlightPass1.size() << "  == > position search: (" << _start << "," << _stop << ")" );
	int64_t elementStart = _start;
	int64_t elementStop = _stop;
	appl::HighlightInfo resultat;
	int64_t startTime = ewol::getTime();
	while (elementStart <= elementStop) {
		//HL_DEBUG("Parse element in the buffer pos=" << elementStart);
		int64_t currentTime = ewol::getTime();
		//try to fond the HL in ALL of we have
		for (int64_t jjj=0; jjj<(int64_t)m_listHighlightPass1.size(); jjj++){
			enum resultFind ret = HLP_FIND_OK;
			/*
			if (_buffer[elementStart] == '\n') {
				HL_DEBUG("Parse HL id=" << jjj << " position search: (" << elementStart << "," << _stop << ") input start='\\n' " << m_listHighlightPass1[jjj]->getPaternString());
			} else {
				HL_DEBUG("Parse HL id=" << jjj << " position search: (" << elementStart << "," << _stop << ") input start='" << _buffer[elementStart] << "' " << m_listHighlightPass1[jjj]->getPaternString());
			}
			*/
			// Stop the search to the end (to get the end of the pattern)
			ret = m_listHighlightPass1[jjj]->find(elementStart, _buffer.size(), resultat, _buffer);
			if (HLP_FIND_ERROR != ret) {
				int64_t currentTimeEnd = ewol::getTime();
				int64_t deltaTime = currentTimeEnd - currentTime;
				HL_DEBUG("Find Pattern in the Buffer : time=" << (float)deltaTime/1000.0f << " ms (" << resultat.start << "," << resultat.stop << ") startPos=" << elementStart << " for=" << m_listHighlightPass1[jjj]->getPaternString());
				// remove element in the current List where the current Element have a end inside the next...
				int64_t kkk=_addingPos;
				while(kkk < (int64_t)_metaData.size() ) {
					if (_metaData[kkk].start <= resultat.stop) {
						// remove element
						HL_DEBUG("Erase element=" << kkk);
						_metaData.erase(_metaData.begin()+kkk, _metaData.begin()+kkk+1);
						// Increase the end of search
						if (kkk < (int64_t)_metaData.size()) {
							// just before the end of the next element
							elementStop = _metaData[kkk].start-1;
						} else {
							// end of the buffer
							elementStop = _buffer.size();
						}
					} else {
						// Not find  == > exit the cycle : 
						break;
					}
				}
				// add curent element in the list ...
				_metaData.insert(_metaData.begin()+_addingPos, resultat);
				HL_DEBUG("INSERT at "<< _addingPos << " S=" << resultat.start << " E=" << resultat.stop );
				// update the current research starting element: (set position at the end of the current element
				elementStart = resultat.stop-1;
				// increment the position of insertion:
				_addingPos++;
				// We find a pattern  == > Stop search for the current element
				break;
			}
		}
		// Go to the next element (and search again ...).
		elementStart++;
	}
	int64_t stopTime = ewol::getTime();
	int64_t deltaTimeGlobal = stopTime - startTime;
	APPL_DEBUG("parse in time=" << (float)deltaTimeGlobal/1000.0f << " ms ");
	
	
}


/**
 * @brief second pass of the hightlight
 *
 */
void appl::Highlight::parse2(int64_t _start,
                             int64_t _stop,
                             std::vector<appl::HighlightInfo> &_metaData,
                             std::string&_buffer) {
	HL2_DEBUG("Parse element 0 => " << m_listHighlightPass2.size() <<
	          "  == > position search: (" << _start << "," << _stop << ")" );
	int64_t elementStart = _start;
	int64_t elementStop = _stop;
	appl::HighlightInfo resultat;
	
	while (elementStart < elementStop) {
		if (elementStart == 306) {
			//etk::log::setLevel(etk::log::logLevelVerbose);
		}
		//HL2_DEBUG("Parse element in the buffer pos=" << elementStart << "," << _buffer.size() << ")" );
		//try to fond the HL in ALL of we have
		for (int64_t jjj=0; jjj<(int64_t)m_listHighlightPass2.size(); jjj++){
			enum resultFind ret;
			HL2_DEBUG("Parse HL id=" << jjj << " position search: (" <<
			         elementStart << "," << elementStop << ") in='"
			         << _buffer[elementStart] << "' " << m_listHighlightPass2[jjj]->getPaternString());
			// Stop the search to the end (to get the end of the pattern)
			ret = m_listHighlightPass2[jjj]->find(elementStart, elementStop, resultat, _buffer);
			if (ret != HLP_FIND_ERROR) {
				_metaData.push_back(resultat);
				elementStart = resultat.stop-1;
				break;
			}
		}
		// Go to the next element (and search again ...).
		elementStart++;
	}
}
