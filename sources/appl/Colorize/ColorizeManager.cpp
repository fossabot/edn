/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <ColorizeManager.h>
#include <exml/exml.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>
#include <etk/os/FSNode.h>

#define PFX	"ColorizeManager "

class classColorManager: public ewol::EObject {
	private:
		etk::UString m_fileColor;
		etk::Vector<Colorize*> listMyColor; //!< List of ALL Color
		Colorize * errorColor;
		etk::Color<> basicColors[COLOR_NUMBER_MAX];
		
	public:
		// Constructeur
		classColorManager(void) {
			//ewol::widgetMessageMultiCast::add(getWidgetId(), ednMsgGuiChangeColor);
		}
		~classColorManager(void) {
			delete(errorColor);
			
			int32_t i;
			// clean all Element
			for (i=0; i< listMyColor.size(); i++) {
				if (NULL != listMyColor[i]) {
					delete(listMyColor[i]);
					listMyColor[i] = NULL;
				}
			}
			// clear the compleate list
			listMyColor.clear();
		}
		
		const char * const getObjectType(void) {
			return "Appl::ColorManager";
		}
		void onReceiveMessage(const ewol::EMessage& _msg) {
			/*
			switch (id)
			{
				case APPL_MSG__RELOAD_COLOR_FILE:
					{
						// Reaload file
						// TODO : Check this : Pb in the recopy etk::UString element
						etk::UString plop = m_fileColor;
						loadFile(plop);
					}
					break;
			}
			*/
		}
	public:
		void loadFile(const etk::UString& _xmlFilename);
		Colorize* get(const etk::UString& _colorName) {
			int32_t i;
			for (i=0; i<listMyColor.size(); i++) {
				if (listMyColor[i]->getName() == _colorName) {
					return listMyColor[i];
				}
			}
			APPL_ERROR(PFX"Color does not Existed ["<< _colorName<<"]" );
			// an error
			return errorColor;
		}
		etk::Color<>& get(basicColor_te _myColor) {
			if (_myColor < COLOR_NUMBER_MAX) {
				return basicColors[_myColor];
			} else {
				return basicColors[0];
			}
		}
		bool exist(const etk::UString& _colorName) {
			int32_t i;
			for (i=0; i<listMyColor.size(); i++) {
				if (listMyColor[i]->getName() == _colorName) {
					return true;
				}
			}
			return false;
		}
		void displayListOfColor(void) {
			int32_t i;
			APPL_INFO(PFX"List of ALL COLOR : ");
			for (i=0; i<listMyColor.size(); i++) {
				//etk::UString elementName = listMyColor[i]->getName();
				//APPL_INFO(i << " : \"" <<  elementName.c_str() << "\"" );
				listMyColor[i]->display(i);
			}
		}
};

void classColorManager::loadFile(const etk::UString& _xmlFilename) {
	// remove all old color : 
	for (int32_t iii=0; iii< listMyColor.size(); iii++) {
		if (NULL != listMyColor[iii]) {
			delete(listMyColor[iii]);
			listMyColor[iii] = NULL;
		}
	}
	// clear the compleate list
	listMyColor.clear();

	m_fileColor = _xmlFilename;
	APPL_DEBUG("open file (COLOR) \"" << _xmlFilename << "\" ? = \"" << m_fileColor << "\"");
	errorColor = new Colorize();
	errorColor->setBgColor("#00FF00FF");
	errorColor->setFgColor("#FF00FFFF");

	// open the curent file
	etk::UString fileName(etk::UString("DATA:color/") + _xmlFilename + etk::UString(".xml"));
	exml::Document doc;
	if (doc.load(fileName) == false) {
		APPL_ERROR(" can not load file XML : " << fileName);
		return;
	}
	exml::Element* root = (exml::Element*)doc.getNamed("EdnColor");
	if (NULL == root ) {
		APPL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l ?) main node not find: \"EdnColor\" ...");
		return;
	}
	

	// parse all the elements :
	for(int32_t iii=0; iii< root->size(); iii++) {
		exml::Element* pNode = root->getElement(iii);
		if (pNode == NULL) {
			// trash here all that is not element.
			continue;
		}
		if (pNode->getValue() == "gui") {
			for(int32_t iii=0; iii< pNode->size(); iii++) {
				exml::Element* pGuiNode = pNode->getElement(iii);
				if (pGuiNode == NULL) {
					// trash here all that is not element.
					continue;
				}
				if (pGuiNode->getValue()!="color") {
					APPL_ERROR("(l "<<pGuiNode->getPos()<<") node not suported : \""<<pGuiNode->getValue()<<"\" must be [color]");
					continue;
				}
				//--------------------------------------------------------------------------------------------
				//<color name="basicBackground" val="#000000"/>
				//--------------------------------------------------------------------------------------------
				etk::UString colorName = pGuiNode->getAttribute("name");
				if (colorName.size() == 0) {
					APPL_ERROR("(l "<< pGuiNode->getPos() <<") node with no name");
					continue;
				}
				int32_t id = 0;
				if (colorName == "CODE_space") {
					id = COLOR_CODE_SPACE;
				} else if (colorName == "CODE_tabulation") {
					id = COLOR_CODE_TAB;
				} else if (colorName == "CODE_basicBackgroung") {
					id = COLOR_CODE_BASIC_BG;
				} else if (colorName == "CODE_cursor") {
					id = COLOR_CODE_CURSOR;
				} else if (colorName == "CODE_lineNumber") {
					id = COLOR_CODE_LINE_NUMBER;
				} else if (colorName == "LIST_backgroung1") {
					id = COLOR_LIST_BG_1;
				} else if (colorName == "LIST_backgroung2") {
					id = COLOR_LIST_BG_2;
				} else if (colorName == "LIST_backgroungSelected") {
					id = COLOR_LIST_BG_SELECTED;
				} else if (colorName == "LIST_textNormal") {
					id = COLOR_LIST_TEXT_NORMAL;
				} else if (colorName == "LIST_textModify") {
					id = COLOR_LIST_TEXT_MODIFY;
				} else {
					APPL_ERROR("(l "<<pGuiNode->getPos()<<") Unknown basic gui color : \"" << colorName << "\"" );
					continue;
				}
				etk::UString color = pGuiNode->getAttribute("val");
				if (color.size()!=0) {
					basicColors[id] = color;
				}
			}
		} else if (pNode->getValue() == "syntax") {
			for(int32_t iii=0; iii< pNode->size(); iii++) {
				exml::Element* pGuiNode = pNode->getElement(iii);
				if (pGuiNode == NULL) {
					continue;
				}
				if (pGuiNode->getValue()!="color") {
					APPL_ERROR(PFX"(l "<<pGuiNode->getPos()<<") node not suported : \""<<pGuiNode->getValue()<<"\" must be [color]");
					continue;
				}
				//--------------------------------------------------------------------------------------------
				//<color name="basicBackground"		FG="#000000"	BG="#000000"	bold="no"	italic="no"/>
				//--------------------------------------------------------------------------------------------
				// get the name of the Chaine
				etk::UString colorName = pGuiNode->getAttribute("name");
				if (colorName.size() == 0) {
					APPL_ERROR(PFX"(l "<< pGuiNode->getPos() <<") node with no name");
					continue;
				}
				Colorize* myNewColor = new Colorize();
				if (NULL == myNewColor) {
					APPL_ERROR(PFX"(l "<< pGuiNode->getPos() <<")  == > allocation error");
					continue;
				}
				myNewColor->setName(colorName);
				etk::UString colorBG = pGuiNode->getAttribute("BG");
				if (colorBG.size()!=0) {
					myNewColor->setBgColor(colorBG);
				}
				etk::UString colorFG = pGuiNode->getAttribute("FG");
				if (colorFG.size()!=0) {
					myNewColor->setFgColor(colorFG);
				}
				etk::UString bold = pGuiNode->getAttribute("bold");
				if (bold.size()!=0) {
					myNewColor->setBold(bold.toBool());
				}
				etk::UString italic = pGuiNode->getAttribute("italic");
				if (italic.size()!=0) {
					myNewColor->setItalic(italic.toBool());
				}
				listMyColor.pushBack(myNewColor);
			}
		}
	}
	//SendMessage(APPL_MSG__COLOR_HAS_CHANGE);
	//SendMessage(APPL_MSG__USER_DISPLAY_CHANGE);
}




static classColorManager * localManager = NULL;


void ColorizeManager::init(void) {
	if (NULL != localManager) {
		EWOL_ERROR("ColorizeManager  == > already exist, just unlink the previous ...");
		localManager = NULL;
	}
	localManager = new classColorManager();
	
	if (NULL == localManager) {
		EWOL_CRITICAL("Allocation of HighlightManager not done ...");
	}
}

void ColorizeManager::unInit(void) {
	if (NULL == localManager) {
		EWOL_ERROR("ColorizeManager  == > request UnInit, but does not exist ...");
		return;
	}
	delete(localManager);
	localManager = NULL;
}

void ColorizeManager::loadFile(const etk::UString& _xmlFilename) {
	if (NULL == localManager) {
		return;
	}
	localManager->loadFile(_xmlFilename);
}

Colorize* ColorizeManager::get(const etk::UString& _colorName) {
	if (NULL == localManager) {
		return NULL;
	}
	return localManager->get(_colorName);
}


etk::Color<>& ColorizeManager::get(basicColor_te _myColor) {
	static etk::Color<> errorColor;
	if (NULL == localManager) {
		return errorColor;
	}
	return localManager->get(_myColor);
}

bool ColorizeManager::exist(const etk::UString& _colorName) {
	if (NULL == localManager) {
		return false;
	}
	return localManager->exist(_colorName);
}

void ColorizeManager::displayListOfColor(void) {
	if (NULL == localManager) {
		return;
	}
	localManager->displayListOfColor();
}


