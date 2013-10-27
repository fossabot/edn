/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
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
	// find HL system
	//APPL_DEBUG("check name change");
	if (true == HighlightManager::Exist(m_fileName)) {
		Highlight * myHL = HighlightManager::get(m_fileName);
		// set the new HL
		if (NULL != myHL) {
			m_EdnBuf.setHLSystem(myHL);
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
bool BufferText::isModify(void)
{
	return m_fileModify;
}

void BufferText::setModify(bool status)
{
	if (status != m_fileModify) {
		m_fileModify = status;
		// TODO : remove from here
		etk::UString data = "Modify";
		ewol::getContext().getEObjectManager().MultiCast().AnonymousSend(ednMsgBufferState, data);
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
bool BufferText::needToUpdateDisplayPosition(void)
{
	bool tmpVal = m_updatePositionRequested;
	m_updatePositionRequested = false;
	return tmpVal;
}

vec2  BufferText::getBorderSize(void)
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
	setFileName(mString);
	m_haveName = false;
	EWOL_DEBUG("Create buffer with name : " << m_fileName );
	
	BasicInit();
	setModify(true);
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
	setFileName(fileName);
	EWOL_DEBUG("Create buffer with name : " << m_fileName );
	
	BasicInit();
	NameChange();
	APPL_INFO("Add Data from file(" << getFileName() << ")");
	etk::FSNode myFile(fileName);
	if (true == myFile.Exist()) {
		if (false == myFile.fileOpenRead()) {
			APPL_WARNING("File can not be open in read mode : " << myFile);
			setModify(true);
		} else {
			m_EdnBuf.DumpFrom(myFile);
			myFile.fileClose();
			setModify(false);
		}
	} else {
		// fichier inexistant... creation d'un nouveaux
		APPL_WARNING("No file  == > created a new one(" << getFileName() << ")");
		setModify(true);
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
	APPL_INFO("Save file : \"" <<  getFileName() << "\"" );
	etk::FSNode myFile(getFileName());
	if (false == myFile.fileOpenWrite()) {
		APPL_ERROR("Can not open in writing the specify file");
	} else {
		m_EdnBuf.DumpIn(myFile);
		myFile.fileClose();
		setModify(false);
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
void BufferText::getInfo(infoStatBuffer_ts &infoToUpdate)
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
void BufferText::setLineDisplay(uint32_t lineNumber)
{

}

#define SEPARATION_SIZE_LINE_NUMBER         (3)

void BufferText::drawLineNumber(ewol::Text* OOText, int32_t sizeX, int32_t sizeY, int32_t nbColomn,  int32_t lineNumber, int32_t positionY)
{
	char tmpLineNumber[50];
	sprintf(tmpLineNumber, "%*d", nbColomn, lineNumber);
	OOText->setColorBg(ColorizeManager::get(COLOR_LIST_BG_2));
	OOText->setColor(ColorizeManager::get(COLOR_CODE_LINE_NUMBER));
	OOText->setPos(vec3(1.0f, (float)positionY, 0.0f) );
	OOText->print(tmpLineNumber);
}


/**
 * @brief get the number of colomn neede to display lineNumber
 *
 * @param[in,out] ---
 *
 * @return the number of colomn
 *
 */
int32_t BufferText::getLineNumberNumberOfElement(void)
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

int32_t BufferText::getNumberOfLine(void)
{
	return m_EdnBuf.CountLines();
}

/**
 * @brief display the curent buffer with all the probematic imposed by the xharset and the user contraint.
 *
 * @param[in,out] drawer the basic user drawer of EDN.
 *
 * @return 
 *
 */
int32_t BufferText::display(ewol::Text& OOText,
                            int32_t offsetX, int32_t offsetY,
                            int32_t sizeX, int32_t sizeY)
{
	int32_t selStart, selEnd, selRectStart, selRectEnd;
	bool selIsRect;
	int32_t selHave;
	
	vec3 tmpLetterSize = OOText.calculateSize((etk::UChar)'A');
	
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
	int32_t nbColoneForLineNumber = getLineNumberNumberOfElement();
	
	// update the number of element that can be displayed
	m_displaySize.setX((sizeX/letterWidth) + 1 - nbColoneForLineNumber);
	m_displaySize.setY((sizeY/letterHeight) + 1);
	APPL_VERBOSE("main DIPLAY " << m_displaySize.x() << " char * " << m_displaySize.y() << " char");
	
	selHave = m_EdnBuf.getSelectionPos(selStart, selEnd, selIsRect, selRectStart, selRectEnd);
	
	appl::HighlightInfo * HLColor = NULL;
	
	int32_t iii, new_i;
	// get color : 
	Colorize *  myColor           = ColorizeManager::get("normal");
	Colorize *  myColorSel        = ColorizeManager::get("SelectedText");
	etk::Color<> & myColorSpace   = ColorizeManager::get(COLOR_CODE_SPACE);
	etk::Color<> & myColorTab     = ColorizeManager::get(COLOR_CODE_TAB);
	Colorize *  selectColor       = NULL;
	int mylen = m_EdnBuf.size();
	int32_t x_base=nbColoneForLineNumber*letterWidth;
	int32_t idX = 0;
	
	int64_t startTime = ewol::getTime();
	int displayLines = 0;
	// Regenerate the colorizing if necessary ...
	appl::DisplayHLData m_displayLocalSyntax;
	m_EdnBuf.HightlightGenerateLines(m_displayLocalSyntax, displayStartBufferPos, m_displaySize.y());
	
	int64_t stopTime = ewol::getTime();
	APPL_DEBUG("Parsing Highlight = " << stopTime - startTime << " micro-s");
	
	etk::UChar displayChar[MAX_EXP_CHAR_LEN];
	memset(displayChar, 0, sizeof(etk::UChar)*MAX_EXP_CHAR_LEN);
	etk::UString myStringToDisplay;
	// draw the lineNumber : 
	int32_t currentLineID = displayStartLineId+1;
	APPL_VERBOSE("Start display of text buffer [" << displayStartBufferPos<< ".." << mylen << "]");
	APPL_VERBOSE("cursor Pos : " << m_cursorPos << "start at pos=" << displayStartBufferPos);
	
	// note corection of the openGl invertion system :
	y = sizeY - y;
	y -= letterHeight;
	
	OOText.setClippingMode(false);
	drawLineNumber(&OOText, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
	int32_t pixelX = x_base + SEPARATION_SIZE_LINE_NUMBER;
	
	vec3 drawClippingPos(0,0,-0.5);
	vec3 drawClippingSize(sizeX, sizeY, 1);
	OOText.setClippingWidth(vec3((float)pixelX, 0.0f, -0.5f),
	                        vec3((float)(sizeX - drawClippingPos.x()), (float)sizeY, 0.5f) );
	
	// clear the line intexation :
	m_elmentList.clear();
	// every char element is register to find the diplay pos when mouse event arrive
	CharElement tmpElementProperty;
	tmpElementProperty.m_yOffset = y;
	tmpElementProperty.m_xOffset = 0;
	tmpElementProperty.m_ySize = 10;
	tmpElementProperty.m_bufferPos = displayStartBufferPos;
	m_elmentList.pushBack(tmpElementProperty);
	
	vec3 tmpCursorPosition(0, 0, -1); //Cursor is display only at the end to be all time over the background ... (-1 in z no cursor)
	
	float lineMaxSize = 0.0;
	for (iii=displayStartBufferPos; iii<mylen && displayLines  >= 0 && y >= -2*letterHeight; iii = new_i) {
		//APPL_DEBUG("display pos =" << y);
		int displaywidth;
		uint32_t currentChar = '\0';
		new_i = iii;
		// update the element buffer pos:
		tmpElementProperty.m_bufferPos = new_i;
		displaywidth = m_EdnBuf.getExpandedChar(new_i, idX, displayChar, currentChar);
		int32_t drawSize = 0;
		
		// update display position :
		vec2  textPos(pixelX-offsetX, y);
		// update X pos
		tmpElementProperty.m_xOffset = textPos.x();
		tmpElementProperty.m_yOffset = textPos.y();
		
		//APPL_INFO("diplay element=" << new_i);
		if (currentChar!='\n') {
			selectColor = myColor;
			HLColor = m_EdnBuf.getElementColorAtPosition(m_displayLocalSyntax, iii);
			if (HLColor != NULL) {
				if (HLColor->patern != NULL) {
					selectColor = HLColor->patern->getColor();
				}
			}
			OOText.setColorBg(etk::color::none);
			if(    true == selHave
			    && selStart <= iii
			    && selEnd > iii)
			{
				selectColor = myColorSel;
				OOText.setColorBg(selectColor->getBG() );
			} else {
				if(false == selectColor->haveBg()) {
					if(    currentChar == ' '
					    && globals::isSetDisplaySpaceChar() == true )
					{
						OOText.setColorBg(myColorSpace);
					} else if(    currentChar == '\t'
					           && globals::isSetDisplayTabChar() == true )
					{
						OOText.setColorBg(myColorTab);
					}
				} else {
					OOText.setColorBg(selectColor->getBG());
				}
			}
			tmpElementProperty.m_ySize = 20;//OOText.getHeight();
			OOText.setColor(selectColor->getFG() );
			//OOText.setColorBg(selectColor->getFG());
			OOText.setFontBold(selectColor->getBold());
			OOText.setFontItalic(selectColor->getItalic());
			myStringToDisplay = displayChar;
			OOText.setPos(vec3(textPos.x(), textPos.y(), 0.0f) );
			OOText.print(myStringToDisplay);
			// To update the display position
			drawSize = OOText.getPos().x() - textPos.x();
			//APPL_DEBUG("add element : " << tmpElementProperty.m_yOffset << "," << tmpElementProperty.m_xOffset);
			m_elmentList.pushBack(tmpElementProperty);
			
		}
		idX += displaywidth;
		// display cursor : 
		if (m_cursorPos == iii) {
			tmpCursorPosition = vec3(pixelX - offsetX, y, 0);
		}
		lineMaxSize += drawSize;
		pixelX += drawSize;
		// move to next line ...
		if (currentChar == '\n') {
			maxSize.setX(etk_max(lineMaxSize, maxSize.x()));
			lineMaxSize = 0.0;
			idX =0;
			pixelX = x_base + SEPARATION_SIZE_LINE_NUMBER;
			y -= letterHeight;
			//APPL_DEBUG("display pos =" << y);
			displayLines++;
			currentLineID++;
			OOText.setClippingMode(false);
			OOText.setFontBold(false);
			OOText.setFontItalic(false);
			drawLineNumber(&OOText, x_base, sizeY, nbColoneForLineNumber, currentLineID, y);
			OOText.setClippingMode(true);
			// add elements :
			m_elmentList.pushBack(tmpElementProperty);
		}
	}
	//APPL_DEBUG("end at pos buf =" << iii << " / " << m_EdnBuf.size());
	// special case : the cursor is at the end of the buffer...
	if (m_cursorPos == iii) {
		tmpCursorPosition = vec3(pixelX - offsetX, y, 0);
	}
	if (tmpCursorPosition.z()!=-1) {
		// display the cursor:
		OOText.setPos(tmpCursorPosition);
		OOText.setColor(ColorizeManager::get(COLOR_CODE_CURSOR));
		OOText.setColorBg(ColorizeManager::get(COLOR_CODE_CURSOR));
		OOText.printCursor(false);
	}
	// display the 80 colomn limit line :
	
	OOText.setClippingMode(false);
	OOText.getDrawing().setThickness(2);
	OOText.getDrawing().setColor(etk::Color<>(200,200,0,255));
	OOText.getDrawing().setPos(vec2((float)((nbColoneForLineNumber+80)*tmpLetterSize.x()), 0.0f));
	OOText.getDrawing().lineRel(vec2(0.0f, 2500.0f));
	// set the maximum size for the display ...
	setMaximumSize(maxSize);
	int64_t stopTime2 = ewol::getTime();
	APPL_DEBUG("DRAW text (brut) = " << stopTime2 - stopTime << " micro-s");

	return 0;
}



int32_t BufferText::getMousePosition(vec2 pos)
{
	bool inLineDone=false;
	//APPL_DEBUG("try to find in : " << width << "," << height);
	for(int32_t iii=0; iii<m_elmentList.size()-1; iii++) {
		//APPL_DEBUG("check element : " << m_elmentList[iii].m_yOffset << " <=  " << pos.y << " <" << (m_elmentList[iii].m_yOffset + m_elmentList[iii].m_ySize));
		if(false == inLineDone) {
			if(    pos.y() >= m_elmentList[iii].m_yOffset
			    && pos.y()<m_elmentList[iii].m_yOffset + m_elmentList[iii].m_ySize ) {
				// we find the line (int theory)  == > note : Some problem can appear here when the size are not the same ...
				// this is to prevent multiple size font ...
				inLineDone = true;
				//APPL_DEBUG("         == > " << m_elmentList[iii+1].m_xOffset << "> " << pos.x << "  >= " << m_elmentList[iii].m_xOffset);
			}
		}
		// we detected the line
		if(true == inLineDone) {
			if(    pos.x() >= m_elmentList[iii].m_xOffset
			    && pos.x()<m_elmentList[iii+1].m_xOffset ) {
				// we get the position ...
				return m_elmentList[iii].m_bufferPos;
			} else if (m_elmentList[iii].m_xOffset >= m_elmentList[iii+1].m_xOffset) {
				// prevent "end of line" cursor pos ...
				return m_elmentList[iii].m_bufferPos;
			}
		}
	}
	if (m_elmentList.size()>0) {
		if(pos.y()<m_elmentList[m_elmentList.size()/2].m_yOffset) {
			//APPL_DEBUG("Error to get position (return Last)");
			return m_elmentList[m_elmentList.size()-1].m_bufferPos;
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
	if (false) { //ewol::getCurrentSpecialKeyStatus().isSetShift() ) {
		MouseSelectFromCursorTo(pos);
	} else {
		// get the caracter mouse position
		int32_t newPos = getMousePosition(pos);
		// move the cursor
		setInsertPosition(newPos);

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
 * @todo : set the move up and DOWN...
 *
 */
void BufferText::MouseSelectFromCursorTo(vec2 pos)
{
	// get the caracter mouse position
	int32_t newPos = getMousePosition(pos);
	
	int32_t selStart, selEnd, selRectStart, selRectEnd;
	bool selIsRect;
	int32_t selHave = m_EdnBuf.getSelectionPos(selStart, selEnd, selIsRect, selRectStart, selRectEnd);
	//APPL_DEBUG("BufferText:: " << selHave << " = BufGetSelectionPos(SELECTION_PRIMARY," << selStart << "," << selEnd << "," << selIsRect << "," << selRectStart << "," << selRectEnd << ");" );
	int32_t rememberCursorPos = m_cursorPos;
	// move the cursor
	setInsertPosition(newPos);
	
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

void BufferText::removeLine(void)
{
	int32_t start = m_EdnBuf.StartOfLine(m_cursorPos);
	int32_t stop = m_EdnBuf.EndOfLine(m_cursorPos);
	m_EdnBuf.remove(start, stop+1);
	setInsertPosition(start);
	setModify(true);
}

void BufferText::SelectAll(void)
{
	m_EdnBuf.Select(0, m_EdnBuf.size());
	m_cursorPos = m_EdnBuf.size();
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
void BufferText::setInsertPosition(int32_t newPos, bool insertChar)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.getSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	int32_t rememberCursorPos = m_cursorPos;

	//APPL_DEBUG("newPos=" << newPos);

	// unselect buffer:
	m_EdnBuf.Unselect();
	/* make sure new position is ok, do nothing if it hasn't changed */
	if (newPos != m_cursorPos) {
		if (newPos < 0){
			newPos = 0;
		}
		if (newPos > m_EdnBuf.size()) {
			newPos = m_EdnBuf.size();
		}
		m_cursorPos = newPos;
	}
	m_cursorPreferredCol = -1;
	
	// special case when insert char ...
	if (true == insertChar) {
		return;
	}
	
	if(    false == haveSelectionActive
	    && true  == false ) //ewol::getCurrentSpecialKeyStatus().isSetShift() )
	{
		// new selection
		m_EdnBuf.Select(rememberCursorPos, m_cursorPos);
	} else if(    true == false //ewol::getCurrentSpecialKeyStatus().isSetShift()
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

	// find the position of the start of the line.
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

	// get the previous line
	prevLineStartPos = m_EdnBuf.CountBackwardNLines(lineStartPos, offset);
	//APPL_INFO("Move line UP result : prevLineStartPos=" << prevLineStartPos);
	// get the display char position
	newPos = m_EdnBuf.CountForwardDispChars(prevLineStartPos, column);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);

	// move the cursor
	setInsertPosition(newPos);

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
	if (m_cursorPos == m_EdnBuf.size() ) {
		return false;
	}
	// find the position of the start of the line.
	lineStartPos = m_EdnBuf.StartOfLine(m_cursorPos);
	
	if (m_cursorPreferredCol >= 0) {
		column = m_cursorPreferredCol;
	} else {
		column = m_EdnBuf.CountDispChars(lineStartPos, m_cursorPos);
	}
	// get the next line :
	nextLineStartPos = m_EdnBuf.CountForwardNLines(lineStartPos, offset);
	//APPL_INFO("Move line DOWN result : nextLineStartPos=" << nextLineStartPos);
	// get the display char position
	newPos = m_EdnBuf.CountForwardDispChars(nextLineStartPos, column);
	//APPL_INFO("Move to colomn : column=" << column << " newPos=" << newPos);
	
	setInsertPosition(newPos);
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
				setInsertPosition(m_cursorPos - 1); 
			}
			break;
		case ewol::keyEvent::keyboardRight:
			//APPL_INFO("keyEvent : <RIGHT>");
			if (m_cursorPos < m_EdnBuf.size() ) {
				setInsertPosition(m_cursorPos + 1);
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
			setInsertPosition(m_EdnBuf.StartOfLine(m_cursorPos) );
			break;
		case ewol::keyEvent::keyboardEnd:
			//APPL_INFO("keyEvent : <End of line>");
			setInsertPosition(m_EdnBuf.EndOfLine(m_cursorPos) );
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
vec2  BufferText::getPosition(int32_t fontId, bool& centerRequested)
{
	centerRequested = m_centerRequested;
	m_centerRequested = false;
	vec2  outputPosition(0,0);

	// display position (Y mode):
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
	// TODO : set it back ...
	/*
	float letterWidth = ewol::getWidth(fontId, "A");
	float letterHeight = ewol::getHeight(fontId);
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
		//APPL_DEBUG(" cursor position : " << m_cursorPos << "  == > ligne=" << outputPosition.y);
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
void BufferText::addChar(etk::UChar unicodeData)
{
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.getSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	if (unicodeData == 0x09) {
		if (false == haveSelectionActive) {
			etk::Vector<int8_t> tmpVect;
			tmpVect.pushBack(0x09);
			m_EdnBuf.insert(m_cursorPos, tmpVect);
			setInsertPosition(m_cursorPos+1, true);
		} else {
			// Indent depend of the multiline in the selection ...
			// count the number of line : 
			int32_t nbSelectedLines = m_EdnBuf.CountLines(SelectionStart, SelectionEnd);
			if (0 == nbSelectedLines) {
				etk::Vector<int8_t> tmpVect;
				tmpVect.pushBack(0x09);
				m_EdnBuf.ReplaceSelected(tmpVect);
				setInsertPosition(SelectionStart+tmpVect.size(), true);
			} else {
				if (true == false ) { //ewol::getCurrentSpecialKeyStatus().isSetShift() ) {
					m_cursorPos = m_EdnBuf.UnIndent();
				} else {
					m_cursorPos = m_EdnBuf.Indent();
				}
			}
		}
	} else if (unicodeData == '\n') {
		etk::Vector<int8_t> tmpVect;
		if (true == false ) { //ewol::getCurrentSpecialKeyStatus().isSetShift()) {
			tmpVect.pushBack('\r');
		} else {
			tmpVect.pushBack('\n');
			// if Auto indent enable  == > we get the start of the previous line and add it to tne new one
			if (true == globals::isSetAutoIndent() ) {
				int32_t l_lineStart;
				// get the begin of the line or the begin of the line befor selection
				if (false == haveSelectionActive) {
					l_lineStart = m_EdnBuf.StartOfLine(m_cursorPos);
				} else {
					l_lineStart = m_EdnBuf.StartOfLine(SelectionStart);
				}
				// add same characters in the temporar buffer
				for (int32_t kk=l_lineStart; kk<m_cursorPos; kk++) {
					if (' ' == m_EdnBuf[kk]) {
						tmpVect.pushBack(' ');
					} else if('\t' == m_EdnBuf[kk]) {
						tmpVect.pushBack('\t');
					} else {
						break;
					}
				}
			}
		}
		// set temporary buffer in the real buffer
		if (false == haveSelectionActive) {
			m_EdnBuf.insert(m_cursorPos, tmpVect);
			setInsertPosition(m_cursorPos+tmpVect.size(), true);
		} else {
			m_EdnBuf.ReplaceSelected(tmpVect);
			setInsertPosition(SelectionStart+tmpVect.size(), true);
		}
	} else if (unicodeData == 0x7F ) {
		//APPL_INFO("keyEvent : <Suppr>	pos=" << m_cursorPos);
		if (false == haveSelectionActive) {
			m_EdnBuf.remove(m_cursorPos, m_cursorPos+1);
		} else {
			m_EdnBuf.removeSelected();
			setInsertPosition(SelectionStart, true);
		}
	} else if (unicodeData == 0x08) {
		//APPL_INFO("keyEvent : <Del>		pos=" << m_cursorPos);
		if (false == haveSelectionActive) {
			m_EdnBuf.remove(m_cursorPos-1, m_cursorPos);
			setInsertPosition(m_cursorPos-1, true);
		} else {
			m_EdnBuf.removeSelected();
			setInsertPosition(SelectionStart, true);
		}
	} else {
		// normal adding char ...
		if (true == m_EdnBuf.getUTF8Mode()) {
			char tmpUTF8[16];
			unicodeData.getUtf8(tmpUTF8);
			etk::Vector<int8_t> tmpVect;
			int32_t localOfset = strlen(tmpUTF8);
			tmpVect.pushBack((int8_t*)tmpUTF8, localOfset);
			if (false == haveSelectionActive) {
				m_EdnBuf.insert(m_cursorPos, tmpVect);
				setInsertPosition(m_cursorPos+localOfset, true);
			} else {
				m_EdnBuf.ReplaceSelected(tmpVect);
				setInsertPosition(SelectionStart+localOfset, true);
			}
		} else {
			// convert in the Good ISO format : 
			char output_ISO;
			unicode::convertUnicodeToIso(m_EdnBuf.getCharsetType(), unicodeData, output_ISO);
			//printf(" insert : \"%s\" == > 0x%08x=%d ", UTF8data, (unsigned int)output_ISO, (int)output_ISO);
			etk::Vector<int8_t> tmpVect;
			tmpVect.pushBack(output_ISO);
			if (false == haveSelectionActive) {
				m_EdnBuf.insert(m_cursorPos, tmpVect);
				setInsertPosition(m_cursorPos+1, true);
			} else {
				m_EdnBuf.ReplaceSelected(tmpVect);
				setInsertPosition(SelectionStart+1, true);
			}
		}
	}

	setModify(true);
	RequestUpdateOfThePosition();
}


int32_t BufferText::findLine(etk::UString &data)
{
	if ( 0 == data.size()) {
		APPL_WARNING("no search data");
		return 0;
	}
	APPL_INFO("Search data line : \"" << data << "\"");
	etk::Vector<etk::UChar> mVectSearch;
	mVectSearch = data.getVector();
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
	setInsertPosition(positionLine);
	m_centerRequested = true;
	RequestUpdateOfThePosition();
}

/**
 * @brief get the current line (to know where to jump)
 *
 * @param ---
 *
 * @return Return the current line number
 *
 */
int32_t BufferText::getCurrentLine(void)
{
	return m_EdnBuf.CountLines(0, m_cursorPos);
}



void BufferText::Search(etk::UString &data, bool back, bool caseSensitive, bool wrap, bool regExp)
{
	APPL_INFO("Search data : \"" << data << "\"");
	
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.getSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	int32_t startSearchPos = m_cursorPos;
	if (true == haveSelectionActive) {
		if (false == back) {
			startSearchPos = SelectionEnd;
		} else {
			startSearchPos = SelectionStart;
		}
	}
	
	if ( 0 == data.size()) {
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
			//APPL_INFO("WrapMode !!! 0  == > end");
			findSomething = m_EdnBuf.SearchForward(0, data, &foundPos, &foundPosEnd, caseSensitive);
		}
		// if find data : 
		if (true == findSomething) {
			// select new position
			setInsertPosition(foundPosEnd);
			m_EdnBuf.Select(foundPos, foundPosEnd);
		}
	} else {
		//APPL_INFO("search data Backward : " << data.getDirectPointer() );
		int32_t foundPos;
		int32_t foundPosEnd;
		bool findSomething = m_EdnBuf.SearchBackward(startSearchPos, data, &foundPos, &foundPosEnd, caseSensitive);
		if(		false == findSomething
			&&	true == wrap)
		{
			//APPL_INFO("WrapMode !!! end  == > 0");
			findSomething = m_EdnBuf.SearchBackward(m_EdnBuf.size(), data, &foundPos, &foundPosEnd, caseSensitive);
		}
		// if find data : 
		if (true == findSomething) {
			// select new position
			setInsertPosition(foundPos);
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
	bool haveSelectionActive = m_EdnBuf.getSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	if (true == haveSelectionActive) {
		// Replace Data : 
		int32_t size = m_EdnBuf.ReplaceSelected(data);
		setInsertPosition(SelectionStart + size);
	}
	setModify(true);
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
		m_EdnBuf.getSelectionText(mVect);
	}
	// copy data in the click board : 
	ewol::clipBoard::set(clipboardID, mVect);
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
	bool haveSelectionActive = m_EdnBuf.getSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	// copy data
	Copy(clipboardID);
	// remove data : 
	if (true == haveSelectionActive ) {
		APPL_INFO("REMOVE SELECTION");
		m_EdnBuf.removeSelected();
		m_cursorPos = SelectionStart;
	}
	RequestUpdateOfThePosition();
	setModify(true);
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
	mVect = ewol::clipBoard::get(clipboardID);
	
	int32_t SelectionStart, SelectionEnd, SelectionRectStart, SelectionRectEnd;
	bool SelectionIsRect;
	bool haveSelectionActive = m_EdnBuf.getSelectionPos(SelectionStart, SelectionEnd, SelectionIsRect, SelectionRectStart, SelectionRectEnd);
	
	if (true == haveSelectionActive ) {
		// replace data
		int32_t size = m_EdnBuf.ReplaceSelected(mVect );
		m_cursorPos = SelectionStart + size;
	} else {
		// insert data
		int32_t size = m_EdnBuf.insert(m_cursorPos, mVect);
		m_cursorPos += size;
	}
	RequestUpdateOfThePosition();
	setModify(true);
}


void BufferText::Undo(void)
{
	int32_t newPos = m_EdnBuf.Undo();
	if (newPos >= 0) {
		setInsertPosition(newPos, true);
		RequestUpdateOfThePosition();
		setModify(true);
	}
}

void BufferText::Redo(void)
{
	int32_t newPos = m_EdnBuf.Redo();
	if (newPos >= 0) {
		setInsertPosition(newPos, true);
		RequestUpdateOfThePosition();
		setModify(true);
	}
}


void BufferText::setCharset(unicode::charset_te newCharset)
{
	m_EdnBuf.setCharsetType(newCharset);
}

