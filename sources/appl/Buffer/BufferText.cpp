/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <BufferText.h>
#include <etk/RegExp.h>
#include <etk/unicode.h>

#include <ewol/ewol.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/Widget.h>
#include <ewol/clipBoard.h>
#include <ewol/renderer/eContext.h>


#undef __class__
#define __class__	"BufferText"


const uint32_t nbLineAllocatedInBase = 300;

extern "C"
{
	const char * g_pointerForTheDisplayLine[] = {"%1d", "%2d","%3d","%4d","%5d","%6d","%7d","%8d","%9d","%d"};
}



/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::BasicInit(void)
{
	// set basic position
	m_cursorPos = 0;
	m_cursorPreferredCol = -1;
	//m_cursorMode = CURSOR_DISPLAY_MODE_NORMAL;
	m_displaySize.setValue(200,20);
	m_centerRequested = false;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::NameChange(void)
{
	// Find HL system
	//APPL_DEBUG("check name change");
	if (true == HighlightManager::Exist(m_fileName)) {
		Highlight * myHL = HighlightManager::Get(m_fileName);
		// Set the new HL
		if (NULL != myHL) {
			m_EdnBuf.SetHLSystem(myHL);
		}
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
bool BufferText::IsModify(void)
{
	return m_fileModify;
}

void BufferText::SetModify(bool status)
{
	if (status != m_fileModify) {
		m_fileModify = status;
		// TODO : Remove from here
		etk::UString data = "Modify";
		ewol::GetContext().GetEObjectManager().MultiCast().AnonymousSend(ednMsgBufferState, data);
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
bool BufferText::NeedToUpdateDisplayPosition(void)
{
	bool tmpVal = m_updatePositionRequested;
	m_updatePositionRequested = false;
	return tmpVal;
}

vec2  BufferText::GetBorderSize(void)
{
	return vec2(30,30);;
}




/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
BufferText::BufferText()
{

	static int32_t fileBasicID = 0;
	m_updatePositionRequested = false;
	m_fileModify = true;
	m_haveName = false;
	etk::UString mString = "Untitle - ";
	mString += fileBasicID++;
	EWOL_DEBUG("Create buffer try  name : \"" << mString << "\"");
	SetFileName(mString);
	m_haveName = false;
	EWOL_DEBUG("Create buffer with name : " << m_fileName );
	
	BasicInit();
	SetModify(true);
	APPL_INFO("New(Empty-Buffer)");
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
BufferText::BufferText(etk::FSNode &fileName)
{
	m_fileModify = false;
	EWOL_DEBUG("Create buffer try  name : \"" << fileName << "\"");
	SetFileName(fileName);
	EWOL_DEBUG("Create buffer with name : " << m_fileName );
	
	BasicInit();
	NameChange();
	APPL_INFO("Add Data from file(" << GetFileName() << ")");
	etk::FSNode myFile(fileName);
	if (true == myFile.Exist()) {
		if (false == myFile.FileOpenRead()) {
			APPL_WARNING("File can not be open in read mode : " << myFile);
			SetModify(true);
		} else {
			m_EdnBuf.DumpFrom(myFile);
			myFile.FileClose();
			SetModify(false);
		}
	} else {
		// fichier inexistant... creation d'un nouveaux
		APPL_WARNING("No File ==> created a new one(" << GetFileName() << ")");
		SetModify(true);
	}
	RequestUpdateOfThePosition();
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::Save(void)
{
	APPL_INFO("Save File : \"" <<  GetFileName() << "\"" );
	etk::FSNode myFile(GetFileName());
	if (false == myFile.FileOpenWrite()) {
		APPL_ERROR("Can not open in writing the specify file");
	} else {
		m_EdnBuf.DumpIn(myFile);
		myFile.FileClose();
		SetModify(false);
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
BufferText::~BufferText(void)
{
	APPL_INFO("Delete(BufferText)");
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::GetInfo(infoStatBuffer_ts &infoToUpdate)
{

}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::SetLineDisplay(uint32_t lineNumber)
{

}

#define SEPARATION_SIZE_LINE_NUMBER         (3)

void BufferText::DrawLineNumber(ewol::Text* OOText, int32_t sizeX, int32_t sizeY, int32_t nbColomn,  int32_t lineNumber, int32_t positionY)
{
	char tmpLineNumber[50];
	sprintf(tmpLineNumber, "%*d", nbColomn, lineNumber);
	OOText->SetColorBg(ColorizeManager::Get(COLOR_LIST_BG_2));
	OOText->SetColor(ColorizeManager::Get(COLOR_CODE_LINE_NUMBER));
	OOText->SetPos(vec3(1.0f, (float)positionY, 0.0f) );
	OOText->Print(tmpLineNumber);
}


/**
 * @brief get the number of colomn neede to display lineNumber
 *
 * @param[in,out] ---
 *
 * @return the number of colomn
 *
 */
int32_t BufferText::GetLineNumberNumberOfElement(void)
{
	int32_t nbColoneForLineNumber = 1;
	// get the number of line in the buffer
	int32_t maxNumberLine = m_EdnBuf.NumberOfLines();
	if (10 > maxNumberLine) {					nbColoneForLineNumber = 1;
	} else if (100 > maxNumberLine) {			nbColoneForLineNumber = 2;
	} else if (1000 > maxNumberLine) {			nbColoneForLineNumber = 3;
	} else if (10000 > maxNumberLine) {			nbColoneForLineNumber = 4;
	} else if (100000 > maxNumberLine) {		nbColoneForLineNumber = 5;
	} else if (1000000 > maxNumberLine) {		nbColoneForLineNumber = 6;
	} else if (10000000 > maxNumberLine) {		nbColoneForLineNumber = 7;
	} else if (100000000 > maxNumberLine) {		nbColoneForLineNumber = 8;
	} else if (1000000000 > maxNumberLine) {	nbColoneForLineNumber = 9;
	} else {									nbColoneForLineNumber = 10;
	}
	return nbColoneForLineNumber;
}

int32_t BufferText::GetNumberOfLine(void)
{
	return m_EdnBuf.CountLines();
}

/**
 * @brief Display the curent buffer with all the probematic imposed by the xharset and the user contraint.
 *
 * @param[in,out] drawer the basic user drawer of EDN.
 *
 * @return 
 *
 */
int32_t BufferText::Display(ewol::Text& OOText,
                            int32_t offsetX, int32_t offsetY,
                            int32_t sizeX, int32_t sizeY)
{
	int32_t selStart, selEnd, selRectStart, selRectEnd;
	bool selIsRect;
	int32_t selHave;
	
	vec3 tmpLetterSize = OOText.CalculateSize((uniChar_t)'A');
	
	int32_t letterWidth = tmpLetterSize.x();
	int32_t letterHeight = tmpLetterSize.y();
	
	int32_t displayStartLineId = offsetY / letterHeight - 1;
	displayStartLineId = etk_max(0, displayStartLineId);
	int32_t y = - offsetY + displayStartLineId*letterHeight;
	
	// update the display position with the scroll ofset : 
	int32_t displayStartBufferPos = m_EdnBuf.CountForwardNLines(0, displayStartLineId);
	vec2  maxSize;
	maxSize.setX(0);
	maxSize.setY(m_EdnBuf.NumberOfLines() * tmpLetterSize.y());
	int32_t nbColoneForLineNumber = GetLineNumberNumberOfElement();
	
	// update the number of element that can be displayed
	m_displaySize.setX((sizeX/letterWidth) + 1 - nbColoneForLineNumber);
	m_displaySize.setY((sizeY/letterHeight) + 1);
	APPL_VERBOSE("main DIPLAY " << m_displaySize.x() << " char * " << m_displaySize.y() << " char");
	
	selHave = m_EdnBuf.GetSelectionPos(selStart, selEnd, selIsRect, selRectStart, selRectEnd);
	
	colorInformation_ts * HLColor = NULL;
	
	int32_t iii, new_i;
	// Get color : 
	Colorize *  myColor           = ColorizeManager::Get("normal");
	Colorize *  myColorSel        = ColorizeManager::Get("SelectedText");
	etk::Color<> & myColorSpace   = ColorizeManager::Get(COLOR_CODE_SPACE);
	etk::Color<> & myColorTab     = ColorizeManager::Get(COLOR_CODE_TAB);
	Colorize *  selectColor       = NULL;
	int mylen = m_EdnBuf.Size();
	int32_t x_base=nbColoneForLineNumber*letterWidth;
	int32_t idX = 0;
	
	int64_t startTime = ewol::GetTime();
	int displayLines = 0;
	// Regenerate the colorizing if necessary ...
	displayHLData_ts m_displayLocalSyntax;
	m_EdnBuf.HightlightGenerateLines(m_displayLocalSyntax, displayStartBufferPos, m_displaySize.y());
	
	int64_t stopTime = ewol::GetTime();
	APPL_DEBUG("Parsing Highlight = " << stopTime - startTime << " micro-s");
	
	uniChar_t displayChar[MAX_EXP_CHAR_LEN];
	memset(displayChar, 0, sizeof(uniChar_t)*MAX_EXP_CHAR_LEN);
	etk::UString myStringToDisplay;
	// draw the lineNumber : 
	int32_t currentLineID = displayStartLineId+1;
	APPL_VERBOSE("Start display of text buffer [" << displayStartBufferPos<< ".." << mylen << "]");
	APPL_VERBOSE("cursor Pos : " << m_cursorPos << "start at pos=" << displayStartBufferPos);
	
	// note corection of the openGl invertion system :
	y = sizeY - y;
	y -= letterHeight;
	
	OOText.SetClippingMode(false);
	DrawLineNumber(&OOText, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
	int32_t pixelX = x_base + SEPARATION_SIZE_LINE_NUMBER;
	
	vec3 drawClippingPos(0,0,-0.5);
	vec3 drawClippingSize(sizeX, sizeY, 1);
	OOText.SetClippingWidth(vec3((float)pixelX, 0.0f, -0.5f),
	                        vec3((float)(sizeX - drawClippingPos.x()), (float)sizeY, 0.5f) );
	
	// Clear the line intexation :
	m_elmentList.Clear();
	// every char element is register to find the diplay pos when mouse event arrive
	CharElement tmpElementProperty;
	tmpElementProperty.m_yOffset = y;
	tmpElementProperty.m_xOffset = 0;
	tmpElementProperty.m_ySize = 10;
	tmpElementProperty.m_bufferPos = displayStartBufferPos;
	m_elmentList.PushBack(tmpElementProperty);
	
	vec3 tmpCursorPosition(0, 0, -1); //Cursor is display only at the end to be all time over the background ... (-1 in z no cursor)
	
	float lineMaxSize = 0.0;
	for (iii=displayStartBufferPos; iii<mylen && displayLines >=0 && y >= -2*letterHeight; iii = new_i) {
		//APPL_DEBUG("display pos =" << y);
		int displaywidth;
		uint32_t currentChar = '\0';
		new_i = iii;
		// update the element buffer pos:
		tmpElementProperty.m_bufferPos = new_i;
		displaywidth = m_EdnBuf.GetExpandedChar(new_i, idX, displayChar, currentChar);
		int32_t drawSize = 0;
		
		// update display position :
		vec2  textPos(pixelX-offsetX, y);
		// update X pos
		tmpElementProperty.m_xOffset = textPos.x();
		tmpElementProperty.m_yOffset = textPos.y();
		
		//APPL_INFO("diplay element=" << new_i);
		if (currentChar!='\n') {
			selectColor = myColor;
			HLColor = m_EdnBuf.GetElementColorAtPosition(m_displayLocalSyntax, iii);
			if (NULL != HLColor) {
				if (NULL != HLColor->patern) {
					selectColor = HLColor->patern->GetColor();
				}
			}
			OOText.SetColorBg(etk::color::none);
			if(    true == selHave
			    && selStart <= iii
			    && selEnd > iii)
			{
				selectColor = myColorSel;
				OOText.SetColorBg(selectColor->GetBG() );
			} else {
				if(false == selectColor->HaveBg()) {
					if(    (uniChar_t)' ' == currentChar
					    && true == globals::IsSetDisplaySpaceChar() )
					{
						OOText.SetColorBg(myColorSpace);
					} else if(    '\t' == currentChar
					           && true == globals::IsSetDisplayTabChar() )
					{
						OOText.SetColorBg(myColorTab);
					}
				} else {
					OOText.SetColorBg(selectColor->GetBG());
				}
			}
			tmpElementProperty.m_ySize = 20;//OOText.GetHeight();
			OOText.SetColor(selectColor->GetFG() );
			//OOText.SetColorBg(selectColor->GetFG());
			OOText.SetFontBold(selectColor->GetBold());
			OOText.SetFontItalic(selectColor->GetItalic());
			myStringToDisplay = displayChar;
			OOText.SetPos(vec3(textPos.x(), textPos.y(), 0.0f) );
			OOText.Print(myStringToDisplay);
			// To update the display position
			drawSize = OOText.GetPos().x() - textPos.x();
			//APPL_DEBUG("add element : " << tmpElementProperty.m_yOffset << "," << tmpElementProperty.m_xOffset);
			m_elmentList.PushBack(tmpElementProperty);
			
		}
		idX += displaywidth;
		// display cursor : 
		if (m_cursorPos == iii) {
			tmpCursorPosition = vec3(pixelX - offsetX, y, 0);
		}
		lineMaxSize += drawSize;
		pixelX += drawSize;
		// move to next line ...
		if (currentChar=='\n') {
			maxSize.setX(etk_max(lineMaxSize, maxSize.x()));
			lineMaxSize = 0.0;
			idX =0;
			pixelX = x_base + SEPARATION_SIZE_LINE_NUMBER;
			y -= letterHeight;
			//APPL_DEBUG("display pos =" << y);
			displayLines++;
			currentLineID++;
			OOText.SetClippingMode(false);
			OOText.SetFontBold(false);
			OOText.SetFontItalic(false);
			DrawLineNumber(&OOText, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
			OOText.SetClippingMode(true);
			// add elements :
			m_elmentList.PushBack(tmpElementProperty);
		}
	}
	//APPL_DEBUG("end at pos buf =" << iii << " / " << m_EdnBuf.Size());
	// special case : the cursor is at the end of the buffer...
	if (m_cursorPos == iii) {
		tmpCursorPosition = vec3(pixelX - offsetX, y, 0);
	}
	if (tmpCursorPosition.z()!=-1) {
		// display the cursor:
		OOText.SetPos(tmpCursorPosition);
		OOText.SetColor(ColorizeManager::Get(COLOR_CODE_CURSOR));
		OOText.SetColorBg(ColorizeManager::Get(COLOR_CODE_CURSOR));
		OOText.PrintCursor(ewol::GetCurrentSpecialKeyStatus().IsSetInsert());
	}
	// set the maximum size for the display ...
	SetMaximumSize(maxSize);
	int64_t stopTime2 = ewol::GetTime();
	APPL_DEBUG("DRAW text (brut) = " << stopTime2 - stopTime << " micro-s");

	return 0;
}



int32_t BufferText::GetMousePosition(vec2 pos)
{
	bool inLineDone=false;
	//APPL_DEBUG("try to find in : " << width << "," << height);
	for(int32_t iii=0; iii<m_elmentList.Size()-1; iii++) {
		//APPL_DEBUG("check element : " << m_elmentList[iii].m_yOffset << "<= " << pos.y << " <" << (m_elmentList[iii].m_yOffset + m_elmentList[iii].m_ySize));
		if(false == inLineDone) {
			if(    pos.y()>=m_elmentList[iii].m_yOffset
			    && pos.y()<m_elmentList[iii].m_yOffset + m_elmentList[iii].m_ySize ) {
				// we find the line (int theory) ==> note : Some problem can appear here when the size are not the same ...
				// this is to prevent multiple size font ...
				inLineDone = true;
				//APPL_DEBUG("        ==> " << m_elmentList[iii+1].m_xOffset << "> " << pos.x << " >=" << m_elmentList[iii].m_xOffset);
			}
		}
		// we detected the line
		if(true == inLineDone) {
			if(    pos.x()>=m_elmentList[iii].m_xOffset
			    && pos.x()<m_elmentList[iii+1].m_xOffset ) {
				// we get the position ...
				return m_elmentList[iii].m_bufferPos;
			} else if (m_elmentList[iii].m_xOffset>=m_elmentList[iii+1].m_xOffset) {
				// prevent "end of line" cursor pos ...
				return m_elmentList[iii].m_bufferPos;
			}
		}
	}
	if (m_elmentList.Size()>0) {
		if(pos.y()<m_elmentList[m_elmentList.Size()/2].m_yOffset) {
			//APPL_DEBUG("Error to get position (return Last)");
			return m_elmentList[m_elmentList.Size()-1].m_bufferPos;
		} else {
			//APPL_DEBUG("Error to get position (return begin)");
			return m_elmentList[0].m_bufferPos;
		}
	} else {
		APPL_CRITICAL("Error to get position (very bad)");
		return 0;
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
// TODO : Simplify selection ....
void BufferText::MouseEvent(vec2 pos)
{
	if (ewol::GetCurrentSpecialKeyStatus().IsSetShift() ) {
		MouseSelectFromCursorTo(pos);
	} else {
		// Get the caracter mouse position
		int32_t newPos = GetMousePosition(pos);
		// move the cursor
		SetInsertPosition(newPos);

		// if a preferred column wasn't aleady established, establish it
		/*if (m_cursorPreferredCol < 0) {
			m_cursorPreferredCol = posX;
		}*/
		m_EdnBuf.Unselect();

		RequestUpdateOfThePosition();
	}
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 * @todo : Set the move up and DOWN...
 *
 */
void BufferText::MouseSelectFromCursorTo(vec2 pos)
{
	// Get the caracter mouse position
	int32_t newPos = GetMousePosition(pos);
	
	int32_t selStart, selEnd, selRectStart, selRectEnd;
	bool selIsRect;
	int32_t selHave = m_EdnBuf.GetSelectionPos(selStart, selEnd, selIsRect, selRectStart, selRectEnd);
	//APPL_DEBUG("BufferText:: " << selHave << " = BufGetSelectionPos(SELECTION_PRIMARY," << selStart << "," << selEnd << "," << selIsRect << "," << selRectStart << "," << selRectEnd << ");" );
	int32_t rememberCursorPos = m_cursorPos;
	// move the cursor
	SetInsertPosition(newPos);
	
	// if a preferred column wasn't aleady established, establish it
	/*if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = posX;
	}*/
	
	if (false == selHave) {
		m_EdnBuf.Select(rememberCursorPos, m_cursorPos);
	} else {
		if (rememberCursorPos == selStart) {
			m_EdnBuf.Select(m_cursorPos, selEnd);
		} else {
			m_EdnBuf.Select(selStart, m_cursorPos);
		}
	}
	Copy(ewol::clipBoard::clipboardSelection);
	RequestUpdateOfThePosition();
}



/**
 * @brief double click : select the data around the selection
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::MouseEventDouble(void)
{
	int32_t beginPos, endPos;
	if (true == m_EdnBuf.SelectAround(m_cursorPos, beginPos, endPos)) {
		m_EdnBuf.Select(beginPos, endPos);
		m_cursorPos = endPos;
	}
	Copy(ewol::clipBoard::clipboardSelection);
	// no else
}

/**
 * @brief triple click select all the line
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::MouseEventTriple(void)
{
	m_EdnBuf.Select(m_EdnBuf.StartOfLine(m_cursorPos), m_EdnBuf.EndOfLine(m_cursorPos));
	m_cursorPos = m_EdnBuf.EndOfLine(m_cursorPos);
	Copy(ewol::clipBoard::clipboardSelection);
}

void BufferText::RemoveLine(void)
{
	int32_t start = m_EdnBuf.StartOfLine(m_cursorPos);
	int32_t stop = m_EdnBuf.EndOfLine(m_cursorPos);
	m_EdnBuf.Remove(start, stop+1);
	SetInsertPosition(start);
	SetModify(true);
}

void BufferText::SelectAll(void)
{
	m_EdnBuf.Select(0, m_EdnBuf.Size());
	m_cursorPos = m_EdnBuf.Size();
	Copy(ewol::clipBoard::clipboardSelection);
}

void BufferText::SelectNone(void)
{
	m_EdnBuf.Unselect();
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::SetInsertPosition(int32_t newPos, bool insertChar)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	int32_t rememberCursorPos = m_cursorPos;

	//APPL_DEBUG("newPos=" << newPos);

	// unselect buffer:
	m_EdnBuf.Unselect();
	/* make sure new position is ok, do nothing if it hasn't changed */
	if (newPos != m_cursorPos) {
		if (newPos < 0){
			newPos = 0;
		}
		if (newPos > m_EdnBuf.Size()) {
			newPos = m_EdnBuf.Size();
		}
		m_cursorPos = newPos;
	}
	m_cursorPreferredCol = -1;
	
	// special case when insert char ...
	if (true == insertChar) {
		return;
	}
	
	if(    false == haveSelectionActive
	    && true  == ewol::GetCurrentSpecialKeyStatus().IsSetShift() )
	{
		// new selection
		m_EdnBuf.Select(rememberCursorPos, m_cursorPos);
	} else if(    true == ewol::GetCurrentSpecialKeyStatus().IsSetShift()
	           && true == haveSelectionActive)
	{
		// update selection
		if (rememberCursorPos == SelectionStart) {
			m_EdnBuf.Select(m_cursorPos, SelectionEnd);
		} else {
			m_EdnBuf.Select(SelectionStart, m_cursorPos);
		}
	}
}

bool BufferText::TextDMoveUp(int32_t offset)
{
	int32_t lineStartPos, column, prevLineStartPos, newPos;

	// Find the position of the start of the line.
	lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
	// check if we can go up ...
	if (lineStartPos == 0) {
		return false;
	}

	// Decide what column to move to, if there's a preferred column use that
	if (m_cursorPreferredCol >= 0) {
		column = m_cursorPreferredCol;
	} else {
		column = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
	}

	// Get the previous line
	prevLineStartPos = m_EdnBuf.CountBackwardNLines(lineStartPos, offset);
	//APPL_INFO("Move Line UP result : prevLineStartPos=" << prevLineStartPos);
	// Get the display char position
	newPos = m_EdnBuf.CountForwardDispChars(prevLineStartPos, column);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);

	// move the cursor
	SetInsertPosition(newPos);

	// if a preferred column wasn't aleady established, establish it
	if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = column;
	}

	return true;
}

bool BufferText::TextDMoveDown(int32_t offset)
{
	int32_t lineStartPos, column, nextLineStartPos, newPos;

	// check if we are not at the end of Buffer
	if (m_cursorPos == m_EdnBuf.Size() ) {
		return false;
	}
	// Find the position of the start of the line.
	lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
	
	if (m_cursorPreferredCol >= 0) {
		column = m_cursorPreferredCol;
	} else {
		column = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
	}
	// get the next line :
	nextLineStartPos = m_EdnBuf.CountForwardNLines(lineStartPos, offset);
	//APPL_INFO("Move Line DOWN result : nextLineStartPos=" << nextLineStartPos);
	// Get the display char position
	newPos = m_EdnBuf.CountForwardDispChars(nextLineStartPos, column);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	
	SetInsertPosition(newPos);
	// if a preferred column wasn't aleady established, establish it
	if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = column;
	}
	return true;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::cursorMove(ewol::keyEvent::keyboard_te moveTypeEvent)
{
	bool needUpdatePosition = true;
	// check selection event ...
	switch(moveTypeEvent) {
		case ewol::keyEvent::keyboardLeft:
			//APPL_INFO("keyEvent : <LEFT>");
			if (m_cursorPos > 0) {
				SetInsertPosition(m_cursorPos - 1); 
			}
			break;
		case ewol::keyEvent::keyboardRight:
			//APPL_INFO("keyEvent : <RIGHT>");
			if (m_cursorPos < m_EdnBuf.Size() ) {
				SetInsertPosition(m_cursorPos + 1);
			}
			break;
		case ewol::keyEvent::keyboardUp:
			//APPL_INFO("keyEvent : <UP>");
			TextDMoveUp(1);
			break;
		case ewol::keyEvent::keyboardDown:
			//APPL_INFO("keyEvent : <DOWN>");
			// check if we have enought line ...
			TextDMoveDown(1);
			break;
		case ewol::keyEvent::keyboardPageUp:
			//APPL_INFO("keyEvent : <PAGE-UP>");
			TextDMoveUp(m_displaySize.y());
			break;
		case ewol::keyEvent::keyboardPageDown:
			//APPL_INFO("keyEvent : <PAGE-DOWN>");
			TextDMoveDown(m_displaySize.y());
			break;
		case ewol::keyEvent::keyboardStart:
			//APPL_INFO("keyEvent : <Start of line>");
			SetInsertPosition(m_EdnBuf.StartOfLine(m_cursorPos) );
			break;
		case ewol::keyEvent::keyboardEnd:
			//APPL_INFO("keyEvent : <End of line>");
			SetInsertPosition(m_EdnBuf.EndOfLine(m_cursorPos) );
			break;
		default:
			//LastUpDownoutputPosition = -1;
			needUpdatePosition = false;
			break;
	}
	if ( true == needUpdatePosition) {
		RequestUpdateOfThePosition();
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
vec2  BufferText::GetPosition(int32_t fontId, bool& centerRequested)
{
	centerRequested = m_centerRequested;
	m_centerRequested = false;
	vec2  outputPosition(0,0);

	// Display position (Y mode):
	APPL_INFO("change the position : " << m_cursorPos);
	// get the line id of the curent position of the cursor :
	outputPosition.setY(m_EdnBuf.CountLines(0, m_cursorPos));
	// get the first position of the current line
	int32_t lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
	// count the number of char in the line (real displayed char with whar like <kjkj>)
	outputPosition.setX(m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos));
	APPL_INFO("Curent cursor pos=" << outputPosition);
	
	// get font porperties :
	// TODO : change this :
	// TODO : Set it back ...
	/*
	float letterWidth = ewol::GetWidth(fontId, "A");
	float letterHeight = ewol::GetHeight(fontId);
	*/
	float letterWidth = 10;
	float letterHeight = 15;
	outputPosition.setValue(outputPosition.x() * letterWidth,
	                        outputPosition.y() * letterHeight);
	return outputPosition;

	/* if we request a center : 
	} else {
		// center the line at the middle of the screen :
		vec2  outputPosition(0,0);
		//APPL_DEBUG(" -------------------------------------------------");
		outputPosition.y = m_EdnBuf.CountLines(0, m_cursorPos);
		//APPL_DEBUG(" cursor position : " << m_cursorPos << " ==> ligne=" << outputPosition.y);
		outputPosition.x = 0;
		
		m_displayStartPixelX = 0;
		//APPL_DEBUG(" display size : " << m_displaySize.y);
		m_displayStartLineId = outputPosition.y - m_displaySize.y/2;
		m_displayStartLineId = edn_max(m_displayStartLineId, 0);
		
		m_displayStartBufferPos = m_EdnBuf.CountForwardNLines(0, m_displayStartLineId);
		//APPL_DEBUG(" display start : " << m_displayStartPixelX << "x" << m_displayStartLineId);
		//APPL_DEBUG(" -------------------------------------------------");
	}
	*/
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::AddChar(uniChar_t unicodeData)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	if (unicodeData == 0x09) {
		if (false == haveSelectionActive) {
			etk::Vector<int8_t> tmpVect;
			tmpVect.PushBack(0x09);
			m_EdnBuf.Insert(m_cursorPos, tmpVect);
			SetInsertPosition(m_cursorPos+1, true);
		} else {
			// Indent depend of the multiline in the selection ...
			// count the number of line : 
			int32_t nbSelectedLines = m_EdnBuf.CountLines(SelectionStart, SelectionEnd);
			if (0 == nbSelectedLines) {
				etk::Vector<int8_t> tmpVect;
				tmpVect.PushBack(0x09);
				m_EdnBuf.ReplaceSelected(tmpVect);
				SetInsertPosition(SelectionStart+tmpVect.Size(), true);
			} else {
				if (true == ewol::GetCurrentSpecialKeyStatus().IsSetShift() ) {
					m_cursorPos = m_EdnBuf.UnIndent();
				} else {
					m_cursorPos = m_EdnBuf.Indent();
				}
			}
		}
	} else if (unicodeData == '\n') {
		etk::Vector<int8_t> tmpVect;
		if (true == ewol::GetCurrentSpecialKeyStatus().IsSetShift()) {
			tmpVect.PushBack('\r');
		} else {
			tmpVect.PushBack('\n');
			// if Auto indent Enable ==> we get the start of the previous line and add it to tne new one
			if (true == globals::IsSetAutoIndent() ) {
				int32_t l_lineStart;
				// Get the begin of the line or the begin of the line befor selection
				if (false == haveSelectionActive) {
					l_lineStart = m_EdnBuf.StartOfLine(m_cursorPos);
				} else {
					l_lineStart = m_EdnBuf.StartOfLine(SelectionStart);
				}
				// add same characters in the temporar buffer
				for (int32_t kk=l_lineStart; kk<m_cursorPos; kk++) {
					if (' ' == m_EdnBuf[kk]) {
						tmpVect.PushBack(' ');
					} else if('\t' == m_EdnBuf[kk]) {
						tmpVect.PushBack('\t');
					} else {
						break;
					}
				}
			}
		}
		// Set temporary buffer in the real buffer
		if (false == haveSelectionActive) {
			m_EdnBuf.Insert(m_cursorPos, tmpVect);
			SetInsertPosition(m_cursorPos+tmpVect.Size(), true);
		} else {
			m_EdnBuf.ReplaceSelected(tmpVect);
			SetInsertPosition(SelectionStart+tmpVect.Size(), true);
		}
	} else if (unicodeData == 0x7F ) {
		//APPL_INFO("keyEvent : <Suppr>	pos=" << m_cursorPos);
		if (false == haveSelectionActive) {
			m_EdnBuf.Remove(m_cursorPos, m_cursorPos+1);
		} else {
			m_EdnBuf.RemoveSelected();
			SetInsertPosition(SelectionStart, true);
		}
	} else if (unicodeData == 0x08) {
		//APPL_INFO("keyEvent : <Del>		pos=" << m_cursorPos);
		if (false == haveSelectionActive) {
			m_EdnBuf.Remove(m_cursorPos-1, m_cursorPos);
			SetInsertPosition(m_cursorPos-1, true);
		} else {
			m_EdnBuf.RemoveSelected();
			SetInsertPosition(SelectionStart, true);
		}
	} else {
		// normal adding char ...
		if (true == m_EdnBuf.GetUTF8Mode()) {
			char tmpUTF8[16];
			unicodeData.GetUtf8(tmpUTF8);
			etk::Vector<int8_t> tmpVect;
			int32_t localOfset = strlen(tmpUTF8);
			tmpVect.PushBack((int8_t*)tmpUTF8, localOfset);
			if (false == haveSelectionActive) {
				m_EdnBuf.Insert(m_cursorPos, tmpVect);
				SetInsertPosition(m_cursorPos+localOfset, true);
			} else {
				m_EdnBuf.ReplaceSelected(tmpVect);
				SetInsertPosition(SelectionStart+localOfset, true);
			}
		} else {
			// convert in the Good ISO format : 
			char output_ISO;
			unicode::convertUnicodeToIso(m_EdnBuf.GetCharsetType(), unicodeData, output_ISO);
			//printf(" insert : \"%s\"==> 0x%08x=%d ", UTF8data, (unsigned int)output_ISO, (int)output_ISO);
			etk::Vector<int8_t> tmpVect;
			tmpVect.PushBack(output_ISO);
			if (false == haveSelectionActive) {
				m_EdnBuf.Insert(m_cursorPos, tmpVect);
				SetInsertPosition(m_cursorPos+1, true);
			} else {
				m_EdnBuf.ReplaceSelected(tmpVect);
				SetInsertPosition(SelectionStart+1, true);
			}
		}
	}

	SetModify(true);
	RequestUpdateOfThePosition();
}


int32_t BufferText::FindLine(etk::UString &data)
{
	if ( 0 == data.Size()) {
		APPL_WARNING("no search data");
		return 0;
	}
	APPL_INFO("Search data line : \"" << data << "\"");
	etk::Vector<uniChar_t> mVectSearch;
	mVectSearch = data.GetVector();
	//APPL_INFO("search data Forward : startSearchPos=" << startSearchPos );
	/*
	int32_t foundPos;
	bool findSomething = m_EdnBuf.SearchForward(0, mVectSearch, &foundPos, true);
	// if find data : 
	if (true == findSomething) {
		return m_EdnBuf.CountLines(0, foundPos);
	} else {
		return 0;
	}
	*/
	APPL_TODO("Remove for now ...");
	return 0;
}

void BufferText::JumpAtLine(int32_t newLine)
{
	int32_t positionLine = m_EdnBuf.CountForwardNLines(0, newLine);
	m_EdnBuf.Unselect();
	APPL_DEBUG("jump at the line : " << newLine );
	SetInsertPosition(positionLine);
	m_centerRequested = true;
	RequestUpdateOfThePosition();
}

/**
 * @brief Get the current line (to know where to jump)
 *
 * @param ---
 *
 * @return Return the current line number
 *
 */
int32_t BufferText::GetCurrentLine(void)
{
	return m_EdnBuf.CountLines(0, m_cursorPos);
}



void BufferText::Search(etk::UString &data, bool back, bool caseSensitive, bool wrap, bool regExp)
{
	APPL_INFO("Search data : \"" << data << "\"");
	
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	int32_t startSearchPos = m_cursorPos;
	if (true == haveSelectionActive) {
		if (false == back) {
			startSearchPos = SelectionEnd;
		} else {
			startSearchPos = SelectionStart;
		}
	}
	
	if ( 0 == data.Size()) {
		APPL_WARNING("no search data");
		return;
	}
	if (false == back) {
		//APPL_INFO("search data Forward : startSearchPos=" << startSearchPos );
		int32_t foundPos;
		int32_t foundPosEnd;
		bool findSomething = m_EdnBuf.SearchForward(startSearchPos, data, &foundPos, &foundPosEnd, caseSensitive);
		if(		false == findSomething
			&&	true == wrap)
		{
			//APPL_INFO("WrapMode !!! 0 ==> end");
			findSomething = m_EdnBuf.SearchForward(0, data, &foundPos, &foundPosEnd, caseSensitive);
		}
		// if find data : 
		if (true == findSomething) {
			// select new position
			SetInsertPosition(foundPosEnd);
			m_EdnBuf.Select(foundPos, foundPosEnd);
		}
	} else {
		//APPL_INFO("search data Backward : " << data.GetDirectPointer() );
		int32_t foundPos;
		int32_t foundPosEnd;
		bool findSomething = m_EdnBuf.SearchBackward(startSearchPos, data, &foundPos, &foundPosEnd, caseSensitive);
		if(		false == findSomething
			&&	true == wrap)
		{
			//APPL_INFO("WrapMode !!! end ==> 0");
			findSomething = m_EdnBuf.SearchBackward(m_EdnBuf.Size(), data, &foundPos, &foundPosEnd, caseSensitive);
		}
		// if find data : 
		if (true == findSomething) {
			// select new position
			SetInsertPosition(foundPos);
			m_EdnBuf.Select(foundPos, foundPosEnd);
		}
	}
	m_centerRequested = true;
	RequestUpdateOfThePosition();
}


void BufferText::Replace(etk::UString &data)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	if (true == haveSelectionActive) {
		// Replace Data : 
		int32_t size = m_EdnBuf.ReplaceSelected(data);
		SetInsertPosition(SelectionStart + size);
	}
	SetModify(true);
}


/**
 * @brief request a copy of the selection in the named clipBoard ID
 *
 * @param[in] clipboardID		Id of the buffer we want to get data [0..10] (0 copy normal / 10 middle button)
 *
 * @return ---
 *
 */
void BufferText::Copy(ewol::clipBoard::clipboardListe_te clipboardID)
{
	etk::UString mVect;
	// get the curent selected data
	if (true == m_EdnBuf.SelectHasSelection() ) {
		m_EdnBuf.GetSelectionText(mVect);
	}
	// copy data in the click board : 
	ewol::clipBoard::Set(clipboardID, mVect);
}


/**
 * @brief Request a copy and a remove of the curent selection in the named clipBoard ID
 *
 * @param[in] clipboardID		Id of the buffer we want to get data [0..10] (0 copy normal / 10 middle button)
 *
 * @return ---
 *
 */
void BufferText::Cut(ewol::clipBoard::clipboardListe_te clipboardID)
{

	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	// copy data
	Copy(clipboardID);
	// remove data : 
	if (true == haveSelectionActive ) {
		APPL_INFO("REMOVE SELECTION");
		m_EdnBuf.RemoveSelected();
		m_cursorPos = SelectionStart;
	}
	RequestUpdateOfThePosition();
	SetModify(true);
}


/**
 * @brief request the past of a specific clipboard on the curent position or selection
 *
 * @param[in] clipboardID		Id of the buffer we want to get data [0..10] (0 copy normal / 10 middle button)
 *
 * @return ---
 *
 */
void BufferText::Paste(ewol::clipBoard::clipboardListe_te clipboardID)
{
	etk::UString mVect;
	// copy data from the click board :
	mVect = ewol::clipBoard::Get(clipboardID);
	
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	if (true == haveSelectionActive ) {
		// replace data
		int32_t size = m_EdnBuf.ReplaceSelected(mVect );
		m_cursorPos = SelectionStart + size;
	} else {
		// insert data
		int32_t size = m_EdnBuf.Insert(m_cursorPos, mVect);
		m_cursorPos += size;
	}
	RequestUpdateOfThePosition();
	SetModify(true);
}


void BufferText::Undo(void)
{
	int32_t newPos = m_EdnBuf.Undo();
	if (newPos >= 0) {
		SetInsertPosition(newPos, true);
		RequestUpdateOfThePosition();
		SetModify(true);
	}
}

void BufferText::Redo(void)
{
	int32_t newPos = m_EdnBuf.Redo();
	if (newPos >= 0) {
		SetInsertPosition(newPos, true);
		RequestUpdateOfThePosition();
		SetModify(true);
	}
}


void BufferText::SetCharset(unicode::charset_te newCharset)
{
	m_EdnBuf.SetCharsetType(newCharset);
}

