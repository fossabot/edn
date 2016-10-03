/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/debug.hpp>
#include <appl/global.hpp>
#include <appl/HighlightManager.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/object/Manager.hpp>


// TODO : Review this in a generic unique resource ...

static std::vector<ememory::SharedPtr<appl::Highlight>>& s_list() {
	static std::vector<ememory::SharedPtr<appl::Highlight>> list;
	return list;
}


void appl::highlightManager::init() {
	std::vector<ememory::SharedPtr<appl::Highlight>>& hlList = s_list();
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
		if (it->getNodeType() != etk::typeNode_folder) {
			continue;
		}
		std::string filename = it->getName() + "/highlight.xml";
		APPL_DEBUG("Load xml name : " << filename);
		ememory::SharedPtr<appl::Highlight> myHightLine = appl::Highlight::create(filename);
		if (myHightLine != nullptr) {
			// Check if the language name already exist
			for (auto &it2 : hlList) {
				if (    it2 != nullptr
				     && it2->getTypeName() == myHightLine->getTypeName() ) {
					APPL_WARNING("LANGUAGE : replace pattern name: '" << myHightLine->getTypeName() << "' with file '" << filename << "' replace: " << it2->getName());
				}
			}
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
	std::vector<ememory::SharedPtr<Highlight>>& hlList = s_list();
	if (hlList.size() == 0) {
		APPL_DEBUG("HighlightManager  ==> no highlight");
		hlList.clear();
		return;
	}
	hlList.clear();
}

std::string appl::highlightManager::getTypeFile(const std::string& _fileName) {
	if (_fileName.size() == 0) {
		return "";
	}
	APPL_DEBUG("Try to find type for extention : '" << _fileName << "' in " << s_list().size() << " types");
	std::vector<ememory::SharedPtr<Highlight>>& hlList = s_list();
	for (auto &it : hlList) {
		if (it == nullptr) {
			continue;
		}
		APPL_DEBUG("    check : " << it->getTypeName());
		if (it->isCompatible(_fileName) == true) {
			APPL_DEBUG("Find type for extention : " << _fileName
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


