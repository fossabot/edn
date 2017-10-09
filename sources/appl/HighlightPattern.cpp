/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/debug.hpp>
#include <appl/global.hpp>
#include <appl/HighlightPattern.hpp>

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

void appl::HighlightPattern::setPatern(const etk::String& _regExp, const etk::String& _regExpStop, bool _hasEndRegEx) {
	m_regexValue[0] = _regExp;
	m_regexValue[1] = _regExpStop;
	m_hasEndRegEx = _hasEndRegEx;
	APPL_DEBUG("parse regex='" << _regExp << "' -> '" << _regExpStop << "'");
	m_hasParsingError = false;
	if (_regExp != "") {
		m_regExp[0].compile(_regExp);
		if (m_regExp[0].getStatus() == false) {
			m_hasParsingError = true;
			APPL_ERROR("can not parse regex for : " << _regExp);
		}
	}
	if (_regExpStop != "") {
		m_regExp[1].compile(_regExpStop);
		if (m_regExp[1].getStatus() == false) {
			m_hasParsingError = true;
			APPL_ERROR("can not parse regex for : " << _regExpStop);
		}
	}
}

etk::Pair<etk::String,etk::String> appl::HighlightPattern::getPaternString() {
	return etk::makePair(m_regexValue[0], m_regexValue[1]);
}

void appl::HighlightPattern::setColorGlyph(const etk::String& _colorName) {
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
	etk::String highLightName = _child.attributes["name"];
	etk::String myEdnDataTmp = "???";
	if (highLightName.size()!=0) {
		myEdnDataTmp = highLightName;
	}
	setName(myEdnDataTmp);
	setLevel(_level);
	
	exml::Element xChild = _child.nodes["color"];
	if (xChild.exist() == true) {
		etk::String myData = xChild.getText();
		if (myData.size() != 0) {
			setColorGlyph(myData);
		}
	}
	etk::String paterStart;
	etk::String paterStop;
	xChild = _child.nodes["regex"];
	if (xChild.exist() == true) {
		if (xChild.nodes.size() == 1 && xChild.nodes[0].getType() == exml::nodeType::text) {
			etk::String myData = xChild.getText();
			if (myData.size() != 0) {
				paterStart = myData;
			}
			setPatern(paterStart);
		} else {
			exml::Element xxChild = xChild.nodes["start"];
			if (xxChild.exist() == true) {
				etk::String myData = xxChild.getText();
				if (myData.size() != 0) {
					paterStart = myData;
				}
			}
			xxChild = xChild.nodes["stop"];
			if (xxChild.exist() == true) {
				etk::String myData = xxChild.getText();
				if (myData.size() != 0) {
					paterStop = myData;
				}
			}
			setPatern(paterStart, paterStop, true);
		}
	}
	xChild = _child.nodes["sub"];
	if (xChild.exist() == true) {
		etk::String myData = xChild.getText();
		if (myData.size() != 0) {
			setSubPatternName(myData);
		}
	}
}

bool appl::HighlightPattern::find(int32_t _start,
                                  int32_t _stop,
                                  appl::HighlightInfo& _resultat,
                                  const etk::Buffer& _buffer) {
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
	// when we have only one element:
	if (m_regExp[0].processOneElement(_buffer, _start, _stop) == true) {
		_resultat.start = m_regExp[0].start();
		_resultat.stop = m_regExp[0].stop();
		//APPL_DEBUG("find data at : start=" << _resultat.start << " stop=" << _resultat.stop << " data='" <<etk::String(_buffer, _resultat.start, _resultat.stop-_resultat.start) << "'" );
		//APPL_DEBUG("find data at : start=" << _resultat.start << " stop=" << _resultat.stop );
		if (m_hasEndRegEx == true) {
			// when no regex specify ==> get all the buffer ...
			if (m_regexValue[1].size() == 0) {
				_resultat.stop = _stop;
				return true;
			}
			_start = _resultat.stop;
			while (_start < _stop) {
				if (m_regExp[1].processOneElement(_buffer, _start, _stop) == true) {
					_resultat.stop = m_regExp[1].stop();
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
