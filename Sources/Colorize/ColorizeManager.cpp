/**
 *******************************************************************************
 * @file ColoriseManager.cpp
 * @brief Editeur De N'ours : Colorising Manager
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
#include "ColorizeManager.h"
#include "tinyxml.h"

#define PFX	"ColorizeManager "


ColorizeManager::ColorizeManager(void)
{
}

ColorizeManager::~ColorizeManager(void)
{
	delete(errorColor);
	// TODO : delete all color previously
	listMyColor.clear();
}


void ColorizeManager::LoadFile(Edn::String &xmlFilename)
{
	LoadFile(xmlFilename.c_str());
}

void ColorizeManager::LoadFile(const char * xmlFilename)
{
	errorColor = new Colorize();
	errorColor->SetBgColor("#000000");
	errorColor->SetFgColor("#FFFFFF");

	// allocate the document in the stack
	TiXmlDocument XmlDocument;
	// open the curent File
	XmlDocument.LoadFile(xmlFilename);
	TiXmlElement* root = XmlDocument.FirstChildElement( "EdnColor" );
	if (NULL == root )
	{
		EDN_ERROR(PFX"(l ?) main node not find: \"EdnColor\"");
		return;
	}
	else
	{
		TiXmlNode * pNode = root->FirstChild();
		while(NULL != pNode)
		{
			if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
				// nothing to do, just proceed to next step
			} else if (!strcmp(pNode->Value(), "gui")) {
				TiXmlNode * pGuiNode = pNode->FirstChild();
				while(NULL != pGuiNode)
				{
					if (pGuiNode->Type()==TiXmlNode::TINYXML_COMMENT) {
						// nothing to do, just proceed to next step
					} else if (!strcmp(pGuiNode->Value(), "color")) {
						//--------------------------------------------------------------------------------------------
						//<color name="basicBackground" val="#000000"/>
						//--------------------------------------------------------------------------------------------
						const char *colorName = pGuiNode->ToElement()->Attribute("name");
						int32_t id = 0;
						if (NULL == colorName) {
							EDN_ERROR("(l "<< pGuiNode->Row() <<") node with no name");
							// get next node element
							pGuiNode = pGuiNode->NextSibling();
							continue;
						}
						if (!strcmp(colorName, "CODE_space")) {
							id = COLOR_CODE_SPACE;
						} else if (!strcmp(colorName, "CODE_tabulation")) {
							id = COLOR_CODE_TAB;
						} else if (!strcmp(colorName, "CODE_basicBackgroung")) {
							id = COLOR_CODE_BASIC_BG;
						} else if (!strcmp(colorName, "CODE_cursor")) {
							id = COLOR_CODE_CURSOR;
						} else if (!strcmp(colorName, "CODE_lineNumber")) {
							id = COLOR_CODE_LINE_NUMBER;
						} else if (!strcmp(colorName, "LIST_backgroung1")) {
							id = COLOR_LIST_BG_1;
						} else if (!strcmp(colorName, "LIST_backgroung2")) {
							id = COLOR_LIST_BG_2;
						} else if (!strcmp(colorName, "LIST_backgroungSelected")) {
							id = COLOR_LIST_BG_SELECTED;
						} else if (!strcmp(colorName, "LIST_textNormal")) {
							id = COLOR_LIST_TEXT_NORMAL;
						} else if (!strcmp(colorName, "LIST_textModify")) {
							id = COLOR_LIST_TEXT_MODIFY;
						} else {
							EDN_ERROR("(l "<<pGuiNode->Row()<<") Unknown basic gui color : \"" << colorName << "\"" );
							// get next node element
							pGuiNode = pGuiNode->NextSibling();
							continue;
						}
						const char *color = pGuiNode->ToElement()->Attribute("val");
						if (NULL != color) {
							unsigned int r=0;
							unsigned int v=0;
							unsigned int b=0;
							sscanf(color, "#%02x%02x%02x", &r,&v,&b);
							basicColors[id].red = (float)r/255.0;
							basicColors[id].green = (float)v/255.0;
							basicColors[id].blue = (float)b/255.0;
							/*
							EDN_INFO(" Specify color for system ID="<< id );
							EDN_INFO("    " << color << " ==> r="<< r <<" v="<< v <<" b="<< b );
							EDN_INFO("    " << color << " ==> r="<< basicColors[id].red <<" v="<< basicColors[id].green <<" b="<< basicColors[id].blue );
							*/
						}
					} else {
						EDN_ERROR("(l "<<pGuiNode->Row()<<") node not suported : \""<<pGuiNode->Value()<<"\" must be [color]");
					}
					// get next node element
					pGuiNode = pGuiNode->NextSibling();
				}
			} else if (!strcmp(pNode->Value(), "syntax")) {
				TiXmlNode * pGuiNode = pNode->FirstChild();
				while(NULL != pGuiNode)
				{
					if (pGuiNode->Type()==TiXmlNode::TINYXML_COMMENT) {
						// nothing to do, just proceed to next step
					} else if (!strcmp(pGuiNode->Value(), "color")) {
						Colorize *myNewColor = new Colorize();
						//--------------------------------------------------------------------------------------------
						//<color name="basicBackground"		FG="#000000"	BG="#000000"	bold="no"	italic="no"/>
						//--------------------------------------------------------------------------------------------
						// get the name of the Chaine
						const char *colorName = pGuiNode->ToElement()->Attribute("name");
						if (NULL == colorName) {
							EDN_ERROR(PFX"(l "<< pGuiNode->Row() <<") node with no name");
							// get next node element
							pGuiNode = pGuiNode->NextSibling();
							continue;
						} else {
							myNewColor->SetName(colorName);
							//EDN_INFO(PFX"Add a new color in the panel : \"%s\"", colorName);
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
						listMyColor.push_back(myNewColor);
					} else {
						EDN_ERROR(PFX"(l "<<pNode->Row()<<") node not suported : \""<<pNode->Value()<<"\" must be [color]");
					}
					pGuiNode = pGuiNode->NextSibling();
				}
			} else {
				EDN_ERROR(PFX"(l "<<pNode->Row()<<") node not suported : \""<<pNode->Value()<<"\" must be [gui,syntax]");
			}
			// get next node element
			pNode = pNode->NextSibling();
		}
	}
}

Colorize *ColorizeManager::Get(const char *colorName)
{
	uint32_t i;
	for (i=0; i<listMyColor.size(); i++) {
		Edn::String elementName = listMyColor[i]->GetName();
		if (elementName == colorName) {
			return listMyColor[i];
		}
	}
	EDN_ERROR(PFX"Color does not Existed ["<< colorName<<"]" );
	// an error
	return errorColor;
}

Colorize *ColorizeManager::Get(Edn::String &colorName)
{
	return Get(colorName.c_str());
}

color_ts & ColorizeManager::Get(basicColor_te myColor)
{
	if (myColor < COLOR_NUMBER_MAX) {
		return basicColors[myColor];
	} else {
		return basicColors[0];
	}
}


bool ColorizeManager::Exist(const char *colorName)
{
	uint32_t i;
	for (i=0; i<listMyColor.size(); i++) {
		Edn::String elementName = listMyColor[i]->GetName();
		if (elementName == colorName) {
			return true;
		}
	}
	return false;
}
bool ColorizeManager::Exist(Edn::String &colorName)
{
	return Exist(colorName.c_str());
}

void ColorizeManager::DisplayListOfColor(void)
{
	uint32_t i;
	EDN_INFO(PFX"List of ALL COLOR : ");
	for (i=0; i<listMyColor.size(); i++) {
		//Edn::String elementName = listMyColor[i]->GetName();
		//EDN_INFO(i << " : \"" <<  elementName.c_str() << "\"" );
		listMyColor[i]->Display(i);
	}
}



