/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/BufferManager.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>

#undef __class__
#define __class__ "BufferManager"

appl::BufferManager::BufferManager(void) :
  ewol::Resource("???BufferManager???"),
  m_bufferSelected(NULL) {
	addObjectType("appl::BufferManager");
}

appl::BufferManager::~BufferManager(void) {
	esize_t previousCount = m_list.size();
	for (esize_t iii = m_list.size()-1; iii >= 0 ; --iii) {
		if (m_list[iii] == NULL) {
			continue;
		}
		delete(m_list[iii]);
		if (previousCount == m_list.size()) {
			APPL_ERROR("Error in removing buffer !! ");
		}
		previousCount = m_list.size();
	}
	m_list.clear();
}


appl::Buffer* appl::BufferManager::createNewBuffer(void) {
	appl::Buffer* tmp = new appl::Buffer();
	if (tmp == NULL) {
		APPL_ERROR("Can not allocate the Buffer (empty).");
		return NULL;
	}
	m_list.push_back(tmp);
	sendMultiCast(appl::MsgSelectNewFile, tmp->getFileName());
	return tmp;
}

appl::Buffer* appl::BufferManager::get(const std::string& _fileName, bool _createIfNeeded) {
	for (esize_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii] == NULL) {
			continue;
		}
		if (m_list[iii]->getFileName() == _fileName) {
			return m_list[iii];
		}
	}
	if (_createIfNeeded == true) {
		appl::Buffer* tmp = new appl::Buffer();
		if (tmp == NULL) {
			APPL_ERROR("Can not allocate the Buffer class : " << _fileName);
			return NULL;
		}
		tmp->loadFile(_fileName);
		m_list.push_back(tmp);
		return tmp;
	}
	return NULL;
}
void appl::BufferManager::setBufferSelected(appl::Buffer* _bufferSelected) {
	m_bufferSelected = _bufferSelected;
	sendMultiCast(appl::MsgSelectChange, "");
}

void appl::BufferManager::onObjectRemove(ewol::EObject * _removeObject) {
	if (m_bufferSelected == _removeObject) {
		setBufferSelected(NULL);
	}
	for (esize_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii] != _removeObject) {
			continue;
		}
		m_list[iii] = NULL;
		m_list.erase(m_list.begin()+iii);
		return;
	}
}

bool appl::BufferManager::exist(const std::string& _fileName) {
	for (esize_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii] == NULL) {
			continue;
		}
		if (m_list[iii]->getFileName() == _fileName) {
			return true;
		}
	}
	return false;
}

void appl::BufferManager::open(const std::string& _fileName) {
	if (exist(_fileName) == false) {
		(void)get(_fileName, true);
		sendMultiCast(appl::MsgSelectNewFile, _fileName);
	}
}

void appl::BufferManager::onReceiveMessage(const ewol::EMessage& _msg) {
	APPL_DEBUG("receive message !!! " << _msg);
}

appl::BufferManager* appl::BufferManager::keep(void) {
	//EWOL_INFO("KEEP : appl::GlyphPainting : file : \"" << _filename << "\"");
	appl::BufferManager* object = static_cast<appl::BufferManager*>(getManager().localKeep("???BufferManager???"));
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	EWOL_INFO("CREATE : appl::BufferManager: ???BufferManager???");
	object = new appl::BufferManager();
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ???BufferManager???");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void appl::BufferManager::release(appl::BufferManager*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}
