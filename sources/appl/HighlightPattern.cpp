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

appl::HighlightPattern::HighlightPattern(const std::shared_ptr<appl::GlyphPainting>& _glyphPainting) :
  m_glyphPainting(_glyphPainting),
  m_paternName(""),
  m_hasParsingError(true),
  m_regexValue(""),
  m_regExp(),
  m_colorName(""),
  m_level(0) {
	
}

appl::HighlightPattern::~HighlightPattern() {
	
}

void appl::HighlightPattern::setPatern(const std::string& _regExp, bool forceMaximize) {
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
	//m_regExp.setMaximize(forceMaximize);
}
std::string appl::HighlightPattern::getPaternString() {
	return m_regexValue;
}

void appl::HighlightPattern::setColorGlyph(std::string& _colorName) {
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

void appl::HighlightPattern::parseRules(exml::Element* _child, int32_t _level, bool forceMaximize) {
	//--------------------------------------------------------------------------------------------
	/*
		<rule name="my preprocesseur">
			<color>preprocesseur</color>
			<regex>#</regex>
			<max>false</max>
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
	xChild = _child->getNamed("max");
	if (nullptr != xChild) {
		forceMaximize = etk::string_to_bool(xChild->getText());
	}
	xChild = _child->getNamed("regex");
	if (nullptr != xChild) {
		std::string myData = xChild->getText();
		if (myData.size() != 0) {
			//APPL_INFO(PFX"(l %d) node fined : %s=\"%s\"", xChild->Row(), xChild->Value() , myData);
			std::string myEdnData = myData;
			setPatern(myEdnData, forceMaximize);
		}
	}
}


enum resultFind appl::HighlightPattern::find(int32_t _start,
                                             int32_t _stop,
                                             appl::HighlightInfo& _resultat,
                                             const std::string& _buffer) {
	//APPL_DEBUG(" try to find the element");
	_resultat.start = -1;
	_resultat.stop = -1;
	_resultat.notEnded = false;
	_resultat.patern = this;
	if (m_hasParsingError == true) {
		return HLP_FIND_ERROR;
	}
	
	/*
	// when we have only one element:
	if (true == m_regExp.processOneElement(_buffer, _start, _stop)) {
		_resultat.start = m_regExp->start();
		_resultat.stop  = m_regExp->stop();
		return HLP_FIND_OK;
	}
	//APPL_DEBUG("NOT find hightlightpatern ...");
	return HLP_FIND_ERROR;
	*/
	
	std::smatch resultMatch;
	std::regex_constants::match_flag_type flags = std::regex_constants::match_continuous;
	
	//APPL_DEBUG("find data at : start=" << _start << " stop=" << _stop << " regex='" << m_regexValue << "'");
	if ((int64_t)_stop <= (int64_t)_buffer.size()) {
		char val = _buffer[_stop];
		if (    val != '\n'
		     && val != '\r') {
			//after last char ==> not end of line ($ would not work))
			flags |= std::regex_constants::match_not_eol;
		}
		if (!(    ('a' <= val && val <= 'z')
		       || ('A' <= val && val <= 'Z')
		       || ('0' <= val && val <= '9')
		       || val == '_')) {
			flags |= std::regex_constants::match_not_eow;
		}
	}
	if (_start>0) {
		flags |= std::regex_constants::match_prev_avail;
	}
	std::regex_search(_buffer.begin() + _start, _buffer.begin() + _stop, resultMatch, m_regExp, flags);
	if (resultMatch.size() > 0) {
		_resultat.start = std::distance(_buffer.begin(), resultMatch[0].first);;
		_resultat.stop = std::distance(_buffer.begin(), resultMatch[0].second);
		//APPL_DEBUG("find data at : start=" << _resultat.start << " stop=" << _resultat.stop << " data='" <<std::string(_buffer, _resultat.start, _resultat.stop-_resultat.start) << "'" );
		/*
		if (true){
			//TK_DEBUG("in line : '" << etk::to_string(_buffer) << "'");
			TK_DEBUG("    Find " << resultMatch.size() << " elements");
			for (size_t iii=0; iii<resultMatch.size(); ++iii) {
				int32_t posStart = std::distance(_buffer.begin(), resultMatch[iii].first);
				int32_t posStop = std::distance(_buffer.begin(), resultMatch[iii].second);
				TK_DEBUG("          [" << iii << "] " << posStart << " to " << posStop);
			}
		}
		*/
		/*
		if (false){
			TK_DEBUG("in line : '" << etk::to_string(_buffer) << "'");
			TK_DEBUG("    Find " << resultMatch.size() << " elements");
			for (size_t iii=0; iii<resultMatch.size(); ++iii) {
				int32_t posStart = std::distance(_buffer.begin(), resultMatch[iii].first);
				int32_t posStop = std::distance(_buffer.begin(), resultMatch[iii].second);
				TK_DEBUG("          [" << iii << "] " << *resultMatch[iii].first);
				TK_DEBUG("          [" << iii << "] " << *resultMatch[iii].second);
				TK_DEBUG("          [" << iii << "] " << std::string(_buffer, posStart, posStop-posStart));
			}
		}
		*/
		return HLP_FIND_OK;
	}
	return HLP_FIND_ERROR;
}
