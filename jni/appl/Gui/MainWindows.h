/**
 *******************************************************************************
 * @file MainWindows.h
 * @brief Editeur De N'ours : main Windows diplayer (header)
 * @author Edouard DUPIN
 * @date 04/01/2011
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
#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__

#include <appl/Debug.h>
#include <appl/globalMsg.h>

#include <CodeView.h>
#include <BufferView.h>
#include <BufferManager.h>
#include <ewol/widget/Label.h>

extern const char * const TYPE_EOBJECT_EDN_MAIN_WINDOWS;

class MainWindows : public ewol::Windows
{
	private:
		int32_t m_currentSavingAsIdBuffer;
		ewol::Label* m_widgetLabelFileName;
	public:
		// Constructeur
		MainWindows(void);
		~MainWindows(void);
		/**
		 * @brief Check if the object has the specific type.
		 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
		 * @param[in] objectType type of the object we want to check
		 * @return true if the object is compatible, otherwise false
		 */
		bool CheckObjectType(const char * const objectType);
		/**
		 * @brief Get the current Object type of the EObject
		 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
		 * @param[in] objectType type description
		 * @return true if the object is compatible, otherwise false
		 */
		const char * const GetObjectType(void);
		/**
		 * @brief Receive a message from an other EObject with a specific eventId and data
		 * @param[in] CallerObject Pointer on the EObject that information came from
		 * @param[in] eventId Message registered by this class
		 * @param[in] data Data registered by this class
		 * @return ---
		 */
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		/**
		 * @brief Inform object that an other object is removed ...
		 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
		 * @note : Sub classes must call this class
		 * @return ---
		 */
		virtual void OnObjectRemove(ewol::EObject * removeObject);
};

#define EDN_CAST_MAIN_WINDOWS(curentPointer) EWOL_CAST(TYPE_EOBJECT_EDN_MAIN_WINDOWS,MainWindows,curentPointer)

#endif


