/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <CodeView.h>

#include <appl/Debug.h>
#include <appl/global.h>
#include <TextViewer.h>
#include <BufferManager.h>
#include <ColorizeManager.h>
#include <ewol/clipBoard.h>
#include <SearchData.h>

#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/EObject.h>

#undef __class__
#define __class__	"TextViewer"

appl::TextViewer::TextViewer(const etk::UString& _fontName, int32_t _fontSize) :
	m_buffer(NULL),
	m_displayText(_fontName, _fontSize),
	m_insertMode(false)
{
	setCanHaveFocus(true);
	RegisterMultiCast(ednMsgBufferId);
	RegisterMultiCast(ednMsgGuiCopy);
	RegisterMultiCast(ednMsgGuiPaste);
	RegisterMultiCast(ednMsgGuiCut);
	RegisterMultiCast(ednMsgGuiRedo);
	RegisterMultiCast(ednMsgGuiUndo);
	RegisterMultiCast(ednMsgGuiRm);
	RegisterMultiCast(ednMsgGuiSelect);
	RegisterMultiCast(ednMsgGuiChangeCharset);
	RegisterMultiCast(ednMsgGuiFind);
	RegisterMultiCast(ednMsgGuiReplace);
	RegisterMultiCast(ednMsgGuiGotoLine);
	setLimitScrolling(0.2);
	
	ShortCutAdd("ctrl+w",       ednMsgGuiRm,     "Line");
	ShortCutAdd("ctrl+shift+w", ednMsgGuiRm,     "Paragraph");
	ShortCutAdd("ctrl+x",       ednMsgGuiCut,    "STD");
	ShortCutAdd("ctrl+c",       ednMsgGuiCopy,   "STD");
	ShortCutAdd("ctrl+v",       ednMsgGuiPaste,  "STD");
	ShortCutAdd("ctrl+a",       ednMsgGuiSelect, "ALL");
	ShortCutAdd("ctrl+shift+a", ednMsgGuiSelect, "NONE");
	
	// by default we load an example object:
	
	m_buffer = new appl::Buffer();
	if (m_buffer == NULL) {
		APPL_ERROR("can not create buffer ... ");
		return;
	}
	m_buffer->loadFile("./example.txt");
	
}

appl::TextViewer::~TextViewer(void)
{
	
}


bool appl::TextViewer::calculateMinSize(void)
{
	m_minSize.setValue(50,50);
	return true;
}

void appl::TextViewer::calculateMaxSize(void)
{
	m_maxSize.setX(256);
	m_maxSize.setY(256);
}


void appl::TextViewer::onDraw(void)
{
	m_displayDrawing.draw();
	m_displayText.draw();
	WidgetScrooled::onDraw();
}

void appl::TextViewer::onRegenerateDisplay(void)
{
	if (false == needRedraw()) {
		return;
	}
	// For the scrooling windows
	calculateMaxSize();
	m_displayDrawing.clear();
	m_displayText.clear();
	
	// reset the background :
	m_displayDrawing.setPos(vec3(0, 0, 0));
	m_displayDrawing.setColor(etk::Color<>(220, 220, 220, 256));
	m_displayDrawing.rectangleWidth(m_size);
	
	if (m_buffer == NULL) {
		m_displayText.setTextAlignement(10, m_size.x()-20, ewol::Text::alignLeft);
		m_displayText.setRelPos(vec3(10, 0, 0));
		etk::UString tmpString("<br/>\n"
		                       "<font color=\"red\">\n"
		                       "	<b>\n"
		                       "		edn - Editeur De N'ours\n"
		                       "	</b>\n"
		                       "</font>\n"
		                       "<br/>\n"
		                       "<br/>\n"
		                       "<font color=\"indigo\">\n"
		                       "	<i>\n"
		                       "		No Buffer Availlable to display\n"
		                       "	</i>\n"
		                       "</font>\n");
		m_displayText.setPos(vec3(0.0f, m_size.y(), 0.0f) );
		m_displayText.forceLineReturn();
		m_displayText.printDecorated(tmpString);
		// call the herited class...
		WidgetScrooled::onRegenerateDisplay();
		return;
	}
	// normal displa of the buffer :
	vec3 tmpCursorPosition(0, 0, -1);
	// real display ...
	etk::Buffer& buf = m_buffer->getData();
	m_displayText.setColor(etk::Color<>(0, 0, 0, 256));
	float countNbLine = 1;
	esize_t countColomn = 0;
	vec3 tmpLetterSize = m_displayText.calculateSize((uniChar_t)'A');
	vec3 positionCurentDisplay(0.0f, m_size.y()-tmpLetterSize.y(), 0.0f);
	m_displayText.setPos(positionCurentDisplay);
	// the siplay string :
	etk::UString stringToDisplay;
	esize_t bufferElementSize = 0;
	etk::UniChar currentValue;
	for (int32_t iii=0; iii<buf.size(); iii+=bufferElementSize) {
		if (iii == m_buffer->m_cursorPos) {
			// need to display the cursor :
			tmpCursorPosition = positionCurentDisplay;
		}
		bufferElementSize = m_buffer->get(iii, currentValue);
		//APPL_DEBUG(" element size : " << iii << " : " << bufferElementSize);
		if (currentValue == etk::UniChar::Return) {
			countNbLine += 1;
			countColomn = 0;
			if (bufferElementSize  == 0) {
				bufferElementSize = 1;
			}
			positionCurentDisplay.setX(0);
			positionCurentDisplay.setY(positionCurentDisplay.y()-tmpLetterSize.y());
			m_displayText.setPos(positionCurentDisplay);
			continue;
		}
		m_buffer->Expand(countColomn, currentValue, stringToDisplay);
		//APPL_DEBUG("display : '" << currentValue << "'  == > '" << stringToDisplay << "'");
		//m_displayText.setPos(positionCurentDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.size(); ++kkk) {
			m_displayText.print(stringToDisplay[kkk]);
		}
		positionCurentDisplay = m_displayText.getPos();
		countColomn += stringToDisplay.size();
		
		if (bufferElementSize  == 0) {
			bufferElementSize = 1;
		}
	}
	if (tmpCursorPosition.z()!=-1) {
		// display the cursor:
		//APPL_DEBUG("display cursor at position : " << tmpCursorPosition);
		m_displayText.setPos(tmpCursorPosition);
		m_displayText.setColor(etk::Color<>(0xFF0000FF));
		m_displayText.setColorBg(etk::Color<>(0xFF0000FF));
		m_displayText.printCursor(m_insertMode);
	}
	// call the herited class...
	WidgetScrooled::onRegenerateDisplay();
}


bool appl::TextViewer::onEventEntry(const ewol::EventEntry& _event)
{
	if (m_buffer == NULL) {
		return false;
	}
	// just forward event  == > manage directly in the buffer
	if (m_buffer->onEventEntry(_event) == true) {
		markToRedraw();
		return true;
	}
	return false;
}


bool appl::TextViewer::onEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = relativePosition(_event.getPos());
	if (m_buffer != NULL) {
		
	}
	keepFocus();
	return true;
}

void appl::TextViewer::onEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	if (m_buffer != NULL) {
		//tmpBuffer->Paste(_clipboardID);
	}
	markToRedraw();
}

void appl::TextViewer::onReceiveMessage(const ewol::EMessage& _msg)
{
	// force redraw of the widget
	markToRedraw();
}


void appl::TextViewer::onGetFocus(void)
{
	ShowKeyboard();
	APPL_INFO("Focus - In");
}


void appl::TextViewer::onLostFocus(void)
{
	HideKeyboard();
	APPL_INFO("Focus - out");
}

void appl::TextViewer::setFontSize(int32_t _size)
{
	m_displayText.setFontSize(_size);
	setScrollingSize(_size*3.0*1.46); // 1.46 is a magic number ...
}

void appl::TextViewer::setFontName(const etk::UString& _fontName)
{
	m_displayText.setFontName(_fontName);
}

