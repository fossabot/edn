/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <HighlightPattern.h>
#include <ColorizeManager.h>

#undef __class__
#define __class__ "HighlightPattern"

HighlightPattern::HighlightPattern(void) {
	m_haveStopPatern = false;
	m_multiline = false;
	m_color = ColorizeManager::get("normal");
	m_regExpStart = new etk::RegExp<etk::Buffer>();
	m_regExpStop = new etk::RegExp<etk::Buffer>();
	m_escapeChar = 0;
}

HighlightPattern::~HighlightPattern(void) {
	delete(m_regExpStart);
	delete(m_regExpStop);
}

void HighlightPattern::setPaternStart(etk::UString& _regExp) {
	m_regExpStart->setRegExp(_regExp);
}

void HighlightPattern::setPaternStop(etk::UString& _regExp) {
	if (_regExp.size() != 0) {
		m_regExpStop->setRegExp(_regExp);
		m_haveStopPatern = true;
	} else {
		m_haveStopPatern = false;
	}
}

void HighlightPattern::setEscapeChar(etk::UString& _EscapeChar) {
	if (_EscapeChar.size()>0) {
		m_escapeChar = _EscapeChar[0];
	} else {
		m_escapeChar = 0;
	}
}

void  HighlightPattern::setColorGlyph(etk::UString& _colorName) {
	m_colorName = _colorName;
	m_color = ColorizeManager::get(m_colorName);
}

bool HighlightPattern::isEnable(void) {
	return true;
}

void HighlightPattern::reloadColor(void) {
	m_color = ColorizeManager::get(m_colorName);
}

void HighlightPattern::display(void) {
	/*
	APPL_INFO("patern : \"" << m_paternName << "\" level=" << m_level );
	APPL_INFO("  == > colorName \"" << m_colorName << "\"");
	APPL_INFO("  == > regExpStart \"" << m_regExpStart->getRegExp() << "\"");
	APPL_INFO("  == > regExpStop \"" << m_regExpStop->getRegExp() << "\"");
	if (true == m_haveStopPatern) {
		APPL_INFO("  == > stop pattern: YES");
	} else {
		APPL_INFO("  == > stop pattern: NO");
	}
	if (true == m_multiline) {
		APPL_INFO("  == > multiline pattern: YES");
	} else {
		APPL_INFO("  == > multiline pattern: NO");
	}
	*/
}

void HighlightPattern::parseRules(exml::Element *child, int32_t level) {
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
	etk::UString highLightName = child->getAttribute("name");
	etk::UString myEdnDataTmp = "???";
	if (highLightName.size()!=0) {
		myEdnDataTmp = highLightName;
	}
	setName(myEdnDataTmp);
	setLevel(level);
	
	exml::Element* xChild = (exml::Element*)child->getNamed("color");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size()!=0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			etk::UString myEdnData = myData;
			setColor(myEdnData);
		}
	}
	xChild = (exml::Element*)child->getNamed("start");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size()!=0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			etk::UString myEdnData = myData;
			setPaternStart(myEdnData);
		}
	}
	xChild = (exml::Element*)child->getNamed("end");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size()!=0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			etk::UString myEdnData = myData;
			setPaternStop(myEdnData);
		}
	}
	xChild = (exml::Element*)child->getNamed("EscapeChar");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size()!=0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			etk::UString myEdnData = myData;
			setEscapeChar(myEdnData);
		}
	}
}


/**
 * @brief find Element only in the specify start characters and find the end with the range done
 *
 * @param[in] start First character to search data (if recognise it start here)
 * @param[in] stop End of the possibility whe search can continue
 * @param[out] resultat Position where find data
 * @param[in] buffer : Where to search data
 *
 * @return HLP_FIND_OK We find a compleate pattern
 * @return HLP_FIND_OK_NO_END Xe find a partial pattern (missing end)
 * @return HLP_FIND_ERROR Not find the pattern
 */
resultFind_te HighlightPattern::find(int32_t start, int32_t stop, colorInformation_ts &resultat, etk::Buffer &buffer) {
	//APPL_DEBUG(" try to find the element");
	resultat.beginStart = -1;
	resultat.beginStop = -1;
	resultat.endStart = -1;
	resultat.endStop = -1;
	resultat.notEnded = false;
	resultat.patern = this;
	
	// when we have only one element : 
	if (false == m_haveStopPatern) {
		if (true == m_regExpStart->processOneElement(buffer, start, stop)) {
			resultat.beginStart = m_regExpStart->start();
			resultat.beginStop  = m_regExpStart->stop();
			resultat.endStart = m_regExpStart->start();
			resultat.endStop  = m_regExpStart->stop();
			return HLP_FIND_OK;
		}
		//APPL_DEBUG("NOT find hightlightpatern ...");
	} else {
		// try while we find the first element
		if (true == m_regExpStart->processOneElement(buffer, start, stop, m_escapeChar)) {
			resultat.beginStart = m_regExpStart->start();
			resultat.beginStop  = m_regExpStart->stop();
			if (true == m_regExpStop->process(buffer, resultat.beginStop, stop, m_escapeChar)) {
				resultat.endStart = m_regExpStop->start();
				resultat.endStop  = m_regExpStop->stop();
				return HLP_FIND_OK;
			} else {
				resultat.endStart = stop+1;
				resultat.endStop = stop+1;
				resultat.notEnded = true;
				return HLP_FIND_OK_NO_END;
			}
		}
		//APPL_DEBUG("NOT find start hightlightpatern ...");
	}
	return HLP_FIND_ERROR;
}
