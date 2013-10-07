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
#define __class__	"HighlightPattern"



HighlightPattern::HighlightPattern(void)
{
	m_haveStopPatern = false;
	m_multiline = false;
	m_color = ColorizeManager::get("normal");
	m_regExpStart = new etk::RegExp<etk::Buffer>();
	m_regExpStop = new etk::RegExp<etk::Buffer>();
	m_escapeChar = 0;
}

HighlightPattern::~HighlightPattern(void)
{
	delete(m_regExpStart);
	delete(m_regExpStop);
}

void  HighlightPattern::setPaternStart(etk::UString &regExp)
{
	m_regExpStart->setRegExp(regExp);
}

void  HighlightPattern::setPaternStop(etk::UString &regExp)
{
	if (regExp.size() != 0) {
		m_regExpStop->setRegExp(regExp);
		m_haveStopPatern = true;
	} else {
		m_haveStopPatern = false;
	}
}

void HighlightPattern::setEscapeChar(etk::UString &EscapeChar)
{
	if (EscapeChar.size()>0) {
		m_escapeChar = EscapeChar[0];
	} else {
		m_escapeChar = 0;
	}
}


void  HighlightPattern::setColor(etk::UString &colorName)
{
	m_colorName = colorName;
	m_color = ColorizeManager::get(m_colorName);
}

bool HighlightPattern::isEnable(void)
{
	return true;
}


void HighlightPattern::ReloadColor(void)
{
	m_color = ColorizeManager::get(m_colorName);
}

/**
 * @brief
 *
 * @param[in,out]
 *
 * @eturn
 *
 */
void HighlightPattern::display(void)
{
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
	// display all elements
	for (int32_t i=0; i< m_subPatern.size(); i++) {
		APPL_INFO("        " << i << " subPattern : " << m_subPatern[i]->getName() );
		m_subPatern[i]->display();
	}
}
void HighlightPattern::ParseRules(exml::Element *child, int32_t level)
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
	xChild = (exml::Element*)child->getNamed("rule");
	if (NULL != xChild) {
		/*
			// Create the patern ...
			HighlightPattern *myPattern = new HighlightPattern();
			// parse under Element
			myPattern->ParseRules(ruleChild, level+1);
			// add element in the list
			m_subPatern.pushBack(myPattern);
			//ParseRules(passChild, m_listHighlightPass1, level1++);
		*/
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
resultFind_te HighlightPattern::find(int32_t start, int32_t stop, colorInformation_ts &resultat, etk::Buffer &buffer)
{
	//APPL_DEBUG(" try to find the element");
	resultat.beginStart = -1;
	resultat.beginStop = -1;
	resultat.endStart = -1;
	resultat.endStop = -1;
	resultat.notEnded = false;
	resultat.patern = this;
	
	// when we have only one element : 
	if (false == m_haveStopPatern) {
		if (true == m_regExpStart->ProcessOneElement(buffer, start, stop)) {
			resultat.beginStart = m_regExpStart->Start();
			resultat.beginStop  = m_regExpStart->Stop();
			resultat.endStart = m_regExpStart->Start();
			resultat.endStop  = m_regExpStart->Stop();
			return HLP_FIND_OK;
		}
		//APPL_DEBUG("NOT find hightlightpatern ...");
	} else {
		// try while we find the first element
		if (true == m_regExpStart->ProcessOneElement(buffer, start, stop, m_escapeChar)) {
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
		//APPL_DEBUG("NOT find start hightlightpatern ...");
	}
	return HLP_FIND_ERROR;
}
