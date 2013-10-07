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
			//ewol::widgetMessageMultiCast::add(getWidgetId(), ednMsgBufferColor);
		};
		~localClassHighlightManager(void) {
			int32_t i;
			// clean all Element
			for (i=0; i< listHighlight.size(); i++) {
				if (NULL != listHighlight[i]) {
					delete(listHighlight[i]);
					listHighlight[i] = NULL;
				}
			}
			// clear the compleate list
			listHighlight.clear();
		};
		
		// herited function
		const char * const getObjectType(void)
		{
			return "ApplHighlightManager";
		}
		
		// herited function
		virtual void onReceiveMessage(const ewol::EMessage& _msg)
		{
			/*
			switch (id)
			{
				case APPL_MSG__COLOR_HAS_CHANGE:
					APPL_INFO("UPDATE the color pointer on the HL");
					for (int32_t i=0; i<listHighlight.size(); i++) {
						if (NULL != listHighlight[i]) {
							listHighlight[i]->ReloadColor();
						}
					}
					break;
			}
			*/
		}
		
		Highlight* get(etk::FSNode &fileName)
		{
			int32_t i;
			for (i=0; i<listHighlight.size(); i++) {
				if (true == listHighlight[i]->fileNameCompatible(fileName) ) {
					return listHighlight[i];
				}
			}
			return NULL;
		}
		
		bool Exist(etk::FSNode &fileName)
		{
			if (NULL != get(fileName) ) {
				return true;
			}
			return false;
		}
		
		
		void loadLanguages(void)
		{
			etk::FSNode myFile("DATA:languages/");
			// get the subfolder list :
			etk::Vector<etk::FSNode *> list = myFile.FolderGetSubList(false, true, false,false);
			for ( int32_t iii=0 ; iii<list.size() ; iii++ ) {
				if (NULL!=list[iii]) {
					if (list[iii]->getNodeType() == etk::FSN_FOLDER) {
						etk::UString filename = list[iii]->getName() + "/highlight.xml";
						APPL_DEBUG("Load xml name : " << filename);
						Highlight *myHightline = new Highlight(filename);
						listHighlight.pushBack(myHightline);
					}
				}
			}
			//myHightline->display();
		}

};

static localClassHighlightManager * localManager = NULL;



void HighlightManager::init(void)
{
	if (NULL != localManager) {
		APPL_ERROR("HighlightManager  == > already exist, just unlink the previous ...");
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
		APPL_ERROR("HighlightManager  == > request UnInit, but does not exist ...");
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

Highlight* HighlightManager::get(etk::FSNode &fileName)
{
	if (NULL == localManager) {
		return NULL;
	}
	return localManager->get(fileName);
}

bool HighlightManager::Exist(etk::FSNode &fileName)
{
	if (NULL == localManager) {
		return false;
	}
	return localManager->Exist(fileName);
}



