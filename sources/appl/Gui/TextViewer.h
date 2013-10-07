/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_TEXT_VIEWER_H__
#define __APPL_TEXT_VIEWER_H__

#include <appl/Debug.h>
#include <CodeView.h>
#include <appl/Buffer/Buffer.h>
#include <appl/globalMsg.h>

#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>

namespace appl
{
	class TextViewer : public widget::WidgetScrooled
	{
		public:
			TextViewer(const etk::UString& _fontName="", int32_t _fontSize=-1);
			virtual ~TextViewer(void);
		private:
			appl::Buffer* m_buffer; //!< pointer on the current buffer to display (can be null if the buffer is remover or in state of changing buffer)
			ewol::Text m_displayText; //!< Text display properties.
			ewol::drawing m_displayDrawing; //!< Other diaplay requested.
		public:
			void setFontSize(int32_t _size);
			void setFontName(const etk::UString& _fontName);
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
		private:
			bool m_insertMode; //!< the insert mode is enable
	};
};

#endif

