/**
 *******************************************************************************
 * @file HighlightPattern.cpp
 * @brief Editeur De N'ours : Hightlight Patern (header)
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
#include "HighlightPattern.h"
#include "ColorizeManager.h"


#undef __class__
#define __class__	"HighlightPattern"



HighlightPattern::HighlightPattern(void)
{
	m_haveStopPatern = false;
	m_multiline = false;
	ColorizeManager *myColorManager = ColorizeManager::getInstance();
	m_color = myColorManager->Get("normal");
	m_regExpStart = new EdnRegExp<EdnVectorBuf>();
	m_regExpStop = new EdnRegExp<EdnVectorBuf>();
	m_escapeChar = 0;
}

HighlightPattern::~HighlightPattern(void)
{
	delete(m_regExpStart);
	delete(m_regExpStop);
}

void  HighlightPattern::SetPaternStart(Edn::String &regExp)
{
	m_regExpStart->SetRegExp(regExp);
}

void  HighlightPattern::SetPaternStop(Edn::String &regExp)
{
	if (regExp.Size() != 0) {
		m_regExpStop->SetRegExp(regExp);
		m_haveStopPatern = true;
	} else {
		m_haveStopPatern = false;
	}
}

void HighlightPattern::SetEscapeChar(Edn::String &EscapeChar)
{
	if (EscapeChar.Size()>0) {
		m_escapeChar = EscapeChar.c_str()[0];
	} else {
		m_escapeChar = 0;
	}
}


void  HighlightPattern::SetColor(Edn::String &colorName)
{
	m_colorName = colorName;
	ColorizeManager *myColorManager = ColorizeManager::getInstance();
	m_color = myColorManager->Get(m_colorName);
}

bool HighlightPattern::IsEnable(void)
{
	return true;
}

/**
 * @brief
 *
 * @param[in,out]
 *
 * @eturn
 *
 */
void HighlightPattern::Display(void)
{
	/*
	EDN_INFO("patern : \"" << m_paternName.c_str() << "\" level=" << m_level );
	EDN_INFO(" ==> colorName \"" << m_colorName.c_str() << "\"");
	EDN_INFO(" ==> regExpStart \"" << m_regExpStart->GetRegExp().c_str() << "\"");
	EDN_INFO(" ==> regExpStop \"" << m_regExpStop->GetRegExp().c_str() << "\"");
	if (true == m_haveStopPatern) {
		EDN_INFO(" ==> stop pattern: YES");
	} else {
		EDN_INFO(" ==> stop pattern: NO");
	}
	if (true == m_multiline) {
		EDN_INFO(" ==> multiline pattern: YES");
	} else {
		EDN_INFO(" ==> multiline pattern: NO");
	}
	*/
	// Display all elements
	for (int32_t i=0; i< m_subPatern.Size(); i++) {
		EDN_INFO("        " << i << " SubPattern : " << m_subPatern[i]->GetName().c_str() );
		m_subPatern[i]->Display();
	}
}
void HighlightPattern::ParseRules(TiXmlNode *child, int32_t level)
{
	//--------------------------------------------------------------------------------------------
	/*
		<rule name="my preprocesseur">
			<color>preprocesseur</color>
			<start>#</start>
			<end>$</end>
			<multiline>yes</multiline>
		</rule>
	*/
	//--------------------------------------------------------------------------------------------
	// process attribute	
	const char *highLightName = child->ToElement()->Attribute("name");
	Edn::String myEdnDataTmp = "???";
	if (NULL != highLightName) {
		myEdnDataTmp = highLightName;
	}
	SetName(myEdnDataTmp);
	SetLevel(level);
	
	TiXmlElement *xChild = child->FirstChildElement("color");
	if (NULL != xChild) {
		const char *myData = xChild->GetText();
		if (myData) {
			//EDN_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			Edn::String myEdnData = myData;
			SetColor(myEdnData);
		}
	}
	xChild = child->FirstChildElement("start");
	if (NULL != xChild) {
		const char *myData = xChild->GetText();
		if (myData) {
			//EDN_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			Edn::String myEdnData = myData;
			SetPaternStart(myEdnData);
		}
	}
	xChild = child->FirstChildElement("end");
	if (NULL != xChild) {
		const char *myData = xChild->GetText();
		if (myData) {
			//EDN_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			Edn::String myEdnData = myData;
			SetPaternStop(myEdnData);
		}
	}
	xChild = child->FirstChildElement("EscapeChar");
	if (NULL != xChild) {
		const char *myData = xChild->GetText();
		if (myData) {
			//EDN_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			Edn::String myEdnData = myData;
			SetEscapeChar(myEdnData);
		}
	}
	xChild = child->FirstChildElement("rule");
	if (NULL != xChild) {
		/*
			// Create the patern ...
			HighlightPattern *myPattern = new HighlightPattern();
			// parse under Element
			myPattern->ParseRules(ruleChild, level+1);
			// add element in the list
			m_subPatern.PushBack(myPattern);
			//ParseRules(passChild, m_listHighlightPass1, level1++);
		*/
	}
}

resultFind_te HighlightPattern::Find(int32_t start, int32_t stop, colorInformation_ts &resultat, EdnVectorBuf &buffer)
{
	//EDN_DEBUG(" try to find the element");
	resultat.beginStart = -1;
	resultat.beginStop = -1;
	resultat.endStart = -1;
	resultat.endStop = -1;
	resultat.notEnded = false;
	resultat.patern = this;
	
	// when we have only one element : 
	if (false == m_haveStopPatern) {
		if (true == m_regExpStart->Process(buffer, start, stop)) {
			resultat.beginStart = m_regExpStart->Start();
			resultat.beginStop  = m_regExpStart->Stop();
			resultat.endStart = m_regExpStart->Start();
			resultat.endStop  = m_regExpStart->Stop();
			return HLP_FIND_OK;
		}
		//EDN_DEBUG("NOT find hightlightpatern ...");
	} else {
		// try while we find the first element
		if (true == m_regExpStart->Process(buffer, start, stop, m_escapeChar)) {
			resultat.beginStart = m_regExpStart->Start();
			resultat.beginStop  = m_regExpStart->Stop();
			if (true == m_regExpStop->Process(buffer, resultat.beginStop, stop, m_escapeChar)) {
				resultat.endStart = m_regExpStop->Start();
				resultat.endStop  = m_regExpStop->Stop();
				return HLP_FIND_OK;
			} else {
				resultat.endStart = stop+1;
				resultat.endStop = stop+1;
				resultat.notEnded = true;
				return HLP_FIND_OK_NO_END;
			}
		}
		//EDN_DEBUG("NOT find start hightlightpatern ...");
	}
	return HLP_FIND_ERROR;
}
