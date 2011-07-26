/**
 *******************************************************************************
 * @file RegExp.cpp
 * @brief Editeur De N'ours : Regular expression annalyser (sources)
 * @author Edouard DUPIN
 * @date 04/04/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include "tools_debug.h"
#include "tools_globals.h"
#include "toolsMemory.h"
#include "RegExp.h"


// internal define to permit to have all neeed system
#define OPCODE_PTHESE_IN		(-300)	/* ( */
#define OPCODE_PTHESE_OUT		( 300)	/* ) */
#define OPCODE_BRACKET_IN		(-301)	/* [ */
#define OPCODE_BRACKET_OUT		( 301)	/* ] */
#define OPCODE_BRACE_IN			(-302)	/* { */
#define OPCODE_BRACE_OUT		( 302)	/* } */
#define OPCODE_TO				(-305)	/* - */
#define OPCODE_STAR				(-306)	/* * */
#define OPCODE_DOT				(-307)	/* . */
#define OPCODE_QUESTION			(-308)	/* ? */
#define OPCODE_PLUS				(-309)	/* + */
#define OPCODE_PIPE				(-310)	/* | */
#define OPCODE_START_OF_LINE	(-311)	/* ^  this is also NOT, but not manage */
#define OPCODE_END_OF_LINE		(-312)	/* $ */
#define OPCODE_DIGIT			( 313)	/* \d */
#define OPCODE_DIGIT_NOT		(-313)	/* \D */
#define OPCODE_LETTER			( 314)	/* \l */
#define OPCODE_LETTER_NOT		(-314)	/* \L */
#define OPCODE_SPACE			( 315)	/* \s */
#define OPCODE_SPACE_NOT		(-315)	/* \S */
#define OPCODE_WORD				( 316)	/* \w */
#define OPCODE_WORD_NOT			(-316)	/* \W */
#define OPCODE_NO_CHAR			(-317)	/* \@ */

typedef struct {
	bool		haveBackSlash;
	char		inputValue;
	int16_t		newValue;
}convertionTable_ts;

const convertionTable_ts constConvertionTable[] = {
	// haveBackSlash, inputValue, newValue
	{ false			, '('		, OPCODE_PTHESE_IN},
	{ true			, '('		, (int16_t)'('},
	{ false			, ')'		, OPCODE_PTHESE_OUT},
	{ true			, ')'		, (int16_t)')'},
	{ false			, '['		, OPCODE_BRACKET_IN},
	{ true			, '['		, (int16_t)'['},
	{ false			, ']'		, OPCODE_BRACKET_OUT},
	{ true			, ']'		, (int16_t)']'},
	{ false			, '{'		, OPCODE_BRACE_IN},
	{ true			, '{'		, (int16_t)'{'},
	{ false			, '}'		, OPCODE_BRACE_OUT},
	{ true			, '}'		, (int16_t)'}'},
	{ false			, '-'		, OPCODE_TO},
	{ true			, '-'		, (int16_t)'-'},
	{ false			, '*'		, OPCODE_STAR},
	{ true			, '*'		, (int16_t)'*'},
	{ false			, '.'		, OPCODE_DOT},
	{ true			, '.'		, (int16_t)'.'},
	{ false			, '?'		, OPCODE_QUESTION},
	{ true			, '?'		, (int16_t)'?'},
	{ false			, '+'		, OPCODE_PLUS},
	{ true			, '+'		, (int16_t)'+'},
	{ false			, '|'		, OPCODE_PIPE},
	{ true			, '|'		, (int16_t)'|'},
	{ false			, '^'		, OPCODE_START_OF_LINE},
	{ true			, '^'		, (int16_t)'^'},
	{ false			, '$'		, OPCODE_END_OF_LINE},
	{ true			, '$'		, (int16_t)'$'},
	{ true			, 'd'		, OPCODE_DIGIT},
	{ true			, 'D'		, OPCODE_DIGIT_NOT},
	{ true			, 'l'		, OPCODE_LETTER},
	{ true			, 'L'		, OPCODE_LETTER_NOT},
	{ true			, 's'		, OPCODE_SPACE},
	{ true			, 'S'		, OPCODE_SPACE_NOT},
	{ true			, 'w'		, OPCODE_WORD},
	{ true			, 'W'		, OPCODE_WORD_NOT},
	{ true			, 'a'		, (int16_t)'\a'},
	{ true			, 'b'		, (int16_t)'\b'},
	{ true			, 'e'		, 0x001B}, // Escape character <Esc>
	{ true			, 'f'		, (int16_t)'\f'},
	{ true			, 'n'		, (int16_t)'\n'},
	{ true			, 'r'		, (int16_t)'\r'},
	{ true			, 't'		, (int16_t)'\t'},
	{ true			, 'v'		, (int16_t)'\v'},
	{ true			, '\\'		, (int16_t)'\\'},
	{ true			, '&'		, (int16_t)'&'},
	{ true			, '0'		, (int16_t)'\0'},
	{ true			, '@'		, OPCODE_NO_CHAR},
};
const int32_t constConvertionTableSize = sizeof(constConvertionTable) / sizeof(convertionTable_ts) ;


/**
 * @brief Display the internal data of a node
 *
 * @param[in] data	element do display in the console
 *
 * @return ---
 *
 */
static void DisplayData(EdnVectorBin<char> &data)
{
	int32_t i;
	for (i=0; i<(int32_t)data.Size() ; i++) {
		std::cout<< (char)(data[i]&0x00FF );
	}
}

/**
 * @brief Display of a part of the Regexp element
 *
 * @param[in] data		Vector where data is constain
 * @param[in] start		Position where the display might start
 * @param[in] stop		Position where the display might stop
 *
 * @return ---
 *
 */
static void DisplayElem(EdnVectorBin<int16_t> &data, int32_t start=0, int32_t stop=0x7FFFFFFF)
{
	int32_t i;
	std::cout<< COLOR_NORMAL;
	for (i=start; i<(int32_t)data.Size() && i<stop ; i++) {
		switch(data[i])
		{
			case OPCODE_PTHESE_IN:		std::cout<<COLOR_RED		<< (char*)"(" << COLOR_NORMAL;		break;
			case OPCODE_PTHESE_OUT:		std::cout<<COLOR_RED		<< (char*)")" << COLOR_NORMAL;		break;
			case OPCODE_BRACKET_IN:		std::cout<<COLOR_YELLOW		<< (char*)"[" << COLOR_NORMAL;		break;
			case OPCODE_BRACKET_OUT:	std::cout<<COLOR_YELLOW		<< (char*)"]" << COLOR_NORMAL;		break;
			case OPCODE_TO:				std::cout<<COLOR_YELLOW		<< (char*)"-" << COLOR_NORMAL;		break;
			case OPCODE_BRACE_IN:		std::cout<<COLOR_GREEN		<< (char*)"{" << COLOR_NORMAL;		break;
			case OPCODE_BRACE_OUT:		std::cout<<COLOR_GREEN		<< (char*)"}" << COLOR_NORMAL;		break;
			case OPCODE_STAR:			std::cout<<COLOR_BLUE		<< (char*)"*" << COLOR_NORMAL;		break;
			case OPCODE_DOT:			std::cout<<COLOR_BLUE		<< (char*)"." << COLOR_NORMAL;		break;
			case OPCODE_QUESTION:		std::cout<<COLOR_BLUE		<< (char*)"?" << COLOR_NORMAL;		break;
			case OPCODE_PLUS:			std::cout<<COLOR_BLUE		<< (char*)"+" << COLOR_NORMAL;		break;
			case OPCODE_PIPE:			std::cout<<COLOR_BLUE		<< (char*)"|" << COLOR_NORMAL;		break;
			case OPCODE_NO_CHAR:		std::cout<<COLOR_MAGENTA	<< (char*)"@" << COLOR_NORMAL;		break;
			case OPCODE_START_OF_LINE:	std::cout<<COLOR_MAGENTA	<< (char*)"^" << COLOR_NORMAL;		break;
			case OPCODE_END_OF_LINE:	std::cout<<COLOR_MAGENTA	<< (char*)"$" << COLOR_NORMAL;		break;
			case OPCODE_DIGIT:			std::cout<<COLOR_MAGENTA	<< (char*)"\\d" << COLOR_NORMAL;	break;
			case OPCODE_DIGIT_NOT:		std::cout<<COLOR_MAGENTA	<< (char*)"\\D" << COLOR_NORMAL;	break;
			case OPCODE_LETTER:			std::cout<<COLOR_MAGENTA	<< (char*)"\\l" << COLOR_NORMAL;	break;
			case OPCODE_LETTER_NOT:		std::cout<<COLOR_MAGENTA	<< (char*)"\\L" << COLOR_NORMAL;	break;
			case OPCODE_SPACE:			std::cout<<COLOR_MAGENTA	<< (char*)"\\s" << COLOR_NORMAL;	break;
			case OPCODE_SPACE_NOT:		std::cout<<COLOR_MAGENTA	<< (char*)"\\S" << COLOR_NORMAL;	break;
			case OPCODE_WORD:			std::cout<<COLOR_MAGENTA	<< (char*)"\\w" << COLOR_NORMAL;	break;
			case OPCODE_WORD_NOT:		std::cout<<COLOR_MAGENTA	<< (char*)"\\W" << COLOR_NORMAL;	break;
			case '\n':					std::cout<<COLOR_MAGENTA	<< (char*)"\\n" << COLOR_NORMAL;	break;
			case '\t':					std::cout<<COLOR_MAGENTA	<< (char*)"\\t" << COLOR_NORMAL;	break;
			default:					std::cout<< (char)(data[i]&0x00FF );							break;
		}
	}
}
static char * levelSpace(int32_t level)
{
	switch(level)
	{
		case 0:		return (char*)"";
		case 1:		return (char*)"  ";
		case 2:		return (char*)"    ";
		case 3:		return (char*)"      ";
		case 4:		return (char*)"        ";
		case 5:		return (char*)"          ";
		case 6:		return (char*)"            ";
		case 7:		return (char*)"              ";
		case 8:		return (char*)"                ";
		case 9:		return (char*)"                  ";
		case 10:	return (char*)"                    ";
		case 11:	return (char*)"                      ";
		case 12:	return (char*)"                        ";
		case 13:	return (char*)"                          ";
		case 14:	return (char*)"                            ";
		case 15:	return (char*)"                              ";
		case 16:	return (char*)"                                ";
		default:	return (char*)"                                  ";
	}
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
static int32_t GetLenOfPTheseElem(EdnVectorBin<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	int32_t nbOpen = 0;
	// special case of the (...) or | ==> we search '|' or ')'
	if(		OPCODE_PTHESE_OUT	== data[pos]
		||	OPCODE_PIPE			== data[pos]) {
		return 0;
	}
	// find size ...
	while (pos < (int32_t)data.Size() ) {
		if(OPCODE_PTHESE_IN == data[pos]) {
			// find a sub section : 
			nbOpen++;
		} else if(0 < nbOpen) {
			if (OPCODE_PTHESE_OUT == data[pos])
			{
				nbOpen--;
				if (0 > nbOpen) {
					EDN_ERROR("Error in the (...) find element at "<< pos);
					return -1;
				}
			}
		} else if(		OPCODE_PTHESE_OUT	== data[pos]
					||	OPCODE_PIPE			== data[pos])
		{
			// Find the end of the (...)
			// just return the size inside
			int32_t sizeInside = pos - startPos;
			if (0 >= sizeInside) {
				EDN_ERROR("Error in the (...) no data at "<< pos-1);
				return -1;
			} else {
					return sizeInside;
			}
		}
		pos++;
	}
	return pos - startPos;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
static int32_t GetLenOfPThese(EdnVectorBin<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	int32_t nbOpen = 0;
	// special case of the (...) or | ==> we search '|' or ')'
	if( OPCODE_PTHESE_OUT == data[pos]) {
		return 0;
	} else if( OPCODE_PTHESE_IN == data[pos])
	{
		pos++;
		// find size ...
		while (pos < (int32_t)data.Size() ) {
			if(OPCODE_PTHESE_IN == data[pos]) {
				// find a sub section : 
				nbOpen++;
			} else if(0 < nbOpen) {
				if (OPCODE_PTHESE_OUT == data[pos])
				{
					nbOpen--;
					if (0 > nbOpen) {
						EDN_ERROR("Error in the (...) find element at "<< pos);
						return -1;
					}
				}
			} else if( OPCODE_PTHESE_OUT	== data[pos])
			{
				// Find the end of the (...)
				// just return the size inside
				int32_t sizeInside = pos - startPos-1;
				if (0 >= sizeInside) {
					EDN_ERROR("Error in the (...) no data at "<< pos-1);
					return -1;
				} else {
					return sizeInside;
				}
			}
			pos++;
		}
	} else {
		return -1;
	}
	return 0;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
static int32_t GetLenOfBracket(EdnVectorBin<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	// special case of the (...) or | ==> we search '|' or ')'
	if( OPCODE_BRACKET_OUT == data[pos]) {
		return 0;
	} else if( OPCODE_BRACKET_IN == data[pos]) {
		pos++;
		// find size ...
		while (pos < (int32_t)data.Size() ) {
			if(OPCODE_BRACKET_OUT == data[pos]) {
				// Find the end of the [...]
				// just return the size inside
				int32_t sizeInside = pos - startPos -1 ;
				if (0 >= sizeInside) {
					EDN_ERROR("Error in the [...] no data at "<< pos-1);
					return sizeInside;
				} else {
					return sizeInside;
				}
			} else if(		OPCODE_TO != data[pos]
						&&	(		0 > data[pos]
								||	0xFF < data[pos]) )
			{
				EDN_ERROR("Error in the [...] not permited element at "<< pos << " '" << (char)data[pos] << "'");
				return false;
			}
			pos++;
		}
	} else {
		return -1;
	}
	return 0;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
static int32_t GetLenOfBrace(EdnVectorBin<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;
	// special case of the (...) or | ==> we search '|' or ')'
	if( OPCODE_BRACE_OUT == data[pos]) {
		return 0;
	} else if( OPCODE_BRACE_IN == data[pos]) {
		pos++;
		// find size ...
		while (pos < (int32_t)data.Size() ) {
			if(OPCODE_BRACE_OUT == data[pos]) {
				// Find the end of the [...]
				// just return the size inside
				int32_t sizeInside = pos - startPos -1 ;
				if (0 >= sizeInside) {
					EDN_ERROR("Error in the {...} no data at "<< pos-1);
					return sizeInside;
				} else {
					return sizeInside;
				}
			} else if(		',' != data[pos]
						&&	(		'0' > data[pos]
								||	'9' < data[pos]) )
			{
				EDN_ERROR("Error in the {...} not permited element at "<< pos << " '" << (char)data[pos] << "'");
				return false;
			}
			pos++;
		}
	} else {
		return -1;
	}
	return 0;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
static int32_t GetLenOfNormal(EdnVectorBin<int16_t> &data, int32_t startPos)
{
	int32_t pos = startPos;

	// find size ...
	while (pos < (int32_t)data.Size() ) {
		switch(data[pos])
		{
			case OPCODE_PTHESE_IN:
			case OPCODE_PTHESE_OUT:
			case OPCODE_BRACKET_IN:
			case OPCODE_BRACKET_OUT:
			case OPCODE_BRACE_IN:
			case OPCODE_BRACE_OUT:
			case OPCODE_TO:
			case OPCODE_STAR:
			case OPCODE_DOT:
			case OPCODE_QUESTION:
			case OPCODE_PLUS:
			case OPCODE_PIPE:
			case OPCODE_START_OF_LINE:
			case OPCODE_END_OF_LINE:
			case OPCODE_DIGIT:
			case OPCODE_DIGIT_NOT:
			case OPCODE_LETTER:
			case OPCODE_LETTER_NOT:
			case OPCODE_SPACE:
			case OPCODE_SPACE_NOT:
			case OPCODE_WORD:
			case OPCODE_WORD_NOT:
				{
					// just return the size inside
					int32_t sizeInside = pos - startPos;
					if (0 >= sizeInside) {
						EDN_ERROR("Error in the normal data :  no data ...");
					}
					return sizeInside;
				}
				break;
			default :
				// nothing to do ...
				break;
		}
		pos++;
	}
	return pos - startPos ;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
static bool ParseBrace(EdnVectorBin<int16_t> &data, int32_t &min, int32_t &max)
{
	//EDN_INFO("parse {...} in "; DisplayElem(data); );
	int32_t k=0;
	
	int32_t firstElement = 0;
	int32_t SecondElement = 0;
	
	while(k<data.Size()) {
		if (',' == (char)data[k]) {
			k++;
			break;
		} if ('}' == (char)data[k]) {
			SecondElement = firstElement;
			goto allIsSet;
		} else if ('0' <= (char)data[k] && '9' >= (char)data[k]) {
			firstElement *=10;
			firstElement += (char)data[k] - '0';
		} else {
			EDN_ERROR("Can not parse this element " << (char)data[k] << " at pos " << k);
			return false;
		}
		k++;
	}
	if (k==data.Size()) {
		SecondElement = firstElement;
	}
	while(k<data.Size()) {
		if (',' == (char)data[k]) {
			EDN_ERROR("Can not find a second , in {} at pos " << k);
			return false;
		} if ('}' == (char)data[k]) {
			goto allIsSet;
		} else if ('0' <= (char)data[k] && '9' >= (char)data[k]) {
			SecondElement *=10;
			SecondElement += (char)data[k] - '0';
		} else {
			EDN_ERROR("Can not parse this element " << (char)data[k] << " at pos " << k);
			return false;
		}
		k++;
	}
	
allIsSet:
	if (SecondElement == 0 && firstElement != 0) {
		min = 0;
		max = firstElement;
	} else {
		min = firstElement;
		max = SecondElement;
	}
	if (min > max) {
		EDN_ERROR("Minimum=" << min << " can not be < maximum=" << max );
		return false;
	}
	return true;
}














/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNode"

RegExpNode::RegExpNode(void)
{
	SetMult(1,1);
}


int32_t RegExpNode::Generate(EdnVectorBin<int16_t> &data, int32_t start, int32_t stop)
{
	return 0;
}

bool RegExpNode::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	return false;
}

void RegExpNode::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@???@ {" << GetMultMin() << "," << GetMultMax() << "}  subdata="; DisplayElem(m_RegExpData););
}

void RegExpNode::SetMult(int32_t min, int32_t max)
{
	m_multipleMin = edn_max(min, 0);
	m_multipleMax = edn_max(max, 1);
}
/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodePThese"
RegExpNodePThese::~RegExpNodePThese(void)
{
	
}
int32_t RegExpNodePThese::Generate(EdnVectorBin<int16_t> &data)
{
	m_RegExpData = data;
	//EDN_DEBUG("Request Parse (...) data="; DisplayElem(m_RegExpData););
	//Find all the '|' in the string (and at the good level ...) 
	int32_t pos = 0;
	int32_t elementSize = GetLenOfPTheseElem(m_RegExpData, pos);
	// generate all the "elemTypePTheseElem" of the Node
	while (elementSize>0) {
		// geerate output deta ...
		EdnVectorBin<int16_t> tmpData;
		for (int32_t k=pos; k<pos+elementSize; k++) {
			tmpData.PushBack(m_RegExpData[k]);
		}
		RegExpNodePTheseElem * myElem = new RegExpNodePTheseElem();
		(void)myElem->Generate(tmpData);
		// add to the subnode list : 
		m_subNode.PushBack(myElem);
		pos += elementSize+1;
		//EDN_DEBUG("plop="; DisplayElem(data, pos, pos+1););
		elementSize = GetLenOfPTheseElem(m_RegExpData, pos);
		//EDN_DEBUG("find " << elementSize << " elements");
	}
	if (0 == pos && 0 == elementSize) {
		EDN_ERROR("No data in the (...) element at " << pos);
		return false;
	}
	return data.Size();
}
bool RegExpNodePThese::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : (...){" << m_multipleMin << "," << m_multipleMax << "}");
	if (0 == m_subNode.Size()) {
		return false;
	}
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind == true ; j++) {
		tmpFind = false;
		for (int32_t i=0; i<m_subNode.Size(); i++) {
			int32_t tmpFindLen;
			if (true == m_subNode[i]->Parse(data, currentPos+findLen, lenMax, tmpFindLen)) {
				findLen += tmpFindLen;
				tmpFind = true;
			}
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodePThese::Display(int32_t level)
{
	if (-1 == level) {
		EDN_INFO("regExp :"; DisplayElem(m_RegExpData););
	} else {
		EDN_INFO("Find NODE : " << levelSpace(level) << "@(...)@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
		for(int32_t i=0; i<m_subNode.Size(); i++) {
			m_subNode[i]->Display(level+1);
		}
	}
}




/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodePTheseElem"
RegExpNodePTheseElem::~RegExpNodePTheseElem(void)
{
	
}
int32_t RegExpNodePTheseElem::Generate(EdnVectorBin<int16_t> &data)
{
	m_RegExpData = data;
	//EDN_DEBUG("Request Parse (elem) data="; DisplayElem(m_RegExpData););

	int32_t pos = 0;
	int32_t elementSize = 0;
	EdnVectorBin<int16_t> tmpData;
	while (pos < m_RegExpData.Size()) {
		tmpData.Clear();
		switch (m_RegExpData[pos])
		{
			case OPCODE_PTHESE_IN:
				{
					elementSize=GetLenOfPThese(m_RegExpData, pos);
					for (int32_t k=pos+1; k<pos+elementSize+1; k++) {
						tmpData.PushBack(m_RegExpData[k]);
					}
					RegExpNodePThese * myElem = new RegExpNodePThese();
					(void)myElem->Generate(tmpData);
					// add to the subnode list : 
					m_subNode.PushBack(myElem);
					// move current position ...
					pos += elementSize+1;
				}
				break;
			case OPCODE_PTHESE_OUT:
				EDN_ERROR("Impossible case :  ')' " << pos);
				return false;
		
			case OPCODE_BRACKET_IN:
				{
					elementSize=GetLenOfBracket(m_RegExpData, pos);
					for (int32_t k=pos+1; k<pos+elementSize+1; k++) {
						tmpData.PushBack(m_RegExpData[k]);
					}
					RegExpNodeBracket * myElem = new RegExpNodeBracket();
					(void)myElem->Generate(tmpData);
					// add to the subnode list : 
					m_subNode.PushBack(myElem);
					// move current position ...
					pos += elementSize+1;
				}
				break;
			case OPCODE_BRACKET_OUT:
				EDN_ERROR("Impossible case :  ']' " << pos);
				return false;
		
			case OPCODE_BRACE_IN:
				{
					elementSize=GetLenOfBrace(m_RegExpData, pos);
					for (int32_t k=pos+1; k<pos+elementSize+1; k++) {
						tmpData.PushBack(m_RegExpData[k]);
					}
					int32_t min = 0;
					int32_t max = 0;
					if (false == ParseBrace(tmpData, min, max)) {
						return false;
					}
					SetMultiplicityOnLastNode(min, max);
					pos += elementSize+1;
				}
				break;
			case OPCODE_BRACE_OUT:
				EDN_ERROR("Impossible case :  '}' " << pos);
				return false;
		
			case OPCODE_TO:
				EDN_ERROR("Impossible case :  '-' " << pos);
				return false;
		
			case OPCODE_STAR:
				SetMultiplicityOnLastNode(0, 0x7FFFFFFF);
				break;
		
			case OPCODE_QUESTION:
				SetMultiplicityOnLastNode(0, 1);
				break;
		
			case OPCODE_PLUS:
				SetMultiplicityOnLastNode(1, 0x7FFFFFFF);
				break;
		
			case OPCODE_PIPE:
				EDN_ERROR("Impossible case :  '|' " << pos);
				return false;
		
			case OPCODE_DOT:
				m_subNode.PushBack(new RegExpNodeDot());
				break;
		
			case OPCODE_START_OF_LINE:
				m_subNode.PushBack(new RegExpNodeSOL());
				break;
		
			case OPCODE_END_OF_LINE:
				m_subNode.PushBack(new RegExpNodeEOL());
				break;
		
			case OPCODE_DIGIT:
				m_subNode.PushBack(new RegExpNodeDigit());
				break;
		
			case OPCODE_DIGIT_NOT:
				m_subNode.PushBack(new RegExpNodeDigitNot());
				break;
		
			case OPCODE_LETTER:
				m_subNode.PushBack(new RegExpNodeLetter());
				break;
		
			case OPCODE_LETTER_NOT:
				m_subNode.PushBack(new RegExpNodeLetterNot());
				break;
		
			case OPCODE_SPACE:
				m_subNode.PushBack(new RegExpNodeWhiteSpace());
				break;
		
			case OPCODE_SPACE_NOT:
				m_subNode.PushBack(new RegExpNodeWhiteSpaceNot());
				break;
		
			case OPCODE_WORD:
				m_subNode.PushBack(new RegExpNodeWordChar());
				break;
		
			case OPCODE_WORD_NOT:
				m_subNode.PushBack(new RegExpNodeWordCharNot());
				break;

			default:
				{
					elementSize=GetLenOfNormal(m_RegExpData, pos);
					for (int32_t k=pos; k<pos+elementSize; k++) {
						tmpData.PushBack(m_RegExpData[k]);
					}
					RegExpNodeValue * myElem = new RegExpNodeValue();
					(void)myElem->Generate(tmpData);
					// add to the subnode list : 
					m_subNode.PushBack(myElem);
					// move current position ...
					pos += elementSize-1;
				}
				break;
		}
		pos++;
	}
	return data.Size();
}
/**
 * @brief Set the number of repeate time on a the last node in the list ...
 *
 * @param[in] min		Minimum of the multiplicity
 * @param[in] max		Maximum of the multiplicity
 *
 * @return true if we find the node, false otherwise
 *
 */
bool RegExpNodePTheseElem::SetMultiplicityOnLastNode(int32_t min, int32_t max)
{
	if (0==m_subNode.Size()) {
		EDN_ERROR("Set multiplicity on an inexistant element ....");
		return false;
	}
	RegExpNode * myNode = m_subNode[m_subNode.Size()-1];
	if (NULL==myNode) {
		EDN_ERROR("INTERNAL error ==> node not generated");
		return false;
	}
	myNode->SetMult(min, max);
	return true;
}


bool RegExpNodePTheseElem::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : (Elem){" << m_multipleMin << "," << m_multipleMax << "}");
	// NOTE 1 : Must done only one time in EVERY case ...
	// NOTE 2 : All element inside must be OK
	if (0 == m_subNode.Size()) {
		return false;
	}
	int32_t tmpCurrentPos = currentPos;
	for (int32_t i=0; i<m_subNode.Size(); i++) {
		int32_t tmpFindLen;
		if (false == m_subNode[i]->Parse(data, tmpCurrentPos, lenMax, tmpFindLen)) {
			findLen = 0;
			return false;
		} else {
			tmpCurrentPos += tmpFindLen;
		}
	}
	findLen = tmpCurrentPos - currentPos;
	return true;
}

void RegExpNodePTheseElem::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@(Elem)@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
	for(int32_t i=0; i<m_subNode.Size(); i++) {
		m_subNode[i]->Display(level+1);
	}
}



/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeValue"
int32_t RegExpNodeValue::Generate(EdnVectorBin<int16_t> &data)
{
	m_RegExpData = data;
	//EDN_DEBUG("Request Parse \"Value\" data="; DisplayElem(m_RegExpData););
	m_data.Clear();
	for (int32_t i=0; i<m_RegExpData.Size(); i++) {
		m_data.PushBack((char)m_RegExpData[i]);
	}
	return data.Size();
}
bool RegExpNodeValue::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : Value{" << m_multipleMin << "," << m_multipleMax << "}");
	if (0==m_data.Size()) {
		EDN_ERROR("No data inside type elemTypeValue");
		return false;
	}
	//EDN_DEBUG("check element value : '" << m_data[0] << "'");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind == true; j++) {
		int32_t ofset = 0;
		int32_t k;
		for (k=0; findLen+k<lenMax && k < m_data.Size(); k++) {
			if (m_data[k] != data[currentPos+findLen+k]) {
				tmpFind=false;
				break;
			}
			ofset++;
		}
		if (k != (int32_t)m_data.Size()) {
			// parsing not ended ...
			tmpFind=false;
		}
		// Update local ofset of data
		if (true == tmpFind) {
			findLen += ofset;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeValue::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@Value@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData); std::cout<< " data: "; DisplayData(m_data); );
}




/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeBracket"
int32_t RegExpNodeBracket::Generate(EdnVectorBin<int16_t> &data)
{
	m_RegExpData = data;
	//EDN_DEBUG("Request Parse [...] data="; DisplayElem(m_RegExpData););
	m_data.Clear();
	
	char lastElement = 'a';
	bool multipleElement = false;
	//
	for (int32_t k=0; k<m_RegExpData.Size(); k++) {
		if (m_RegExpData[k] == OPCODE_TO && multipleElement == true) {
			EDN_ERROR("Can not have 2 consecutive - in [...]");
			return 0;
		} else if (multipleElement == true) {
			char j='\0';
			for (j=lastElement+1; j <= (char)m_RegExpData[k]; j++) {
				m_data.PushBack(j);
			}
			multipleElement = false;
		} else if(m_RegExpData[k] == OPCODE_TO) {
			multipleElement = true;
		} else {
			lastElement = (char)m_RegExpData[k];
			m_data.PushBack(lastElement);
		}
	}
	// check size ...
	if (m_data.Size() == 0) {
		EDN_ERROR("No data inside [...] ");
		return 0;
	}
	return data.Size();
}
bool RegExpNodeBracket::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : [...]{" << m_multipleMin << "," << m_multipleMax << "}");
	if (0==m_data.Size()) {
		EDN_ERROR("No data inside type elemTypeValue");
		return false;
	}
	//EDN_DEBUG("one of element value List : "; DisplayData(element->m_data););
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		int32_t i;
		tmpFind=false;
		for (i=0; i<m_data.Size(); i++) {
			if (m_data[i] == data[currentPos+j]) {
				findLen += 1;
				tmpFind=true;
				break;
			}
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeBracket::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@[...]@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData); std::cout<< " data: "; DisplayData(m_data); );
}




/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeDigit"
bool RegExpNodeDigit::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : Digit{" << m_multipleMin << "," << m_multipleMax << "} : "<< data[currentPos] << " lenMax=" << lenMax);
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		//EDN_DEBUG("compare : " << tmpVal);
		if(		'0' <= tmpVal
			&&	'9' >= tmpVal)
		{
			//EDN_DEBUG("find ++");
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeDigit::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@Digit@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}




/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeDigitNot"
bool RegExpNodeDigitNot::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : DigitNot{" << m_multipleMin << "," << m_multipleMax << "}");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		'0' > tmpVal
			||	'9' < tmpVal)
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeDigitNot::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@DigitNot@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}



/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeLetter"
bool RegExpNodeLetter::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : Letter{" << m_multipleMin << "," << m_multipleMax << "}");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		(		'a' <= tmpVal
					&&	'z' >= tmpVal	)
			||	(		'A' <= tmpVal
					&&	'Z' >= tmpVal	))
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeLetter::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@Letter@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}



/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeLetterNot"
bool RegExpNodeLetterNot::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : LetterNot{" << m_multipleMin << "," << m_multipleMax << "}");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		(		'a' > tmpVal
					&&	'Z' < tmpVal	)
			||	'A' > tmpVal
			||	'z' < tmpVal	)
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeLetterNot::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@LetterNot@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}



/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeWhiteSpace"
bool RegExpNodeWhiteSpace::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : Space{" << m_multipleMin << "," << m_multipleMax << "}");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		' '  == tmpVal
			||	'\t' == tmpVal
			||	'\n' == tmpVal
			||	'\r' == tmpVal
			||	'\f' == tmpVal
			||	'\v' == tmpVal	)
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeWhiteSpace::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@Space@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}
/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeWhiteSpaceNot"
bool RegExpNodeWhiteSpaceNot::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : SpaceNot{" << m_multipleMin << "," << m_multipleMax << "}");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		' '  != tmpVal
			&&	'\t' != tmpVal
			&&	'\n' != tmpVal
			&&	'\r' != tmpVal
			&&	'\f' != tmpVal
			&&	'\v' != tmpVal	)
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeWhiteSpaceNot::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@SpaceNot@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}
/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeWordChar"
bool RegExpNodeWordChar::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : Word{" << m_multipleMin << "," << m_multipleMax << "}");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		(		'a' <= tmpVal
					&&	'z' >= tmpVal	)
			||	(		'A' <= tmpVal
					&&	'Z' >= tmpVal	)
			||	(		'0' <= tmpVal
					&&	'9' >= tmpVal	))
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeWordChar::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@Word@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}
/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeWordCharNot"
bool RegExpNodeWordCharNot::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : WordNot{" << m_multipleMin << "," << m_multipleMax << "}");
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		(		'A' > tmpVal
					&&	'9' < tmpVal	)
			||	(		'a' > tmpVal
					&&	'Z' < tmpVal	)
			||	'0' > tmpVal
			||	'z' < tmpVal	)
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeWordCharNot::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@WordNot@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}
/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeDot"
bool RegExpNodeDot::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	//EDN_INFO("Parse node : '.'{" << m_multipleMin << "," << m_multipleMax << "}");
	// equivalent a : [^\x00-\x08\x0A-\x1F\x7F]
	bool tmpFind = true;
	int32_t j;
	for (j=0; j<m_multipleMax && tmpFind ==true && j < lenMax; j++) {
		char tmpVal = data[currentPos+j];
		if(		(		0x08 < tmpVal
					&&	0x0A > tmpVal	)
			||	(		0x1F < tmpVal
					&&	0x7F > tmpVal	)
			||	(		0x7F < tmpVal
					&&	0xFF > tmpVal	))
		{
			findLen += 1;
		} else {
			tmpFind=false;
		}
	}
	if(		j>=m_multipleMin
		&&	j<=m_multipleMax
		&&	findLen>0	)
	{
		//EDN_DEBUG("find " << findLen);
		return true;
	} else if( 0 == m_multipleMin ) {
		//EDN_DEBUG("find size=0");
		return true;
	}
	return false;
}
void RegExpNodeDot::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@.@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}

/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeSOL"
bool RegExpNodeSOL::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	EDN_INFO("Parse node : SOL{" << m_multipleMin << "," << m_multipleMax << "}");
	return false;
}
void RegExpNodeSOL::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@SOL@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}

/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"RegExpNodeEOL"

bool RegExpNodeEOL::Parse(EdnVectorBuf &data, int32_t currentPos, int32_t lenMax, int32_t &findLen)
{
	findLen = 0;
	EDN_INFO("Parse node : EOL{" << m_multipleMin << "," << m_multipleMax << "}");
	return false;
}
void RegExpNodeEOL::Display(int32_t level)
{
	EDN_INFO("Find NODE : " << levelSpace(level) << "@EOL@ {" << m_multipleMin << "," << m_multipleMax << "}  subdata="; DisplayElem(m_RegExpData););
}






/* ******************************************************************************************************************************************************************************************** */
#undef __class__
#define __class__	"EdnRegExp"

EdnRegExp::EdnRegExp(void)
{
	m_isOk = false;
	m_areaFind.start=0;
	m_areaFind.stop=0;
	m_notBeginWithChar = false;
	m_notEndWithChar = false;
}

EdnRegExp::~EdnRegExp(void)
{
	// TODO : remove all under nodes...
	m_isOk = false;
}

void EdnRegExp::SetRegExp(const char *exp)
{
	EDN_CHECK_INOUT(exp);
	Edn::String expressionRequested = exp;
	SetRegExp(expressionRequested);
}

void EdnRegExp::SetRegExp(Edn::String &expressionRequested)
{
	m_expressionRequested = expressionRequested;		// TODO : Must be deprecated ...
	EdnVectorBin<int16_t> tmpExp;
	
	//EDN_DEBUG("Parse RegExp : " << expressionRequested.c_str() );
	m_isOk = false;
	m_areaFind.start=0;
	m_areaFind.stop=0;
	m_notBeginWithChar = false;
	m_notEndWithChar = false;
	
	char * exp = expressionRequested.c_str();
	int32_t regExpLen = strlen(exp);
	// change in the regular Opcode ==> replace \x with the corect element ... x if needed
	int32_t iii;
	int32_t countBraceIn = 0;
	int32_t countBraceOut = 0;
	int32_t countPTheseIn = 0;
	int32_t countPTheseOut = 0;
	int32_t countBracketIn = 0;
	int32_t countBracketOut = 0;
	for (iii=0; iii<regExpLen; iii++) {
		if ('\\' == exp[iii]) {
			if(iii+1>=regExpLen) {
				EDN_ERROR("Dangerous parse of the element pos " << iii << " \\ with nothing after");
				// TODO : Generate Exeption ...
				return;
			}
			int32_t j;
			// Find the element in the list...
			for (j=0; j<constConvertionTableSize; j++) {
				if(		true == constConvertionTable[j].haveBackSlash 
					&&	exp[iii+1] == constConvertionTable[j].inputValue)
				{
					tmpExp.PushBack(constConvertionTable[j].newValue);
					break;
				}
			}
			// check error : 
			if (j==constConvertionTableSize) {
				EDN_ERROR("Dangerous parse of the \\" << exp[iii+1] << " at element " << iii);
				// TODO : Generate Exeption ...
				return;
			}
			// less one char in the regular expression ...
			iii++;
		} else {
			if ('(' == exp[iii]) {
				countPTheseIn++;
			} else if (')' == exp[iii]) {
				countPTheseOut++;
			} else if ('[' == exp[iii]) {
				countBracketIn++;
			} else if (']' == exp[iii]) {
				countBracketOut++;
			} else if ('{' == exp[iii]) {
				countBraceIn++;
			} else if ('}' == exp[iii]) {
				countBraceOut++;
			}
			int32_t j;
			// find the element in the list...
			for (j=0; j<constConvertionTableSize; j++) {
				if(		false == constConvertionTable[j].haveBackSlash 
					&&	exp[iii] == constConvertionTable[j].inputValue)
				{
					tmpExp.PushBack(constConvertionTable[j].newValue);
					break;
				}
			}
			// not find : normal element
			if (j==constConvertionTableSize) {
				tmpExp.PushBack( ((int16_t)exp[iii]) & 0x00FF);
			}
		}
	}
	
	// count the number of '(' and ')'
	if (countPTheseIn != countPTheseOut ) {
		EDN_ERROR("Error in the number of '('=" << countPTheseIn << " and ')'=" << countPTheseOut << " elements");
		return;
	}
	// count the number of '{' and '}'
	if (countBraceIn != countBraceOut ) {
		EDN_ERROR("Error in the number of '{'=" << countBraceIn << " and '}'=" << countBraceOut << " elements");
		return;
	}
	// count the number of '[' and ']'
	if (countBracketIn != countBracketOut ) {
		EDN_ERROR("Error in the number of '['=" << countBracketIn << " and ']'=" << countBracketOut << " elements");
		return;
	}
	// need to check if all () [] and {} is well set ...
	if (false == CheckGoodPosition(tmpExp) ) {
		return;
	}
	
	//EDN_DEBUG("Main element :"; DisplayElem(tmpExp, 0, tmpExp.Size()); );
	if(		tmpExp.Size()>0
		&&	OPCODE_NO_CHAR == tmpExp[0])
	{
		//EDN_DEBUG("=> must not begin with char");
		m_notBeginWithChar = true;
		// remove element
		tmpExp.Erase(0);
	}
	if(		tmpExp.Size()>0
		&&	OPCODE_NO_CHAR == tmpExp[tmpExp.Size()-1])
	{
		//EDN_DEBUG("=> must not end with char");
		m_notEndWithChar = true;
		// remove element
		tmpExp.Erase(tmpExp.Size()-1);
	}
	
	if (tmpExp.Size() != m_exprRootNode.Generate(tmpExp) ) {
		return;
	}
	// TODO : optimize node here ...
	
	//Display();

	// all OK ... play again 
	m_isOk = true;
}



/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
bool EdnRegExp::CheckGoodPosition(EdnVectorBin<int16_t> tmpExp, int32_t &pos)
{
	int16_t curentCode = tmpExp[pos];
	int16_t endCode = OPCODE_PTHESE_OUT;
	char *input = (char*)"(...)";
	if (OPCODE_BRACKET_IN == curentCode) {
		endCode = OPCODE_BRACKET_OUT;
		input = (char*)"[...]";
	} else if (OPCODE_BRACE_IN == curentCode){
		endCode = OPCODE_BRACE_OUT;
		input = (char*)"{x,x}";
	}
	pos++;
	if (pos >= (int32_t)tmpExp.Size()) {
		EDN_ERROR("ended with: ( or { or [ ... not permited");
		return false;
	}
	//EDN_DEBUG(" ==> Find ELEMENT : ([{");
	// case dependent : 
	if(		OPCODE_BRACKET_IN == curentCode
		||	OPCODE_BRACE_IN == curentCode) {
		while(pos< (int32_t)tmpExp.Size()) {
			//EDN_DEBUG("check : " << tmpExp[pos]);
			// if we find the end : 
			if (endCode == tmpExp[pos]) {
				return true;
			} else {
				// otherwise, we check the error in the element ...
				char *find = NULL;
				switch (tmpExp[pos])
				{
					case OPCODE_PTHESE_IN:		find = (char*)"(";			break;
					case OPCODE_BRACKET_IN:		find = (char*)"[";			break;
					case OPCODE_BRACE_IN:		find = (char*)"{";			break;
					case OPCODE_PTHESE_OUT:		find = (char*)")";			break;
					case OPCODE_BRACKET_OUT:	find = (char*)"]";			break;
					case OPCODE_BRACE_OUT:		find = (char*)"}";			break;
					case OPCODE_STAR:			find = (char*)"*";			break;
					case OPCODE_DOT:			find = (char*)".";			break;
					case OPCODE_QUESTION:		find = (char*)"?";			break;
					case OPCODE_PLUS:			find = (char*)"+";			break;
					case OPCODE_PIPE:			find = (char*)"|";			break;
					case OPCODE_START_OF_LINE:	find = (char*)"^";			break;
					case OPCODE_END_OF_LINE:	find = (char*)"$";			break;
					case OPCODE_DIGIT:			find = (char*)"\\d";		break;
					case OPCODE_DIGIT_NOT:		find = (char*)"\\D";		break;
					case OPCODE_LETTER:			find = (char*)"\\l";		break;
					case OPCODE_LETTER_NOT:		find = (char*)"\\L";		break;
					case OPCODE_SPACE:			find = (char*)"\\s";		break;
					case OPCODE_SPACE_NOT:		find = (char*)"\\S";		break;
					case OPCODE_WORD:			find = (char*)"\\w";		break;
					case OPCODE_WORD_NOT:		find = (char*)"\\W";		break;
					case OPCODE_NO_CHAR:		find = (char*)"\\@";		break;
					default:					break;
				}
				if (NULL != find) {
					EDN_ERROR("can not have : '" << find << "' inside " << input << " element");
					return false;
				}
			}
			pos++;
		}
	} else {
		while(pos< (int32_t)tmpExp.Size()) {
			if (endCode == tmpExp[pos]) {
				// find the last element
				return true;
			} else if (	OPCODE_BRACE_OUT	== tmpExp[pos]) {
				EDN_ERROR("find } inside a (...) without start {");
				return false;
			} else if (	OPCODE_BRACKET_OUT	== tmpExp[pos]) {
				EDN_ERROR("find ] inside a (...) without start [");
				return false;
			} else {
				if(		OPCODE_PTHESE_IN	== tmpExp[pos]
					||	OPCODE_BRACKET_IN	== tmpExp[pos]
					||	OPCODE_BRACE_IN		== tmpExp[pos])
				{
					if (false==CheckGoodPosition(tmpExp, pos) ) {
						return false;
					}
				}
			}
			pos++;
		}
	}
	
	// we did not find the cloder . ...
	if (endCode == OPCODE_BRACKET_OUT) {
		EDN_ERROR("Missing ']' at the end");
	}
	if (endCode == OPCODE_BRACE_OUT) {
		EDN_ERROR("Missing '}' at the end");
	}
	if (endCode == OPCODE_PTHESE_OUT) {
		EDN_ERROR("Missing ')' at the end");
	}
	return false;
}



/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
bool EdnRegExp::CheckGoodPosition(EdnVectorBin<int16_t> tmpExp)
{
	int32_t pos = 0;
	while (pos < (int32_t)tmpExp.Size()) {
		//EDN_DEBUG("check : " << tmpExp[pos]);
		if(		OPCODE_PTHESE_IN	== tmpExp[pos]
			||	OPCODE_BRACKET_IN	== tmpExp[pos]
			||	OPCODE_BRACE_IN		== tmpExp[pos])
		{
			// attention the i position change inside the finction...
			if (false==CheckGoodPosition(tmpExp, pos) ) {
				EDN_ERROR("Error at position : " << pos+1 );
				return false;
			} else {
				//EDN_DEBUG(" <== Find ELEMENT : ]})");
			}
		} else if(OPCODE_PTHESE_OUT	== tmpExp[pos]) {
			EDN_ERROR("can find ')' with no start : ')'");
			return false;
		} else if(OPCODE_BRACKET_OUT	== tmpExp[pos]) {
			EDN_ERROR("can find ']' with no start : '['");
			return false;
		} else if(OPCODE_BRACE_OUT	== tmpExp[pos]) {
			EDN_ERROR("can find '}' with no start : '{'");
			return false;
		}
		pos++;
	}
	return true;
}





// process the regular expression
bool EdnRegExp::Process(	EdnVectorBuf			&SearchIn,
							int32_t					startPos,
							int32_t					endPos,
							char					escapeChar)
{
	if (false == m_isOk) {
		return false;
	}
	int32_t buflen = SearchIn.Size();
	if (endPos > buflen) {
		endPos = buflen;
	}
	if (startPos > endPos) {
		return false;
	}
	int32_t i = 0;
	for (i=startPos; i<endPos; i++) {
		int32_t findLen=0;
		int32_t maxlen = endPos-i;
		if (true == m_notBeginWithChar) {
			if (i>0) {
				char tmpVal = SearchIn[i-1];
				if(		(		'a' <= tmpVal
							&&	'z' >= tmpVal	)
					||	(		'A' <= tmpVal
							&&	'Z' >= tmpVal	)
					||	(		'0' <= tmpVal
							&&	'9' >= tmpVal	)
					||	(		'_' == tmpVal	)	)
				{
					// go on the next char ...
					continue;
				}
			}
		}
		if (true == m_exprRootNode.Parse(SearchIn, i, maxlen, findLen)) {
			if(		0!=escapeChar
				&&	i>0)
			{
				if (escapeChar == (char)SearchIn[i-1]) {
					//==> detected escape char ==> try find again ...
					continue;
				}
			}
			// Check end :
			if (true == m_notEndWithChar) {
				if (i+findLen < SearchIn.Size() ) {
					char tmpVal = SearchIn[i+findLen];
					if(		(		'a' <= tmpVal
								&&	'z' >= tmpVal	)
						||	(		'A' <= tmpVal
								&&	'Z' >= tmpVal	)
						||	(		'0' <= tmpVal
								&&	'9' >= tmpVal	)
						||	(		'_' == tmpVal	)	)
					{
						// go on the next char ...
						continue;
					}
				}
			}
			m_areaFind.start = i;
			m_areaFind.stop  = i + findLen;
			/*
			if (i == 812) {
				std::cout << std::endl;
				for(int32_t k=startPos; k<endPos; k++){
					std::cout << SearchIn[k];
				}
				std::cout << std::endl;
			}
			EDN_DEBUG("Find RegExp at position : " << i << " startpos=" << startPos << " endPos=" << endPos << " with size :" << findLen << " type : "; );
			m_exprRootNode.Display(-1);
			EDN_DEBUG("---------------------------------------------------");
			*/
			return true;
		}
	}
	return false;
}



void EdnRegExp::Display(void)
{
	m_exprRootNode.Display(0);
}

