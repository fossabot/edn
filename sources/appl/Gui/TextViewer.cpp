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
	m_displayText(_fontName, _fontSize)
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


esize_t appl::TextViewer::Get(esize_t _pos, UniChar& _value, charset_te _charset) const
{
	_value = '\0';
	if (_charset == unicode::EDN_CHARSET_UTF8) {
		char tmpVal[8];
		tmpVal[0] = m_buffer[_pos];
		tmpVal[1] = m_buffer[_pos+1];
		tmpVal[2] = m_buffer[_pos+2];
		tmpVal[3] = m_buffer[_pos+3];
		tmpVal[4] = m_buffer[_pos+4];
		tmpVal[5] = m_buffer[_pos+5];
		tmpVal[6] = m_buffer[_pos+6];
		tmpVal[7] = '\0';
		// transform ...
		
	}
	// TODO :: need to trancode iso ==> UNICODE ...
	_value = m_buffer[_pos];
	return _pos+1;
}


void appl::TextViewer::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
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
		} else {
			// real display ...
			etk::Buffer& buf = m_buffer->GetData();
			m_displayText.SetColor(etk::Color<>(0, 0, 0, 256));
			float countNbLine = 1;
			float countColomn = 0;
			vec3 tmpLetterSize = m_displayText.CalculateSize((uniChar_t)'A');
			
			for (int32_t iii=0; iii<buf.Size(); ++iii) {
				char c = buf.Get(iii);
				etk::UniChar myChar(c);
				if (myChar == etk::UniChar('\n')) {
					countNbLine += 1;
					countColomn = 0;
					continue;
				}
				countColomn += 1;
				m_displayText.SetPos(vec3(countColomn*tmpLetterSize.x(),
				                          m_size.y() - countNbLine*tmpLetterSize.y(),
				                          0.0f) );
				m_displayText.Print(myChar);
				//tmpLetterSize.x();
			}
		}
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
	}
}


bool appl::TextViewer::OnEventEntry(const ewol::EventEntry& _event)
{
	if (_event.GetType() == ewol::keyEvent::keyboardChar) {
		//APPL_DEBUG("KB EVENT : \"" << UTF8_data << "\" size=" << strlen(UTF8_data) << "type=" << (int32_t)typeEvent);
		if (_event.GetStatus() == ewol::keyEvent::statusDown) {
			if (m_buffer != NULL) {
				//tmpBuffer->AddChar(_event.GetChar());
			}
			MarkToRedraw();
		}
		return true;
	}
	// move events ...
	if (_event.GetStatus() == ewol::keyEvent::statusDown) {
		if (m_buffer != NULL) {
			//tmpBuffer->cursorMove(_event.GetType());
		}
		MarkToRedraw();
	}
	return true;
}

void appl::TextViewer::OnEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID)
{
	if (m_buffer != NULL) {
		//tmpBuffer->Paste(_clipboardID);
	}
	MarkToRedraw();
}

bool appl::TextViewer::OnEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = RelativePosition(_event.GetPos());
	if (m_buffer != NULL) {
		
	}
	return true;
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

