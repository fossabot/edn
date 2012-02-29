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

#include <tools_debug.h>
#include <tools_globals.h>
#include <HighlightManager.h>
#include <ewol/EObject.h>
#include <ewol/EObjectManager.h>

#undef __class__
#define __class__	"HighlightManager"


class localClassHighlightManager: public ewol::EObject
{
	private:
		etk::VectorType<Highlight*> listHighlight;		//!< List of ALL hightlight modules
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
				case EDN_MSG__COLOR_HAS_CHANGE:
					EDN_INFO("UPDATE the color pointer on the HL");
					for (int32_t i=0; i<listHighlight.Size(); i++) {
						if (NULL != listHighlight[i]) {
							listHighlight[i]->ReloadColor();
						}
					}
					break;
			}
			*/
		}
		
		Highlight* Get(etk::File &fileName)
		{
			int32_t i;
			for (i=0; i<listHighlight.Size(); i++) {
				if (true == listHighlight[i]->FileNameCompatible(fileName) ) {
					return listHighlight[i];
				}
			}
			return NULL;
		}
		
		bool Exist(etk::File &fileName)
		{
			if (NULL != Get(fileName) ) {
				return true;
			}
			return false;
		}
		
		
		void loadLanguages(void)
		{
			etk::UString xmlFilename = "lang_c.xml";
			Highlight *myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_boo.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_Makefile.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_asm.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_xml.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_php.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_bash.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_matlab.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			xmlFilename = "lang_java.xml";
			myHightline = new Highlight(xmlFilename);
			listHighlight.PushBack(myHightline);
			
			//myHightline->Display();
		}

};


static localClassHighlightManager * localManager = NULL;



void HighlightManager::Init(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("HighlightManager ==> already exist, just unlink the previous ...");
		localManager = NULL;
	}
	localManager = new localClassHighlightManager();
	
	if (NULL == localManager) {
		EWOL_CRITICAL("Allocation of HighlightManager not done ...");
	}
}

void HighlightManager::UnInit(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("HighlightManager ==> request UnInit, but does not exist ...");
		return;
	}
	ewol::EObjectManager::MarkToRemoved(localManager);
	localManager = NULL;
}

void HighlightManager::loadLanguages(void)
{
	if (NULL == localManager) {
		return;
	}
	localManager->loadLanguages();
}

Highlight* HighlightManager::Get(etk::File &fileName)
{
	if (NULL == localManager) {
		return NULL;
	}
	return localManager->Get(fileName);
}

bool HighlightManager::Exist(etk::File &fileName)
{
	if (NULL == localManager) {
		return false;
	}
	return localManager->Exist(fileName);
}



