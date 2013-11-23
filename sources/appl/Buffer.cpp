/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <appl/Buffer.h>
#include <appl/debug.h>
#include <ewol/clipBoard.h>
#include <appl/HighlightManager.h>


const char* const appl::Buffer::eventIsModify = "edn-is-modify";
const char* const appl::Buffer::eventIsSave = "edn-is-save";
const char* const appl::Buffer::eventSelectChange = "edn-select-change";
const char* const appl::Buffer::eventChangeName = "edn-buffer-name-change";

appl::Buffer::Iterator& appl::Buffer::Iterator::operator++ (void) {
	m_value = etk::UChar::Null;
	if (m_current < 0) {
		m_current = 0;
		return *this;
	}
	if (m_data != NULL) {
		if (m_current < m_data->m_data.size() ) {
			int8_t nbChar = etk::UChar::theoricUTF8Len(m_data->m_data[m_current]);
			APPL_DEBUG("get pos=" << m_current << "len=" << nbChar);
			if (nbChar != 0) {
				m_current+=nbChar;
			} else {
				m_current++;
			}
		}
		if (m_current >= m_data->m_data.size()) {
			m_current = m_data->m_data.size();
		}
	}
	APPL_DEBUG("    ==> return " << m_current);
	return *this;
}

appl::Buffer::Iterator& appl::Buffer::Iterator::operator-- (void) {
	m_value = etk::UChar::Null;
	if (m_data != NULL) {
		if (m_current > 0) {
			int32_t iii = -1;
			while(    etk::UChar::theoricUTF8First(m_data->m_data[m_current+iii]) == false
			       && iii >= -6
			       && m_current-iii>0) {
				--iii;
			};
			m_current += iii;
		} else {
			m_current = -1;
		}
		return *this;
	} else {
		m_current = -1;
	}
	return *this;
}

char32_t appl::Buffer::Iterator::operator* (void) {
	if (m_value != etk::UChar::Null) {
		return m_value;
	}
	if (m_data == NULL) {
		APPL_ERROR("request an element that iterator not link");
		return m_value;
	}
	if (    m_current < 0
	     || m_current >= m_data->m_data.size()) {
		APPL_ERROR("request an element out of bounding !!! 0 <= " << m_current << " < " << m_data->m_data.size());
		return m_value;
	}
	char tmpVal[5];
	memset(tmpVal, 0, sizeof(tmpVal));
	tmpVal[0] = m_data->m_data[m_current];
	int8_t nbChar = etk::UChar::theoricUTF8Len(tmpVal[0]);
	for (int32_t iii=1; iii<nbChar && m_current+iii<m_data->m_data.size(); ++iii) {
		tmpVal[iii] = m_data->m_data[m_current+iii];
	}
	// transform ...
	m_value = etk::setUtf8(tmpVal);
	return m_value;
}


appl::Buffer::Iterator appl::Buffer::position(int64_t _pos) {
	return appl::Buffer::Iterator(this, _pos);
}

appl::Buffer::Iterator appl::Buffer::begin(void) {
	return position(0);
}

appl::Buffer::Iterator appl::Buffer::end(void) {
	// TODO : chek the validity of the char ...
	return position( m_data.size() );
}

appl::Buffer::Iterator appl::Buffer::cursor(void) {
	if (m_cursorPos<= 0) {
		return begin();
	}
	return position( m_cursorPos );
}

appl::Buffer::Iterator appl::Buffer::selectStart(void) {
	return position( getStartSelectionPos() );
}

appl::Buffer::Iterator appl::Buffer::selectStop(void) {
	return position( getStopSelectionPos() );
}


appl::Buffer::Buffer(void) :
  m_hasFileName(false),
  m_fileName(""),
  m_isModify(false),
  m_cursorPos(0),
  m_cursorSelectPos(-1),
  m_cursorPreferredCol(-1),
  m_nbLines(1),
  m_highlight(NULL) {
	addObjectType("appl::Buffer");
	static int32_t bufferBaseId = 0;
	m_fileName = "No Name " + std::to_string(bufferBaseId);
	bufferBaseId++;
	addEventId(eventIsModify);
	addEventId(eventIsSave);
	addEventId(eventSelectChange);
	addEventId(eventChangeName);
}

appl::Buffer::~Buffer(void) {
	if (m_highlight == NULL) {
		appl::Highlight::release(m_highlight);
	}
}

bool appl::Buffer::loadFile(const std::string& _name) {
	APPL_DEBUG("Load file : '" << _name << "'");
	m_fileName = _name;
	m_hasFileName = true;
	m_isModify = true;
	m_cursorPos = 0;
	setHighlightType("");
	etk::FSNode file(m_fileName);
	if (file.exist() == false) {
		APPL_INFO("File doesn not exist !!! " << file);
		return false;
	}
	m_nbLines = 0;
	if (true == m_data.dumpFrom(file) ) {
		countNumberofLine();
		tryFindHighlightType();
		m_isModify = false;
		return true;
	}
	return false;
}

void appl::Buffer::setFileName(const std::string& _name) {
	if (m_fileName == _name) {
		return;
	}
	m_fileName = _name;
	m_hasFileName = true;
	generateEventId(eventChangeName);
	setModification(true);
}

bool appl::Buffer::storeFile(void) {
	etk::FSNode file(m_fileName);
	if (true == m_data.dumpIn(file) ) {
		APPL_INFO("saving file : " << file);
		setModification(false);
		return true;
	}
	return false;
}

void appl::Buffer::setModification(bool _status) {
	if (m_isModify == _status) {
		return;
	}
	m_isModify = _status;
	if (m_isModify == true) {
		generateEventId(eventIsModify);
	} else {
		generateEventId(eventIsSave);
	}
}

// TODO : Naming error
void appl::Buffer::countNumberofLine(void) {
	m_nbLines = 1;
	for (Iterator it = begin();
	     (bool)it == true;
	     ++it) {
		if (*it == etk::UChar::Return) {
			++m_nbLines;
		}
	}
}


appl::Buffer::Iterator appl::Buffer::getStartLine(const appl::Buffer::Iterator& _pos) {
	appl::Buffer::Iterator startPos;
	if (false == searchBack(_pos, etk::UChar::Return, startPos)) {
		return begin();
	}
	// note search will return the position of \n ==> the lione start just after ...
	return startPos+1;
}

appl::Buffer::Iterator appl::Buffer::getEndLine(const appl::Buffer::Iterator& _pos) {
	appl::Buffer::Iterator endPos;
	if (false == search(_pos, etk::UChar::Return, endPos)) {
		endPos = end();
	}
	// Note the line end at the \n
	return endPos;
}


bool appl::Buffer::search(const appl::Buffer::Iterator& _pos, const char32_t& _search, appl::Buffer::Iterator& _result) {
	// move in the string
	char32_t value;
	for (Iterator it = _pos;
	     (bool)it == true;
	     ++it) {
		if (*it == _search) {
			_result = it;
			return true;
		}
	}
	_result = end();
	return false;
}

bool appl::Buffer::searchBack(const appl::Buffer::Iterator& _pos, const char32_t& _search, appl::Buffer::Iterator& _result) {
	// move in the string
	char32_t value;
	for (Iterator it = _pos - 1;
	     (bool)it == true;
	     --it) {
		//APPL_DEBUG("compare : " << *it << " ?= " << _search);
		if (*it == _search) {
			//APPL_DEBUG("find : " << (esize_t)it);
			_result = it;
			return true;
		}
	}
	_result = begin();
	return false;
}

void appl::Buffer::moveCursor(int64_t _pos) {
	m_cursorPreferredCol = -1;
	APPL_DEBUG("move cursor : " << _pos << "/" << m_data.size());
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
		generateEventId(eventSelectChange);
		return;
	}
	// move mode
	m_cursorPos = _pos;
	m_cursorSelectPos = -1;
	generateEventId(eventSelectChange);
}

bool appl::Buffer::getPosAround(const appl::Buffer::Iterator& _startPos,
                                appl::Buffer::Iterator &_beginPos,
                                appl::Buffer::Iterator &_endPos) {
	char32_t currentValue = *position(_startPos);
	_beginPos = begin();
	_endPos = end();
	if (    currentValue == etk::UChar::Tabulation
	     || currentValue == etk::UChar::Space) {
		APPL_DEBUG("select spacer");
		// Search back
		for (Iterator it = --position(_startPos);
		     (bool)it == true;
		     --it) {
			currentValue = *it;
			if (    currentValue != etk::UChar::Tabulation
			     && currentValue != etk::UChar::Space) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = position(_startPos);
		     (bool)it == true;
		     ++it) {
			currentValue = *it;
			if (    currentValue != etk::UChar::Tabulation
			     && currentValue != etk::UChar::Space) {
				_endPos = it;
				break;
			}
		}
		return true;
	} else if(    etk::isSpecialChar(currentValue) == false
	           || currentValue == '_') {
		APPL_DEBUG("select normal Char");
		// Search back
		for (Iterator it = --position(_startPos);
		     (bool)it == true;
		     --it) {
			currentValue = *it;
			if (    currentValue != '_'
			     && true == etk::isSpecialChar(currentValue)) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = position(_startPos);
		     (bool)it == true;
		     ++it) {
			currentValue = *it;
			if (    currentValue != '_'
			     && true == etk::isSpecialChar(currentValue)) {
				_endPos = it;
				break;
			}
		}
		return true;
	} else {
		APPL_DEBUG("select same char");
		char32_t comparechar = currentValue;
		// Search back
		for (Iterator it = --position(_startPos);
		     (bool)it == true;
		     --it) {
			currentValue = *it;
			if (comparechar != currentValue) {
				_beginPos = ++it;
				break;
			}
		}
		// Search forward
		for (Iterator it = position(_startPos);
		     (bool)it == true;
		     ++it) {
			currentValue = *it;
			if (comparechar != currentValue) {
				_endPos = it;
				break;
			}
		}
		return true;
	}
	_beginPos = begin();
	_endPos = begin();
	return false;
}

void appl::Buffer::setSelectionPos(const appl::Buffer::Iterator& _pos) {
	m_cursorSelectPos = _pos;
	generateEventId(eventSelectChange);
}

void appl::Buffer::unSelect(void) {
	m_cursorSelectPos = -1;
	generateEventId(eventSelectChange);
}

static const char *ControlCodeTable[32] = {
	 "NUL", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs",  "ht", "nl",  "vt",  "np", "cr", "so", "si",
	 "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};

void appl::Buffer::expand(esize_t& _indent, const char32_t& _value, std::u32string& _out) const {
	_out.clear();
	int32_t tabDist = 4;
	if (_value == etk::UChar::Tabulation) {
		int32_t nSpaces = tabDist - (_indent % tabDist);
		for (int32_t iii=0; iii<nSpaces; iii++) {
			_out += etk::UChar::Space;
		}
		return;
	}
	// convert ASCII control codes to readable character sequences
	if (_value == etk::UChar::Null) {
		_out += U"<nul>";
		return;
	}
	if (_value == etk::UChar::Return) {
		// nothing to display...
		_out += etk::UChar::Return;
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
	if (_value == etk::UChar::Delete) {
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
	for (Iterator it = position(_startPos);
	     (bool)it == true;
	     ++it) {
		value = *it;
		if (value == etk::UChar::Return) {
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
	for (Iterator it = --position(_startPos);
	     (bool)it == true;
	     --it) {
		value = *it;
		if (value == etk::UChar::Return) {
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
		esize_t startPos = getStartSelectionPos();
		esize_t endPos = getStopSelectionPos();
		for (Iterator it = position(startPos);
		     it != position(endPos) &&
		     (bool)it == true;
		     ++it) {
			_data += *it;
		}
		return true;
	}
	return false;
}

void appl::Buffer::copy(std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
	_data.clear();
	int64_t startPos = getStartSelectionPos();
	int64_t endPos = getStopSelectionPos();
	for (Iterator it = _pos;
	     it != _posEnd &&
	     (bool)it == true;
	     ++it) {
		_data += *it;
	}
}

bool appl::Buffer::write(const std::string& _data, const appl::Buffer::Iterator& _pos) {
	int64_t position = (int64_t)_pos;
	if (position < 0){
		position = 0;
	}
	APPL_ERROR("writye at pos: " << (int64_t)_pos << " ==> " << position);
	m_data.insert(position, (int8_t*)(_data.c_str()), _data.size());
	if (m_cursorPos < 0) {
		m_cursorPos = 0;
	}
	regenerateHighLightAt(position, 0, _data.size());
	m_selectMode = false;
	moveCursor(position+_data.size());
	countNumberofLine(); // TODO : use more intelligent counter
	setModification(true);
	return true;
}

bool appl::Buffer::replace(const std::string& _data, const appl::Buffer::Iterator& _pos, const appl::Buffer::Iterator& _posEnd) {
	int64_t position = (int64_t)_pos;
	if (position < 0){
		position = 0;
	}
	m_data.replace(position, (int64_t)_posEnd-(int64_t)_pos, (int8_t*)(_data.c_str()), _data.size());
	regenerateHighLightAt(position, (int64_t)_posEnd-(int64_t)_pos, _data.size());
	m_selectMode = false;
	moveCursor(position+_data.size());
	countNumberofLine(); // TODO : use more intelligent counter
	setModification(true);
	return true;
}

void appl::Buffer::removeSelection(void) {
	if (hasTextSelected() == false) {
		return;
	}
	int64_t startPos = getStartSelectionPos();
	int64_t endPos = getStopSelectionPos();
	m_data.remove(startPos, endPos-startPos);
	regenerateHighLightAt(startPos, endPos-startPos, 0);
	m_selectMode = false;
	moveCursor(startPos);
	countNumberofLine(); // TODO : use more intelligent counter
	setModification(true);
}

void appl::Buffer::tryFindHighlightType(void) {
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
	if (m_highlight == NULL) {
		appl::Highlight::release(m_highlight);
	}
	std::string resourceName = appl::highlightManager::getFileWithTypeType(_type);
	if (resourceName == "") {
		return;
	}
	m_highlightType = _type;
	m_highlight = appl::Highlight::keep(resourceName);
	generateHighLightAt(0, m_data.size());
}

void appl::Buffer::regenerateHighLightAt(int64_t _pos, int64_t _nbDeleted, int64_t _nbAdded) {
	// prevent ERROR...
	if (NULL == m_highlight) {
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
	for (int64_t iii = elemStart; iii < m_HLDataPass1.size(); ++iii) {
		//APPL_DEBUG("move element=" << i);
		m_HLDataPass1[iii].beginStart += _nbAdded - _nbDeleted;
		m_HLDataPass1[iii].beginStop  += _nbAdded - _nbDeleted;
		m_HLDataPass1[iii].endStart   += _nbAdded - _nbDeleted;
		m_HLDataPass1[iii].endStop    += _nbAdded - _nbDeleted;
	}
	//Regenerate Element inside range
	if (    startId == -1
	     && stopId == -1) {
		//APPL_DEBUG("*******  Regenerate ALL");
		generateHighLightAt(0, m_data.size());
	} else if(-1 == startId) {
		//APPL_DEBUG("*******  Regenerate START");
		generateHighLightAt(0, m_HLDataPass1[0].beginStart, 0);
	} else if(-1 == stopId) {
		//APPL_DEBUG("*******  Regenerate STOP");
		generateHighLightAt(m_HLDataPass1[m_HLDataPass1.size() -1].endStop, m_data.size(), m_HLDataPass1.size());
	} else {
		//APPL_DEBUG("*******  Regenerate RANGE");
		generateHighLightAt(m_HLDataPass1[startId].endStop, m_HLDataPass1[startId+1].beginStart, startId+1);
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
	for (esize_t iii = 0; iii < m_HLDataPass1.size(); ++iii) {
		if (m_HLDataPass1[iii].endStop > _startPos) {
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
	for (esize_t iii = elemStart; iii < m_HLDataPass1.size(); ++iii) {
		if (m_HLDataPass1[iii].beginStart > _endPos) {
			_stopId = iii;
			break;
		}
	}
}

void appl::Buffer::generateHighLightAt(int64_t _pos, int64_t _endPos, int64_t _addingPos) {
	if (NULL == m_highlight) {
		return;
	}
	//APPL_DEBUG("area : ("<<pos<<","<<endPos<<") insert at : " << addingPos);
	m_highlight->parse(_pos, _endPos, m_HLDataPass1, _addingPos, m_data);
}

void appl::Buffer::cleanHighLight(void) {
	// remove all element in the list...
	m_HLDataPass1.clear();
}


appl::HighlightInfo* appl::Buffer::getElementColorAtPosition(int64_t _pos, int64_t &_starPos) {
	int32_t start = etk_max(0, _starPos-1);
	for (esize_t iii = start; iii < m_HLDataPass1.size(); ++iii) {
		_starPos = iii;
		if (    m_HLDataPass1[iii].beginStart <= _pos
		     && m_HLDataPass1[iii].endStop > _pos) {
			return &m_HLDataPass1[iii];
		}
		if(m_HLDataPass1[iii].beginStart > _pos) {
			return NULL;
		}
	}
	return NULL;
}


void appl::Buffer::hightlightGenerateLines(appl::DisplayHLData& _MData, int64_t _HLStart, int64_t _nbLines) {
	_MData.posHLPass1 = 0;
	_MData.posHLPass2 = 0;
	if (NULL == m_highlight) {
		return;
	}
	//GTimeVal timeStart;
	//g_get_current_time(&timeStart);
	_HLStart = (esize_t)getStartLine(position(_HLStart));
	_MData.HLData.clear();
	int64_t HLStop = countForwardNLines(position(_HLStart), _nbLines);
	int64_t startId = 0;
	int64_t stopId = 0;
	// find element previous
	findMainHighLightPosition(_HLStart, HLStop, startId, stopId, true);

	//APPL_DEBUG("List of section between : "<< startId << " & " << stopId);
	int64_t endSearch = stopId+1;
	if (stopId == -1) {
		endSearch = m_HLDataPass1.size();
	}
	int64_t kkk;
	for (kkk = etk_max(startId, 0); kkk < endSearch; ++kkk) {
		// empty section :
		if (kkk == 0) {
			if (_HLStart < m_HLDataPass1[kkk].beginStart) {
				//APPL_DEBUG("   == > (empty section 1 ) k="<<k<<" start="<<HLStart<<" stop="<<m_HLDataPass1[k].beginStart );
				m_highlight->parse2(_HLStart,
									m_HLDataPass1[kkk].beginStart,
									_MData.HLData,
									m_data);
			} // else : nothing to do ...
		} else {
			//APPL_DEBUG("   == > (empty section 2 ) k="<<k<<" start="<<m_HLDataPass1[k-1].endStop<<" stop="<<m_HLDataPass1[k].beginStart );
			m_highlight->parse2(m_HLDataPass1[kkk-1].endStop,
								m_HLDataPass1[kkk].beginStart,
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
			//APPL_DEBUG("   == > (empty section 3 ) k="<<k<<" start="<<m_HLDataPass1[k-1].endStop<<" stop="<<HLStop );
			m_highlight->parse2(m_HLDataPass1[kkk-1].endStop,
								HLStop,
								_MData.HLData,
								m_data);
		} else {
			//APPL_DEBUG("   == > (empty section 4 ) k="<<k<<" start=0 stop="<<HLStop );
			m_highlight->parse2(0,
								HLStop,
								_MData.HLData,
								m_data);
		}
	}
	
	//GTimeVal timeStop;
	//g_get_current_time(&timeStop);
	//APPL_DEBUG("Display reAnnalyse = " << timeStop.tv_usec - timeStart.tv_usec << " micro-s");
}


appl::HighlightInfo* appl::Buffer::getElementColorAtPosition(appl::DisplayHLData& _MData, int64_t _pos) {
	int64_t start = etk_max(0, _MData.posHLPass2-1);
	for (int64_t iii=start; iii<(int32_t)_MData.HLData.size(); iii++) {
		_MData.posHLPass2 = iii;
		if(		_MData.HLData[iii].beginStart <= _pos
			&&	_MData.HLData[iii].endStop    > _pos)
		{
			return &_MData.HLData[iii];
		}
		if(_MData.HLData[iii].beginStart > _pos) {
			return getElementColorAtPosition(_pos, _MData.posHLPass1);
		}
	}
	return getElementColorAtPosition(_pos, _MData.posHLPass1);
}

uint32_t appl::Buffer::getCursorLinesId(void) {
	if (m_data.size() == 0) {
		return 0;
	}
	uint32_t line = 0;
	for (Iterator it = begin();
	    (bool)it == true && it <= cursor();
	     ++it) {
		if (*it == etk::UChar::Return) {
			++line;
		}
	}
	return line;
}