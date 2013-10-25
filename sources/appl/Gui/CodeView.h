/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __CODE_VIEW_H__
#define __CODE_VIEW_H__

#include <appl/debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <appl/globalMsg.h>

#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>

class CodeView :public widget::WidgetScrooled
{
	public:
		void init(void);
		CodeView(etk::UString fontName, int32_t fontSize);
		CodeView(void);
		virtual ~CodeView(void);
	private:
		etk::UString m_label;
		etk::Color<> m_textColorFg;  //!< Text color
		etk::Color<> m_textColorBg;  //!< Background color
		int32_t m_bufferID;
		bool m_buttunOneSelected;
		etk::Vector<vec2 > m_lineNumberList;
		void updateNumberOfLineReference(int32_t bufferID);
		// drawing elements :
		ewol::Text m_displayText;
		ewol::drawing m_displayDrawing;
	public:
		void setFontSize(int32_t size);
		void setFontName(etk::UString fontName);
	private:
		void calculateMaxSize(void);
	protected: // derived function
		virtual void onDraw(void);
	public:  // Derived function
		const char * const getObjectType(void) { return "ApplCodeView"; };
		virtual bool calculateMinSize(void);
		virtual void onRegenerateDisplay(void);
		virtual void onReceiveMessage(const ewol::EMessage& _msg);
		virtual bool onEventInput(const ewol::EventInput& _event);
		virtual bool onEventEntry(const ewol::EventEntry& _event);
		virtual void onEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID);
		virtual void onGetFocus(void);
		virtual void onLostFocus(void);
};

#endif

