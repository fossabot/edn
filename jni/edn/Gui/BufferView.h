/**
 *******************************************************************************
 * @file BufferView.h
 * @brief Editeur De N'ours : main List Buffer Viewer (header)
 * @author Edouard DUPIN
 * @date 09/12/2010
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
#ifndef __BUFFER_VIEW_H__
#define __BUFFER_VIEW_H__

#include <tools_debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <Display.h>
#include <MsgBroadcast.h>
#include <ewol/widget/List.h>



class BufferView : public ewol::List
{
	public:
		// Constructeur
		BufferView(void);
		~BufferView(void);
		bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
	protected:
		// function call to display the list :
		virtual color_ts GetBasicBG(void);
		virtual uint32_t GetNuberOfColomn(void);
		virtual bool GetTitle(int32_t colomn, etk::UString &myTitle, color_ts &fg, color_ts &bg);
		virtual uint32_t GetNuberOfRaw(void);
		virtual bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, color_ts &fg, color_ts &bg);
		virtual bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent,  int32_t colomn, int32_t raw, etkFloat_t x, etkFloat_t y);
	private:
		// récupération des proprieter général...
		BufferManager *       m_bufferManager;
		ColorizeManager *     m_colorManager;
		int32_t               m_selectedID;
};


#endif


