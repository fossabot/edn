/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/debug.h>
#include <appl/global.h>
#include <appl/GlyphPainting.h>
#include <ejson/ejson.h>
#include <etk/os/FSNode.h>
#include <gale/resource/Manager.h>


appl::GlyphPainting::GlyphPainting() {
	addResourceType("appl::GlyphPainting");
}

void appl::GlyphPainting::init(const std::string& _filename) {
	gale::Resource::init(_filename);
	APPL_DEBUG("SFP : load \"" << _filename << "\"");
	reload();
}

appl::GlyphPainting::~GlyphPainting() {
	
}

void appl::GlyphPainting::reload() {
	ejson::Document doc;
	if (doc.load(m_name) == false) {
		APPL_ERROR("Can not load file : '" << m_name << "' = " << etk::FSNode(m_name).getFileSystemName());
		return;
	}
	// for debug only :
	/*
	APPL_WARNING("Load file : '" << m_name << "' = " << etk::FSNode(m_name).getFileSystemName());
	std::string tmppppp;
	doc.generate(tmppppp);
	APPL_DEBUG(tmppppp);
	*/
	ejson::Array baseArray = doc["ednColor"].toArray();
	if (baseArray.exist() == false) {
		APPL_ERROR("Can not get basic array : 'ednColor'");
		return;
	}
	for (const auto it : baseArray) {
		ejson::Object tmpObj = it.toObject();
		if (tmpObj.exist() == false) {
			APPL_DEBUG(" can not get object in 'ednColor' it=" << it);
			continue;
		}
		std::string name = tmpObj["name"].toString().get();
		std::string background = tmpObj["background"].toString().get("#FFF0");
		std::string foreground = tmpObj["foreground"].toString().get("#000F");
		bool italic = tmpObj["italic"].toBoolean().get(false);
		bool bold = tmpObj["bold"].toBoolean().get(false);
		APPL_VERBOSE("find new color : '" << name << "' fg='" << foreground << "' bg='" << background << "' italic='" << italic << "' bold='" << bold << "'");
		bool findElement = false;
		for (size_t jjj=0; jjj<m_list.size(); ++jjj) {
			if (m_list[jjj].getName() != name) {
				continue;
			}
			m_list[jjj].setForeground(foreground);
			m_list[jjj].setBackground(background);
			m_list[jjj].setItalic(italic);
			m_list[jjj].setBold(bold);
			findElement = true;
		}
		if (findElement == true) {
			continue;
		}
		appl::GlyphDecoration tmpDeco(name);
		tmpDeco.setForeground(foreground);
		tmpDeco.setBackground(background);
		tmpDeco.setItalic(italic);
		tmpDeco.setBold(bold);
		m_list.push_back(tmpDeco);
	}
}


int32_t appl::GlyphPainting::request(const std::string& _name) {
	for (size_t iii=0; iii<m_list.size(); ++iii) {
		if (m_list[iii].getName() == _name) {
			return iii;
		}
	}
	// create an empty deco ...
	appl::GlyphDecoration tmpDeco(_name);
	m_list.push_back(tmpDeco);
	return m_list.size()-1;
}
