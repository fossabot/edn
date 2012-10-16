/**
 *******************************************************************************
 * @file TagFileList.h
 * @brief Editeur De N'ours : Tags list display to jump (header)
 * @author Edouard DUPIN
 * @date 16/10/2012
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

#ifndef __APPL_CTAGS_LIST_H__
#define __APPL_CTAGS_LIST_H__


#include <etk/Types.h>
#include <appl/Debug.h>
#include <ewol/widget/List.h>


extern const char * const applEventCtagsListSelect;
extern const char * const applEventCtagsListValidate;

namespace appl {
	class TagFileList : public ewol::List
	{
		private:
			int32_t                          m_selectedLine;
			etk::Vector<etk::UString *>     m_list;
		public:
			TagFileList(void);
			~TagFileList(void);
			// display API :
			virtual draw::Color GetBasicBG(void);
			uint32_t GetNuberOfColomn(void);
			bool GetTitle(int32_t colomn, etk::UString &myTitle, draw::Color &fg, draw::Color &bg);
			uint32_t GetNuberOfRaw(void);
			bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, draw::Color &fg, draw::Color &bg);
			bool OnItemEvent(int32_t IdInput, ewol::eventInputType_te typeEvent, int32_t colomn, int32_t raw, float x, float y);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			const char * const GetObjectType(void) { return "TagFileList"; };
		public:
			/**
			 * @brief Add a Ctags item on the curent list
			 * @param[in] file Compleate file name
			 * @param[in] jump line id
			 * @return ---
			 */
			void         Add(etk::UString file, int32_t line);
	};
};



#endif


