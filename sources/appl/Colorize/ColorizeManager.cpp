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
#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>
#include <etk/os/FSNode.h>

#define PFX	"ColorizeManager "

class classColorManager: public ewol::EObject
{
	private:
		etk::UString m_fileColor;
		etk::Vector<Colorize*> listMyColor; //!< List of ALL Color
		Colorize * errorColor;
		etk::Color<> basicColors[COLOR_NUMBER_MAX];
		
	public:
		// Constructeur
		classColorManager(void)
		{
			//ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgGuiChangeColor);
		}
		~classColorManager(void)
		{
			delete(errorColor);
			
			int32_t i;
			// clean all Element
			for (i=0; i< listMyColor.Size(); i++) {
				if (NULL != listMyColor[i]) {
					delete(listMyColor[i]);
					listMyColor[i] = NULL;
				}
			}
			// clear the compleate list
			listMyColor.Clear();
		}
		
		const char * const GetObjectType(void)
		{
			return "Appl::ColorManager";
		}
		void OnReceiveMessage(const ewol::EMessage& _msg)
		{
			/*
			switch (id)
			{
				case APPL_MSG__RELOAD_COLOR_FILE:
					{
						// Reaload File
						// TODO : Check this : Pb in the recopy etk::UString element
						etk::UString plop = m_fileColor;
						LoadFile(plop);
					}
					break;
			}
			*/
		}
	public:
		void LoadFile(const etk::UString& _xmlFilename);
		Colorize* Get(const etk::UString& _colorName)
		{
			int32_t i;
			for (i=0; i<listMyColor.Size(); i++) {
				if (listMyColor[i]->GetName() == _colorName) {
					return listMyColor[i];
				}
			}
			APPL_ERROR(PFX"Color does not Existed ["<< _colorName<<"]" );
			// an error
			return errorColor;
		}
		etk::Color<>& Get(basicColor_te _myColor)
		{
			if (_myColor < COLOR_NUMBER_MAX) {
				return basicColors[_myColor];
			} else {
				return basicColors[0];
			}
		}
		bool Exist(const etk::UString& _colorName)
		{
			int32_t i;
			for (i=0; i<listMyColor.Size(); i++) {
				if (listMyColor[i]->GetName() == _colorName) {
					return true;
				}
			}
			return false;
		}
		void DisplayListOfColor(void)
		{
			int32_t i;
			APPL_INFO(PFX"List of ALL COLOR : ");
			for (i=0; i<listMyColor.Size(); i++) {
				//etk::UString elementName = listMyColor[i]->GetName();
				//APPL_INFO(i << " : \"" <<  elementName.c_str() << "\"" );
				listMyColor[i]->Display(i);
			}
		}
};

void classColorManager::LoadFile(const etk::UString& _xmlFilename)
{
	// Remove all old color : 
	for (int32_t iii=0; iii< listMyColor.Size(); iii++) {
		if (NULL != listMyColor[iii]) {
			delete(listMyColor[iii]);
			listMyColor[iii] = NULL;
		}
	}
	// clear the compleate list
	listMyColor.Clear();

	m_fileColor = _xmlFilename;
	APPL_DEBUG("open file (COLOR) \"" << _xmlFilename << "\" ? = \"" << m_fileColor << "\"");
	errorColor = new Colorize();
	errorColor->SetBgColor("#00FF00FF");
	errorColor->SetFgColor("#FF00FFFF");

	// open the curent File
	etk::UString fileName(etk::UString("DATA:color/") + _xmlFilename + etk::UString(".xml"));
	exml::Document doc;
	if (doc.Load(fileName)==false) {
		APPL_ERROR(" can not load file XML : " << fileName);
		return;
	}
	exml::Element* root = (exml::Element*)doc.GetNamed("EdnColor");
	if (NULL == root ) {
		APPL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} (l ?) main node not find: \"EdnColor\" ...");
		return;
	}
	

	// parse all the elements :
	for(int32_t iii=0; iii< root->Size(); iii++) {
		exml::Element* pNode = root->GetElement(iii);
		if (pNode==NULL) {
			// trash here all that is not element.
			continue;
		}
		if (pNode->GetValue()=="gui") {
			for(int32_t iii=0; iii< pNode->Size(); iii++) {
				exml::Element* pGuiNode = pNode->GetElement(iii);
				if (pGuiNode==NULL) {
					// trash here all that is not element.
					continue;
				}
				if (pGuiNode->GetValue()!="color") {
					APPL_ERROR("(l "<<pGuiNode->GetPos()<<") node not suported : \""<<pGuiNode->GetValue()<<"\" must be [color]");
					continue;
				}
				//--------------------------------------------------------------------------------------------
				//<color name="basicBackground" val="#000000"/>
				//--------------------------------------------------------------------------------------------
				etk::UString colorName = pGuiNode->GetAttribute("name");
				if (colorName.Size()==0) {
					APPL_ERROR("(l "<< pGuiNode->GetPos() <<") node with no name");
					continue;
				}
				int32_t id = 0;
				if (colorName=="CODE_space") {
					id = COLOR_CODE_SPACE;
				} else if (colorName=="CODE_tabulation") {
					id = COLOR_CODE_TAB;
				} else if (colorName=="CODE_basicBackgroung") {
					id = COLOR_CODE_BASIC_BG;
				} else if (colorName=="CODE_cursor") {
					id = COLOR_CODE_CURSOR;
				} else if (colorName=="CODE_lineNumber") {
					id = COLOR_CODE_LINE_NUMBER;
				} else if (colorName=="LIST_backgroung1") {
					id = COLOR_LIST_BG_1;
				} else if (colorName=="LIST_backgroung2") {
					id = COLOR_LIST_BG_2;
				} else if (colorName=="LIST_backgroungSelected") {
					id = COLOR_LIST_BG_SELECTED;
				} else if (colorName=="LIST_textNormal") {
					id = COLOR_LIST_TEXT_NORMAL;
				} else if (colorName=="LIST_textModify") {
					id = COLOR_LIST_TEXT_MODIFY;
				} else {
					APPL_ERROR("(l "<<pGuiNode->GetPos()<<") Unknown basic gui color : \"" << colorName << "\"" );
					continue;
				}
				etk::UString color = pGuiNode->GetAttribute("val");
				if (color.Size()!=0) {
					basicColors[id] = color;
				}
			}
		} else if (pNode->GetValue()=="syntax") {
			for(int32_t iii=0; iii< pNode->Size(); iii++) {
				exml::Element* pGuiNode = pNode->GetElement(iii);
				if (pGuiNode==NULL) {
					continue;
				}
				if (pGuiNode->GetValue()!="color") {
					APPL_ERROR(PFX"(l "<<pGuiNode->GetPos()<<") node not suported : \""<<pGuiNode->GetValue()<<"\" must be [color]");
					continue;
				}
				//--------------------------------------------------------------------------------------------
				//<color name="basicBackground"		FG="#000000"	BG="#000000"	bold="no"	italic="no"/>
				//--------------------------------------------------------------------------------------------
				// get the name of the Chaine
				etk::UString colorName = pGuiNode->GetAttribute("name");
				if (colorName.Size()==0) {
					APPL_ERROR(PFX"(l "<< pGuiNode->GetPos() <<") node with no name");
					continue;
				}
				Colorize* myNewColor = new Colorize();
				if (NULL==myNewColor) {
					APPL_ERROR(PFX"(l "<< pGuiNode->GetPos() <<") ==> allocation error");
					continue;
				}
				myNewColor->SetName(colorName);
				etk::UString colorBG = pGuiNode->GetAttribute("BG");
				if (colorBG.Size()!=0) {
					myNewColor->SetBgColor(colorBG);
				}
				etk::UString colorFG = pGuiNode->GetAttribute("FG");
				if (colorFG.Size()!=0) {
					myNewColor->SetFgColor(colorFG);
				}
				etk::UString bold = pGuiNode->GetAttribute("bold");
				if (bold.Size()!=0) {
					myNewColor->SetBold(bold.ToBool());
				}
				etk::UString italic = pGuiNode->GetAttribute("italic");
				if (italic.Size()!=0) {
					myNewColor->SetItalic(italic.ToBool());
				}
				listMyColor.PushBack(myNewColor);
			}
		}
	}
	//SendMessage(APPL_MSG__COLOR_HAS_CHANGE);
	//SendMessage(APPL_MSG__USER_DISPLAY_CHANGE);
}




static classColorManager * localManager = NULL;


void ColorizeManager::Init(void)
{
	if (NULL != localManager) {
		EWOL_ERROR("ColorizeManager ==> already exist, just unlink the previous ...");
		localManager = NULL;
	}
	localManager = new classColorManager();
	
	if (NULL == localManager) {
		EWOL_CRITICAL("Allocation of HighlightManager not done ...");
	}
}

void ColorizeManager::UnInit(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("ColorizeManager ==> request UnInit, but does not exist ...");
		return;
	}
	delete(localManager);
	localManager = NULL;
}

void ColorizeManager::LoadFile(const etk::UString& _xmlFilename)
{
	if (NULL == localManager) {
		return;
	}
	localManager->LoadFile(_xmlFilename);
}

Colorize* ColorizeManager::Get(const etk::UString& _colorName)
{
	if (NULL == localManager) {
		return NULL;
	}
	return localManager->Get(_colorName);
}


etk::Color<>& ColorizeManager::Get(basicColor_te _myColor)
{
	static etk::Color<> errorColor;
	if (NULL == localManager) {
		return errorColor;
	}
	return localManager->Get(_myColor);
}

bool ColorizeManager::Exist(const etk::UString& _colorName)
{
	if (NULL == localManager) {
		return false;
	}
	return localManager->Exist(_colorName);
}

void ColorizeManager::DisplayListOfColor(void)
{
	if (NULL == localManager) {
		return;
	}
	localManager->DisplayListOfColor();
}


