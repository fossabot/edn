/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <HighlightPattern.h>

#undef __class__
#define __class__ "HighlightPattern"

appl::HighlightPattern::HighlightPattern(appl::GlyphPainting*& _glyphPainting) :
  m_glyphPainting(_glyphPainting),
  m_paternName(""),
  m_regExpStart(NULL),
  m_regExpStop(NULL),
  m_colorName(""),
  m_escapeChar(etk::UChar::Null),
  m_multiline(false),
  m_level(0) {
	m_regExpStart = new etk::RegExp<etk::Buffer>();
}

appl::HighlightPattern::~HighlightPattern(void) {
	if (m_regExpStart != NULL) {
		delete(m_regExpStart);
		m_regExpStart = NULL;
	}
	if (m_regExpStop != NULL) {
		delete(m_regExpStop);
		m_regExpStop = NULL;
	}
}

void appl::HighlightPattern::setPaternStart(etk::UString& _regExp) {
	if (m_regExpStart == NULL) {
		return;
	}
	m_regExpStart->setRegExp(_regExp);
}

void appl::HighlightPattern::setPaternStop(etk::UString& _regExp) {
	if (m_regExpStop != NULL) {
		delete(m_regExpStop);
		m_regExpStop = NULL;
	}
	if (_regExp.size() != 0) {
		m_regExpStop = new etk::RegExp<etk::Buffer>();
		if (m_regExpStop != NULL) {
			m_regExpStop->setRegExp(_regExp);
		} else {
			APPL_ERROR("Allocation error");
		}
	}
}

void appl::HighlightPattern::setEscapeChar(const etk::UChar& _EscapeChar) {
	m_escapeChar = _EscapeChar;
}

void appl::HighlightPattern::setColorGlyph(etk::UString& _colorName) {
	m_colorName = _colorName;
	m_colorId = m_glyphPainting->request(m_colorName);
	APPL_DEBUG("Resuest color name '" << m_colorName << "' => id=" << m_colorId);
}

void appl::HighlightPattern::display(void) {
	APPL_INFO("patern : \"" << m_paternName << "\" level=" << m_level );
	APPL_INFO("  == > colorName \"" << m_colorName << "\"");
	APPL_INFO("  == > regExpStart \"" << m_regExpStart->getRegExp() << "\"");
	if (m_regExpStop != NULL) {
		APPL_INFO("  == > regExpStop \"" << m_regExpStop->getRegExp() << "\"");
	}
	if (m_multiline == true) {
		APPL_INFO("  == > multiline pattern: YES");
	} else {
		APPL_INFO("  == > multiline pattern: NO");
	}
}

void appl::HighlightPattern::parseRules(exml::Element* _child, int32_t _level) {
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
	etk::UString highLightName = _child->getAttribute("name");
	etk::UString myEdnDataTmp = "???";
	if (highLightName.size()!=0) {
		myEdnDataTmp = highLightName;
	}
	setName(myEdnDataTmp);
	setLevel(_level);
	
	exml::Element* xChild = _child->getNamed("color");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			etk::UString myEdnData = myData;
			setColorGlyph(myEdnData);
		}
	}
	xChild = _child->getNamed("start");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			etk::UString myEdnData = myData;
			setPaternStart(myEdnData);
		}
	}
	xChild = _child->getNamed("end");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			etk::UString myEdnData = myData;
			setPaternStop(myEdnData);
		}
	}
	xChild = _child->getNamed("EscapeChar");
	if (NULL != xChild) {
		etk::UString myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			setEscapeChar(myData[0]);
		}
	}
}


resultFind_te appl::HighlightPattern::find(int32_t _start,
                                     int32_t _stop,
                                     appl::HighlightInfo& _resultat,
                                     etk::Buffer& _buffer) {
	//APPL_DEBUG(" try to find the element");
	_resultat.beginStart = -1;
	_resultat.beginStop = -1;
	_resultat.endStart = -1;
	_resultat.endStop = -1;
	_resultat.notEnded = false;
	_resultat.patern = this;
	
	// when we have only one element:
	if (m_regExpStop == NULL) {
		if (true == m_regExpStart->processOneElement(_buffer, _start, _stop)) {
			_resultat.beginStart = m_regExpStart->start();
			_resultat.beginStop  = m_regExpStart->stop();
			_resultat.endStart = m_regExpStart->start();
			_resultat.endStop  = m_regExpStart->stop();
			return HLP_FIND_OK;
		}
		//APPL_DEBUG("NOT find hightlightpatern ...");
		return HLP_FIND_ERROR;
	}
	// try while we find the first element
	if (m_regExpStart->processOneElement(_buffer, _start, _stop, m_escapeChar) == false) {
		return HLP_FIND_ERROR;
	}
	_resultat.beginStart = m_regExpStart->start();
	_resultat.beginStop  = m_regExpStart->stop();
	if (m_regExpStop->process(_buffer, _resultat.beginStop, _stop, m_escapeChar) == true) {
		_resultat.endStart = m_regExpStop->start();
		_resultat.endStop  = m_regExpStop->stop();
		return HLP_FIND_OK;
	} else {
		_resultat.endStart = _stop+1;
		_resultat.endStop = _stop+1;
		_resultat.notEnded = true;
		return HLP_FIND_OK_NO_END;
	}
	//APPL_DEBUG("NOT find start hightlightpatern ...");
	return HLP_FIND_ERROR;
}
