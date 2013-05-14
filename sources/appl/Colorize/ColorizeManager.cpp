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
#include <tinyXML/tinyxml.h>
#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>
#include <etk/os/FSNode.h>

#define PFX	"ColorizeManager "

class classColorManager: public ewol::EObject
{
	public:
		// Constructeur
		classColorManager(void);
		~classColorManager(void);
		
		const char * const GetObjectType(void)
		{
			return "ApplColorManager";
		}
		virtual void OnReceiveMessage(const ewol::EMessage& _msg);
	public:
		void LoadFile(const etk::UString& _xmlFilename);
		appl::Colorize* Get(const etk::UString& _colorName);
		bool Exist(void etk::UString& _colorName);
		void DisplayListOfColor(void);

	private:
		etk::UString m_fileColor;
		etk::Vector<appl::Colorize*> m_list; //!< List of ALL Color
		appl::Colorize * m_colorizeError;
};


classColorManager::classColorManager(void)
{
	//ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgGuiChangeColor);
}

classColorManager::~classColorManager(void)
{
	delete(m_colorizeError);

	int32_t i;
	// clean all Element
	for (i=0; i< m_list.Size(); i++) {
		if (NULL != m_list[i]) {
			delete(m_list[i]);
			m_list[i] = NULL;
		}
	}
	// clear the compleate list
	m_list.Clear();
}

void classColorManager::OnReceiveMessage(const ewol::EMessage& _msg)
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

void classColorManager::LoadFile(const etk::UString& _xmlFilename)
{
	// Remove all old color : 
	int32_t i;
	// clean all Element
	for (i=0; i< m_list.Size(); i++) {
		if (NULL != m_list[i]) {
			delete(m_list[i]);
			m_list[i] = NULL;
		}
	}
	// clear the compleate list
	m_list.Clear();

	m_fileColor = _xmlFilename;
	APPL_DEBUG("open file (COLOR) \"" << _xmlFilename << "\" ? = \"" << m_fileColor << "\"");
	m_colorizeError = new Colorize();
	m_colorizeError->SetBgColor("#00FF00FF");
	m_colorizeError->SetFgColor("#FF00FFFF");

	// allocate the document in the stack
	TiXmlDocument XmlDocument;
	// open the curent File
	etk::FSNode fileName(etk::UString("DATA:color/") + _xmlFilename + etk::UString(".xml"));
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
	XmlDocument.Parse((const char*)fileBuffer, 0, TIXML_ENCODING_UTF8);

	TiXmlElement* root = XmlDocument.FirstChildElement( "EdnColor" );
	if (NULL == root ) {
		APPL_ERROR(PFX"(l ?) main node not find: \"EdnColor\" in \"" << xmlFilename << "\"");
		return;
	} else {
		TiXmlNode * pNode = root->FirstChild();
		while(NULL != pNode) {
			if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
				// nothing to do, just proceed to next step
			} else if (!strcmp(pNode->Value(), "color")) {
				appl::Colorize *myNewColor = new appl::Colorize();
				//--------------------------------------------------------------------------------------------
				//<color name="basicBackground"		FG="#000000"	BG="#000000"	bold="no"	italic="no"/>
				//--------------------------------------------------------------------------------------------
				// get the name of the Chaine
				const char *colorName = pGuiNode->ToElement()->Attribute("name");
				if (NULL == colorName) {
					APPL_ERROR(PFX"(l "<< pGuiNode->Row() <<") node with no name");
					// get next node element
					pGuiNode = pGuiNode->NextSibling();
					continue;
				} else {
					myNewColor->SetName(colorName);
					//APPL_INFO(PFX"Add a new color in the panel : \"%s\"", colorName);
				}
				const char *colorBG = pGuiNode->ToElement()->Attribute("BG");
				if (NULL != colorBG) {
					myNewColor->SetBgColor(colorBG);
				}
				const char *colorFG = pGuiNode->ToElement()->Attribute("FG");
				if (NULL != colorFG) {
					myNewColor->SetFgColor(colorFG);
				}
				const char *bold = pGuiNode->ToElement()->Attribute("bold");
				if (NULL != bold) {
					if(0 == strcmp(bold, "yes") ) {
						myNewColor->SetBold(true);
					}
				}
				const char *italic = pGuiNode->ToElement()->Attribute("italic");
				if (NULL != italic) {
					if(0 == strcmp(italic, "yes") ) {
						myNewColor->SetItalic(true);
					}
				}
				m_list.PushBack(myNewColor);
			} else {
				APPL_ERROR(PFX"(l "<<pNode->Row()<<") node not suported : \""<<pNode->Value()<<"\" must be [color]");
			}
			// get next node element
			pNode = pNode->NextSibling();
		}
	}
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
	//SendMessage(APPL_MSG__COLOR_HAS_CHANGE);
	//SendMessage(APPL_MSG__USER_DISPLAY_CHANGE);
}

Colorize *classColorManager::Get(const etk::UString& _colorName)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (_colorName == m_list[iii]->GetName()) {
			return m_list[iii];
		}
	}
	APPL_ERROR(PFX"Color does not Existed ["<< _colorName<<"]" );
	// an error
	return m_colorizeError;
}

bool classColorManager::Exist(const etk::UString& _colorName)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (_colorName == m_list[iii]->GetName()) {
			return true;
		}
	}
	return false;
}

void classColorManager::DisplayListOfColor(void)
{
	int32_t i;
	APPL_INFO(PFX"List of ALL COLOR : ");
	for (iii=0; iii<m_list.Size(); iii++) {
		APPL_INFO("        " << i << " : " << *m_list[i]);
	}
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


