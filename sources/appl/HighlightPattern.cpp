/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/debug.h>
#include <appl/global.h>
#include <appl/HighlightPattern.h>

appl::HighlightPattern::HighlightPattern(const ememory::SharedPtr<appl::GlyphPainting>& _glyphPainting, const exml::Element& _child, int32_t _level) :
  m_glyphPainting(_glyphPainting),
  m_paternName(""),
  m_hasParsingError(true),
  m_regexValue(),
  m_hasEndRegEx(false),
  m_regExp(),
  m_colorName(""),
  m_level(0) {
	parseRules(_child, _level);
}

appl::HighlightPattern::HighlightPattern() :
  m_glyphPainting(),
  m_paternName(""),
  m_hasParsingError(true),
  m_regexValue(),
  m_hasEndRegEx(false),
  m_regExp(),
  m_colorName(""),
  m_level(0) {
	
}

appl::HighlightPattern::~HighlightPattern() {
	
}

void appl::HighlightPattern::setPatern(const std::string& _regExp, const std::string& _regExpStop, bool _hasEndRegEx) {
	m_regexValue[0] = _regExp;
	m_regexValue[1] = _regExpStop;
	m_hasEndRegEx = _hasEndRegEx;
	APPL_DEBUG("parse regex='" << _regExp << "' -> '" << _regExpStop << "'");
	m_hasParsingError = false;
	if (_regExp != "") {
		try {
			m_regExp[0].assign(_regExp, std::regex_constants::optimize | std::regex_constants::ECMAScript);
		} catch (std::regex_error e) {
			m_hasParsingError = true;
			APPL_ERROR("can not parse regex : '" << e.what() << "' for : " << _regExp);
		}
	}
	if (_regExpStop != "") {
		try {
			m_regExp[1].assign(_regExpStop, std::regex_constants::optimize | std::regex_constants::ECMAScript);
		} catch (std::regex_error e) {
			m_hasParsingError = true;
			APPL_ERROR("can not parse regex : '" << e.what() << "' for : " << _regExpStop);
		}
	}
}

std::pair<std::string,std::string> appl::HighlightPattern::getPaternString() {
	return std::make_pair(m_regexValue[0], m_regexValue[1]);
}

void appl::HighlightPattern::setColorGlyph(const std::string& _colorName) {
	m_colorName = _colorName;
	m_colorId = m_glyphPainting->request(m_colorName);
	APPL_VERBOSE("Resuest color name '" << m_colorName << "' => id=" << m_colorId);
}

void appl::HighlightPattern::display() {
	APPL_INFO("patern : '" << m_paternName << "' level=" << m_level );
	APPL_INFO("  == > colorName '" << m_colorName << "'");
	APPL_INFO("  == > regex '" << m_regexValue[0] << "' ==> '" << m_regexValue[1] << "'");
}

void appl::HighlightPattern::parseRules(const exml::Element& _child, int32_t _level) {
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
	std::string highLightName = _child.attributes["name"];
	std::string myEdnDataTmp = "???";
	if (highLightName.size()!=0) {
		myEdnDataTmp = highLightName;
	}
	setName(myEdnDataTmp);
	setLevel(_level);
	
	exml::Element xChild = _child.nodes["color"];
	if (xChild.exist() == true) {
		std::string myData = xChild.getText();
		if (myData.size() != 0) {
			setColorGlyph(myData);
		}
	}
	std::string paterStart;
	std::string paterStop;
	xChild = _child.nodes["regex"];
	if (xChild.exist() == true) {
		if (xChild.nodes.size() == 1 && xChild.nodes[0].getType() == exml::nodeType::text) {
			std::string myData = xChild.getText();
			if (myData.size() != 0) {
				paterStart = myData;
			}
			setPatern(paterStart);
		} else {
			exml::Element xxChild = xChild.nodes["start"];
			if (xxChild.exist() == true) {
				std::string myData = xxChild.getText();
				if (myData.size() != 0) {
					paterStart = myData;
				}
			}
			xxChild = xChild.nodes["stop"];
			if (xxChild.exist() == true) {
				std::string myData = xxChild.getText();
				if (myData.size() != 0) {
					paterStop = myData;
				}
			}
			setPatern(paterStart, paterStop, true);
		}
	}
	xChild = _child.nodes["sub"];
	if (xChild.exist() == true) {
		std::string myData = xChild.getText();
		if (myData.size() != 0) {
			setSubPatternName(myData);
		}
	}
}


static std::pair<int64_t,int64_t> findRegex(int32_t _start,
                                            int32_t _stop,
                                            std::regex& _regex,
                                            const std::string& _buffer) {
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
	if (    _stop < 0
	     || size_t(_stop) > _buffer.size()) {
		APPL_ERROR(" error in indexing for regex ... _stop=" << _stop << " >= _buffer.size()=" << _buffer.size());
		return std::pair<int64_t,int64_t>(-1,0);
	}
	if (    _start < 0
	     || size_t(_start) > _buffer.size()) {
		APPL_ERROR(" error in indexing for regex ... _start=" << _start << " >= _buffer.size()=" << _buffer.size());
		return std::pair<int64_t,int64_t>(-1,0);
	}
	if (_start > _stop) {
		APPL_ERROR(" error in indexing for regex ... _start=" << _start << " > _stop=" << _stop);
		return std::pair<int64_t,int64_t>(-1,0);
	}
	std::regex_search(_buffer.begin() + _start, _buffer.begin() + _stop, resultMatch, _regex, flags);
	if (resultMatch.size() > 0) {
		int64_t start = std::distance(_buffer.begin(), resultMatch[0].first);
		int64_t stop = std::distance(_buffer.begin(), resultMatch[0].second);
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
		return std::pair<int64_t,int64_t>(start,stop);
	}
	return std::pair<int64_t,int64_t>(-1,0);
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
	// phase 1 start regEx or global regex:
	if (m_regexValue[0].size() == 0) {
		return false;
	}
	std::pair<int64_t,int64_t> ret0 = findRegex(_start, _stop, m_regExp[0], _buffer);
	if (ret0.first >= 0) {
		_resultat.start = ret0.first;
		_resultat.stop = ret0.second;
		//APPL_DEBUG("find data at : start=" << _resultat.start << " stop=" << _resultat.stop << " data='" <<std::string(_buffer, _resultat.start, _resultat.stop-_resultat.start) << "'" );
		// second step : Complex searching ...
		if (m_hasEndRegEx == true) {
			// when no regex specify ==> get all the buffer ...
			if (m_regexValue[1].size() == 0) {
				_resultat.stop = _stop;
				return true;
			}
			_start = _resultat.stop;
			while (_start < _stop) {
				std::pair<int64_t,int64_t> ret1 = findRegex(_start, _stop, m_regExp[1], _buffer);
				if (ret1.first >= 0) {
					_resultat.stop = ret1.second;
					return true;
				}
				_start++;
			}
			_resultat.stop = _stop;
			_resultat.notEnded = true; // TODO : Manege back the not ended ...
			return false;
		}
		return true;
	}
	return false;
}
