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

#include <appl/Debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <appl/globalMsg.h>
#include <ewol/widget/List.h>

class BufferView : public ewol::List
{
	public:
		// Constructeur
		BufferView(void);
		~BufferView(void);
		/**
		 * @brief Get the current Object type of the EObject
		 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
		 * @param[in] objectType type description
		 * @return true if the object is compatible, otherwise false
		 */
		const char * const GetObjectType(void) { return "ApplBufferView"; };
		/**
		 * @brief Receive a message from an other EObject with a specific eventId and data
		 * @param[in] CallerObject Pointer on the EObject that information came from
		 * @param[in] eventId Message registered by this class
		 * @param[in] data Data registered by this class
		 * @return ---
		 */
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
	protected:
		// function call to display the list :
		virtual etk::Color GetBasicBG(void);
		virtual uint32_t GetNuberOfColomn(void);
		virtual bool GetTitle(int32_t colomn, etk::UString &myTitle, etk::Color &fg, etk::Color &bg);
		virtual uint32_t GetNuberOfRaw(void);
		virtual bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, etk::Color &fg, etk::Color &bg);
		virtual bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent,  int32_t colomn, int32_t raw, float x, float y);
	private:
		int32_t               m_selectedIdRequested;
		int32_t               m_selectedID;
};


#endif

