/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <HighlightManager.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>

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
		
		// herited function
		const char * const GetObjectType(void)
		{
			return "ApplHighlightManager";
		}
		
		// herited function
		virtual void OnReceiveMessage(const ewol::EMessage& _msg)
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
			etk::FSNode myFile("DATA:languages/");
			// get the subfolder list :
			etk::Vector<etk::FSNode *> list = myFile.FolderGetSubList(false, true, false,false);
			for ( int32_t iii=0 ; iii<list.Size() ; iii++ ) {
				if (NULL!=list[iii]) {
					if (list[iii]->GetNodeType()==etk::FSN_FOLDER) {
						etk::UString filename = list[iii]->GetName() + "/highlight.xml";
						APPL_DEBUG("Load xml name : " << filename);
						Highlight *myHightline = new Highlight(filename);
						listHighlight.PushBack(myHightline);
					}
				}
			}
			//myHightline->Display();
		}

};

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



