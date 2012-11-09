/**
 *******************************************************************************
 * @file HighlightManager.cpp
 * @brief Editeur De N'ours : Hightlining Manager
 * @author Edouard DUPIN
 * @date 16/12/2010
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

#include <appl/Debug.h>
#include <appl/global.h>
#include <HighlightManager.h>
#include <ewol/eObject/EObject.h>
#include <ewol/eObject/EObjectManager.h>

#undef __class__
#define __class__	"HighlightManager"

class localClassHighlightManager: public ewol::EObject
{
	private:
		etk::Vector<Highlight*> listHighlight;		//!< List of ALL hightlight modules
	public:
		// Constructeur
		localClassHighlightManager(void) {
			//ewol::widgetMessageMultiCast::Add(GetWidgetId(), ednMsgBufferColor);
		};
		~localClassHighlightManager(void) {
			int32_t i;
			// clean all Element
			for (i=0; i< listHighlight.Size(); i++) {
				if (NULL != listHighlight[i]) {
					delete(listHighlight[i]);
					listHighlight[i] = NULL;
				}
			}
			// clear the compleate list
			listHighlight.Clear();
		};
		
		/**
		 * @brief Get the current Object type of the EObject
		 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
		 * @param[in] objectType type description
		 * @return true if the object is compatible, otherwise false
		 */
		const char * const GetObjectType(void)
		{
			return "ApplHighlightManager";
		}
		
		/**
		 * @brief Receive a message from an other EObject with a specific eventId and data
		 * @param[in] CallerObject Pointer on the EObject that information came from
		 * @param[in] eventId Message registered by this class
		 * @param[in] data Data registered by this class
		 * @return ---
		 */
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
		{
			/*
			switch (id)
			{
				case APPL_MSG__COLOR_HAS_CHANGE:
					APPL_INFO("UPDATE the color pointer on the HL");
					for (int32_t i=0; i<listHighlight.Size(); i++) {
						if (NULL != listHighlight[i]) {
							listHighlight[i]->ReloadColor();
						}
					}
					break;
			}
			*/
		}
		
		Highlight* Get(etk::FSNode &fileName)
		{
			int32_t i;
			for (i=0; i<listHighlight.Size(); i++) {
				if (true == listHighlight[i]->FileNameCompatible(fileName) ) {
					return listHighlight[i];
				}
			}
			return NULL;
		}
		
		bool Exist(etk::FSNode &fileName)
		{
			if (NULL != Get(fileName) ) {
				return true;
			}
			return false;
		}
		
		
		void loadLanguages(void)
		{
			etk::UString xmlFilename = "languages/c/highlight.xml";
			Highlight *myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/boo/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/makefile/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/asm/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/xml/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/php/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/bash/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/matlab/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/java/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/lua/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/in/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "languages/glsl/highlight.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			//myHightline->Display();
		}

};

#define EDN_CAST_HIGHLIGHT_MANAGER(curentPointer) EWOL_CAST(TYPE_EOBJECT_EDN_HIGHLIGHT_MANAGER,localClassHighlightManager,curentPointer)

static localClassHighlightManager * localManager = NULL;



void HighlightManager::Init(void)
{
	if (NULL != localManager) {
		APPL_ERROR("HighlightManager ==> already exist, just unlink the previous ...");
		localManager = NULL;
	}
	localManager = new localClassHighlightManager();
	
	if (NULL == localManager) {
		APPL_CRITICAL("Allocation of HighlightManager not done ...");
	}
}

void HighlightManager::UnInit(void)
{
	if (NULL == localManager) {
		APPL_ERROR("HighlightManager ==> request UnInit, but does not exist ...");
		return;
	}
	delete(localManager);
	localManager = NULL;
}

void HighlightManager::loadLanguages(void)
{
	if (NULL == localManager) {
		return;
	}
	localManager->loadLanguages();
}

Highlight* HighlightManager::Get(etk::FSNode &fileName)
{
	if (NULL == localManager) {
		return NULL;
	}
	return localManager->Get(fileName);
}

bool HighlightManager::Exist(etk::FSNode &fileName)
{
	if (NULL == localManager) {
		return false;
	}
	return localManager->Exist(fileName);
}



