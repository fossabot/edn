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
	private:
		etk::UString m_label;
		draw::Color m_textColorFg;  //!< Text color
		draw::Color m_textColorBg;  //!< Background color
		int32_t m_bufferID;
		bool m_buttunOneSelected;
		etk::Vector<vec2 > m_lineNumberList;
		void UpdateNumberOfLineReference(int32_t bufferID);
		// drawing elements :
		ewol::Text m_displayText;
		ewol::Drawing m_displayDrawing;
	public:
		void SetFontSize(int32_t size);
		void SetFontName(etk::UString fontName);
	private:
		void CalculateMaxSize(void);
	protected: // derived function
		virtual void OnDraw(void);
	public:  // Derived function
		const char * const GetObjectType(void) { return "ApplCodeView"; };
		virtual bool CalculateMinSize(void);
		virtual void OnRegenerateDisplay(void);
		virtual void OnReceiveMessage(const ewol::EMessage& _msg);
		virtual bool OnEventInput(const ewol::EventInput& _event);
		virtual bool OnEventEntry(const ewol::EventEntry& _event);
		virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID);
		virtual void OnGetFocus(void);
		virtual void OnLostFocus(void);
};

#endif

