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
 
#include "tools_debug.h"
#include "tools_globals.h"
#include "ClipBoard.h"
#include "BufferText.h"
#include "toolsMemory.h"
#include "Edn.h"
#include "RegExp.h"

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
	// set the number of the lineNumber;
	m_nbColoneForLineNumber = 1;
	// init the link with the buffer manager
	myColorManager = ColorizeManager::getInstance();
	//EDN_INFO("Init");
	// new mode : 
	m_cursorPos = 0;
	m_cursorPosPrevious = m_cursorPos;
	m_cursorPreferredCol = -1;
	m_cursorOn = true;
	// set at the sustem buffer internal
	m_lineWidth = Display::GetFontWidth();
	m_lineHeight = Display::GetFontHeight();
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
	//EDN_DEBUG("check name change");
	if (true == HighlightManager::getInstance()->Exist(m_fileName)) {
		Highlight * myHL = HighlightManager::getInstance()->Get(m_fileName);
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
	EDN_INFO("New(Empty-Buffer)");
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
BufferText::BufferText(Edn::File &fileName) : Buffer(fileName)
{
	BasicInit();
	NameChange();
	EDN_INFO("Add Data from file(" << GetFileName() << ")");
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
		EDN_WARNING("No File ==> created a new one(" << GetFileName() << ")");
		SetModify(true);
	}
	UpdateWindowsPosition();
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
	EDN_INFO("Save File : \"" <<  GetFileName() << "\"" );
	FILE * myFile = NULL;
	myFile = fopen(GetFileName().GetCompleateName().c_str(), "w");
	if (NULL != myFile) {
		m_EdnBuf.DumpIn(myFile);
		fclose(myFile);
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
	EDN_INFO("Delete(BufferText)");
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
	EDN_WARNING("TODO ...");
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
	EDN_WARNING("TODO ...");
}




void BufferText::DrawLineNumber(DrawerManager &drawer, int32_t lineNumber, int32_t positionY)
{
	int32_t letterHeight = Display::GetFontHeight();
	char tmpLineNumber[50];
	sprintf(tmpLineNumber, g_pointerForTheDisplayLine[m_nbColoneForLineNumber-1], lineNumber);
	drawer.Text(myColorManager->Get(COLOR_CODE_LINE_NUMBER), 1, positionY, tmpLineNumber);
}

/**
 * @brief Update internal data of the pointer to display
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::UpdatePointerNumber(void)
{
	// get the number of line in the buffer
	int32_t maxNumberLine = m_EdnBuf.NumberOfLines();
	//int32_t maxNumberLine = 2096;
	if (10 > maxNumberLine) {				m_nbColoneForLineNumber = 1;
	} else if (100 > maxNumberLine) {		m_nbColoneForLineNumber = 2;
	} else if (1000 > maxNumberLine) {		m_nbColoneForLineNumber = 3;
	} else if (10000 > maxNumberLine) {		m_nbColoneForLineNumber = 4;
	} else if (100000 > maxNumberLine) {	m_nbColoneForLineNumber = 5;
	} else if (1000000 > maxNumberLine) {	m_nbColoneForLineNumber = 6;
	} else if (1000000 > maxNumberLine) {	m_nbColoneForLineNumber = 7;
	} else if (10000000 > maxNumberLine) {	m_nbColoneForLineNumber = 8;
	} else if (100000000 > maxNumberLine) {	m_nbColoneForLineNumber = 9;
	} else {								m_nbColoneForLineNumber = 10;
	}
}


void BufferText::DrawLine(DrawerManager &drawer, bufferAnchor_ts &anchor)
{

	int32_t letterHeight = Display::GetFontHeight();
	int32_t letterWidth = Display::GetFontWidth();
	int32_t positionY = letterHeight * (anchor.m_lineNumber - anchor.m_displayStart.y - 1);

	int32_t idX = 0;
	int32_t pixelX = m_nbColoneForLineNumber*letterWidth + 3;

	Colorize * myColorNormal     = myColorManager->Get("normal");
	Colorize * myColorSelected   = myColorManager->Get("SelectedText");
	Colorize * selectColor = NULL;
	colorInformation_ts * HLColor = NULL;
	
	// Regenerate the colorizing if necessary ...
	displayHLData_ts  myDisplayLocalSyntax;
	m_EdnBuf.HightlightOneLine(myDisplayLocalSyntax, anchor.m_posStart, anchor.m_posStop);
	
	// clean the current Line
	drawer.Rectangle(myColorManager->Get(COLOR_CODE_BASIC_BG), 0, positionY, drawer.GetWidth(), letterHeight);
	
	DrawLineNumber(drawer, anchor.m_lineNumber, positionY);
	
	bool selHave = anchor.m_selectionPosStart == -1 ? false : true;
	char displayChar[MAX_EXP_CHAR_LEN];
	memset(displayChar, 0, sizeof(char)*MAX_EXP_CHAR_LEN);
	int32_t iii;
	for (iii=anchor.m_posStart; iii<anchor.m_posStop; ) {
		uint32_t currentChar;
		int32_t savePositionForCursor = iii;
		int32_t  displaywidth = m_EdnBuf.GetExpandedChar(iii, idX, displayChar, currentChar);
		selectColor = myColorNormal;
		//kwow size to display
		int32_t widthToDisplay;
		char * tmpDisplayOfset;
		bool inTheScreen = true;
		if (anchor.m_displayStart.x <= idX) {
			// Normal display
			tmpDisplayOfset = displayChar;
			widthToDisplay = displaywidth;
		} else if (anchor.m_displayStart.x < idX + displaywidth) {
			// special case of partial display : 
			widthToDisplay = idX + displaywidth - anchor.m_displayStart.x;
			tmpDisplayOfset = displayChar + (displaywidth-widthToDisplay);
		} else {
			// Out of range ...
			widthToDisplay = displaywidth;
			tmpDisplayOfset = displayChar;
			inTheScreen = false;
		}
		if (true==inTheScreen) {
			HLColor = m_EdnBuf.GetElementColorAtPosition(myDisplayLocalSyntax, savePositionForCursor);
			if (NULL != HLColor) {
				if (NULL != HLColor->patern) {
					selectColor = HLColor->patern->GetColor();
				}
			}
			// If user want to display space char : overwrite curent color
			if(		' ' == currentChar
				&&	true == globals::IsSetDisplaySpaceChar() )
			{
				//selectColor = myColorSelected;
				//SpaceText(color_ts & SelectColor, int32_t x, int32_t y,int32_t nbChar)
				if(	true == selHave
					&&	anchor.m_selectionPosStart <= iii
					&&	anchor.m_selectionPosStop   > iii)
				{
					drawer.SpaceText(myColorSelected->GetBG(), pixelX ,positionY , 1);
				} else if (true == selectColor->HaveBg()) {
					drawer.SpaceText(selectColor->GetBG(), pixelX ,positionY , 1);
				} else {
					drawer.SpaceText(myColorManager->Get(COLOR_CODE_SPACE), pixelX ,positionY , 1);
				}
			} else if(		'\t' == currentChar
						&&	true == globals::IsSetDisplaySpaceChar() )
			{
				if(	true == selHave
					&&	anchor.m_selectionPosStart <= iii
					&&	anchor.m_selectionPosStop   > iii)
				{
					drawer.SpaceText(myColorSelected->GetBG(), pixelX ,positionY , strlen(tmpDisplayOfset));
				} else if (true == selectColor->HaveBg()) {
					drawer.SpaceText(selectColor->GetBG(), pixelX ,positionY , strlen(tmpDisplayOfset));
				} else {
					drawer.SpaceText(myColorManager->Get(COLOR_CODE_TAB), pixelX ,positionY , strlen(tmpDisplayOfset));
				}
			} else {
				if(	true == selHave
					&&	anchor.m_selectionPosStart <= iii
					&&	anchor.m_selectionPosStop   > iii)
				{
					selectColor = myColorSelected;
				}
				if (currentChar <= 0x7F) {
					drawer.Text(selectColor, pixelX ,positionY, tmpDisplayOfset);
				} else {
					drawer.Text(selectColor, pixelX ,positionY, displayChar);
				}
			}
		}
		// display cursor : 
		if (m_cursorPos == savePositionForCursor) {
			// display the cursor:
			if (true == m_cursorOn) {
				drawer.Cursor(pixelX, positionY+letterHeight);
				//m_cursorOn = false;
			} else {
				m_cursorOn = true;
			}
		}
		if (true==inTheScreen) {
			pixelX += widthToDisplay*letterWidth;
		}
		idX += displaywidth;
	}
	// special case : the cursor is at the end of the buffer...
	if (m_cursorPos == iii) {
		// display the cursor:
		if (true == m_cursorOn) {
			drawer.Cursor(pixelX, positionY+letterHeight);
			//m_cursorOn = false;
		} else {
			m_cursorOn = true;
		}
	}
}


void BufferText::GetMousePosition(int32_t width, int32_t height, int32_t &x, int32_t &y)
{
	x = (width - 3) / Display::GetFontWidth() - m_nbColoneForLineNumber;
	y = height / Display::GetFontHeight();
	if (x < 0) {
		x = 0;
	}
	x += m_AnchorList[AnchorCurrentId()].m_displayStart.x;
	y += m_AnchorList[AnchorCurrentId()].m_displayStart.y;
	//EDN_DEBUG("BufferText::GetMousePosition(" << width << "," << height << "); ==> (" << x << "," << y << ")" );
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
void BufferText::MouseEvent(int32_t width, int32_t height)
{
	if (true == globals::IsSetShift() ) {
		MouseSelectFromCursorTo(width, height);
	} else {
		int32_t posX, posY;
		// Get the caracter mouse position
		GetMousePosition(width, height, posX, posY);
		// find the current selected line : 
		int32_t newPos = m_EdnBuf.CountForwardNLines(0, posY);
		// Get the display char position
		newPos = m_EdnBuf.CountForwardDispChars(newPos, posX);
		// move the cursor
		SetInsertPosition(newPos);

		// if a preferred column wasn't aleady established, establish it
		if (m_cursorPreferredCol < 0) {
			m_cursorPreferredCol = posX;
		}
		m_EdnBuf.Unselect(SELECTION_PRIMARY);

		UpdateWindowsPosition();
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
// TODO : Simplify selection ....
void BufferText::MouseSelectFromCursorTo(int32_t width, int32_t height)
{
	int32_t posX, posY;
	// Get the caracter mouse position
	GetMousePosition(width, height, posX, posY);
	
	int32_t selStart, selEnd, selRectStart, selRectEnd;
	bool selIsRect;
	int32_t selHave = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, selStart, selEnd, selIsRect, selRectStart, selRectEnd);
	//EDN_DEBUG("BufferText:: " << selHave << " = BufGetSelectionPos(SELECTION_PRIMARY," << selStart << "," << selEnd << "," << selIsRect << "," << selRectStart << "," << selRectEnd << ");" );
	
	int32_t rememberCursorPos = m_cursorPos;
	
	// find the current selected line : 
	int32_t newPos = m_EdnBuf.CountForwardNLines(0, posY);
	// Get the display char position
	newPos = m_EdnBuf.CountForwardDispChars(newPos, posX);
	// move the cursor
	SetInsertPosition(newPos);
	
	// if a preferred column wasn't aleady established, establish it
	if (m_cursorPreferredCol < 0) {
		m_cursorPreferredCol = posX;
	}
	
	if (false == selHave) {
		m_EdnBuf.Select(SELECTION_PRIMARY, rememberCursorPos, m_cursorPos);
	} else {
		if (rememberCursorPos == selStart) {
			m_EdnBuf.Select(SELECTION_PRIMARY, m_cursorPos, selEnd);
		} else {
			m_EdnBuf.Select(SELECTION_PRIMARY, selStart, m_cursorPos);
		}
	}
	UpdateWindowsPosition();
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
		m_EdnBuf.Select(SELECTION_PRIMARY, beginPos, endPos);
		m_cursorPosPrevious = m_cursorPos;
		m_cursorPos = endPos;
	}
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
	m_EdnBuf.Select(SELECTION_PRIMARY, m_EdnBuf.StartOfLine(m_cursorPos), m_EdnBuf.EndOfLine(m_cursorPos));
	m_cursorPosPrevious = m_cursorPos;
	m_cursorPos = m_EdnBuf.EndOfLine(m_cursorPos);
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
	m_EdnBuf.Select(SELECTION_PRIMARY, 0, m_EdnBuf.Size());
	m_cursorPosPrevious = m_cursorPos;
	m_cursorPos = m_EdnBuf.Size();
	AnchorForceRedrawAll();
}

void BufferText::SelectNone(void)
{
	m_EdnBuf.Unselect(SELECTION_PRIMARY);
	AnchorForceRedrawAll();
}

#define SCROLL_NB_LINE (3)

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::ScrollDown(void)
{
	MoveUpDown(SCROLL_NB_LINE);
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::ScrollUp(void)
{
	MoveUpDown(-1 * SCROLL_NB_LINE);
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::MoveUpDown(int32_t ofset)
{
	int32_t iii=AnchorCurrentId();
	if (ofset >= 0) {
		int32_t nbLine = m_EdnBuf.NumberOfLines();
		if (m_AnchorList[iii].m_displayStart.y+ofset+3 > nbLine) {
			m_AnchorList[iii].m_displayStart.y = nbLine-3;
			AnchorForceRedrawOffsef(m_AnchorList[iii].m_displayStart.y-(nbLine-3));
		} else {
			AnchorForceRedrawOffsef(ofset);
			m_AnchorList[iii].m_displayStart.y += ofset;
		}
		m_AnchorList[iii].m_bufferPos = m_EdnBuf.CountForwardNLines(0, m_AnchorList[iii].m_displayStart.y);
		m_AnchorList[iii].m_lineId = m_AnchorList[iii].m_displayStart.y;
	} else {
		ofset *= -1;
		if (m_AnchorList[iii].m_displayStart.y < ofset) {
			AnchorForceRedrawOffsef(-1 * m_AnchorList[iii].m_displayStart.y);
			m_AnchorList[iii].m_displayStart.y = 0;
			m_AnchorList[iii].m_bufferPos = 0;
			m_AnchorList[iii].m_lineId = 0;
		} else {
			AnchorForceRedrawOffsef(-1 * ofset);
			m_AnchorList[iii].m_displayStart.y -= ofset;
			m_AnchorList[iii].m_bufferPos = m_EdnBuf.CountForwardNLines(0, m_AnchorList[iii].m_displayStart.y);
			m_AnchorList[iii].m_lineId = m_AnchorList[iii].m_displayStart.y;
		}
	}
}


void BufferText::SetInsertPosition(int32_t newPos, bool insertChar)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	int32_t rememberCursorPos = m_cursorPos;

	//EDN_DEBUG("newPos=" << newPos);
	// unselect buffer:
	m_EdnBuf.Unselect(SELECTION_PRIMARY);
	/* make sure new position is ok, do nothing if it hasn't changed */
	if (newPos != m_cursorPos) {
		if (newPos < 0){
			newPos = 0;
		}
		if (newPos > m_EdnBuf.Size()) {
			newPos = m_EdnBuf.Size();
		}
		m_cursorPosPrevious = m_cursorPos;
		m_cursorPos = newPos;
	}
	m_cursorPreferredCol = -1;
	// Force the cursor display
	m_cursorOn = true;
	
	// special case when insert char ...
	if (true == insertChar) {
		return;
	}
	
	if(		false == haveSelectionActive
		&&	true  == globals::IsSetShift() )
	{
		// new selection
		m_EdnBuf.Select(SELECTION_PRIMARY, rememberCursorPos, m_cursorPos);
	} else if(		true == globals::IsSetShift()
				&&	true == haveSelectionActive)
	{
		// update selection
		if (rememberCursorPos == SelectionStart) {
			m_EdnBuf.Select(SELECTION_PRIMARY, m_cursorPos, SelectionEnd);
		} else {
			m_EdnBuf.Select(SELECTION_PRIMARY, SelectionStart, m_cursorPos);
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
	//EDN_INFO("Move Line UP result : prevLineStartPos=" << prevLineStartPos);
	// Get the display char position
	newPos = m_EdnBuf.CountForwardDispChars(prevLineStartPos, column);
	//EDN_INFO("Move to colomn : column=" << column << " newPos=" << newPos);

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
	//EDN_INFO("Move Line DOWN result : nextLineStartPos=" << nextLineStartPos);
	// Get the display char position
	newPos = m_EdnBuf.CountForwardDispChars(nextLineStartPos, column);
	//EDN_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	
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
void BufferText::cursorMove(int32_t gtkKey)
{
	int32_t tmplineID;
	bool needUpdatePosition = true;
	switch(gtkKey) {
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Left:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Left:
#		endif
			//EDN_INFO("keyEvent : <LEFT>");
			if (m_cursorPos > 0) {
				SetInsertPosition(m_cursorPos - 1); 
			}
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Right:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Right:
#		endif
			//EDN_INFO("keyEvent : <RIGHT>");
			if (m_cursorPos < m_EdnBuf.Size() ) {
				SetInsertPosition(m_cursorPos + 1);
			}
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Up:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Up:
#		endif
			//EDN_INFO("keyEvent : <UP>");
			TextDMoveUp(1);
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Down:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Down:
#		endif
			//EDN_INFO("keyEvent : <DOWN>");
			// check if we have enought line ...
			TextDMoveDown(1);
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Page_Up:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Page_Up:
#		endif
			//EDN_INFO("keyEvent : <PAGE-UP>");
			TextDMoveUp(m_AnchorList[AnchorCurrentId()].m_displaySize.y);
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Page_Down:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Page_Down:
#		endif
			//EDN_INFO("keyEvent : <PAGE-DOWN>");
			TextDMoveDown(m_AnchorList[AnchorCurrentId()].m_displaySize.y);
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Begin:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Begin:
#		endif
			//EDN_INFO("keyEvent : <Start of line>");
			SetInsertPosition(m_EdnBuf.StartOfLine(m_cursorPos) );
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_End:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_End:
#		endif
			//EDN_INFO("keyEvent : <End of line>");
			SetInsertPosition(m_EdnBuf.EndOfLine(m_cursorPos) );
			break;
		default:
			//LastUpDownCursorPosition = -1;
			needUpdatePosition = false;
			break;
	}
	if ( true == needUpdatePosition) {
		UpdateWindowsPosition();
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
void BufferText::UpdateWindowsPosition(bool centerPage)
{
	int32_t linePreviousID = m_EdnBuf.CountLines(0, m_cursorPosPrevious);
	AnchorForceRedrawLine(linePreviousID);
	
	if (centerPage == false) {
		// Display position (Y mode):
		int32_t lineStartPos;
		// Get current position of cursor :
		position_ts cursorPosition;
		cursorPosition.y = m_EdnBuf.CountLines(0, m_cursorPos);
		AnchorForceRedrawLine(cursorPosition.y);
		lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
		cursorPosition.x = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
		//EDN_INFO(" curent cursor position : (" << cursorPosition.x << "," << cursorPosition.y << ")");
		
		// Done for all Anchor elements ...
		//for (int32_t iii=0; iii < m_AnchorList.Size() ; iii++) {
		int32_t iii = AnchorCurrentId();
		if (iii >=0) {
			position_ts displayPreviousStart = m_AnchorList[iii].m_displayStart;
			//EDN_INFO(" m_displayStart(" << m_AnchorList[iii].m_displayStart.x << "," << m_AnchorList[iii].m_displayStart.y << ") m_displaySize(" << m_AnchorList[iii].m_displaySize.x << "," << m_AnchorList[iii].m_displaySize.y << ")");
			if (m_AnchorList[iii].m_displayStart.y > (int32_t)cursorPosition.y - globals::getNbLineBorder() ) {
				m_AnchorList[iii].m_displayStart.y = cursorPosition.y - globals::getNbLineBorder();
				if (m_AnchorList[iii].m_displayStart.y < 0) {
					m_AnchorList[iii].m_displayStart.y = 0;
				}
			} else if (m_AnchorList[iii].m_displayStart.y + m_AnchorList[iii].m_displaySize.y <= (int32_t)cursorPosition.y + globals::getNbLineBorder() ) {
				m_AnchorList[iii].m_displayStart.y = cursorPosition.y - m_AnchorList[iii].m_displaySize.y + globals::getNbLineBorder() + 1;
			}
			// Display position (X mode):
			//EDN_INFO("cursorPosition X : " << cursorPosition.y << " windows " << m_displayStart.y << "=>" << m_displayStart.x + m_displaySize.x);
			if (m_AnchorList[iii].m_displayStart.x > cursorPosition.x - globals::getNbColoneBorder() ) {
				m_AnchorList[iii].m_displayStart.x = cursorPosition.x - globals::getNbColoneBorder();
				if (m_AnchorList[iii].m_displayStart.x < 0) {
					m_AnchorList[iii].m_displayStart.x = 0;
				}
			} else if (m_AnchorList[iii].m_displayStart.x + m_AnchorList[iii].m_displaySize.x <= cursorPosition.x + globals::getNbColoneBorder() ) {
				m_AnchorList[iii].m_displayStart.x = cursorPosition.x - m_AnchorList[iii].m_displaySize.x + globals::getNbColoneBorder() + 1;
			}
			
			//update the buffer position ID : 
			m_AnchorList[iii].m_bufferPos = m_EdnBuf.CountForwardNLines(0, m_AnchorList[iii].m_displayStart.y);
			m_AnchorList[iii].m_lineId = m_AnchorList[iii].m_displayStart.y;
			if (m_AnchorList[iii].m_displayStart.x != displayPreviousStart.x) {
				AnchorForceRedrawAll(iii);
			} else {
				if (m_AnchorList[iii].m_displayStart.y != displayPreviousStart.y) {
					EDN_WARNING("SELECT an ofset : displayPreviousStart.y=" << displayPreviousStart.y << "  m_AnchorList[iii].m_displayStart.y=" << m_AnchorList[iii].m_displayStart.y << " ==>" << m_AnchorList[iii].m_displayStart.y - displayPreviousStart.y);
					EDN_WARNING("SELECT ... offset = " << m_AnchorList[iii].m_BufferNumberLineOffset);
					AnchorForceRedrawOffsef(m_AnchorList[iii].m_displayStart.y - displayPreviousStart.y);
					EDN_WARNING("SELECT ... offset = " << m_AnchorList[iii].m_BufferNumberLineOffset);
				}
				int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
				bool SelectionIsRect;
				bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
				if (true == haveSelectionActive) {
					int32_t start = edn_min(linePreviousID, cursorPosition.y);
					int32_t stop = edn_max(linePreviousID, cursorPosition.y);
					EDN_WARNING("SELECT force redraw range of lines : (" << start << "," << stop << ")");
					for (int32_t jjj=start; jjj <= stop; jjj++) {
						AnchorForceRedrawLine(jjj);
					}
				}
			}
		}
	} else {
		// center the line at the middle of the screen :
		position_ts cursorPosition;
		//EDN_DEBUG(" -------------------------------------------------");
		cursorPosition.y = m_EdnBuf.CountLines(0, m_cursorPos);
		//EDN_DEBUG(" cursor position : " << m_cursorPos << " ==> ligne=" << cursorPosition.y);
		cursorPosition.x = 0;
		// Done for all Anchor elements ...
		//for (int32_t iii=0; iii < m_AnchorList.Size() ; iii++) {
		int32_t iii = AnchorCurrentId();
		if (iii >=0) {
			m_AnchorList[iii].m_displayStart.x = 0;
			//EDN_DEBUG(" display size : " << m_displaySize.y);
			m_AnchorList[iii].m_displayStart.y = cursorPosition.y - m_AnchorList[iii].m_displaySize.y/2;
			m_AnchorList[iii].m_displayStart.y = edn_max(m_AnchorList[iii].m_displayStart.y, 0);
			m_AnchorList[iii].m_bufferPos = m_EdnBuf.CountForwardNLines(0, m_AnchorList[iii].m_displayStart.y);
			m_AnchorList[iii].m_lineId = m_AnchorList[iii].m_displayStart.y;
			//EDN_DEBUG(" display start : " << m_displayStart.x << "x" << m_displayStart.y);
			//EDN_DEBUG(" -------------------------------------------------");
			AnchorForceRedrawAll(iii);
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
void BufferText::AddChar(char * UTF8data)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);

	int32_t size=strlen(UTF8data);
	bool actionDone = false;
	if (1==size) {
		if (UTF8data[0] == 0x09) {
			if (false == haveSelectionActive) {
				Edn::VectorType<int8_t> tmpVect;
				tmpVect.PushBack(0x09);
				m_EdnBuf.Insert(m_cursorPos, tmpVect);
				SetInsertPosition(m_cursorPos+1, true);
			} else {
				// Indent depend of the multiline in the selection ...
				// count the number of line : 
				int32_t nbSelectedLines = m_EdnBuf.CountLines(SelectionStart, SelectionEnd);
				if (0 == nbSelectedLines) {
					Edn::VectorType<int8_t> tmpVect;
					tmpVect.PushBack(0x09);
					m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, tmpVect);
					SetInsertPosition(SelectionStart+tmpVect.Size(), true);
				} else {
					int32_t tmpPos = m_cursorPos;
					if (true == globals::IsSetShift() ) {
						tmpPos = m_EdnBuf.UnIndent(SELECTION_PRIMARY);
					} else {
						tmpPos = m_EdnBuf.Indent(SELECTION_PRIMARY);
					}
					SetInsertPosition(tmpPos, true);
				}
			}
			actionDone = true;
		} else if (UTF8data[0] == '\n') {
			Edn::VectorType<int8_t> tmpVect;
			if (true == globals::IsSetShift()) {
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
				m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, tmpVect);
				SetInsertPosition(SelectionStart+tmpVect.Size(), true);
			}
			actionDone = true;
		} else if (UTF8data[0] == 0x7F ) {
			//EDN_INFO("keyEvent : <Suppr>	pos=" << m_cursorPos);
			if (false == haveSelectionActive) {
				m_EdnBuf.Remove(m_cursorPos, m_cursorPos+1);
			} else {
				m_EdnBuf.RemoveSelected(SELECTION_PRIMARY);
				SetInsertPosition(SelectionStart, true);
			}
			actionDone = true;
		} else if (UTF8data[0] == 0x08) {
			//EDN_INFO("keyEvent : <Del>		pos=" << m_cursorPos);
			if (false == haveSelectionActive) {
				m_EdnBuf.Remove(m_cursorPos-1, m_cursorPos);
				SetInsertPosition(m_cursorPos-1, true);
			} else {
				m_EdnBuf.RemoveSelected(SELECTION_PRIMARY);
				SetInsertPosition(SelectionStart, true);
			}
			actionDone = true;
		}
	}
	
	if (false == actionDone) {
		if (true == m_EdnBuf.GetUTF8Mode()) {
			Edn::VectorType<int8_t> tmpVect;
			int32_t localOfset = strlen(UTF8data);
			tmpVect.PushBack((int8_t*)UTF8data, localOfset);
			if (false == haveSelectionActive) {
				m_EdnBuf.Insert(m_cursorPos, tmpVect);
				SetInsertPosition(m_cursorPos+localOfset, true);
			} else {
				m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, tmpVect);
				SetInsertPosition(SelectionStart+localOfset, true);
			}
		} else {
			// convert in the Good ISO format : 
			char output_ISO;
			convertUtf8ToIso(m_EdnBuf.GetCharsetType(), UTF8data, output_ISO);
			//printf(" insert : \"%s\"==> 0x%08x=%d ", UTF8data, (unsigned int)output_ISO, (int)output_ISO);
			Edn::VectorType<int8_t> tmpVect;
			tmpVect.PushBack(output_ISO);
			if (false == haveSelectionActive) {
				m_EdnBuf.Insert(m_cursorPos, tmpVect);
				SetInsertPosition(m_cursorPos+1, true);
			} else {
				m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, tmpVect);
				SetInsertPosition(SelectionStart+1, true);
			}
		}
	}
	SetModify(true);
	UpdateWindowsPosition();
}


int32_t BufferText::FindLine(Edn::String &data)
{
	if ( 0 == data.Size()) {
		EDN_WARNING("no search data");
		return 0;
	}
	EDN_INFO("Search data line : \"" << data << "\"");
	Edn::VectorType<int8_t> mVectSearch;
	mVectSearch = data.GetVector();
	//EDN_INFO("search data Forward : startSearchPos=" << startSearchPos );
	int32_t foundPos;
	bool findSomething = m_EdnBuf.SearchForward(0, mVectSearch, &foundPos, true);
	// if find data : 
	if (true == findSomething) {
		return m_EdnBuf.CountLines(0, foundPos);
	} else {
		return 0;
	}
}

void BufferText::JumpAtLine(int32_t newLine)
{
	int32_t positionLine = m_EdnBuf.CountForwardNLines(0, newLine);
	m_EdnBuf.Unselect(SELECTION_PRIMARY);
	EDN_DEBUG("jump at the line : " << newLine );
	SetInsertPosition(positionLine);
	UpdateWindowsPosition(true);
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



void BufferText::Search(Edn::String &data, bool back, bool caseSensitive, bool wrap, bool regExp)
{
	EDN_INFO("Search data : \"" << data << "\"");
	
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	int32_t startSearchPos = m_cursorPos;
	if (true == haveSelectionActive) {
		if (false == back) {
			startSearchPos = SelectionEnd;
		} else {
			startSearchPos = SelectionStart;
		}
	}
	
	if ( 0 == data.Size()) {
		EDN_WARNING("no search data");
		return;
	}
	Edn::VectorType<int8_t> mVectSearch;
	mVectSearch = data.GetVector();
	if (false == back) {
		//EDN_INFO("search data Forward : startSearchPos=" << startSearchPos );
		int32_t foundPos;
		bool findSomething = m_EdnBuf.SearchForward(startSearchPos, mVectSearch, &foundPos, caseSensitive);
		if(		false == findSomething
			&&	true == wrap)
		{
			//EDN_INFO("WrapMode !!! 0 ==> end");
			findSomething = m_EdnBuf.SearchForward(0, mVectSearch, &foundPos, caseSensitive);
		}
		// if find data : 
		if (true == findSomething) {
			// select new position
			int32_t endSelectionPos = foundPos+mVectSearch.Size();
			SetInsertPosition(endSelectionPos);
			m_EdnBuf.Select(SELECTION_PRIMARY, foundPos, endSelectionPos);
			UpdateWindowsPosition();
		}
	} else {
		//EDN_INFO("search data Backward : " << data.GetDirectPointer() );
		int32_t foundPos;
		bool findSomething = m_EdnBuf.SearchBackward(startSearchPos, mVectSearch, &foundPos, caseSensitive);
		if(		false == findSomething
			&&	true == wrap)
		{
			//EDN_INFO("WrapMode !!! end ==> 0");
			findSomething = m_EdnBuf.SearchBackward(m_EdnBuf.Size(), mVectSearch, &foundPos, caseSensitive);
		}
		// if find data : 
		if (true == findSomething) {
			// select new position
			int32_t endSelectionPos = foundPos+mVectSearch.Size();
			SetInsertPosition(foundPos);
			m_EdnBuf.Select(SELECTION_PRIMARY, foundPos, endSelectionPos);
			UpdateWindowsPosition();
		}
	}
	
	// NOTE : Need to be use in the highligner and the current buffer when we select Regular Expression ...
/*
	// Test avec le truc de regExp : 
	char * myBuf = m_EdnBuf.GetRange(0, m_EdnBuf.Size());
	regexp *compiledRE;
	char *compileMsg;
	
	compiledRE = CompileRE(data.GetDirectPointer(), &compileMsg, REDFLT_STANDARD);
	if (compiledRE == NULL) {
		EDN_ERROR("RegExpression : \"" << data.GetDirectPointer() << "\" error named : \"" << compileMsg << "\"" );
	} else {
		EDN_DEBUG("RegExpression : \"" << data.GetDirectPointer() << "\" OK ... ");
		if (true == ExecRE(compiledRE, NULL, myBuf, NULL, false, '\0', '\0', NULL, NULL)) {
			EDN_INFO("    ==> top_branch=" << compiledRE->top_branch << "; extentpBW=" << (int32_t)(compiledRE->extentpBW - myBuf) << "; extentpFW=" << (int32_t)(compiledRE->extentpFW - myBuf));
		} else {
			EDN_INFO("    ==> not fined ... ");
		}
	}
	
	// free copy of buffer ...
	if (myBuf != NULL) {
		free(myBuf);
	}
*/
}


void BufferText::Replace(Edn::String &data)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	if (true == haveSelectionActive) {
		// Replace Data : 
		Edn::VectorType<int8_t> myData = data.GetVector();
		m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, myData);
		SetInsertPosition(SelectionStart + myData.Size());
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
void BufferText::Copy(int8_t clipboardID)
{
	Edn::VectorType<int8_t> mVect;
	// get the curent selected data
	if (true == m_EdnBuf.SelectHasSelection(SELECTION_PRIMARY) ) {
		m_EdnBuf.GetSelectionText(SELECTION_PRIMARY, mVect);
	}
	// copy data in the click board : 
	ClipBoard::Set(clipboardID, mVect);
}


/**
 * @brief Request a copy and a remove of the curent selection in the named clipBoard ID
 *
 * @param[in] clipboardID		Id of the buffer we want to get data [0..10] (0 copy normal / 10 middle button)
 *
 * @return ---
 *
 */
void BufferText::Cut(int8_t clipboardID)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	// copy data
	Copy(clipboardID);
	// remove data : 
	if (true == haveSelectionActive ) {
		EDN_INFO("REMOVE SELECTION");
		m_EdnBuf.RemoveSelected(SELECTION_PRIMARY);
		SetInsertPosition(SelectionStart, true);
	}
	UpdateWindowsPosition();
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
void BufferText::Paste(int8_t clipboardID)
{
	Edn::VectorType<int8_t> mVect;
	// copy data from the click board : 
	ClipBoard::Get(clipboardID, mVect);
	
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	if (true == haveSelectionActive ) {
		// replace data
		m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, mVect );
		SetInsertPosition(SelectionStart + mVect.Size(), true);
	} else {
		// insert data
		m_EdnBuf.Insert(m_cursorPos, mVect);
		SetInsertPosition(mVect.Size(), true);
	}
	
	UpdateWindowsPosition();
	SetModify(true);
}


void BufferText::Undo(void)
{
	int32_t newPos = m_EdnBuf.Undo();
	if (newPos >= 0) {
		SetInsertPosition(newPos, true);
		UpdateWindowsPosition();
		SetModify(true);
	}
}

void BufferText::Redo(void)
{
	int32_t newPos = m_EdnBuf.Redo();
	if (newPos >= 0) {
		SetInsertPosition(newPos, true);
		UpdateWindowsPosition();
		SetModify(true);
	}
}


void BufferText::SetCharset(charset_te newCharset)
{
	m_EdnBuf.SetCharsetType(newCharset);
}


bool BufferText::AnchorGet(int32_t anchorID, bufferAnchor_ts & anchor)
{
	int32_t localID = AnchorRealId(anchorID);
	if (localID >=0) {
		// update internal sise of the width of lineID
		UpdatePointerNumber();
		// Updata uper size of display
		anchor.m_displaySize.x = m_AnchorList[localID].m_displaySize.x;
		anchor.m_displaySize.y = m_AnchorList[localID].m_displaySize.y;
		anchor.m_displayStart.x = m_AnchorList[localID].m_displayStart.x;
		anchor.m_displayStart.y = m_AnchorList[localID].m_displayStart.y;
		anchor.m_nbIterationMax = anchor.m_displaySize.y;
		// update to buffer position
		anchor.m_lineNumber = m_AnchorList[localID].m_lineId+1;
		anchor.m_posStart = m_AnchorList[localID].m_bufferPos;
		if (anchor.m_posStart >= m_EdnBuf.Size()+1) {
			return false;
		}
		anchor.m_posStop = m_EdnBuf.EndOfLine(anchor.m_posStart);
		// Update selection current
		int32_t selStart, selEnd, selRectStart, selRectEnd;
		bool selIsRect;
		bool selHave = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, selStart, selEnd, selIsRect, selRectStart, selRectEnd);
		if (false == selHave){
			anchor.m_selectionPosStart = -1;
			anchor.m_selectionPosStop = -1;
		} else {
			anchor.m_selectionPosStart = selStart+1;
			anchor.m_selectionPosStop = selEnd+1;
		}
		EDN_DEBUG("SET in anchor " << m_AnchorList[localID].m_displaySize.y << " lines to display");
		for(int32_t iii=0; iii</*edn_min(*/m_AnchorList[localID].m_displaySize.y/*, MAX_LINE_DISPLAYABLE_BY_BUFFER)*/; iii++) {
			anchor.m_redrawLine[iii] = m_AnchorList[localID].m_redrawLine[iii];
			m_AnchorList[localID].m_redrawLine[iii] = false;
			/*if (iii > m_AnchorList[localID].m_displaySize.y - 4) {
				anchor.m_redrawLine[iii] = true;
			}*/
		}
		anchor.m_BufferNumberLineOffset = m_AnchorList[localID].m_BufferNumberLineOffset;
		m_AnchorList[localID].m_BufferNumberLineOffset = 0;
		
		EDN_DEBUG("Request display : line=" << anchor.m_lineNumber << " (" << anchor.m_posStart << "," << anchor.m_posStop << ")");
		EDN_DEBUG("     ==> select : (" << anchor.m_selectionPosStart << "," << anchor.m_selectionPosStop << ")");
		return true;
	} else {
		return false;
	}
}


bool BufferText::AnchorNext(bufferAnchor_ts & anchor)
{
	anchor.m_lineNumber++;
	anchor.m_posStart = anchor.m_posStop+1;
	if (anchor.m_posStart >= m_EdnBuf.Size()+1) {
		return false;
	}
	anchor.m_posStop = m_EdnBuf.EndOfLine(anchor.m_posStart);
	anchor.m_nbIterationMax--;
	if (anchor.m_nbIterationMax<0) {
		return false;
	}
	return true;
}
