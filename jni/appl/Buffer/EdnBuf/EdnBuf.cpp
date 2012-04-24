/**
 *******************************************************************************
 * @file EdnBuf.cpp
 * @brief Editeur De N'ours : Buffer for internal Data (Sources)
 * @author Edouard DUPIN
 * @date 23/03/2011
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

#include <etk/Types.h>
#include <etk/unicode.h>
#include <appl/Debug.h>
#include <appl/global.h>
#include <EdnBuf.h>


#undef __class__
#define __class__	"EdnBuf"

/**
 *  @brief convertion table for non printable control caracters
 */
static const char *ControlCodeTable[32] = {
	 "NUL", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs",  "ht", "nl",  "vt",  "np", "cr", "so", "si",
	 "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};



/**
 * @brief Constructor of the Edn buffer Text :
 * 
 * Create an empty text buffer of a pre-determined size
 * 
 * @param[in] requestedSize		use this to avoid unnecessary re-allocation if you know exactly how much the buffer will need to hold
 * 
 */
EdnBuf::EdnBuf(void)
{
	m_tabDist = 4;
	m_useTabs = true;

	// Current selection
	m_selectionList[SELECTION_PRIMARY].selected = false;
	m_selectionList[SELECTION_PRIMARY].zeroWidth = false;
	m_selectionList[SELECTION_PRIMARY].rectangular = false;
	m_selectionList[SELECTION_PRIMARY].start = m_selectionList[SELECTION_PRIMARY].end = 0;
	m_selectionList[SELECTION_SECONDARY].selected = false;
	m_selectionList[SELECTION_SECONDARY].zeroWidth = false;
	m_selectionList[SELECTION_SECONDARY].rectangular = false;
	m_selectionList[SELECTION_SECONDARY].start = m_selectionList[SELECTION_SECONDARY].end = 0;
	m_selectionList[SELECTION_HIGHTLIGHT].selected = false;
	m_selectionList[SELECTION_HIGHTLIGHT].zeroWidth = false;
	m_selectionList[SELECTION_HIGHTLIGHT].rectangular = false;
	m_selectionList[SELECTION_HIGHTLIGHT].start = m_selectionList[SELECTION_HIGHTLIGHT].end = 0;

	// charset : 
	m_isUtf8 = false;
	m_charsetType = unicode::EDN_CHARSET_ISO_8859_15;
	
	m_isUndoProcessing = false;
	m_isRedoProcessing = false;
	
	// basicly no HL system ...
	m_Highlight = NULL;
	m_nbLine = 1;
}

/**
 * @brief Destructor of the Edn buffer Text :
 */
EdnBuf::~EdnBuf(void)
{
	// TODO : Remove history and Future
}

/**
 * @brief Save in the current file open
 * 
 * @param[in,out] myFile	pointer on the file where data might be writed
 *
 * @return true if OK / false if an error occured
 *
 */
bool EdnBuf::DumpIn(FILE *myFile)
{
	// write Data
	return m_data.DumpIn(myFile);
}


/**
 * @brief Load in the current file open
 * 
 * @param[in,out] myFile	pointer on the file where data might be read
 *
 * @return true if OK / false if an error occured
 *
 */
bool EdnBuf::DumpFrom(FILE *myFile)
{
	if (true == m_data.DumpFrom(myFile) ) {
		// set no selection
		UpdateSelections(0, 0, m_data.Size() );
		// generate HighLight
		CleanHighLight();
		GenerateHighLightAt(0, m_data.Size());
		CountNumberOfLines();
		return true;
	}
	return false;
}


void EdnBuf::GetAll(etk::VectorType<int8_t> &text)
{
	// Clean output vector
	text.Clear();
	// Set data on the vector
	m_data.Get(0, m_data.Size(), text);
}


void EdnBuf::SetAll(etk::VectorType<int8_t> &text)
{
	etk::VectorType<int8_t> deletedText;
	
	// extract all data of the buffer :
	GetAll(deletedText);
	
	// Remove all data in the buffer:
	m_data.Clear();
	
	// inset text data : 
	m_data.Insert(0, text);
	
	// Zero all of the existing selections
	UpdateSelections(0, deletedText.Size(), 0);
	
	// Call the modification Event Manager
	eventModification(0, m_data.Size(), deletedText); 
}

void EdnBuf::GetRange(int32_t start, int32_t end, etk::VectorType<int8_t> &output)
{
	// Remove all data ...
	output.Clear();
	// import data : 
	m_data.Get(start, end-start, output);
	//APPL_DEBUG("request start=" << start << " end="<< end << " size="<< end-start << " result size=" << output.Size() );
}

void EdnBuf::GetRange(int32_t start, int32_t end, etk::UString &output)
{
	// Remove all data ...
	output = "";
	// import data : 
	etk::VectorType<int8_t> localOutput;
	m_data.Get(start, end-start, localOutput);
	// transcript in UNICODE ...
	if (true == m_isUtf8) {
		localOutput.PushBack('\0');
		output = (char*)&localOutput[0];
	} else {
		etk::VectorType<uniChar_t> tmpUnicodeData;
		// transform in unicode :
		convertIsoToUnicode(m_charsetType, localOutput, tmpUnicodeData);
		output = tmpUnicodeData;
	}
	//APPL_DEBUG("request start=" << start << " end="<< end << " size="<< end-start << " result size=" << output.Size() );
}


/**
 * @brief Get an element at the selected position
 * 
 * @param[in] pos Charecters Position, [0..n]
 * 
 * @return The character at buffer position "pos"
 * 
 */
int8_t EdnBuf::operator[] (int32_t pos)
{
	int8_t res = m_data.Get(pos);
	return res;
}


/**
 * @brief Insert Data in the Buffer
 * 
 * @param[in] pos Position in the Buffer
 * @param[in] insertText Text to insert
 * 
 * @return ---
 * 
 */
int32_t EdnBuf::Insert(int32_t pos, etk::VectorType<int8_t> &insertText)
{
	// if pos is not contiguous to existing text, make it
	pos = etk_avg(0, pos, m_data.Size() );
	// insert Data
	int32_t sizeInsert=LocalInsert(pos, insertText);

	// Call the redisplay ...
	etk::VectorType<int8_t> deletedText;
	eventModification(pos, insertText.Size(), deletedText);
	return sizeInsert;
}
int32_t EdnBuf::Insert(int32_t pos, etk::UString &insertText)
{
	// if pos is not contiguous to existing text, make it
	pos = etk_avg(0, pos, m_data.Size() );
	// insert Data
	int32_t sizeInsert=LocalInsert(pos, insertText);

	// Call the redisplay ...
	etk::VectorType<int8_t> deletedText;
	eventModification(pos, insertText.Size(), deletedText);
	return sizeInsert;
}

/**
 * @brief Replace data in the buffer
 * 
 * @param[in] start Position started in the buffer
 * @param[in] end Position ended in the buffer
 * @param[in] insertText Test to set in the range [start..end]
 * 
 * @return nb Octet inserted
 * 
 */
int32_t EdnBuf::Replace(int32_t start, int32_t end, etk::VectorType<int8_t> &insertText)
{
	etk::VectorType<int8_t> deletedText;
	GetRange(start, end, deletedText);
	m_data.Replace(start, end-start, insertText);
	// update internal elements
	eventModification(start, insertText.Size(), deletedText);
	return insertText.Size();
}
int32_t EdnBuf::Replace(int32_t start, int32_t end, etk::UString &insertText)
{
	etk::VectorType<int8_t> deletedText;
	GetRange(start, end, deletedText);
	etk::VectorType<int8_t> tmpInsertText;
	if (true == m_isUtf8) {
		char * tmpPointer = insertText.Utf8Data();
		while (*tmpPointer != '\0') {
			tmpInsertText.PushBack(*tmpPointer++);
		}
	} else {
		etk::VectorType<unsigned int> tmppp = insertText.GetVector();
		convertUnicodeToIso(m_charsetType, tmppp, tmpInsertText);
	}
	if (tmpInsertText.Size()>0) {
		if (tmpInsertText[tmpInsertText.Size()-1] == '\0') {
			tmpInsertText.PopBack();
		}
	}
	if (tmpInsertText.Size()>0) {
		if (tmpInsertText[tmpInsertText.Size()-1] == '\0') {
			tmpInsertText.PopBack();
		}
	}
	m_data.Replace(start, end-start, tmpInsertText);
	// update internal elements
	eventModification(start, tmpInsertText.Size(), deletedText);
	return tmpInsertText.Size();
}


/**
 * @brief Remove data between [start..end]
 * 
 * @param[in] start	 Position started in the buffer
 * @param[in] end	   Position ended in the buffer
 * 
 * @return ---
 * 
 */
void EdnBuf::Remove(int32_t start, int32_t end)
{

	etk::VectorType<int8_t> deletedText;
	// Make sure the arguments make sense
	if (start > end) {
		int32_t temp = start;
		start = end;
		end = temp;
	}
	start = etk_avg(0 , start, m_data.Size());
	end = etk_avg(0 , end, m_data.Size());

	// Remove and redisplay
	GetRange(start, end, deletedText);
	m_data.Remove(start, end - start);
	eventModification(start, 0, deletedText);
}


int32_t EdnBuf::Indent(selectionType_te select)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = GetSelectionPos(select, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	if (false == haveSelectionActive) {
		return SelectionEnd;
	}
	// Disable selection:
	Unselect(select);
	// Get Range :
	int32_t l_start = StartOfLine(SelectionStart);
	int32_t l_end = EndOfLine(SelectionEnd);
	etk::VectorType<int8_t> l_tmpData;
	GetRange(l_start, l_end, l_tmpData);
	
	l_tmpData.Insert(0, '\n');
	for (int32_t i=1; i<l_tmpData.Size(); i++) {
		if ('\n' == l_tmpData[i-1]) {
			if (true == m_useTabs) {
				l_tmpData.Insert(i, '\t');
			} else {
				for (int32_t j=0; j<m_tabDist; j++) {
					l_tmpData.Insert(i, ' ');
				}
			}
		}
	}
	l_tmpData.Erase(0);
	// Real replace of DATA :
	Replace(l_start, l_end, l_tmpData);
	// Set the new selection :
	l_end = l_start + l_tmpData.Size();
	Select(select, l_start, l_end);
	// Return the position of the cursor
	return l_end;
}

int32_t EdnBuf::UnIndent(selectionType_te select)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = GetSelectionPos(select, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	if (false == haveSelectionActive) {
		return SelectionEnd;
	}
	// Disable selection:
	Unselect(select);
	// Get Range :
	int32_t l_start = StartOfLine(SelectionStart);
	int32_t l_end = EndOfLine(SelectionEnd);
	etk::VectorType<int8_t> l_tmpData;
	GetRange(l_start, l_end, l_tmpData);
	
	l_tmpData.Insert(0, '\n');
	for (int32_t i=1; i<l_tmpData.Size(); i++) {
		if ('\n' == l_tmpData[i-1]) {
			if('\t' == l_tmpData[i]) {
				l_tmpData.Erase(i);
			} else if(' ' == l_tmpData[i]) {
				for (int32_t j=0; j<m_tabDist && j+i<l_tmpData.Size() ; j++) {
					if(' ' == l_tmpData[i]) {
						l_tmpData.Erase(i);
					} else if('\t' == l_tmpData[i]) {
						l_tmpData.Erase(i);
						break;
					} else {
						break;
					}
				}
			}
		}
	}
	l_tmpData.Erase(0);
	// Real replace of DATA :
	Replace(l_start, l_end, l_tmpData);
	// Set the new selection :
	l_end = l_start + l_tmpData.Size();
	Select(select, l_start, l_end);
	// Return the position of the cursor
	return l_end;
}


/**
 * @brief Get the data of a specific line
 * 
 * @param[in] pos Position in a line that might be geted
 * @param[out] text Data in the current line at pos
 * 
 * @return ---
 * 
 */
void EdnBuf::GetLineText(int32_t pos, etk::VectorType<int8_t> &text)
{
	GetRange( StartOfLine(pos), EndOfLine(pos), text);
}


/**
 * @brief Find the position of the start of the current line
 * 
 * @param[in] pos position inside the line whe we need to find the start
 * 
 * @return position of the start of the line
 * 
 */
int32_t EdnBuf::StartOfLine(int32_t pos)
{
	int32_t startPos;
	if (false == SearchBackward(pos, '\n', &startPos)) {
		return 0;
	}
	return startPos + 1;
}


/**
 * @brief Find the position of the end of the current line
 * 
 * @param[in] pos position inside the line whe we need to find the end
 * 
 * @return position of the end of the line
 * 
 */
int32_t EdnBuf::EndOfLine(int32_t pos)
{
	int32_t endPos;
	if (false == SearchForward(pos, '\n', &endPos)) {
		endPos = m_data.Size();
	}
	return endPos;
}


/**
 * @brief Transform the current caracter in the buffer in a displayable vector of char
 * 
 * @param[in,out] pos position of the char that might be converted (incremented to the next char (in case of UTF8)
 * @param[in] indent Curent indentation befor the curent char
 * @param[out] outUTF8 string of the displayed element
 * @param[out] currentChar curent unicode output char
 * 
 * @return number of displayable char (display char width)
 * 
 */
int32_t EdnBuf::GetExpandedChar(int32_t &pos, int32_t indent, char outUTF8[MAX_EXP_CHAR_LEN], uint32_t &currentChar)
{
	int32_t i, nSpaces;
	char c = m_data.Get(pos);
	currentChar = (uint32_t)c & 0xFF;
	/* Convert tabs to spaces */
	if (c == '\t') {
		nSpaces = m_tabDist - (indent % m_tabDist);
		for (i=0; i<nSpaces; i++) {
			outUTF8[i] = ' ';
		}
		outUTF8[i] = '\0';
		pos++;
		return nSpaces;
	}
	
	// Convert ASCII control codes to readable character sequences
	if (c == '\0') {
		outUTF8[0] = '<';
		outUTF8[1] = 'n';
		outUTF8[2] = 'u';
		outUTF8[3] = 'l';
		outUTF8[4] = '>';
		outUTF8[5] = '\0';
		pos++;
		return 5;
	}
	if (((unsigned char)c) == '\n') {
		outUTF8[0] = (unsigned char)c;
		outUTF8[1] = '\0';
		pos++;
		return 1;
	}
	if (((unsigned char)c) <= 31) {
		const char * tmp = ControlCodeTable[(unsigned char)c];
		int32_t nbElem = 2;
		*outUTF8++ = '<';
		while (*tmp!='\0') {
			*outUTF8++ = *tmp;
			tmp++;
			nbElem++;
		}
		*outUTF8++ = '>';
		*outUTF8++ = '\0';
		pos++;
		return nbElem;
	} else if (c == 127) {
		outUTF8[0] = '<';
		outUTF8[1] = 'd';
		outUTF8[2] = 'e';
		outUTF8[3] = 'l';
		outUTF8[4] = '>';
		outUTF8[5] = '\0';
		pos++;
		return 5;
	}
	
	// Otherwise, just return the character
	if (false ==m_isUtf8) {
		convertIsoToUtf8(m_charsetType, c, outUTF8);
		pos++;
	} else {
		char tmpString[8];
		for (int32_t k=0; k<6 && k< m_data.Size() - pos; k++) {
			tmpString[k] = m_data.Get(pos+k);
		}
		tmpString[6] = '\0';
		uint8_t size;
		bool baseValid;
		unicode::Utf8_SizeElement(tmpString, 6 , size, baseValid);
		currentChar = 0; // TODO : Set UNICODE char ...
		if (true == baseValid) {
			char *tmp = outUTF8;
			for (int32_t k=0; k<size; k++) {
				*tmp++ = tmpString[k];
			}
			*tmp = '\0';
		} else {
			sprintf(outUTF8, "<? ? ? ?>");
		}
		if (0 == size) {
			APPL_ERROR("plop");
		}
		pos+=size;
		return strlen(outUTF8);
	}
	return 1;
}

/**
 * @brief Transform the current caracter in the buffer in a displayable vector of char
 * 
 * @param[in,out] pos position of the char that might be converted (incremented to the next char (in case of UTF8)
 * @param[in] indent Curent indentation befor the curent char
 * @param[out] outUTF8 string of the displayed element
 * @param[out] currentChar curent unicode output char
 * 
 * @return number of displayable char (display char width)
 * 
 */
int32_t EdnBuf::GetExpandedChar(int32_t &pos, int32_t indent, uniChar_t outUnicode[MAX_EXP_CHAR_LEN], uint32_t &currentChar)
{
	int32_t i, nSpaces;
	char c = m_data.Get(pos);
	currentChar = (uint32_t)c & 0xFF;
	/* Convert tabs to spaces */
	if (c == '\t') {
		nSpaces = m_tabDist - (indent % m_tabDist);
		for (i=0; i<nSpaces; i++) {
			outUnicode[i] = ' ';
		}
		outUnicode[i] = '\0';
		pos++;
		return nSpaces;
	}
	
	// Convert ASCII control codes to readable character sequences
	if (c == '\0') {
		outUnicode[0] = '<';
		outUnicode[1] = 'n';
		outUnicode[2] = 'u';
		outUnicode[3] = 'l';
		outUnicode[4] = '>';
		outUnicode[5] = '\0';
		pos++;
		return 5;
	}
	if (((unsigned char)c) == '\n') {
		outUnicode[0] = (unsigned char)c;
		outUnicode[1] = '\0';
		pos++;
		return 1;
	}
	if (((unsigned char)c) <= 31) {
		const char * tmp = ControlCodeTable[(unsigned char)c];
		int32_t nbElem = 2;
		*outUnicode++ = '<';
		while (*tmp!='\0') {
			*outUnicode++ = *tmp;
			tmp++;
			nbElem++;
		}
		*outUnicode++ = '>';
		*outUnicode++ = '\0';
		pos++;
		return nbElem;
	} else if (c == 127) {
		outUnicode[0] = '<';
		outUnicode[1] = 'd';
		outUnicode[2] = 'e';
		outUnicode[3] = 'l';
		outUnicode[4] = '>';
		outUnicode[5] = '\0';
		pos++;
		return 5;
	}
	// clear all the data ...
	memset(outUnicode, 0, sizeof(uniChar_t)*MAX_EXP_CHAR_LEN);
	
	// Otherwise, just return the character
	if (false ==m_isUtf8) {
		unicode::convertIsoToUnicode(m_charsetType, c, outUnicode[0]);
		pos++;
	} else {
		char tmpString[8];
		for (int32_t k=0; k<6 && k< m_data.Size() - pos; k++) {
			tmpString[k] = m_data.Get(pos+k);
		}
		tmpString[6] = '\0';
		uint8_t size;
		bool baseValid;
		unicode::Utf8_SizeElement(tmpString, 6 , size, baseValid);
		currentChar = 0;
		if (true == baseValid) {
			char tmp[20];
			for (int32_t kkk=0; kkk<size; kkk++) {
				tmp[kkk] = tmpString[kkk];
				tmp[kkk+1] = '\0';
			}
			unicode::convertUtf8ToUnicode(tmp, outUnicode[0]);
			outUnicode[1] = 0;
		} else {
			outUnicode[0] = '<';
			outUnicode[1] = '?';
			outUnicode[2] = '?';
			outUnicode[3] = '?';
			outUnicode[4] = '>';
			outUnicode[5] = 0;
		}
		if (0 == size) {
			APPL_ERROR("plop");
		}
		pos+=size;
		int32_t outSize = 0;
		while (outUnicode[outSize]!=0) {
			outSize++;
		}
		return outSize;
	}
	return 1;
}


/**
 * @brief generate the real display of character of the output (ex : \t ==> 4 spaces or less ...)
 * 
 * @param[in] c Char that might be converted
 * @param[in] indent Curent indentation befor the curent char
 * @param[out] outStr string of the displayed element
 * 
 * @return size of the display
 * 
 */
int32_t EdnBuf::ExpandCharacter(char c, int32_t indent, char outUTF8[MAX_EXP_CHAR_LEN])
{
	int32_t i, nSpaces;
	
	/* Convert tabs to spaces */
	if (c == '\t') {
		nSpaces = m_tabDist - (indent % m_tabDist);
		for (i=0; i<nSpaces; i++) {
			outUTF8[i] = ' ';
		}
		outUTF8[i] = '\0';
		return nSpaces;
	}
	
	// Convert ASCII control codes to readable character sequences
	if (c == '\0') {
		outUTF8[0] = '<';
		outUTF8[1] = 'n';
		outUTF8[2] = 'u';
		outUTF8[3] = 'l';
		outUTF8[4] = '>';
		outUTF8[5] = '\0';
		return 5;
	}
	if (((unsigned char)c) <= 31) {
		const char * tmp = ControlCodeTable[(unsigned char)c];
		*outUTF8++ = '<';
		while (*tmp!='\0') {
			*outUTF8++ = *tmp;
			tmp++;
		}
		*outUTF8++ = '>';
		*outUTF8++ = '\0';
		return ControlCodeTable[(unsigned char)c] - tmp + 2;
	} else if (c == 127) {
		outUTF8[0] = '<';
		outUTF8[1] = 'd';
		outUTF8[2] = 'e';
		outUTF8[3] = 'l';
		outUTF8[4] = '>';
		outUTF8[5] = '\0';
		return 5;
	}
	
	// Otherwise, just return the character
	//*outStr = c;   // deprecated 
	convertIsoToUtf8(unicode::EDN_CHARSET_ISO_8859_15, c, outUTF8);
	return 1;
}

/**
 * @brief Generate the size of the display of one element
 * 
 * @param[in] c Char that might be converted
 * @param[in] indent Curent indentation befor the curent char
 * 
 * @return size of the display
 * 
 */
int32_t EdnBuf::CharWidth(char c, int32_t indent)
{
	/* Note, this code must parallel that in ExpandCharacter */
	if (c == '\0') {
		return 5;
	} else if (c == '\t') {
		return m_tabDist - (indent % m_tabDist);
	} else if (((unsigned char)c) <= 31) {
		return strlen(ControlCodeTable[(unsigned char)c]) + 2;
	} else if (c == 127) {
		return 5;
	}
	return 1;
}


/**
 * @brief Count the number of displayed characters between buffer position
 * 
 * Displayed characters are the characters shown on the screen to represent characters in the 
 * buffer, where tabs and control characters are expanded
 * 
 * @param[in] lineStartPos	Start position in the line
 * @param[in] targetPos		Displayed target position in char
 * 
 * @return the ID in the buffer of the requested char
 * 
 */
int32_t EdnBuf::CountDispChars(int32_t lineStartPos, int32_t targetPos)
{
	EdnVectorBuf::Iterator myPosIt = m_data.Position(lineStartPos);
	int32_t charCount = 0;
	
	char expandedChar[MAX_EXP_CHAR_LEN];
	//APPL_DEBUG("lineStartPos="<< lineStartPos << " targetPos=" << targetPos);
	while(		myPosIt
			&&	myPosIt.Position() < targetPos )
	{
		charCount += ExpandCharacter(*myPosIt, charCount, expandedChar);
		myPosIt++;
	}
	//APPL_DEBUG(" result=" << charCount);
	return charCount;
}

/**
 * @brief Return the position of the nth diplaye char
 * 
 * @param[in] lineStartPos Position of the start of the line
 * @param[in] nChars search in the next nChars elements
 * 
 * @return number of diaplay char
 * 
 */
int32_t EdnBuf::CountForwardDispChars(int32_t lineStartPos, int32_t nChars)
{
	EdnVectorBuf::Iterator myPosIt = m_data.Position(lineStartPos);
	int32_t charCount = 0;
	char c;
	
	while(		charCount < nChars
			&&	myPosIt)
	{
		c = *myPosIt;
		if (c == '\n') {
			return myPosIt.Position();
		}
		charCount += CharWidth(c, charCount);
		myPosIt++;
	}
	return myPosIt.Position();
}


/**
 * @brief Count the number of newlines ('\n') between startPos and endPos
 * 
 * @param[in,out] startPos	Fist position in the buffer
 * @param[in,out] endPos	Last position in the buffer (not counted)
 * 
 * @return ---
 * 
 */
int32_t EdnBuf::CountLines(int32_t startPos, int32_t endPos)
{
	EdnVectorBuf::Iterator myPosIt = m_data.Position(startPos);
	int32_t lineCount = 0;
	
	while (myPosIt) {
		if (myPosIt.Position() == endPos) {
			return lineCount;
		}
		if ('\n' == *myPosIt) {
			lineCount++;
		}
		myPosIt++;
	}
	return lineCount;
}

/**
 * @brief count the number of '\n' in the vector of elements
 * 
 * @param[in] text Data to count the lines
 * 
 * @return number of line found
 * 
 */
int32_t EdnBuf::CountLines(etk::VectorType<int8_t> &data)
{
	etk::VectorType<int8_t>::Iterator myPosIt = data.Begin();
	int32_t lineCount = 0;
	
	while(myPosIt) {
		if ('\n' == *myPosIt) {
			lineCount++;
		}
		myPosIt++;
	}
	return lineCount;
}

int32_t EdnBuf::CountLines(void)
{
	EdnVectorBuf::Iterator myPosIt = m_data.Begin();
	int32_t lineCount = 0;
	
	while(myPosIt) {
		if ('\n' == *myPosIt) {
			lineCount++;
		}
		myPosIt++;
	}
	return lineCount;
}

/**
 * @brief Calculate the number of line
 *
 * @param ---
 *
 * @return the number of line in the buffer [1..n]
 *
 */
void EdnBuf::CountNumberOfLines(void)
{
	m_nbLine = CountLines(0, m_data.Size());
	m_nbLine++;
}


/**
 * @brief Find the first character of the line "nLines" forward
 * 
 * @param[in,out] startPos	Start position to count
 * @param[in,out] nLines	number of line to count
 * 
 * @return position of the starting the line
 * 
 */
int32_t EdnBuf::CountForwardNLines(int32_t startPos, int32_t nLines)
{
	if (nLines == 0) {
		return startPos;
	} else if (startPos > m_data.Size() ) {
		return m_data.Size();
	}
	EdnVectorBuf::Iterator myPosIt = m_data.Position(startPos);
	int32_t lineCount = 0;
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	while(myPosIt)
	{
		if ('\n' == *myPosIt) {
			lineCount++;
			if (lineCount == nLines) {
				//APPL_INFO("   ==> (1) at position=" << myPosIt.Position()+1 );
				return myPosIt.Position()+1;
			}
		}
		myPosIt++;
	}
	//APPL_INFO("   ==> (2) at position=" << myPosIt.Position() );
	return myPosIt.Position();
}


/**
 * @brief Find the first character of the line "nLines" backwards
 * 
 * @param[in,out] startPos	Start position to count (this caracter is not counted)
 * @param[in,out] nLines	number of line to count (if ==0 means find the beginning of the line)
 * 
 * @return position of the starting the line
 * 
 */
int32_t EdnBuf::CountBackwardNLines(int32_t startPos, int32_t nLines)
{
	if (startPos <= 0) {
		return 0;
	} else if (startPos > m_data.Size() ) {
		startPos = m_data.Size();
	}
	//APPL_INFO("startPos=" << startPos << " nLines=" << nLines);
	
	EdnVectorBuf::Iterator myPosIt = m_data.Position(startPos-1);
	int32_t lineCount = -1;
	
	while(myPosIt) {
		if ('\n' == *myPosIt) {
			lineCount++;
			if (lineCount >= nLines) {
				//APPL_INFO("   ==> (1) at position=" << myPosIt.Position()+1 );
				return myPosIt.Position()+1;
			}
		}
		myPosIt--;
	}
	//APPL_INFO("   ==> (2) at position=0");
	return 0;
}


bool EdnBuf::charMatch(char first, char second, bool caseSensitive)
{
	if (false == caseSensitive) {
		if ('A' <= first && 'Z' >= first) {
			first = first - 'A' + 'a';
		}
		if ('A' <= second && 'Z' >= second) {
			second = second - 'A' + 'a';
		}
	}
	if(first == second) {
		//APPL_DEBUG("charMatch(" << first << ", " << second << ", " << caseSensitive << ") ==> true");
		return true;
	} else {
		//APPL_DEBUG("charMatch(" << first << ", " << second << ", " << caseSensitive << ") ==> false");
		return false;
	}
}

/**
 * @brief Search forwards in buffer
 * 
 * @param[in] startPos		Position to start the search
 * @param[in] searchVect	String to search
 * @param[out] foundPos		Current position founded
 * 
 * @return true ==> found data
 * @return false ==> not found data
 * 
 */
bool EdnBuf::SearchForward(int32_t startPos, etk::VectorType<int8_t> &searchVect, int32_t *foundPos, bool caseSensitive)
{
	int32_t position;
	int32_t searchLen = searchVect.Size();
	int32_t dataLen   = m_data.Size();
	char currentChar = '\0';
	//APPL_INFO(" startPos=" << startPos << " searchLen=" << searchLen);
	for (position=startPos; position<dataLen - (searchLen-1); position++) {
		currentChar = m_data[position];
		if (true == charMatch(currentChar, searchVect[0], caseSensitive)) {
			int32_t i;
			bool found = true;
			for (i=1; i<searchLen; i++) {
				currentChar = m_data[position + i];
				if (false == charMatch(currentChar, searchVect[i], caseSensitive)) {
					found = false;
					break;
				}
			}
			if (true == found) {
				*foundPos = position;
				return true;
			}
		}
	}
	*foundPos = m_data.Size();
	return false;
}


/**
 * @brief Search backwards in buffer
 * 
 * @param[in] startPos		Position to start the search
 * @param[in] searchChars	String to search
 * @param[out] foundPos		Current position founded
 * 
 * @return true ==> found data
 * @return false ==> not found data
 * 
 */
bool EdnBuf::SearchBackward(int32_t startPos, etk::VectorType<int8_t> &searchVect, int32_t *foundPos, bool caseSensitive)
{
	int32_t position;
	int32_t searchLen = searchVect.Size();
	char currentChar = '\0';
	//APPL_INFO(" startPos=" << startPos << " searchLen=" << searchLen);
	for (position=startPos; position>=searchLen-1; position--) {
		currentChar = m_data[position];
		if (true == charMatch(currentChar, searchVect[searchLen-1], caseSensitive)) {
			int32_t i;
			bool found = true;
			for (i=searchLen-1; i>=0; i--) {
				currentChar = m_data[position - (searchLen-1) + i];
				if (false == charMatch(currentChar, searchVect[i], caseSensitive)) {
					found = false;
					break;
				}
			}
			if (true == found) {
				*foundPos = position - (searchLen-1);
				return true;
			}
		}
	}
	*foundPos = m_data.Size();
	return false;
}

static bool isChar(char value)
{
	if(		('a' <= value && 'z' >= value)
		||	('A' <= value && 'Z' >= value)
		||	('0' <= value && '9' >= value) 
		||	'_' == value
		||	'~' == value)
	{
		//APPL_DEBUG(" is a char \"" << value << "\"");
		return true;
	}
	//APPL_DEBUG(" is NOT a char \"" << value << "\"");
	return false;
}


bool EdnBuf::SelectAround(int32_t startPos, int32_t &beginPos, int32_t &endPos)
{
	char currentChar = m_data[startPos];
	if(		'\t' == currentChar
		||	' '  == currentChar)
	{
		APPL_DEBUG("select spacer");
		// special case we are looking for separation
		for (beginPos=startPos; beginPos>=0; beginPos--) {
			currentChar = m_data[beginPos];
			if(		'\t' != currentChar
				&&	' '  != currentChar)
			{
				beginPos++;
				break;
			}
		}
		// special case we are looking for separation
		for (endPos=startPos; endPos<m_data.Size(); endPos++) {
			currentChar = m_data[endPos];
			if(		'\t' != currentChar
				&&	' '  != currentChar)
			{
				break;
			}
		}
		return true;
	} else if( true == isChar(currentChar)){
		APPL_DEBUG("select normal Char");
		// Search back
		for (beginPos=startPos; beginPos>=0; beginPos--) {
			currentChar = m_data[beginPos];
			if( false == isChar(currentChar)) {
				beginPos++;
				break;
			}
		}
		// Search forward
		for (endPos=startPos; endPos<m_data.Size(); endPos++) {
			currentChar = m_data[endPos];
			if( false == isChar(currentChar)) {
				break;
			}
		}
		return true;
	} else {
		char comparechar = currentChar;
		APPL_DEBUG("select same char");
		// Search back
		for (beginPos=startPos; beginPos>=0; beginPos--) {
			currentChar = m_data[beginPos];
			if(comparechar != currentChar)
			{
				beginPos++;
				break;
			}
		}
		// Search forward
		for (endPos=startPos; endPos<m_data.Size(); endPos++) {
			currentChar = m_data[endPos];
			if(comparechar != currentChar)
			{
				break;
			}
		}
		return true;
	}
	beginPos = 0;
	endPos = 0;
	return false;
}


/**
 * @brief Insert data in the buffer binary and call all needed functions.
 * 
 * @param[in] pos Position to insert data in the buffer.
 * @param[in] insertText Data to insert.
 * 
 * @return number of element inserted.
 * 
 */
int32_t EdnBuf::LocalInsert(int32_t pos, etk::VectorType<int8_t> &insertText)
{
	// Insert data in buffer
	m_data.Insert(pos, insertText);
	// update the current selected area
	UpdateSelections(pos, 0, insertText.Size() );
	// return the number of element inserted ...
	return insertText.Size();
}
int32_t EdnBuf::LocalInsert(int32_t pos, etk::UString &insertText)
{
	etk::VectorType<int8_t> tmpInsertText;
	if (true == m_isUtf8) {
		char * tmpPointer = insertText.Utf8Data();
		while (*tmpPointer != '\0') {
			tmpInsertText.PushBack(*tmpPointer++);
		}
	} else {
		etk::VectorType<unsigned int> tmppp = insertText.GetVector();
		convertUnicodeToIso(m_charsetType, tmppp, tmpInsertText);
	}
	if (tmpInsertText.Size()>0) {
		if (tmpInsertText[tmpInsertText.Size()-1] == '\0') {
			tmpInsertText.PopBack();
		}
	}
	return LocalInsert(pos, tmpInsertText);
}


/**
 * @brief when modification appeare in the buffer we save it in the undo vector...
 * 
 * @param[in] pos			position of the add or remove
 * @param[in] nInserted		nb element inserted
 * @param[in] deletedText	Deleted elevent in a vector
 * 
 * @return ---
 * 
 */
void EdnBuf::eventModification(int32_t pos, int32_t nInserted, etk::VectorType<int8_t> &deletedText)
{
	if(		0 == deletedText.Size()
		&&	0 == nInserted)
	{
		// we do nothing ...
		//APPL_INFO("EdnBuf::eventModification(pos="<<pos<<", ... , nRestyled=" << nRestyled << ", deletedText=\"" << textDisplay << "\");");
	} else {
		APPL_INFO("(pos="<<pos<<", nDeleted="<<deletedText.Size()<<", nInserted=" << nInserted << ", deletedText=\"xx???xx\");");
		// update the number of lines : 
		//CountNumberOfLines(); 	//==> not efficent methode ...
		// ==> better methode : just update the number of line added and removed ...
		//APPL_INFO(" add=" << CountLines(pos, pos+nInserted) << " lines  |  remove="<< CountLines(deletedText) << " lines");
		m_nbLine += CountLines(pos, pos+nInserted) - CountLines(deletedText);
		// Update histories
		if (false == m_isUndoProcessing) {
			// normal or Redo processing
			EdnBufHistory *exempleHistory = new EdnBufHistory(pos, nInserted, deletedText);
			m_historyUndo.PushBack(exempleHistory);
			if (false == m_isRedoProcessing) {
				// remove all element in the redo system ...
				int32_t i;
				for (i=m_historyRedo.Size()-1; i>=0; i--) {
					if (NULL != m_historyRedo[i]) {
						delete(m_historyRedo[i]);
					}
					m_historyRedo.PopBack();
				}
			}
		} else {
			// undo processing ==> add element in Redo vector ...
			EdnBufHistory *exempleHistory = new EdnBufHistory(pos, nInserted, deletedText);
			m_historyRedo.PushBack(exempleHistory);
		}
		// Regenerate the Highlight : 
		RegenerateHighLightAt(pos, deletedText.Size(), nInserted);
	}
}





/**
 * @brief Search a character in the current buffer
 * 
 * @param[in] startPos Position to start the search of the element
 * @param[in] searchChar Character to search
 * @param[out] foundPos Position where it was found
 * 
 * @return true when find element
 * 
 */
bool EdnBuf::SearchForward(int32_t startPos, char searchChar, int32_t *foundPos)
{
	// Create iterator
	EdnVectorBuf::Iterator myPosIt = m_data.Position(startPos);
	// move in the string
	while (myPosIt) {
		if (*myPosIt == searchChar) {
			*foundPos = myPosIt.Position();
			return true;
		}
		myPosIt++;
	}
	*foundPos = m_data.Size();
	return false;
}


/**
 * @brief Search a character in the current buffer (backward
 * 
 * @param[in] startPos Position to start the search of the element
 * @param[in] searchChar Character to search
 * @param[out] foundPos Position where it was found
 * 
 * @return true when find element
 * 
 */
bool EdnBuf::SearchBackward(int32_t startPos, char searchChar, int32_t *foundPos)
{
	// Create iterator
	EdnVectorBuf::Iterator myPosIt = m_data.Position(startPos-1);
	// move in the string
	while (myPosIt) {
		if (*myPosIt == searchChar) {
			*foundPos = myPosIt.Position();
			return true;
		}
		myPosIt--;
	}
	*foundPos = 0;
	return false;
}


