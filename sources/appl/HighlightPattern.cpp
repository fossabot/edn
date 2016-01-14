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

appl::HighlightPattern::HighlightPattern(const std::shared_ptr<appl::GlyphPainting>& _glyphPainting, const std::shared_ptr<const exml::Element>& _child, int32_t _level) :
  m_glyphPainting(_glyphPainting),
  m_paternName(""),
  m_hasParsingError(true),
  m_regexValue(""),
  m_regExp(),
  m_colorName(""),
  m_level(0) {
	parseRules(_child, _level);
}

appl::HighlightPattern::HighlightPattern() :
  m_glyphPainting(),
  m_paternName(""),
  m_hasParsingError(true),
  m_regexValue(""),
  m_regExp(),
  m_colorName(""),
  m_level(0) {
	
}

appl::HighlightPattern::~HighlightPattern() {
	
}

void appl::HighlightPattern::setPatern(const std::string& _regExp) {
	m_regexValue = _regExp;
	APPL_DEBUG("parse regex='" << _regExp << "'");
	try {
		m_regExp.assign(_regExp, std::regex_constants::optimize | std::regex_constants::ECMAScript);
		//m_regExp.assign(_regExp, std::regex_constants::optimize | std::regex_constants::extended);
		m_hasParsingError = false;
	} catch (std::regex_error e) {
		m_hasParsingError = true;
		APPL_ERROR("can not parse regex : '" << e.what() << "' for : " << _regExp);
	}
}
std::string appl::HighlightPattern::getPaternString() {
	return m_regexValue;
}

void appl::HighlightPattern::setColorGlyph(const std::string& _colorName) {
	m_colorName = _colorName;
	m_colorId = m_glyphPainting->request(m_colorName);
	APPL_VERBOSE("Resuest color name '" << m_colorName << "' => id=" << m_colorId);
}

void appl::HighlightPattern::display() {
	APPL_INFO("patern : '" << m_paternName << "' level=" << m_level );
	APPL_INFO("  == > colorName '" << m_colorName << "'");
	//APPL_INFO("  == > regExp '" << m_regExp.getRegExp() << "'");
	APPL_INFO("  == > regex '" << m_regexValue << "'");
}

void appl::HighlightPattern::parseRules(const std::shared_ptr<const exml::Element>& _child, int32_t _level) {
	//--------------------------------------------------------------------------------------------
	/*
		<rule name="my preprocesseur">
			<color>preprocesseur</color>
			<regex>#</regex>
			<sub>namexxx</sub>
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
	
	std::shared_ptr<const exml::Element> xChild = _child->getNamed("color");
	if (nullptr != xChild) {
		std::string myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			setColorGlyph(myData);
		}
	}
	xChild = _child->getNamed("regex");
	if (nullptr != xChild) {
		std::string myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			setPatern(myData);
		}
	}
	xChild = _child->getNamed("sub");
	if (nullptr != xChild) {
		std::string myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			setSubPatternName(myData);
		}
	}
}


bool appl::HighlightPattern::find(int32_t _start,
                                  int32_t _stop,
                                  appl::HighlightInfo& _resultat,
                                  const std::string& _buffer) {
	//APPL_DEBUG(" try to find the element");
	_resultat.start = -1;
	_resultat.stop = -1;
	_resultat.notEnded = false;
	_resultat.patern = this;
	if (m_hasParsingError == true) {
		return false;
	}
	std::smatch resultMatch;
	std::regex_constants::match_flag_type flags = std::regex_constants::match_continuous; // check only the match at the first character.
	//APPL_DEBUG("find data at : start=" << _start << " stop=" << _stop << " regex='" << m_regexValue << "'");
	if ((int64_t)_stop <= (int64_t)_buffer.size()) {
		char val = _buffer[_stop];
		if (    val != '\n'
		     && val != '\r') {
			//after last char ==> not end of line ($ would not work))
			flags |= std::regex_constants::match_not_eol;
		}
		/*
		if (!(    ('a' <= val && val <= 'z')
		       || ('A' <= val && val <= 'Z')
		       || ('0' <= val && val <= '9')
		       || val == '_')) {
			flags |= std::regex_constants::match_not_eow;
		}
		*/
	}
	if (_start>0) {
		flags |= std::regex_constants::match_prev_avail;
	}
	std::regex_search(_buffer.begin() + _start, _buffer.begin() + _stop, resultMatch, m_regExp, flags);
	if (resultMatch.size() > 0) {
		_resultat.start = std::distance(_buffer.begin(), resultMatch[0].first);
		_resultat.stop = std::distance(_buffer.begin(), resultMatch[0].second);
		//APPL_DEBUG("find data at : start=" << _resultat.start << " stop=" << _resultat.stop << " data='" <<std::string(_buffer, _resultat.start, _resultat.stop-_resultat.start) << "'" );
		/*
		if (true){
			//TK_DEBUG("in line : '" << etk::to_string(_buffer) << "'");
			APPL_DEBUG("    Find " << resultMatch.size() << " elements");
			for (size_t iii=0; iii<resultMatch.size(); ++iii) {
				int32_t posStart = std::distance(_buffer.begin(), resultMatch[iii].first);
				int32_t posStop = std::distance(_buffer.begin(), resultMatch[iii].second);
				APPL_DEBUG("          [" << iii << "] " << posStart << " to " << posStop);
			}
		}
		*/
		return true;
	}
	return false;
}
