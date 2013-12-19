/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/HighlightManager.h>
#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>

#undef __class__
#define __class__ "highlightManager"

static std::vector<appl::Highlight*>& s_list(void) {
	static std::vector<appl::Highlight*> list;
	return list;
}


void appl::highlightManager::init(void) {
	std::vector<appl::Highlight*>& hlList = s_list();
	if (hlList.size() != 0) {
		APPL_ERROR("HighlightManager  == > already exist, just unlink the previous ...");
		hlList.clear();
	}
	etk::FSNode myFile("DATA:languages/");
	// get the subfolder list :
	std::vector<etk::FSNode *> list = myFile.folderGetSubList(false, true, false,false);
	for (int32_t iii = 0; iii < list.size(); ++iii) {
		if (list[iii] == NULL) {
			continue;
		}
		if (list[iii]->getNodeType() != etk::FSN_FOLDER) {
			continue;
		}
		std::string filename = list[iii]->getName() + "/highlight.xml";
		APPL_DEBUG("Load xml name : " << filename);
		appl::Highlight *myHightLine = appl::Highlight::keep(filename);
		if (myHightLine != NULL) {
			hlList.push_back(myHightLine);
		} else {
			APPL_ERROR("Can not allocate HighLight");
		}
	}
	// display :
	/*
	for (int32_t iii = 0; iii < hlList.size(); ++iii) {
		if (hlList[iii] == NULL) {
			continue;
		}
		hlList[iii]->display();
	}
	*/
}

void appl::highlightManager::unInit(void) {
	std::vector<Highlight*>& hlList = s_list();
	if (hlList.size() == 0) {
		APPL_DEBUG("HighlightManager  ==> no highlight");
		hlList.clear();
		return;
	}
	for (int32_t iii = 0; iii < hlList.size(); ++iii) {
		if (hlList[iii] == NULL) {
			continue;
		}
		appl::Highlight::release(hlList[iii]);
		hlList[iii] = NULL;
	}
	hlList.clear();
}

std::string appl::highlightManager::getTypeExtention(const std::string& _extention) {
	if (_extention.size() == 0) {
		return "";
	}
	APPL_DEBUG("Try to find type for extention : '" << _extention << "' in " << s_list().size() << " types");
	std::vector<Highlight*>& hlList = s_list();
	for (int32_t iii = 0; iii < hlList.size(); ++iii) {
		if (hlList[iii] == NULL) {
			continue;
		}
		APPL_DEBUG("    check : " << hlList[iii]->getTypeName());
		if (hlList[iii]->hasExtention(_extention) == true) {
			APPL_DEBUG("Find type for extention : " << _extention
			             << " type : " << hlList[iii]->getTypeName());
			return hlList[iii]->getTypeName();
		}
	}
	return "";
}

std::string appl::highlightManager::getFileWithTypeType(const std::string& _type) {
	if (_type.size() == 0) {
		return "";
	}
	std::vector<Highlight*>& hlList = s_list();
	for (int32_t iii = 0; iii < hlList.size(); ++iii) {
		if (hlList[iii] == NULL) {
			continue;
		}
		if (hlList[iii]->getTypeName() == _type) {
			return hlList[iii]->getName();
		}
	}
	return "";
}

std::vector<std::string> appl::highlightManager::getTypeList(void) {
	std::vector<std::string> ret;
	return ret;
}


