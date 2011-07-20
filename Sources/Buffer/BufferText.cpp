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

#undef __class__
#define __class__	"BufferText"


const uint32_t nbLineAllocatedInBase = 300;




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
	static int32_t fileBasicID = 0;
	NeedToCleanEndPage = true;
	// set the first element that is displayed
	m_displayStartBufferPos = 0;
	// set basic filename : 
	filename = "Untitled ";
	filename += fileBasicID;
	// no name ...
	haveName = false;
	fileBasicID++;
	// set the number of the lineNumber;
	nbColoneForLineNumber = 1;
	// init the link with the buffer manager
	myColorManager = ColorizeManager::getInstance();
	// Init Selection mode : 
	SelectionEnd();
	//EDN_INFO("Init");
	// new mode : 
	m_cursorPos = 0;
	m_cursorPreferredCol = -1;
	m_cursorOn = true;
	//m_cursorMode = CURSOR_DISPLAY_MODE_NORMAL;
	m_displayStart.x = 0;
	m_displayStart.y = 0;
	m_displaySize.x = 200;
	m_displaySize.y = 20;
	m_displayLocalSyntax.idSequence = -1;
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
BufferText::BufferText(Edn::String &newFileName)
{
	BasicInit();
	EDN_INFO("Add Data from file(" << newFileName.c_str() << ")");
	FILE * myFile = NULL;
	// set the filename : 
	SetName(newFileName);
	// try to open the file. if not existed, new file
	
	myFile = fopen(newFileName.c_str(), "r");
	if (NULL != myFile) {
		m_EdnBuf.DumpFrom(myFile);
		// close the input file
		fclose(myFile);
		SetModify(false);
	} else {
		// fichier inexistant... creation d'un nouveaux
		EDN_WARNING("No File ==> created a new one(" << newFileName.c_str() << ")");
		SetModify(true);
	}
	UpdateWindowsPosition();
	ForceReDraw(true);
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
	EDN_INFO("Save File : \"" <<  filename.c_str() << "\"" );
	FILE * myFile = NULL;
	myFile = fopen(filename.c_str(), "w");
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


void BufferText::SelectionStart(void)
{
	// start a nex selection
	SelectionCheckMode();
	//EDN_DEBUG("SELECT_start");
}

void BufferText::SelectionEnd(void)
{
	//EDN_DEBUG("SELECT_stop");
}

void BufferText::SelectionCheckMode(void)
{
	/*
	if (true == globals::IsSetCtrl() ) {
	} else {
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
Edn::String BufferText::GetName(void)
{
	return filename;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
Edn::String BufferText::GetShortName(void)
{
	char *ptr = strrchr(filename.c_str(), '/');
	if (NULL == ptr) {
		ptr = strrchr(filename.c_str(), '\\');
	}
	Edn::String out = filename;
	if (NULL != ptr) {
		out = ptr+1;
	}
	return out;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
Edn::String BufferText::GetFolder(void)
{
	char tmpVal[4096];
	strncpy(tmpVal, filename.c_str(), 4096);
	tmpVal[4096-1] = '\0';
	char *ptr = strrchr(tmpVal, '/');
	if (NULL == ptr) {
		ptr = strrchr(tmpVal, '\\');
	}
	Edn::String out = "./";
	if (NULL != ptr) {
		*ptr = '\0';
		out = tmpVal;
	}
	return out;
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
bool BufferText::HaveName(void)
{
	return haveName;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferText::SetName(Edn::String &newName)
{
	filename = newName;
	haveName = true;
	// Find HL system
	if (true == HighlightManager::getInstance()->Exist(newName)) {
		Highlight * myHL = HighlightManager::getInstance()->Get(newName);
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



void BufferText::DrawLineNumber(DrawerManager &drawer,char *myPrint,  int32_t lineNumber, int32_t positionY)
{
	char tmpLineNumber[50];
	sprintf(tmpLineNumber, myPrint, lineNumber);
	drawer.Text(myColorManager->Get(COLOR_CODE_LINE_NUMBER), 1, positionY, tmpLineNumber);
}


/**
 * @brief Display the curent buffer with all the probematic imposed by the xharset and the user contraint.
 *
 * @param[in,out] drawer the basic user drawer of EDN.
 *
 * @return 
 *
 */
int32_t	BufferText::Display(DrawerManager &drawer)
{
	int32_t letterHeight = Display::GetFontHeight();
	int32_t letterWidth = Display::GetFontWidth();
	// update the number of element that can be displayed
	m_displaySize.x = (drawer.GetWidth()/letterWidth) + 1 - nbColoneForLineNumber;;
	m_displaySize.y = (drawer.GetHeight()/letterHeight) + 1;
	EDN_INFO("main DIPLAY " << m_displaySize.x << " char * " << m_displaySize.y << " char");
	
	int32_t selStart, selEnd, selRectStart, selRectEnd;
	bool selIsRect;
	int32_t selHave = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, selStart, selEnd, selIsRect, selRectStart, selRectEnd);
	
	colorInformation_ts * HLColor = NULL;
	
	//displayLineNumber(drawer);
	// get the number of line in the buffer
	int32_t maxNumberLine = m_EdnBuf.NumberOfLines();
	//int32_t maxNumberLine = 2096;
	char *myPrint = NULL;
	if (10 > maxNumberLine) {				nbColoneForLineNumber = 1;	myPrint = (char *)"%1d";
	} else if (100 > maxNumberLine) {		nbColoneForLineNumber = 2;	myPrint = (char *)"%2d";
	} else if (1000 > maxNumberLine) {		nbColoneForLineNumber = 3;	myPrint = (char *)"%3d";
	} else if (10000 > maxNumberLine) {		nbColoneForLineNumber = 4;	myPrint = (char *)"%4d";
	} else if (100000 > maxNumberLine) {	nbColoneForLineNumber = 5;	myPrint = (char *)"%5d";
	} else if (1000000 > maxNumberLine) {	nbColoneForLineNumber = 6;	myPrint = (char *)"%6d";
	} else if (1000000 > maxNumberLine) {	nbColoneForLineNumber = 7;	myPrint = (char *)"%7d";
	} else if (10000000 > maxNumberLine) {	nbColoneForLineNumber = 8;	myPrint = (char *)"%8d";
	} else if (100000000 > maxNumberLine) {	nbColoneForLineNumber = 9;	myPrint = (char *)"%9d";
	} else {								nbColoneForLineNumber = 10;	myPrint = (char *)"%d";
	}
	
	uint32_t y = 0;
	int32_t iii, new_i;
	// Get color : 
	Colorize *  myColor           = myColorManager->Get("normal");
	Colorize *  myColorSel        = myColorManager->Get("SelectedText");
	color_ts &  myColorSpace      = myColorManager->Get(COLOR_CODE_SPACE);
	color_ts &  myColorTab        = myColorManager->Get(COLOR_CODE_TAB);
	Colorize *  selectColor       = NULL;
	char displayChar[MAX_EXP_CHAR_LEN];
	memset(displayChar, 0, sizeof(char)*MAX_EXP_CHAR_LEN);


	int mylen = m_EdnBuf.Size();
	int32_t x_base=nbColoneForLineNumber*letterWidth + 3;
	uint32_t xx = 0;
	int32_t idX = 0;
	drawer.Clean(myColorManager->Get(COLOR_CODE_BASIC_BG));
	int displayLines = 0;
	// Regenerate the colorizing if necessary ...
	m_EdnBuf.HightlightGenerateLines(m_displayLocalSyntax, m_displayStartBufferPos, m_displaySize.y);
	GTimeVal timeStart;
	g_get_current_time(&timeStart);

	// draw the lineNumber : 
	int32_t currentLineID = m_displayStart.y+1;
	DrawLineNumber(drawer, myPrint, currentLineID, y);
	for (iii=m_displayStartBufferPos; iii<mylen && displayLines < m_displaySize.y ; iii = new_i) {
		//EDN_INFO("diplay element=" << iii);
		int32_t pixelX = xx*letterWidth + x_base;
		int displaywidth;
		uint32_t currentChar = '\0';
		new_i = iii;
		displaywidth = m_EdnBuf.GetExpandedChar(new_i, idX, displayChar, currentChar);
		//EDN_INFO("diplay element=" << new_i);
		if (currentChar!='\n') {
			selectColor = myColor;
			//kwow size to display
			int32_t widthToDisplay;
			char * tmpDisplayOfset;
			bool inTheScreen = true;
			if (m_displayStart.x <= idX) {
				// Normal display
				tmpDisplayOfset = displayChar;
				widthToDisplay = displaywidth;
			} else if (m_displayStart.x < idX + displaywidth) {
				// special case of partial display : 
				widthToDisplay = idX + displaywidth - m_displayStart.x;
				tmpDisplayOfset = displayChar + (displaywidth-widthToDisplay);
			} else {
				// Out of range ...
				widthToDisplay = displaywidth;
				tmpDisplayOfset = displayChar;
				inTheScreen = false;
			}
			if (true==inTheScreen) {
				HLColor = m_EdnBuf.GetElementColorAtPosition(m_displayLocalSyntax, iii);
				if (NULL != HLColor) {
					if (NULL != HLColor->patern) {
						selectColor = HLColor->patern->GetColor();
					}
				}

				// If user want to display space char : overwrite curent color
				if(		' ' == currentChar
					&&	true == globals::IsSetDisplaySpaceChar() )
				{
					//selectColor = myColorSel;
					//SpaceText(color_ts & SelectColor, int32_t x, int32_t y,int32_t nbChar)

					if(	true == selHave
						&&	selStart <= iii
						&&	selEnd   > iii)
					{
						drawer.SpaceText(myColorSel->GetBG(), pixelX ,y , 1);
					} else if (true == selectColor->HaveBg()) {
						drawer.SpaceText(selectColor->GetBG(), pixelX ,y , 1);
					} else {
						drawer.SpaceText(myColorSpace, pixelX ,y , 1);
					}
				} else if(		'\t' == currentChar
							&&	true == globals::IsSetDisplaySpaceChar() )
				{
					if(	true == selHave
						&&	selStart <= iii
						&&	selEnd   > iii)
					{
						drawer.SpaceText(myColorSel->GetBG(), pixelX ,y , strlen(tmpDisplayOfset));
					} else if (true == selectColor->HaveBg()) {
						drawer.SpaceText(selectColor->GetBG(), pixelX ,y , strlen(tmpDisplayOfset));
					} else  {
						drawer.SpaceText(myColorTab, pixelX ,y , strlen(tmpDisplayOfset));
					}
				} else {
					if(	true == selHave
						&&	selStart <= iii
						&&	selEnd   > iii)
					{
						selectColor = myColorSel;
					}
					if (currentChar <= 0x7F) {
						drawer.Text(selectColor, pixelX ,y, tmpDisplayOfset);
					} else {
						drawer.Text(selectColor, pixelX ,y, displayChar);
					}
				}
				xx+=widthToDisplay;
			}
			idX += displaywidth;
		}
		// display cursor : 
		if (m_cursorPos == iii) {
			// display the cursor:
			if (true == m_cursorOn) {
				drawer.Cursor(pixelX, y+letterHeight);
				//m_cursorOn = false;
			} else {
				m_cursorOn = true;
			}
		}
		// move to next line ...
		if (currentChar=='\n') {
			drawer.EndOfLine(pixelX, y+letterHeight);
			drawer.Flush();
			xx = 0;
			idX =0;
			y += letterHeight;
			displayLines++;
			currentLineID++;
			DrawLineNumber(drawer, myPrint, currentLineID, y);
		}
	}
	// special case : the cursor is at the end of the buffer...
	if (m_cursorPos == iii) {
		// display the cursor:
		if (true == m_cursorOn) {
			drawer.Cursor(xx*letterWidth + x_base, y+letterHeight);
			m_cursorOn = false;
		} else {
			m_cursorOn = true;
		}
	}
	drawer.Flush();
	
	GTimeVal timeStop;
	g_get_current_time(&timeStop);
	EDN_DEBUG("Display Generation = " << timeStop.tv_usec - timeStart.tv_usec << " micro-s");

	return ERR_NONE;
}



void BufferText::GetMousePosition(int32_t width, int32_t height, int32_t &x, int32_t &y)
{
	x = (width - 3) / Display::GetFontWidth() - nbColoneForLineNumber;
	y = height / Display::GetFontHeight();
	if (x < 0) {
		x = 0;
	}
	x += m_displayStart.x;
	y += m_displayStart.y;
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

		ForceReDraw(true);
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
	ForceReDraw(true);
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
		m_cursorPos = endPos;
		ForceReDraw(true);
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
	m_cursorPos = m_EdnBuf.EndOfLine(m_cursorPos);
	ForceReDraw(true);
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
	m_cursorPos = m_EdnBuf.Size();
	ForceReDraw(true);
}

void BufferText::SelectNone(void)
{
	m_EdnBuf.Unselect(SELECTION_PRIMARY);
	ForceReDraw(true);
}

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
	MoveUpDown(3);
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
	MoveUpDown(-3);
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
	m_displayLocalSyntax.idSequence = -1;
	if (ofset >= 0) {
		int32_t nbLine = m_EdnBuf.NumberOfLines();
		if (m_displayStart.y+ofset+3 > nbLine) {
			m_displayStart.y = nbLine-3;
		} else {
			m_displayStart.y += ofset;
		}
		m_displayStartBufferPos = m_EdnBuf.CountForwardNLines(0, m_displayStart.y);
	} else {
		ofset *= -1;
		if (m_displayStart.y < ofset) {
			m_displayStart.y = 0;
			m_displayStartBufferPos = 0;
		} else {
			m_displayStart.y -= ofset;
			m_displayStartBufferPos = m_EdnBuf.CountForwardNLines(0, m_displayStart.y);
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
void BufferText::ForceReDraw(bool allElement)
{
	NeedToCleanEndPage = true;
	//m_displayLocalSyntax.idSequence = -1;
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
	bool needUpdatePosition = true;
	// check selection event ...
	/*
	if (true == globals::IsSetShift() ) {
		if ( CURSOR_MODE_NORMAL == cursorMode) {
			SelectionStart();
		} else {
			SelectionCheckMode();
		}
	} else {
		SelectionEnd();
	}
	*/
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
			TextDMoveUp(m_displaySize.x);
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Page_Down:
#		elif defined( USE_GTK_VERSION_2_0)
		case GDK_Page_Down:
#		endif
			//EDN_INFO("keyEvent : <PAGE-DOWN>");
			TextDMoveDown(m_displaySize.x);
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
	if (centerPage == false) {
		// Display position (Y mode):
		//EDN_INFO("BufferText::UpdateWindowsPosition() m_displayStart(" << m_displayStart.x << "," << m_displayStart.y << ") m_displaySize(" << m_displaySize.x << "," <<m_displaySize.y << ")");
		position_ts cursorPosition;
		cursorPosition.y = m_EdnBuf.CountLines(0, m_cursorPos);
		int32_t lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
		cursorPosition.x = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
		//EDN_INFO("BufferText::UpdateWindowsPosition() curent cursor position : (" << cursorPosition.x << "," << cursorPosition.y << ")");
		
		if (m_displayStart.y > (int32_t)cursorPosition.y - globals::getNbLineBorder() ) {
			m_displayStart.y = cursorPosition.y - globals::getNbLineBorder();
			if (m_displayStart.y < 0) {
				m_displayStart.y = 0;
				ForceReDraw(true);
			}
		} else if (m_displayStart.y + m_displaySize.y <= (int32_t)cursorPosition.y + globals::getNbLineBorder() ) {
			m_displayStart.y = cursorPosition.y - m_displaySize.y + globals::getNbLineBorder() + 1;
			ForceReDraw(true);
		}
		// Display position (X mode):
		//EDN_INFO("cursorPosition X : " << cursorPosition.y << " windows " << m_displayStart.y << "=>" << m_displayStart.x + m_displaySize.x);
		if (m_displayStart.x > cursorPosition.x - globals::getNbColoneBorder() ) {
			m_displayStart.x = cursorPosition.x - globals::getNbColoneBorder();
			if (m_displayStart.x < 0) {
				m_displayStart.x = 0;
				ForceReDraw(true);
			}
		} else if (m_displayStart.x + m_displaySize.x <= cursorPosition.x + globals::getNbColoneBorder() ) {
			m_displayStart.x = cursorPosition.x - m_displaySize.x + globals::getNbColoneBorder() + 1;
			ForceReDraw(true);
		}
		
		//update the buffer position ID : 
		m_displayStartBufferPos = m_EdnBuf.CountForwardNLines(0, m_displayStart.y);
	} else {
		// center the line at the middle of the screen :
		position_ts cursorPosition;
		//EDN_DEBUG(" -------------------------------------------------");
		cursorPosition.y = m_EdnBuf.CountLines(0, m_cursorPos);
		//EDN_DEBUG(" cursor position : " << m_cursorPos << " ==> ligne=" << cursorPosition.y);
		cursorPosition.x = 0;
		
		m_displayStart.x = 0;
		//EDN_DEBUG(" display size : " << m_displaySize.y);
		m_displayStart.y = cursorPosition.y - m_displaySize.y/2;
		m_displayStart.y = edn_max(m_displayStart.y, 0);
		
		m_displayStartBufferPos = m_EdnBuf.CountForwardNLines(0, m_displayStart.y);
		ForceReDraw(true);
		//EDN_DEBUG(" display start : " << m_displayStart.x << "x" << m_displayStart.y);
		//EDN_DEBUG(" -------------------------------------------------");
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
				EdnVectorBin<int8_t> tmpVect;
				tmpVect.PushBack(0x09);
				m_EdnBuf.Insert(m_cursorPos, tmpVect);
				SetInsertPosition(m_cursorPos+1, true);
			} else {
				if (true == globals::IsSetCtrl() ) {
					m_cursorPos = m_EdnBuf.UnIndent(SELECTION_PRIMARY);
				} else {
					m_cursorPos = m_EdnBuf.Indent(SELECTION_PRIMARY);
				}
			}
			actionDone = true;
		} else if (UTF8data[0] == '\n') {
			EdnVectorBin<int8_t> tmpVect;
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
			EdnVectorBin<int8_t> tmpVect;
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
			printf(" insert : \"%s\"==> 0x%08x=%d ", UTF8data, (unsigned int)output_ISO, (int)output_ISO);
			EdnVectorBin<int8_t> tmpVect;
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

#include "EdnRegExp.h"

int32_t BufferText::FindLine(Edn::String &data)
{
	if ( 0 == data.Size()) {
		EDN_WARNING("no search data");
		return 0;
	}
	EDN_INFO("Search data line : \"" << data.c_str() << "\"");
	EdnVectorBin<int8_t> mVectSearch;
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

void BufferText::Search(Edn::String &data, bool back, bool caseSensitive, bool wrap, bool regExp)
{
	EDN_INFO("Search data : \"" << data.c_str() << "\"");
	
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
	EdnVectorBin<int8_t> mVectSearch;
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
		// TODO : Reset this : ... m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, data.GetDirectPointer());//, strlen(data.GetDirectPointer()));
		// TODO : Reset this : ... SetInsertPosition(SelectionStart + strlen(data.GetDirectPointer()));
		
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
	EdnVectorBin<int8_t> mVect;
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
		m_cursorPos = SelectionStart;
	}
	UpdateWindowsPosition();
	ForceReDraw(true);
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
	EdnVectorBin<int8_t> mVect;
	// copy data from the click board : 
	ClipBoard::Get(clipboardID, mVect);
	
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.GetSelectionPos(SELECTION_PRIMARY, SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	if (true == haveSelectionActive ) {
		// replace data
		m_EdnBuf.ReplaceSelected(SELECTION_PRIMARY, mVect );
		m_cursorPos = SelectionStart + mVect.Size();
	} else {
		// insert data
		m_EdnBuf.Insert(m_cursorPos, mVect);
		m_cursorPos += mVect.Size();
	}
	
	UpdateWindowsPosition();
	ForceReDraw(true);
	SetModify(true);
}


void BufferText::Undo(void)
{
	int32_t newPos = m_EdnBuf.Undo();
	if (newPos >= 0) {
		SetInsertPosition(newPos, true);
		UpdateWindowsPosition();
		ForceReDraw(true);
		SetModify(true);
	}
}

void BufferText::Redo(void)
{
	int32_t newPos = m_EdnBuf.Redo();
	if (newPos >= 0) {
		SetInsertPosition(newPos, true);
		UpdateWindowsPosition();
		ForceReDraw(true);
		SetModify(true);
	}
}


void BufferText::SetCharset(charset_te newCharset)
{
	m_EdnBuf.SetCharsetType(newCharset);
	ForceReDraw(true);
}

