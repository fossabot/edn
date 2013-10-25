/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <HighlightManager.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>

#undef __class__
#define __class__ "highlightManager"

static etk::Vector<appl::Highlight*>& s_list(void) {
	static etk::Vector<appl::Highlight*> list;
	return list;
}


void appl::highlightManager::init(void) {
	etk::Vector<appl::Highlight*>& hlList = s_list();
	if (hlList.size() != 0) {
		APPL_ERROR("HighlightManager  == > already exist, just unlink the previous ...");
		hlList.clear();
	}
	etk::FSNode myFile("DATA:languages/");
	// get the subfolder list :
	etk::Vector<etk::FSNode *> list = myFile.folderGetSubList(false, true, false,false);
	for (esize_t iii = 0; iii < list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->getNodeType() != etk::FSN_FOLDER) {
			continue;
		}
		etk::UString filename = list[iii]->getName() + "/highlight.xml";
		APPL_DEBUG("Load xml name : " << filename);
		appl::Highlight *myHightLine = appl::Highlight::keep(filename);
		if (myHightLine != NULL) {
			hlList.pushBack(myHightLine);
		} else {
			APPL_ERROR("Can not allocate HighLight");
		}
	}
	// display :
	for (esize_t iii = 0; iii < hlList.size(); ++iii) {
		if (hlList[iii] == NULL) {
			continue;
		}
		hlList[iii]->display();
	}
}

void appl::highlightManager::unInit(void) {
	etk::Vector<Highlight*>& hlList = s_list();
	if (hlList.size() == 0) {
		APPL_DEBUG("HighlightManager  ==> no highlight");
		hlList.clear();
		return;
	}
	for (esize_t iii = 0; iii < hlList.size(); ++iii) {
		if (hlList[iii] == NULL) {
			continue;
		}
		appl::Highlight::release(hlList[iii]);
		hlList[iii] = NULL;
	}
	hlList.clear();
}

etk::UString appl::highlightManager::getTypeExtention(const etk::UString& _extention) {
	return "";
}

etk::Vector<etk::UString> appl::highlightManager::getTypeList(void) {
	etk::Vector<etk::UString> ret;
	return ret;
}


