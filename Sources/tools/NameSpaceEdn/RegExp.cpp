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
void DisplayData(EdnVectorBin<char> &data)
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
void DisplayElem(EdnVectorBin<int16_t> &data, int32_t start, int32_t stop)
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
char * levelSpace(int32_t level)
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
int32_t GetLenOfPTheseElem(EdnVectorBin<int16_t> &data, int32_t startPos)
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
int32_t GetLenOfPThese(EdnVectorBin<int16_t> &data, int32_t startPos)
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
int32_t GetLenOfBracket(EdnVectorBin<int16_t> &data, int32_t startPos)
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
int32_t GetLenOfBrace(EdnVectorBin<int16_t> &data, int32_t startPos)
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
int32_t GetLenOfNormal(EdnVectorBin<int16_t> &data, int32_t startPos)
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
bool ParseBrace(EdnVectorBin<int16_t> &data, int32_t &min, int32_t &max)
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


