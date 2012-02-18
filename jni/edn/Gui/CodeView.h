/**
 *******************************************************************************
 * @file CodeView.h
 * @brief Editeur De N'ours : Code Viewer Widget (header)
 * @author Edouard DUPIN
 * @date 05/01/2011
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

#ifndef __CODE_VIEW_H__
#define __CODE_VIEW_H__

#include <tools_debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <Display.h>
#include <MsgBroadcast.h>

#include <etk/Types.h>
#include <ewol/widget/WidgetScrolled.h>

class CodeView :public ewol::WidgetScrooled
{
	public:
		         CodeView(void);
		virtual ~CodeView(void);
		virtual bool   CalculateMinSize(void);
	private:
		etk::UString                 m_label;
		color_ts                     m_textColorFg;  //!< Text color
		color_ts                     m_textColorBg;  //!< Background color
		BufferManager *              m_bufferManager;
		ColorizeManager *            m_colorManager;
		int32_t                      m_bufferID;
		bool                         m_buttunOneSelected;
		// drawing elements :
		ewol::OObject2DTextColored   m_OObjectTextNormal[NB_BOUBLE_BUFFER];
		ewol::OObject2DTextColored   m_OObjectTextBold[NB_BOUBLE_BUFFER];
		ewol::OObject2DTextColored   m_OObjectTextItalic[NB_BOUBLE_BUFFER];
		ewol::OObject2DTextColored   m_OObjectTextBoldItalic[NB_BOUBLE_BUFFER];
		ewol::OObject2DColored       m_OObjectsColored[NB_BOUBLE_BUFFER];
		
	public:
		virtual void   OnRegenerateDisplay(void);
		bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
	public:
		virtual bool OnEventInput(int32_t IdInput, ewol::eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
		virtual bool OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData);
		virtual bool OnEventKbMove(ewol::eventKbType_te typeEvent, ewol::eventKbMoveType_te moveTypeEvent);
		virtual void OnGetFocus(void);
		virtual void OnLostFocus(void);
	private:
		int32_t m_fontSize;
		int32_t m_fontNormal;
		int32_t m_fontBold;
		int32_t m_fontItalic;
		int32_t m_fontBoldItalic;
	public:
		void SetFontSize(int32_t size);
		void SetFontNameNormal(etk::UString fontName);
		void SetFontNameBold(etk::UString fontName);
		void SetFontNameItalic(etk::UString fontName);
		void SetFontNameBoldItalic(etk::UString fontName);
	private:
		void CalculateMaxSize(void);
	// widget drawing : 
	protected:
		virtual bool OnDraw(void);
};

#endif

