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

// TODO : Review this in a generic unique resource ...

static std::vector<std::shared_ptr<appl::Highlight>>& s_list() {
	static std::vector<std::shared_ptr<appl::Highlight>> list;
	return list;
}


void appl::highlightManager::init() {
	std::vector<std::shared_ptr<appl::Highlight>>& hlList = s_list();
	if (hlList.size() != 0) {
		APPL_ERROR("HighlightManager  == > already exist, just unlink the previous ...");
		hlList.clear();
	}
	APPL_DEBUG("HighlightManager  == > INIT");
	etk::FSNode myFile("DATA:languages/");
	// get the subfolder list :
	std::vector<etk::FSNode *> list = myFile.folderGetSubList(false, true, false,false);
	for (auto &it : list) {
		if (it == nullptr) {
			continue;
		}
		if (it->getNodeType() != etk::FSN_FOLDER) {
			continue;
		}
		std::string filename = it->getName() + "/highlight.xml";
		APPL_DEBUG("Load xml name : " << filename);
		std::shared_ptr<appl::Highlight> myHightLine = appl::Highlight::create(filename);
		if (myHightLine != nullptr) {
			hlList.push_back(myHightLine);
		} else {
			APPL_ERROR("Can not allocate HighLight");
		}
	}
	// display :
	/*
	for (auto &it : hlList) {
		if (it == nullptr) {
			continue;
		}
		it->display();
	}
	*/
}

void appl::highlightManager::unInit() {
	std::vector<std::shared_ptr<Highlight>>& hlList = s_list();
	if (hlList.size() == 0) {
		APPL_DEBUG("HighlightManager  ==> no highlight");
		hlList.clear();
		return;
	}
	hlList.clear();
}

std::string appl::highlightManager::getTypeExtention(const std::string& _extention) {
	if (_extention.size() == 0) {
		return "";
	}
	APPL_DEBUG("Try to find type for extention : '" << _extention << "' in " << s_list().size() << " types");
	std::vector<std::shared_ptr<Highlight>>& hlList = s_list();
	for (auto &it : hlList) {
		if (it == nullptr) {
			continue;
		}
		APPL_DEBUG("    check : " << it->getTypeName());
		if (it->hasExtention(_extention) == true) {
			APPL_DEBUG("Find type for extention : " << _extention
			             << " type : " << it->getTypeName());
			return it->getTypeName();
		}
	}
	return "";
}

std::string appl::highlightManager::getFileWithTypeType(const std::string& _type) {
	if (_type.size() == 0) {
		return "";
	}
	for (auto &it : s_list()) {
		if (it == nullptr) {
			continue;
		}
		if (it->getTypeName() == _type) {
			return it->getName();
		}
	}
	return "";
}

std::vector<std::string> appl::highlightManager::getTypeList() {
	std::vector<std::string> ret;
	return ret;
}


