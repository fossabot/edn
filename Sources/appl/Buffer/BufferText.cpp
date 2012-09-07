/**
 *******************************************************************************
 * @file BufferText.cpp
 * @brief Editeur De N'ours : Text Buffer (edit only ASCII text File) (header)
 * @author Edouard DUPIN
 * @date 19/01/2011
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
 
#include <appl/Debug.h>
#include <appl/global.h>
#include <BufferText.h>
#include <etk/RegExp.h>
#include <etk/unicode.h>

#include <ewol/ewol.h>
#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/widget/Widget.h>
#include <ewol/font/Font.h>
#include <ewol/ClipBoard.h>


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
	m_displaySize.x = 200;
	m_displaySize.y = 20;
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
BufferText::BufferText()
{
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
BufferText::BufferText(etk::File &fileName) : Buffer(fileName)
{
	BasicInit();
	NameChange();
	APPL_INFO("Add Data from file(" << GetFileName() << ")");
	FILE * myFile = NULL;
	// try to open the file. if not existed, new file
	
	myFile = fopen(fileName.GetCompleateName().c_str(), "r");
	if (NULL != myFile) {
		m_EdnBuf.DumpFrom(myFile);
		// close the input file
		fclose(myFile);
		SetModify(false);
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
	FILE * myFile = NULL;
	myFile = fopen(GetFileName().GetCompleateName().c_str(), "w");
	if (NULL != myFile) {
		m_EdnBuf.DumpIn(myFile);
		fclose(myFile);
		SetModify(false);
	} else {
		APPL_ERROR("Can not open in writing the specify file");
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

void BufferText::DrawLineNumber(ewol::TEXT_DISPLAY_TYPE* OOText, ewol::OObject2DColored* OOColored, int32_t sizeX, int32_t sizeY, int32_t nbColomn,  int32_t lineNumber, int32_t positionY)
{
	char tmpLineNumber[50];
	sprintf(tmpLineNumber, "%*d", nbColomn, lineNumber);
	OOColored->SetColor(ColorizeManager::Get(COLOR_LIST_BG_2));
	OOColored->Rectangle( 0, positionY, sizeX+0.5*SEPARATION_SIZE_LINE_NUMBER, sizeY);
	OOText->SetColor(ColorizeManager::Get(COLOR_CODE_LINE_NUMBER));
	
	Vector2D<float>  textPos;
	textPos.x = 1;
	textPos.y = positionY;
	etk::UString tmppp = tmpLineNumber;
	OOText->Text(textPos, tmppp);
}

#define CURSOR_WIDTH           (5)
#define CURSOR_THICKNESS       (1.2)
void BufferText::DrawCursor(ewol::OObject2DColored* OOColored, int32_t x, int32_t y, int32_t letterHeight, int32_t letterWidth, clipping_ts &clip)
{
	draw::Color & tmpppppp = ColorizeManager::Get(COLOR_CODE_CURSOR);
	OOColored->SetColor(tmpppppp);
	if (true == ewol::IsSetInsert()) {
		OOColored->Rectangle( x, y, letterWidth, letterHeight);
	} else {
		OOColored->Line( (int32_t)(x-CURSOR_WIDTH), (int32_t)(y)                              , (int32_t)(x+CURSOR_WIDTH), (int32_t)(y)                              , CURSOR_THICKNESS);
		OOColored->Line( (int32_t)(x-CURSOR_WIDTH), (int32_t)(y+letterHeight-CURSOR_THICKNESS), (int32_t)(x+CURSOR_WIDTH), (int32_t)(y+letterHeight-CURSOR_THICKNESS), CURSOR_THICKNESS);
		OOColored->Line( (int32_t)(x)             , (int32_t)(y)                              , (int32_t)(x)             , (int32_t)(y+letterHeight-CURSOR_THICKNESS), CURSOR_THICKNESS);
	}
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
#ifdef __VIDEO__OPENGL_ES_2
int32_t BufferText::Display(ewol::TEXT_DISPLAY_TYPE& OOText,
                            ewol::OObject2DColored& OOColored,
                            int32_t offsetX, int32_t offsetY,
                            int32_t sizeX, int32_t sizeY)
#else
int32_t BufferText::Display(ewol::TEXT_DISPLAY_TYPE& OOTextNormal,
                            ewol::TEXT_DISPLAY_TYPE& OOTextBold,
                            ewol::TEXT_DISPLAY_TYPE& OOTextItalic,
                            ewol::TEXT_DISPLAY_TYPE& OOTextBoldItalic,
                            ewol::OObject2DColored& OOColored,
                            int32_t offsetX, int32_t offsetY,
                            int32_t sizeX, int32_t sizeY)
#endif
{
	int32_t selStart, selEnd, selRectStart, selRectEnd;
	bool selIsRect;
	int32_t selHave;
	
	#ifdef __VIDEO__OPENGL_ES_2
		int32_t letterWidth = OOText.GetSize("A").x;
		int32_t letterHeight = OOText.GetHeight();
	#else
		int32_t letterWidth = OOTextNormal.GetSize("A").x;
		int32_t letterHeight = OOTextNormal.GetHeight();
	#endif
	int32_t displayStartLineId = offsetY / letterHeight - 1;
	displayStartLineId = etk_max(0, displayStartLineId);
	uint32_t y = - offsetY + displayStartLineId*letterHeight;
	
	// update the display position with the scroll ofset : 
	int32_t displayStartBufferPos = m_EdnBuf.CountForwardNLines(0, displayStartLineId);
	Vector2D<float>  maxSize;
	maxSize.x = 0.0;
	maxSize.y = m_EdnBuf.NumberOfLines() * letterHeight;
	int32_t nbColoneForLineNumber = GetLineNumberNumberOfElement();
	
	// update the number of element that can be displayed
	m_displaySize.x = (sizeX/letterWidth) + 1 - nbColoneForLineNumber;
	m_displaySize.y = (sizeY/letterHeight) + 1;
	APPL_VERBOSE("main DIPLAY " << m_displaySize.x << " char * " << m_displaySize.y << " char");
	
	selHave = m_EdnBuf.GetSelectionPos(selStart, selEnd, selIsRect, selRectStart, selRectEnd);
	
	colorInformation_ts * HLColor = NULL;
	
	int32_t iii, new_i;
	// Get color : 
	Colorize *  myColor           = ColorizeManager::Get("normal");
	Colorize *  myColorSel        = ColorizeManager::Get("SelectedText");
	draw::Color &  myColorSpace   = ColorizeManager::Get(COLOR_CODE_SPACE);
	draw::Color &  myColorTab     = ColorizeManager::Get(COLOR_CODE_TAB);
	Colorize *  selectColor       = NULL;
	#ifndef __VIDEO__OPENGL_ES_2
	ewol::TEXT_DISPLAY_TYPE* OOTextSelected = NULL;
	#endif
	int mylen = m_EdnBuf.Size();
	int32_t x_base=nbColoneForLineNumber*letterWidth;
	int32_t idX = 0;
	
	OOColored.SetColor(ColorizeManager::Get(COLOR_CODE_BASIC_BG));
	OOColored.Rectangle( 0, 0, sizeX, sizeY);
	
	int64_t startTime = ewol::GetTime();
	int displayLines = 0;
	// Regenerate the colorizing if necessary ...
	displayHLData_ts m_displayLocalSyntax;
	m_EdnBuf.HightlightGenerateLines(m_displayLocalSyntax, displayStartBufferPos, m_displaySize.y);
	
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
	
	OOColored.clippingDisable();
	#ifdef __VIDEO__OPENGL_ES_2
		OOText.clippingDisable();
		DrawLineNumber(&OOText,       &OOColored, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
	#else
		OOTextNormal.clippingDisable();
		DrawLineNumber(&OOTextNormal, &OOColored, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
	#endif
	int32_t pixelX = x_base + SEPARATION_SIZE_LINE_NUMBER;
	
	clipping_ts drawClipping;
	drawClipping.x = 0;
	drawClipping.y = 0;
	drawClipping.w = sizeX;
	drawClipping.h = sizeY;
	
	clipping_ts drawClippingTextArea;
	drawClippingTextArea.x = pixelX;
	drawClippingTextArea.y = 0;
	drawClippingTextArea.w = sizeX - drawClipping.x;
	drawClippingTextArea.h = sizeY;
	
	#ifdef __VIDEO__OPENGL_ES_2
		OOText.clippingSet(drawClippingTextArea);
	#else
		OOTextNormal.clippingSet(drawClippingTextArea);
		OOTextBold.clippingSet(drawClippingTextArea);
		OOTextItalic.clippingSet(drawClippingTextArea);
		OOTextBoldItalic.clippingSet(drawClippingTextArea);
	#endif
	OOColored.clippingSet(drawClippingTextArea);
	
	// Clear the line intexation :
	m_elmentList.Clear();
	// every char element is register to find the diplay pos when mouse event arrive
	CharElement tmpElementProperty;
	tmpElementProperty.m_yOffset = y;
	tmpElementProperty.m_xOffset = 0;
	tmpElementProperty.m_ySize = 10;
	tmpElementProperty.m_bufferPos = displayStartBufferPos;
	m_elmentList.PushBack(tmpElementProperty);
	
	float lineMaxSize = 0.0;
	for (iii=displayStartBufferPos; iii<mylen && displayLines >=0 ; iii = new_i) {
		//APPL_DEBUG("diplay element=" << iii);
		int displaywidth;
		uint32_t currentChar = '\0';
		new_i = iii;
		// update the element buffer pos:
		tmpElementProperty.m_bufferPos = new_i;
		displaywidth = m_EdnBuf.GetExpandedChar(new_i, idX, displayChar, currentChar);
		int32_t drawSize = 0;
		
		// update display position :
		Vector2D<float>  textPos;
		textPos.x = pixelX-offsetX;
		textPos.y = y;
		// update X pos
		tmpElementProperty.m_xOffset = textPos.x;
		tmpElementProperty.m_yOffset = textPos.y;
		
		//APPL_INFO("diplay element=" << new_i);
		if (currentChar!='\n') {
			selectColor = myColor;
			HLColor = m_EdnBuf.GetElementColorAtPosition(m_displayLocalSyntax, iii);
			if (NULL != HLColor) {
				if (NULL != HLColor->patern) {
					selectColor = HLColor->patern->GetColor();
				}
			}
			bool haveBg = false;
			if(	true == selHave
				&&	selStart <= iii
				&&	selEnd   > iii)
			{
				selectColor = myColorSel;
				OOColored.SetColor(selectColor->GetBG());
				haveBg = selectColor->HaveBg();
			} else {
				if(		' ' == currentChar
				&&	true == globals::IsSetDisplaySpaceChar() )
				{
					OOColored.SetColor(myColorSpace);
					haveBg = true;
				} else if(		'\t' == currentChar
							&&	true == globals::IsSetDisplaySpaceChar() )
				{
					OOColored.SetColor(myColorTab);
					haveBg = true;
				} else {
					OOColored.SetColor(selectColor->GetBG());
					haveBg = selectColor->HaveBg();
				}
			}
			#ifdef __VIDEO__OPENGL_ES_2
				tmpElementProperty.m_ySize = OOText.GetHeight();
				OOText.SetColor(selectColor->GetFG());
				OOText.SetBold(selectColor->GetBold());
				OOText.SetItalic(selectColor->GetItalic());
				myStringToDisplay = displayChar;
				drawSize = OOText.Text(textPos, myStringToDisplay);
			#else
				if (true == selectColor->GetItalic() ) {
					if (true == selectColor->GetBold() ) {
						OOTextSelected = &OOTextBoldItalic;
					} else {
						OOTextSelected = &OOTextItalic;
					}
				} else {
					if (true == selectColor->GetBold() ) {
						OOTextSelected = &OOTextBold;
					} else {
						OOTextSelected = &OOTextNormal;
					}
				}
				tmpElementProperty.m_ySize = OOTextSelected->GetHeight();
				//tmpElementProperty.m_yOffset += tmpElementProperty.m_ySize;
				OOTextSelected->SetColor(selectColor->GetFG());
				// TODO : Remove this unreallistic leak of time
				myStringToDisplay = displayChar;
				drawSize = OOTextSelected->Text(textPos, myStringToDisplay);
			#endif
			//APPL_DEBUG("add element : " << tmpElementProperty.m_yOffset << "," << tmpElementProperty.m_xOffset);
			m_elmentList.PushBack(tmpElementProperty);
			
			if (true == haveBg ) {
				OOColored.Rectangle(textPos.x, y, drawSize, letterHeight);
			}
		}
		idX += displaywidth;
		// display cursor : 
		if (m_cursorPos == iii) {
			// display the cursor:
			DrawCursor(&OOColored, pixelX - offsetX, y, letterHeight, letterWidth, drawClippingTextArea);
		}
		lineMaxSize += drawSize;
		pixelX += drawSize;
		// move to next line ...
		if (currentChar=='\n') {
			maxSize.x = etk_max(lineMaxSize, maxSize.x);
			lineMaxSize = 0.0;
			idX =0;
			pixelX = x_base + SEPARATION_SIZE_LINE_NUMBER;
			y -= letterHeight;
			displayLines++;
			currentLineID++;
			OOColored.clippingDisable();
			#ifdef __VIDEO__OPENGL_ES_2
				OOText.clippingDisable();
				OOText.SetBold(false);
				OOText.SetItalic(false);
				DrawLineNumber(&OOText, &OOColored, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
				OOText.clippingEnable();
			#else
				OOTextNormal.clippingDisable();
				DrawLineNumber(&OOTextNormal, &OOColored, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
				OOTextNormal.clippingEnable();
			#endif
			OOColored.clippingEnable();
			// add elements : 
			m_elmentList.PushBack(tmpElementProperty);
		}
	}
	// special case : the cursor is at the end of the buffer...
	if (m_cursorPos == iii) {
		DrawCursor(&OOColored, pixelX - offsetX, y, letterHeight, letterWidth, drawClippingTextArea);
	}
	// set the maximum size for the display ...
	SetMaximumSize(maxSize);
	int64_t stopTime2 = ewol::GetTime();
	APPL_DEBUG("DRAW text (brut) = " << stopTime2 - stopTime << " micro-s");

	return ERR_NONE;
}



int32_t BufferText::GetMousePosition(Vector2D<float> pos)
{
	bool inLineDone=false;
	//APPL_DEBUG("try to find in : " << width << "," << height);
	for(int32_t iii=0; iii<m_elmentList.Size()-1; iii++) {
		//APPL_DEBUG("check element : " << m_elmentList[iii].m_yOffset << "<= " << pos.y << " <" << (m_elmentList[iii].m_yOffset + m_elmentList[iii].m_ySize));
		if(false == inLineDone) {
			if(    pos.y>=m_elmentList[iii].m_yOffset
			    && pos.y<m_elmentList[iii].m_yOffset + m_elmentList[iii].m_ySize ) {
				// we find the line (int theory) ==> note : Some problem can appear here when the size are not the same ...
				// this is to prevent multiple size font ...
				inLineDone = true;
				//APPL_DEBUG("        ==> " << m_elmentList[iii+1].m_xOffset << "> " << pos.x << " >=" << m_elmentList[iii].m_xOffset);
			}
		}
		// we detected the line
		if(true == inLineDone) {
			if(    pos.x>=m_elmentList[iii].m_xOffset
			    && pos.x<m_elmentList[iii+1].m_xOffset ) {
				// we get the position ...
				return m_elmentList[iii].m_bufferPos;
			} else if (m_elmentList[iii].m_xOffset>=m_elmentList[iii+1].m_xOffset) {
				// prevent "end of line" cursor pos ...
				return m_elmentList[iii].m_bufferPos;
			}
		}
	}
	if (m_elmentList.Size()>0) {
		if(pos.y<m_elmentList[m_elmentList.Size()/2].m_yOffset) {
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
void BufferText::MouseEvent(Vector2D<float> pos)
{
	if (true == ewol::IsSetShift() ) {
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
void BufferText::MouseSelectFromCursorTo(Vector2D<float> pos)
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
	Copy(ewol::clipBoard::CLIPBOARD_SELECTION);
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
	Copy(ewol::clipBoard::CLIPBOARD_SELECTION);
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
	Copy(ewol::clipBoard::CLIPBOARD_SELECTION);
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
	Copy(ewol::clipBoard::CLIPBOARD_SELECTION);
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
	    && true  == ewol::IsSetShift() )
	{
		// new selection
		m_EdnBuf.Select(rememberCursorPos, m_cursorPos);
	} else if(    true == ewol::IsSetShift()
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
void BufferText::cursorMove(ewol::eventKbMoveType_te moveTypeEvent)
{
	bool needUpdatePosition = true;
	// check selection event ...
	switch(moveTypeEvent) {
		case ewol::EVENT_KB_MOVE_TYPE_LEFT:
			//APPL_INFO("keyEvent : <LEFT>");
			if (m_cursorPos > 0) {
				SetInsertPosition(m_cursorPos - 1); 
			}
			break;
		case ewol::EVENT_KB_MOVE_TYPE_RIGHT:
			//APPL_INFO("keyEvent : <RIGHT>");
			if (m_cursorPos < m_EdnBuf.Size() ) {
				SetInsertPosition(m_cursorPos + 1);
			}
			break;
		case ewol::EVENT_KB_MOVE_TYPE_UP:
			//APPL_INFO("keyEvent : <UP>");
			TextDMoveUp(1);
			break;
		case ewol::EVENT_KB_MOVE_TYPE_DOWN:
			//APPL_INFO("keyEvent : <DOWN>");
			// check if we have enought line ...
			TextDMoveDown(1);
			break;
		case ewol::EVENT_KB_MOVE_TYPE_PAGE_UP:
			//APPL_INFO("keyEvent : <PAGE-UP>");
			TextDMoveUp(m_displaySize.y);
			break;
		case ewol::EVENT_KB_MOVE_TYPE_PAGE_DOWN:
			//APPL_INFO("keyEvent : <PAGE-DOWN>");
			TextDMoveDown(m_displaySize.y);
			break;
		case ewol::EVENT_KB_MOVE_TYPE_START:
			//APPL_INFO("keyEvent : <Start of line>");
			SetInsertPosition(m_EdnBuf.StartOfLine(m_cursorPos) );
			break;
		case ewol::EVENT_KB_MOVE_TYPE_END:
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
Vector2D<float>  BufferText::GetPosition(int32_t fontId, bool& centerRequested)
{
	centerRequested = m_centerRequested;
	m_centerRequested = false;
	Vector2D<float>  outputPosition;

	// Display position (Y mode):
	APPL_INFO("change the position : " << m_cursorPos);
	// get the line id of the curent position of the cursor :
	outputPosition.y = m_EdnBuf.CountLines(0, m_cursorPos);
	// get the first position of the current line
	int32_t lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
	// count the number of char in the line (real displayed char with whar like <kjkj>)
	outputPosition.x = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
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
	outputPosition.x *= letterWidth;
	outputPosition.y *= letterHeight;
	return outputPosition;

	/* if we request a center : 
	} else {
		// center the line at the middle of the screen :
		Vector2D<float>  outputPosition;
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
				if (true == ewol::IsSetShift() ) {
					m_cursorPos = m_EdnBuf.UnIndent();
				} else {
					m_cursorPos = m_EdnBuf.Indent();
				}
			}
		}
	} else if (unicodeData == '\n') {
		etk::Vector<int8_t> tmpVect;
		if (true == ewol::IsSetShift()) {
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
			unicode::convertUnicodeToUtf8(unicodeData, tmpUTF8);
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

