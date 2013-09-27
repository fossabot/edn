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
	SetCanHaveFocus(true);
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
	SetLimitScrolling(0.2);
	
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
	m_buffer->LoadFile("./example.txt");
	
}

appl::TextViewer::~TextViewer(void)
{
	
}


bool appl::TextViewer::CalculateMinSize(void)
{
	m_minSize.setValue(50,50);
	return true;
}

void appl::TextViewer::CalculateMaxSize(void)
{
	m_maxSize.setX(256);
	m_maxSize.setY(256);
}


void appl::TextViewer::OnDraw(void)
{
	m_displayDrawing.Draw();
	m_displayText.Draw();
	WidgetScrooled::OnDraw();
}

static const char *ControlCodeTable[32] = {
	 "NUL", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs",  "ht", "nl",  "vt",  "np", "cr", "so", "si",
	 "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb", "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};

void appl::TextViewer::Expand(esize_t& _indent, const etk::UniChar& _value, etk::UString& _out) const
{
	_out.Clear();
	int32_t tabDist = 4;
	if (_value == etk::UniChar::Tabulation) {
		int32_t nSpaces = tabDist - (_indent % tabDist);
		for (int32_t iii=0; iii<nSpaces; iii++) {
			_out.Append(etk::UniChar::Space);
		}
		return;
	}
	// Convert ASCII control codes to readable character sequences
	if (_value == etk::UniChar::Null) {
		_out.Append(etk::UniChar('<'));
		_out.Append(etk::UniChar('n'));
		_out.Append(etk::UniChar('u'));
		_out.Append(etk::UniChar('l'));
		_out.Append(etk::UniChar('>'));
		return;
	}
	if (_value == etk::UniChar::Return) {
		// nothing to display...
		return;
	}
	if (_value.Get() <= 31) {
		_out.Append(etk::UniChar('<'));
		const char * tmp = ControlCodeTable[_value.Get()];
		while (*tmp!='\0') {
			_out.Append(etk::UniChar(*tmp));
			tmp++;
		}
		_out.Append(etk::UniChar('>'));
		return;
	}
	if (_value == etk::UniChar::Delete) {
		_out.Append(etk::UniChar('<'));
		_out.Append(etk::UniChar('d'));
		_out.Append(etk::UniChar('e'));
		_out.Append(etk::UniChar('l'));
		_out.Append(etk::UniChar('>'));
		return;
	}
	// nothing to do ...
	_out.Append(_value);
	//APPL_DEBUG("plop : " << _out);
}



void appl::TextViewer::OnRegenerateDisplay(void)
{
	if (false == NeedRedraw()) {
		return;
	}
	// For the scrooling windows
	CalculateMaxSize();
	m_displayDrawing.Clear();
	m_displayText.Clear();
	
	// Reset the background :
	m_displayDrawing.SetPos(vec3(0, 0, 0));
	m_displayDrawing.SetColor(etk::Color<>(220, 220, 220, 256));
	m_displayDrawing.RectangleWidth(m_size);
	
	if (m_buffer == NULL) {
		m_displayText.SetTextAlignement(10, m_size.x()-20, ewol::Text::alignLeft);
		m_displayText.SetRelPos(vec3(10, 0, 0));
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
		m_displayText.SetPos(vec3(0.0f, m_size.y(), 0.0f) );
		m_displayText.ForceLineReturn();
		m_displayText.PrintDecorated(tmpString);
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
		return;
	}
	// normal displa of the buffer :
	vec3 tmpCursorPosition(0, 0, -1);
	// real display ...
	etk::Buffer& buf = m_buffer->GetData();
	m_displayText.SetColor(etk::Color<>(0, 0, 0, 256));
	float countNbLine = 1;
	esize_t countColomn = 0;
	vec3 tmpLetterSize = m_displayText.CalculateSize((uniChar_t)'A');
	vec3 positionCurentDisplay(0.0f, m_size.y()-tmpLetterSize.y(), 0.0f);
	m_displayText.SetPos(positionCurentDisplay);
	// the siplay string :
	etk::UString stringToDisplay;
	esize_t bufferElementSize = 0;
	etk::UniChar currentValue;
	for (int32_t iii=0; iii<buf.Size(); iii+=bufferElementSize) {
		if (iii==m_buffer->m_cursorPos) {
			// need to display the cursor :
			tmpCursorPosition = positionCurentDisplay;
		}
		bufferElementSize = m_buffer->Get(iii, currentValue, unicode::EDN_CHARSET_UTF8);
		//APPL_DEBUG(" element size : " << iii << " : " << bufferElementSize);
		if (currentValue == etk::UniChar::Return) {
			countNbLine += 1;
			countColomn = 0;
			if (bufferElementSize ==0) {
				bufferElementSize = 1;
			}
			positionCurentDisplay.setX(0);
			positionCurentDisplay.setY(positionCurentDisplay.y()-tmpLetterSize.y());
			m_displayText.SetPos(positionCurentDisplay);
			continue;
		}
		appl::TextViewer::Expand(countColomn, currentValue, stringToDisplay);
		//m_displayText.SetPos(positionCurentDisplay);
		for (esize_t kkk=0; kkk<stringToDisplay.Size(); ++kkk) {
			m_displayText.Print(stringToDisplay[kkk]);
		}
		positionCurentDisplay.setX(positionCurentDisplay.x()+tmpLetterSize.x()*(float)stringToDisplay.Size());
		countColomn += stringToDisplay.Size();
		
		if (bufferElementSize ==0) {
			bufferElementSize = 1;
		}
	}
	if (tmpCursorPosition.z()!=-1) {
		// display the cursor:
		APPL_DEBUG("display cursor at position : " << tmpCursorPosition);
		m_displayText.SetPos(tmpCursorPosition);
		m_displayText.SetColor(etk::Color<>(0xFF0000FF));
		m_displayText.SetColorBg(etk::Color<>(0xFF0000FF));
		m_displayText.PrintCursor(m_insertMode);
	}
	// call the herited class...
	WidgetScrooled::OnRegenerateDisplay();
}


bool appl::TextViewer::OnEventEntry(const ewol::EventEntry& _event)
{
	if (m_buffer == NULL) {
		return false;
	}
	// just forward event ==> manage directly in the buffer
	if (m_buffer->OnEventEntry(_event) == true) {
		MarkToRedraw();
		return true;
	}
	return false;
}


bool appl::TextViewer::OnEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = RelativePosition(_event.GetPos());
	if (m_buffer != NULL) {
		
	}
	KeepFocus();
	return true;
}

void appl::TextViewer::OnEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	if (m_buffer != NULL) {
		//tmpBuffer->Paste(_clipboardID);
	}
	MarkToRedraw();
}

void appl::TextViewer::OnReceiveMessage(const ewol::EMessage& _msg)
{
	// Force redraw of the widget
	MarkToRedraw();
}


void appl::TextViewer::OnGetFocus(void)
{
	ShowKeyboard();
	APPL_INFO("Focus - In");
}


void appl::TextViewer::OnLostFocus(void)
{
	HideKeyboard();
	APPL_INFO("Focus - out");
}

void appl::TextViewer::SetFontSize(int32_t _size)
{
	m_displayText.SetFontSize(_size);
	SetScrollingSize(_size*3.0*1.46); // 1.46 is a magic number ...
}

void appl::TextViewer::SetFontName(const etk::UString& _fontName)
{
	m_displayText.SetFontName(_fontName);
}

