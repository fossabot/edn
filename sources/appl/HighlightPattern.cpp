/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/HighlightPattern.h>

#undef __class__
#define __class__ "HighlightPattern"

appl::HighlightPattern::HighlightPattern(const ewol::object::Shared<appl::GlyphPainting>& _glyphPainting) :
  m_glyphPainting(_glyphPainting),
  m_paternName(""),
  m_regExp(nullptr),
  m_colorName(""),
  m_level(0) {
	m_regExp = std::unique_ptr<etk::RegExp<etk::Buffer>>(new etk::RegExp<etk::Buffer>());
}

appl::HighlightPattern::~HighlightPattern() {
	
}

void appl::HighlightPattern::setPatern(std::string& _regExp) {
	if (m_regExp == nullptr) {
		return;
	}
	m_regExp->compile(_regExp);
}
std::string appl::HighlightPattern::getPaternString() {
	return m_regExp->getRegExDecorated();
}

void appl::HighlightPattern::setColorGlyph(std::string& _colorName) {
	m_colorName = _colorName;
	m_colorId = m_glyphPainting->request(m_colorName);
	APPL_VERBOSE("Resuest color name '" << m_colorName << "' => id=" << m_colorId);
}

void appl::HighlightPattern::display() {
	APPL_INFO("patern : '" << m_paternName << "' level=" << m_level );
	APPL_INFO("  == > colorName '" << m_colorName << "'");
	APPL_INFO("  == > regExp '" << m_regExp->getRegExp() << "'");
}

void appl::HighlightPattern::parseRules(exml::Element* _child, int32_t _level) {
	//--------------------------------------------------------------------------------------------
	/*
		<rule name="my preprocesseur">
			<color>preprocesseur</color>
			<regex>#</regex>
		</rule>
	*/
	//--------------------------------------------------------------------------------------------
	// process attribute	
	std::string highLightName = _child->getAttribute("name");
	std::string myEdnDataTmp = "???";
	if (highLightName.size()!=0) {
		myEdnDataTmp = highLightName;
	}
	setName(myEdnDataTmp);
	setLevel(_level);
	
	exml::Element* xChild = _child->getNamed("color");
	if (nullptr != xChild) {
		std::string myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			std::string myEdnData = myData;
			setColorGlyph(myEdnData);
		}
	}
	xChild = _child->getNamed("regex");
	if (nullptr != xChild) {
		std::string myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			std::string myEdnData = myData;
			setPatern(myEdnData);
		}
	}
}


enum resultFind appl::HighlightPattern::find(int32_t _start,
                                     int32_t _stop,
                                     appl::HighlightInfo& _resultat,
                                     etk::Buffer& _buffer) {
	//APPL_DEBUG(" try to find the element");
	_resultat.start = -1;
	_resultat.stop = -1;
	_resultat.notEnded = false;
	_resultat.patern = this;
	
	// when we have only one element:
	if (true == m_regExp->processOneElement(_buffer, _start, _stop)) {
		_resultat.start = m_regExp->start();
		_resultat.stop  = m_regExp->stop();
		return HLP_FIND_OK;
	}
	//APPL_DEBUG("NOT find hightlightpatern ...");
	return HLP_FIND_ERROR;
}
