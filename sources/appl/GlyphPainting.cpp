/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/GlyphPainting.h>
#include <ejson/ejson.h>
#include <etk/os/FSNode.h>
#include <ewol/resources/ResourceManager.h>

#undef __class__
#define __class__ "GlyphPainting"



appl::GlyphPainting::GlyphPainting(const std::string& _filename) :
  ewol::Resource(_filename) {
	addObjectType("appl::GlyphPainting");
	EWOL_DEBUG("SFP : load \"" << _filename << "\"");
	reload();
}

appl::GlyphPainting::~GlyphPainting(void) {
	
}

void appl::GlyphPainting::reload(void) {
	ejson::Document doc;
	if (false == doc.load(m_name)) {
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
	ejson::Array* baseArray = doc.getArray("ednColor");
	if (baseArray == NULL) {
		APPL_ERROR("Can not get basic array : 'ednColor'");
		return;
	}
	for (esize_t iii = 0; iii < baseArray->size(); ++iii) {
		ejson::Object* tmpObj = baseArray->getObject(iii);
		if (tmpObj == NULL) {
			APPL_DEBUG(" can not get object in 'ednColor' id=" << iii);
			continue;
		}
		std::string name = tmpObj->getStringValue("name", "");
		std::string background = tmpObj->getStringValue("background", "#FFF0");
		std::string foreground = tmpObj->getStringValue("foreground", "#000F");
		bool italic = tmpObj->getBooleanValue("italic", false);
		bool bold = tmpObj->getBooleanValue("bold", false);
		APPL_VERBOSE("find new color : '" << name << "' fg='" << foreground << "' bg='" << background << "' italic='" << italic << "' bold='" << bold << "'");
		bool findElement = false;
		for (esize_t jjj=0; jjj<m_list.size(); ++jjj) {
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


esize_t appl::GlyphPainting::request(const std::string& _name) {
	for (esize_t iii=0; iii<m_list.size(); ++iii) {
		if (m_list[iii].getName() == _name) {
			return iii;
		}
	}
	// create an empty deco ...
	appl::GlyphDecoration tmpDeco(_name);
	m_list.push_back(tmpDeco);
	return m_list.size()-1;
}

appl::GlyphPainting* appl::GlyphPainting::keep(const std::string& _filename) {
	//EWOL_INFO("KEEP : appl::GlyphPainting : file : \"" << _filename << "\"");
	appl::GlyphPainting* object = static_cast<appl::GlyphPainting*>(getManager().localKeep(_filename));
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	EWOL_INFO("CREATE : appl::GlyphPainting : file : \"" << _filename << "\"");
	object = new appl::GlyphPainting(_filename);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??GlyphPainting??");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void appl::GlyphPainting::release(appl::GlyphPainting*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}

