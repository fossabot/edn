/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __CODE_VIEW_H__
#define __CODE_VIEW_H__

#include <appl/Debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <appl/globalMsg.h>

#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>

class CodeView :public widget::WidgetScrooled
{
	public:
		void Init(void);
		CodeView(etk::UString fontName, int32_t fontSize);
		CodeView(void);
		virtual ~CodeView(void);
		// Derived function
		const char * const GetObjectType(void) { return "ApplCodeView"; };
		// Derived function
		virtual bool   CalculateMinSize(void);
	private:
		etk::UString                   m_label;
		draw::Color                    m_textColorFg;  //!< Text color
		draw::Color                    m_textColorBg;  //!< Background color
		int32_t                        m_bufferID;
		bool                           m_buttunOneSelected;
		etk::Vector<vec2 >  m_lineNumberList;
		void UpdateNumberOfLineReference(int32_t bufferID);
		// drawing elements :
		ewol::Text                   m_displayText;
		ewol::Drawing                m_displayDrawing;
	public:
		// Derived function
		virtual void   OnRegenerateDisplay(void);
		// Derived function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
	public:
		// Derived function
		virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, vec2 pos);
		// Derived function
		virtual bool OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData);
		// Derived function
		virtual bool OnEventKbMove(ewol::keyEvent::status_te typeEvent, ewol::keyEvent::keyboard_te moveTypeEvent);
		// Derived function
		virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID);
		// Derived function
		virtual void OnGetFocus(void);
		// Derived function
		virtual void OnLostFocus(void);
	public:
		void SetFontSize(int32_t size);
		void SetFontName(etk::UString fontName);
	private:
		void CalculateMaxSize(void);
	protected:
		virtual void OnDraw(ewol::DrawProperty& displayProp);
};

#endif

