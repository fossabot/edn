/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <etk/types.h>
#include <etk/stdTools.h>
#include <appl/Buffer.h>
#include <appl/debug.h>
#include <ewol/context/clipBoard.h>
#include <appl/HighlightManager.h>

#undef __class__
#define __class__ "Buffer"


appl::Buffer::Iterator appl::Buffer::position(int64_t _pos) {
	return m_data.begin() + _pos;
}

appl::Buffer::Iterator appl::Buffer::begin() {
	return m_data.begin();
}

appl::Buffer::Iterator appl::Buffer::end() {
	// TODO : chek the validity of the char ...
	return m_data.end();
}

appl::Buffer::Iterator appl::Buffer::cursor() {
	if (m_cursorPos<= 0) {
		return m_data.begin();
	}
	return m_data.begin() + m_cursorPos;
}

appl::Buffer::Iterator appl::Buffer::selectStart() {
	return m_data.begin() + getStartSelectionPos();
}

appl::Buffer::Iterator appl::Buffer::selectStop() {
	return m_data.begin() + getStopSelectionPos();
}

appl::Buffer::Buffer() :
  signalIsModify(*this, "is-modify"),
  signalIsSave(*this, "is-save"),
  signalSelectChange(*this, "select-change"),
  signalChangeName(*this, "change-name"),
  m_hasFileName(false),
  m_fileName(""),
  m_isModify(false),
  m_cursorPos(0),
  m_cursorSelectPos(-1),
  m_cursorPreferredCol(-1),
  m_nbLines(1),
  m_highlight(nullptr) {
	addObjectType("appl::Buffer");
	static int32_t bufferBaseId = 0;
	m_fileName = "No Name " + etk::to_string(bufferBaseId);
	bufferBaseId++;
}

void appl::Buffer::init() {
	ewol::Object::init();
}

appl::Buffer::~Buffer() {
	APPL_ERROR("REAL remove buffer : '" << m_name << "'");
}

bool appl::Buffer::loadFile(const std::string& _name) {
	APPL_DEBUG("Convert filename :'" << _name << "'");
	etk::FSNode file(_name);
	std::string name = file.getName();
	APPL_INFO("Load file : '" << name << "'");
	m_fileName = name;
	m_hasFileName = true;
	m_isModify = true;
	m_cursorPos = 0;
	setHighlightType("");
	m_nbLines = 0;
	etk::FSNode filee(m_fileName);
	if (filee.exist() == true) {
		m_data = file.fileReadAllU32String();
		countNumberofLine();
		tryFindHighlightType();
		m_isModify = false;
		return true;
	}
	return false;
}

void appl::Buffer::setFileName(const std::string& _name) {
	APPL_DEBUG("Convert filename :'" << _name << "'");
	etk::FSNode file(_name);
	std::string name = file.getName();
	if (m_fileName == name) {
		return;
	}
	m_fileName = name;
	m_hasFileName = true;
	signalChangeName.emit();
	setModification(true);
}

bool appl::Buffer::storeFile() {
	etk::FSNode filee(m_fileName);
	filee.fileWriteAll(m_data);
	APPL_INFO("saving file : " << m_fileName);
	setModification(false);
	return true;
}

void appl::Buffer::setModification(bool _status) {
	if (m_isModify == _status) {
		return;
	}
	m_isModify = _status;
	if (m_isModify == true) {
		signalIsModify.emit();
	} else {
		signalIsSave.emit();
	}
}

// TODO : Naming error
void appl::Buffer::countNumberofLine() {
	m_nbLines = 1;
	for (auto &it : m_data) {
		if (it == u32char::Return) {
			++m_nbLines;
		}
	}
}


appl::Buffer::Iterator appl::Buffer::getStartLine(const appl::Buffer::Iterator& _pos) {
	appl::Buffer::Iterator startPos;
	if (false == searchBack(_pos, u32char::Return, startPos)) {
		return begin();
	}
	// note search will return the position of \n ==> the lione start just after ...
	return startPos+1;
}

appl::Buffer::Iterator appl::Buffer::getEndLine(const appl::Buffer::Iterator& _pos) {
	appl::Buffer::Iterator endPos;
	if (false == search(_pos, u32char::Return, endPos)) {
		endPos = end();
	}
	// Note the line end at the \n
	return endPos;
}


bool appl::Buffer::search(const appl::Buffer::Iterator& _pos, const char32_t& _search, appl::Buffer::Iterator& _result) {
	// move in the string
	for (Iterator it = _pos;
	     it != m_data.end();
	     ++it) {
		if (*it == _search) {
			_result = it;
			return true;
		}
	}
	_result = m_data.end();
	return false;
}

bool appl::Buffer::searchBack(const appl::Buffer::Iterator& _pos, const char32_t& _search, appl::Buffer::Iterator& _result) {
	// move in the string
	for (Iterator it = _pos - 1;
	     it != m_data.begin();
	     --it) {
		//APPL_DEBUG("compare : " << *it << " ?= " << _search);
		if (*it == _search) {
			//APPL_DEBUG("find : " << (int32_t)it);
			_result = it;
			return true;
		}
	}
	_result = begin();
	return false;
}

bool appl::Buffer::search(const appl::Buffer::Iterator& _pos,
                          const std::u32string& _search,
                          appl::Buffer::Iterator& _result,
                          bool _caseSensitive) {
	if (_search.size() <= 0 ) {
		return false;
	}
	if (_caseSensitive == true) {
		// move in the string
		for (Iterator it = _pos;
		     it == m_data.end();
		     ++it) {
			if (*it == _search[0]) {
				// find the first char ==> check next...
				bool find = true;
				Iterator tmp = it;
				for (size_t iii=0; iii<_search.size(); ++iii) {
					if (*tmp != _search[iii]) {
						find = false;
						break;
					}
					++tmp;
					if (tmp == m_data.end()) {
						if (iii != _search.size()-1) {
							find = false;
						}
						break;
					}
				}
				if (find == true) {
					_result = it;
					return true;
				}
			}
		}
	} else {
		char32_t firstElement = tolower(_search[0]);
		// move in the string
		for (Iterator it = _pos;
		     it == m_data.end();
		     ++it) {
			if ((char32_t)tolower(*it) == firstElement) {
				// find the first char ==> check next...
				bool find = true;
				Iterator tmp = it;
				for (size_t iii=0; iii<_search.size(); ++iii) {
					if (tolower(*tmp) != tolower(_search[iii])) {
						find = false;
						break;
					}
					++tmp;
					if (tmp != m_data.end()) {
						if (iii != _search.size()-1) {
							find = false;
						}
						break;
					}
				}
				if (find == true) {
					_result = it;
					return true;
				}
			}
		}
	}
	_result = end();
	return false;
}

bool appl::Buffer::searchBack(const appl::Buffer::Iterator& _pos,
                              const std::u32string& _search,
                              appl::Buffer::Iterator& _result,
                              bool _caseSensitive) {
	if (_search.size() <= 0 ) {
		return false;
	}
	char32_t lastElement = _search[_search.size()-1];
	if (_caseSensitive == true) {
		// move in the string
		for (Iterator it = _pos - 1;
		     it != m_data.begin();
		     --it) {
			//APPL_DEBUG("compare : " << *it << " ?= " << _search);
			if (*it == lastElement) {
				// find the last char ==> check previous...
				bool find = true;
				_result = it;
				for (int64_t iii=_search.size()-1; iii>=0; --iii) {
					if (*_result != _search[iii]) {
						find = false;
						break;
					}
					--_result;
					if (_result == m_data.begin()) {
						if (iii != 0) {
							find = false;
						}
						break;
					}
				}
				if (find == true) {
					_result++;
					return true;
				}
			}
		}
	} else {
		lastElement = tolower(lastElement);
		// move in the string
		for (Iterator it = _pos - 1;
		     it != m_data.begin();
		     --it) {
			//APPL_DEBUG("compare : " << *it << " ?= " << _search);
			if ((char32_t)tolower(*it) == lastElement) {
				// find the last char ==> check previous...
				bool find = true;
				_result = it;
				for (int64_t iii=_search.size()-1; iii>=0; --iii) {
					if (tolower(*_result) != tolower(_search[iii])) {
						find = false;
						break;
					}
					--_result;
					if (_result == m_data.begin()) {
						if (iii != 0) {
							find = false;
						}
						break;
					}
				}
				if (find == true) {
					_result++;
					return true;
				}
			}
		}
	}
	_result = begin();
	return false;
}

void appl::Buffer::moveCursor(int64_t _pos) {
	m_cursorPreferredCol = -1;
	APPL_VERBOSE("move cursor : " << _pos << "/" << m_data.size());
	// selecting mode ...
	if (m_selectMode == true) {
		if (m_cursorSelectPos == -1) {
			m_cursorSelectPos = m_cursorPos;
			if (m_cursorSelectPos < 0) {
				m_cursorSelectPos = 0;
			}
		}
		//APPL_DEBUG("Select : " << m_cursorSelectPos << " ==> " << _pos);
		m_cursorPos = _pos;
		if (m_cursorPos == m_cursorSelectPos) {
			m_cursorSelectPos = -1;
		}
		signalSelectChange.emit();
		return;
	}
	// move mode
	m_cursorPos = _pos;
	m_cursorSelectPos = -1;
	signalSelectChange.emit();
}

bool appl::Buffer::getPosAround(const appl::Buffer::Iterator& _startPos,
                                appl::Buffer::Iterator &_beginPos,
                                appl::Buffer::Iterator &_endPos) {
	char32_t currentValue = *_startPos;
	_beginPos = begin();
	_endPos = end();
	if (    currentValue == u32char::Tabulation
	     || currentValue == u32char::Space) {
		APPL_DEBUG("select spacer");
		// Search back
		for (Iterator it = --Iterator(_startPos);
		     it != m_data.begin();
		     --it) {
			currentValue = *it;
			if (    currentValue != u32char::Tabulation
			     && currentValue != u32char::Space) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = _startPos;
		     it != m_data.end();
		     ++it) {
			currentValue = *it;
			if (    currentValue != u32char::Tabulation
			     && currentValue != u32char::Space) {
				_endPos = it;
				break;
			}
		}
		return true;
	} else if(    u32char::isSpecialChar(currentValue) == false
	           || currentValue == '_') {
		APPL_DEBUG("select normal Char");
		// Search back
		for (Iterator it = --Iterator(_startPos);
		     it == m_data.begin();
		     --it) {
			currentValue = *it;
			if (    currentValue != '_'
			     && u32char::isSpecialChar(currentValue) == true) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = _startPos;
		     it != m_data.end();
		     ++it) {
			currentValue = *it;
			if (    currentValue != '_'
			     && u32char::isSpecialChar(currentValue) == true) {
				_endPos = it;
				break;
			}
		}
		return true;
	} else {
		APPL_DEBUG("select same char");
		char32_t comparechar = currentValue;
		// Search back
		for (Iterator it = --Iterator(_startPos);
		     it == m_data.begin();
		     --it) {
			currentValue = *it;
			if (comparechar != currentValue) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = --Iterator(_startPos);
		     it != m_data.end();
		     ++it) {
			currentValue = *it;
			if (comparechar != currentValue) {
				_endPos = it;
				break;
			}
		}
		return true;
	}
	_beginPos = m_data.begin();
	_endPos = m_data.begin();
	return false;
}

void appl::Buffer::setSelectionPos(const appl::Buffer::Iterator& _pos) {
	m_cursorSelectPos = std::distance(m_data.begin(), _pos);
	signalSelectChange.emit();
}

void appl::Buffer::unSelect() {
	m_cursorSelectPos = -1;
	signalSelectChange.emit();
}

static const char *ControlCodeTable[32] = {
	 "NUL", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs",  "ht", "nl",  "vt",  "np", "cr", "so", "si",
	 "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};

void appl::Buffer::expand(int32_t& _indent, const char32_t& _value, std::u32string& _out) const {
	_out.clear();
	int32_t tabDist = 4;
	if (_value == u32char::Tabulation) {
		int32_t nSpaces = tabDist - (_indent % tabDist);
		for (int32_t iii=0; iii<nSpaces; iii++) {
			_out += u32char::Space;
		}
		return;
	}
	// convert ASCII control codes to readable character sequences
	if (_value == u32char::Null) {
		_out += U"<nul>";
		return;
	}
	if (_value == u32char::Return) {
		// nothing to display...
		_out += u32char::Return;
		return;
	}
	if (_value <= 31) {
		_out += '<';
		const char * tmp = ControlCodeTable[_value];
		while (*tmp!='\0') {
			_out += *tmp;
			tmp++;
		}
		_out += '>';
		return;
	}
	if (_value == u32char::Delete) {
		_out += U"<del>";
		return;
	}
	// nothing to do ...
	_out += _value;
	//APPL_DEBUG("plop : " << _out);
}

appl::Buffer::Iterator appl::Buffer::countForwardNLines(const appl::Buffer::Iterator& _startPos, int32_t _nLines) {
	char32_t value;
	int32_t lineCount = 0;
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	for (Iterator it = Iterator(_startPos);
	     it != m_data.end();
	     ++it) {
		value = *it;
		if (value == u32char::Return) {
			lineCount++;
			if (lineCount == _nLines) {
				//APPL_INFO("    == > (1) at position=" << myPosIt.Position()+1 );
				return ++it;
			}
		}
	}
	//APPL_INFO("    == > (2) at position=" << myPosIt.Position() );
	return end();
}

appl::Buffer::Iterator appl::Buffer::countBackwardNLines(const appl::Buffer::Iterator& _startPos, int32_t _nLines) {
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	char32_t value;
	int32_t lineCount = 0;
	for (Iterator it = --Iterator(_startPos);
	     it != m_data.begin();
	     --it) {
		value = *it;
		if (value == u32char::Return) {
			lineCount++;
			if (lineCount >= _nLines) {
				//APPL_INFO("    == > (1) at position=" << myPosIt.Position()+1 );
				return ++it;
			}
		}
	}
	//APPL_INFO("    == > (2) at position=0");
	return begin();
}



bool appl::Buffer::copy(std::string& _data) {
	_data.clear();
	if (hasTextSelected() == true) {
		int32_t startPos = getStartSelectionPos();
		int32_t endPos = getStopSelectionPos();
		for (Iterator it = position(startPos);
		     it != position(endPos) &&
		     it != m_data.end();
		     ++it) {
			_data += *it;
		}
		return true;
	}
	return false;
}

void appl::Buffer::copy(std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
	_data.clear();
	for (Iterator it = _pos;
	     it != _posEnd &&
	     it != m_data.end();
	     ++it) {
		_data += *it;
	}
}

bool appl::Buffer::write(const std::string& _data, const appl::Buffer::Iterator& _pos) {
	std::u32string data2 = utf8::convertUnicode(_data);
	APPL_VERBOSE("write at pos: " << std::distance(m_data.begin(), _pos) << " data : " << data2);
	m_data.insert(_pos, data2.begin(), data2.end());
	if (m_cursorPos < 0) {
		m_cursorPos = 0;
	}
	regenerateHighLightAt(std::distance(m_data.begin(), _pos), 0, data2.size());
	m_selectMode = false;
	moveCursor(std::distance(m_data.begin(),_pos+data2.size()));
	countNumberofLine(); // TODO : use more intelligent counter
	setModification(true);
	return true;
}

bool appl::Buffer::replace(const std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
	std::u32string data2 = utf8::convertUnicode(_data);
	m_data.replace(_pos, _posEnd, data2.c_str(), data2.size());
	regenerateHighLightAt(std::distance(m_data.begin(),_pos), std::distance(m_data.begin(),_posEnd)-std::distance(m_data.begin(),_pos), data2.size());
	m_selectMode = false;
	moveCursor(std::distance(m_data.begin(),_pos+data2.size()));
	countNumberofLine(); // TODO : use more intelligent counter
	setModification(true);
	return true;
}

void appl::Buffer::removeSelection() {
	if (hasTextSelected() == false) {
		return;
	}
	int64_t startPos = getStartSelectionPos();
	int64_t endPos = getStopSelectionPos();
	m_data.erase(startPos, endPos-startPos);
	regenerateHighLightAt(startPos, endPos-startPos, 0);
	m_selectMode = false;
	moveCursor(startPos);
	countNumberofLine(); // TODO : use more intelligent counter
	setModification(true);
}

void appl::Buffer::tryFindHighlightType() {
	etk::FSNode file(m_fileName);
	std::string type = appl::highlightManager::getTypeExtention(file.fileGetExtention());
	if (type.size() == 0) {
		return;
	}
	APPL_DEBUG("Find extention type: " << type);
	setHighlightType(type);
}

void appl::Buffer::setHighlightType(const std::string& _type) {
	m_highlightType = "";
	cleanHighLight();
	m_highlight.reset();
	std::string resourceName = appl::highlightManager::getFileWithTypeType(_type);
	if (resourceName == "") {
		return;
	}
	m_highlightType = _type;
	m_highlight = appl::Highlight::create(resourceName);
	generateHighLightAt(0, m_data.size());
}

void appl::Buffer::regenerateHighLightAt(int64_t _pos, int64_t _nbDeleted, int64_t _nbAdded) {
	// prevent ERROR...
	if (nullptr == m_highlight) {
		return;
	}
	// prevent No data Call
	if (    _nbDeleted == 0
	     && _nbAdded == 0) {
		return;
	}
	// normal case
	//APPL_INFO("(pos="<<pos<<", nbDeleted="<<nbDeleted<<", nbAdded=" << nbAdded << "\");");
	int64_t posEnd = _pos + _nbDeleted;
	// search position of the old element to reparse IT...
	int64_t startId;
	int64_t stopId;
	// clean data if needed
	if (m_HLDataPass1.size() == 0) {
		// Parse the new element ...
		generateHighLightAt(0, m_data.size());
		return;
	}
	// find element previous
	findMainHighLightPosition(_pos, posEnd, startId, stopId, true);

	// remove deprecated element
	if (    startId == -1
	     && stopId == -1) {
		m_HLDataPass1.clear();
	} else if (startId == -1) {
		if (stopId == 0){
			m_HLDataPass1.erase(m_HLDataPass1.begin());
			//APPL_DEBUG("1 * Erase 0");
		} else {
			m_HLDataPass1.erase(m_HLDataPass1.begin(), m_HLDataPass1.begin()+stopId);
			//APPL_DEBUG("2 * Erase 0->" << stopId);
		}
	} else if (stopId == -1) {
		//APPL_DEBUG("3 * Erase " << startId+1 << "-> end");
		m_HLDataPass1.erase(m_HLDataPass1.begin()+startId+1, m_HLDataPass1.end());
		stopId = -1;
	} else {
		int32_t currentSize = m_HLDataPass1.size();
		//APPL_DEBUG("4 * Erase " << startId+1 << "->" << stopId << " in " << currentSize << " elements" );
		m_HLDataPass1.erase(m_HLDataPass1.begin()+startId+1, m_HLDataPass1.begin()+stopId);
		if (stopId == currentSize-1) {
			stopId = -1;
		}
	}
	//APPL_DEBUG("new size=" << (int32_t)m_HLDataPass1.size()-1);
	// update position after the range position : 
	int64_t elemStart;
	if (startId == -1) {
		elemStart = 0;
	} else {
		elemStart = startId+1;
	}
	for (auto it(m_HLDataPass1.begin()+elemStart); it != m_HLDataPass1.end(); ++it) {
		//APPL_DEBUG("move element=" << i);
		it->start += _nbAdded - _nbDeleted;
		it->stop  += _nbAdded - _nbDeleted;
	}
	//Regenerate Element inside range
	if (    startId == -1
	     && stopId == -1) {
		//APPL_DEBUG("*******  Regenerate ALL");
		generateHighLightAt(0, m_data.size());
	} else if(-1 == startId) {
		//APPL_DEBUG("*******  Regenerate START");
		generateHighLightAt(0, m_HLDataPass1[0].start, 0);
	} else if(-1 == stopId) {
		//APPL_DEBUG("*******  Regenerate STOP");
		generateHighLightAt(m_HLDataPass1[m_HLDataPass1.size() -1].stop, m_data.size(), m_HLDataPass1.size());
	} else {
		//APPL_DEBUG("*******  Regenerate RANGE");
		generateHighLightAt(m_HLDataPass1[startId].stop, m_HLDataPass1[startId+1].start, startId+1);
	}
}

void appl::Buffer::findMainHighLightPosition(int64_t _startPos,
                                             int64_t _endPos,
                                             int64_t& _startId,
                                             int64_t& _stopId,
                                             bool _backPreviousNotEnded) {
	_startId = -1;
	_stopId = -1;
	/* rules to start stop:
		HighLight data ----
		remove area    ****
		Start pos      S
		End pos        E

	Some Case :
		-----------          ------------          -------------            ----------
			      S              ****              E                                  

		-----------          ------------          -------------            ----------
			      S                **********      E                                  

		-----------          ------------          -------------            ----------
			                            S   ****   E                                  

		-----------          ------------          -------------            ----------
			      S     *********                  E                                  

		-----------          ------------          -------------            ----------
			      S     *********************      E                                  

		-----------          ------------          -------------            ----------
			      S               ************************                  E         

		-----------          ------------          -------------            ----------
			      S     *****************          E                                  

		-----------          ------------          -------------            ----------
			      S          ***************       E                                  

		-----------          ------------       
			      S          ***************       E=-1

			                 ------------          -------------            ----------
			      S=-1      ***************        E                                  
	*/
	for (size_t iii = 0; iii < m_HLDataPass1.size(); ++iii) {
		if (m_HLDataPass1[iii].stop > _startPos) {
			break;
		}
		_startId = iii;
	}
	// go back while the previous element is not eneded
	if (_backPreviousNotEnded == true) {
		for (int64_t iii = _startId; iii >= 0; --iii) {
			if (m_HLDataPass1[iii].notEnded == false) {
				break;
			}
			_startId = iii-1;
		}
	}
	int64_t elemStart;
	if(_startId == -1) {
		elemStart = 0;
	} else {
		elemStart = _startId+1;
	}
	for (size_t iii = elemStart; iii < m_HLDataPass1.size(); ++iii) {
		if (m_HLDataPass1[iii].start > _endPos) {
			_stopId = iii;
			break;
		}
	}
}

void appl::Buffer::generateHighLightAt(int64_t _pos, int64_t _endPos, int64_t _addingPos) {
	if (nullptr == m_highlight) {
		return;
	}
	//APPL_DEBUG("area : ("<<pos<<","<<endPos<<") insert at : " << addingPos);
	m_highlight->parse(_pos, _endPos, m_HLDataPass1, _addingPos, m_data);
}

void appl::Buffer::cleanHighLight() {
	// remove all element in the list...
	m_HLDataPass1.clear();
}


appl::HighlightInfo* appl::Buffer::getElementColorAtPosition(int64_t _pos, int64_t &_starPos) {
	int32_t start = std::max((int64_t)0, _starPos-1);
	for (size_t iii = start; iii < m_HLDataPass1.size(); ++iii) {
		_starPos = iii;
		if (    m_HLDataPass1[iii].start <= _pos
		     && m_HLDataPass1[iii].stop > _pos) {
			return &m_HLDataPass1[iii];
		}
		if(m_HLDataPass1[iii].start > _pos) {
			return nullptr;
		}
	}
	return nullptr;
}


void appl::Buffer::hightlightGenerateLines(appl::DisplayHLData& _MData, const appl::Buffer::Iterator& _HLStart, int64_t _nbLines) {
	_MData.posHLPass1 = 0;
	_MData.posHLPass2 = 0;
	if (nullptr == m_highlight) {
		return;
	}
	//int64_t timeStart = ewol::getTime();
	
	appl::Buffer::Iterator HLStartLine = getStartLine(_HLStart);
	int64_t HLStartPos = std::distance(m_data.begin(),HLStartLine);
	_MData.HLData.clear();
	int64_t HLStop = std::distance(m_data.begin(),countForwardNLines(HLStartLine, _nbLines));
	int64_t startId = 0;
	int64_t stopId = 0;
	// find element previous
	findMainHighLightPosition(std::distance(m_data.begin(),_HLStart), HLStop, startId, stopId, true);

	//APPL_DEBUG("List of section between : "<< startId << " & " << stopId);
	int64_t endSearch = stopId+1;
	if (stopId == -1) {
		endSearch = m_HLDataPass1.size();
	}
	int64_t kkk;
	for (kkk = std::max(startId, (int64_t)0); kkk < endSearch; ++kkk) {
		// empty section :
		if (kkk == 0) {
			if (HLStartPos < m_HLDataPass1[kkk].start) {
				APPL_VERBOSE("   == > (empty section 1 ) kkk=" << kkk <<
				             " start=" << HLStartPos <<
				             " stop=" << m_HLDataPass1[kkk].start );
				m_highlight->parse2(HLStartPos,
									m_HLDataPass1[kkk].start,
									_MData.HLData,
									m_data);
			} // else : nothing to do ...
		} else {
			APPL_VERBOSE("   == > (empty section 2 ) kkk=" << kkk <<
			             " start=" << m_HLDataPass1[kkk-1].stop <<
			             " stop=" << m_HLDataPass1[kkk].start );
			m_highlight->parse2(m_HLDataPass1[kkk-1].stop,
								m_HLDataPass1[kkk].start,
								_MData.HLData,
								m_data);
		}
		// under section :
		//APPL_DEBUG("   == > (under section   ) k="<<k<<" start="<<m_HLDataPass1[k].beginStart<<" stop="<<m_HLDataPass1[k].endStop << " subSectionOfID=" << 99999999);
		// TODO : ...
	}
	if (endSearch == (int32_t)m_HLDataPass1.size() ){
		//if(		k < (int32_t)m_HLDataPass1.size()) {
		if (m_HLDataPass1.size() != 0) {
			APPL_VERBOSE("   == > (empty section 3 ) kkk=" << kkk <<
			             " start=" << m_HLDataPass1[kkk-1].stop <<
			             " stop=" << HLStop );
			m_highlight->parse2(m_HLDataPass1[kkk-1].stop,
								HLStop,
								_MData.HLData,
								m_data);
		} else {
			APPL_VERBOSE("   == > (empty section 4 ) kkk=" << kkk <<
			             " start=0 stop=" << HLStop );
			m_highlight->parse2(0,
								HLStop,
								_MData.HLData,
								m_data);
		}
	}
	/*
	int64_t timeStop = ewol::getTime();
	APPL_DEBUG("Display 2nd pass = " << (timeStop-timeStart)/1000.0f << " milli-second");
	*/
}


appl::HighlightInfo* appl::Buffer::getElementColorAtPosition(appl::DisplayHLData& _MData, int64_t _pos) {
	int64_t start = std::max((int64_t)0, _MData.posHLPass2-1);
	for (int64_t iii=start; iii<(int32_t)_MData.HLData.size(); iii++) {
		_MData.posHLPass2 = iii;
		if(		_MData.HLData[iii].start <= _pos
			&&	_MData.HLData[iii].stop    > _pos)
		{
			return &_MData.HLData[iii];
		}
		if(_MData.HLData[iii].start > _pos) {
			return getElementColorAtPosition(_pos, _MData.posHLPass1);
		}
	}
	return getElementColorAtPosition(_pos, _MData.posHLPass1);
}

uint32_t appl::Buffer::getCursorLinesId() {
	if (m_data.size() == 0) {
		return 0;
	}
	uint32_t line = 0;
	for (Iterator it = begin();
	     it != m_data.end() && it <= cursor();
	     ++it) {
		if (*it == u32char::Return) {
			++line;
		}
	}
	return line;
}

namespace etk {
	template<> std::string to_string<std::shared_ptr<appl::Buffer>>(const std::shared_ptr<appl::Buffer>& _obj) {
		if (_obj != nullptr) {
			return _obj->getFileName();
		}
		return "";
	}
	template<> std::u32string to_u32string<std::shared_ptr<appl::Buffer>>(const std::shared_ptr<appl::Buffer>& _obj) {
		return etk::to_u32string(etk::to_string(_obj));
	}
	
	template<> bool from_string<std::shared_ptr<appl::Buffer>>(std::shared_ptr<appl::Buffer>& _variableRet, const std::string& _value) {
		if (_variableRet != nullptr) {
			_variableRet->loadFile(_value);
			return true;
		}
		return false;
	}
	template<> bool from_string<std::shared_ptr<appl::Buffer>>(std::shared_ptr<appl::Buffer>& _variableRet, const std::u32string& _value) {
		return from_string(_variableRet, etk::to_string(_value));
	}
	template<> std::string to_string<appl::Buffer>(const appl::Buffer& _obj) {
		return _obj.getFileName();
	}
	template<> std::u32string to_u32string<appl::Buffer>(const appl::Buffer& _obj) {
		return etk::to_u32string(etk::to_string(_obj));
	}
	
	template<> bool from_string<appl::Buffer>(appl::Buffer& _variableRet, const std::string& _value) {
		_variableRet.loadFile(_value);
		return true;
	}
	template<> bool from_string<appl::Buffer>(appl::Buffer& _variableRet, const std::u32string& _value) {
		return from_string(_variableRet, etk::to_string(_value));
	}
};

