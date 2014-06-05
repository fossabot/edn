/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <etk/types.h>
#include <etk/os/FSNode.h>
#include <appl/debug.h>
#include <appl/global.h>
#include <appl/BufferManager.h>
#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <ewol/resource/Manager.h>

#undef __class__
#define __class__ "BufferManager"

appl::BufferManager::BufferManager() :
  ewol::Resource("???BufferManager???"),
  m_bufferSelected(nullptr) {
	addObjectType("appl::BufferManager");
}

appl::BufferManager::~BufferManager() {
	m_list.clear();
}


ewol::object::Shared<appl::Buffer> appl::BufferManager::createNewBuffer() {
	ewol::object::Shared<appl::Buffer> tmp = ewol::object::makeShared(new appl::Buffer());
	if (tmp == nullptr) {
		APPL_ERROR("Can not allocate the Buffer (empty).");
		return nullptr;
	}
	m_list.push_back(tmp);
	sendMultiCast(appl::MsgSelectNewFile, tmp->getFileName());
	return tmp;
}

ewol::object::Shared<appl::Buffer> appl::BufferManager::get(const std::string& _fileName, bool _createIfNeeded) {
	APPL_INFO("get(" << _fileName << "," << _createIfNeeded << ")");
	for (auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		if (it->getFileName() == _fileName) {
			return it;
		}
	}
	if (_createIfNeeded == true) {
		if (etk::FSNodeGetType(_fileName) == etk::FSN_FOLDER) {
			APPL_WARNING("try open a folder : " << _fileName);
			APPL_CRITICAL("plop");
			return nullptr;
		}
		ewol::object::Shared<appl::Buffer> tmp = ewol::object::makeShared(new appl::Buffer());
		if (tmp == nullptr) {
			APPL_ERROR("Can not allocate the Buffer class : " << _fileName);
			return nullptr;
		}
		tmp->loadFile(_fileName);
		m_list.push_back(tmp);
		return tmp;
	}
	return nullptr;
}
void appl::BufferManager::setBufferSelected(ewol::object::Shared<appl::Buffer> _bufferSelected) {
	m_bufferSelected = _bufferSelected;
	sendMultiCast(appl::MsgSelectChange, "");
}

void appl::BufferManager::onObjectRemove(const ewol::object::Shared<ewol::Object>& _object) {
	ewol::Resource::onObjectRemove(_object);
	if (m_bufferSelected == _object) {
		setBufferSelected(nullptr);
	}
	for (auto it(m_list.begin()); it!=m_list.end(); ++it) {
		if (*it != _object) {
			continue;
		}
		m_list.erase(it);
		it = m_list.begin();
	}
}

ewol::object::Shared<appl::Buffer> appl::BufferManager::get(int32_t _id) {
	int32_t id = 0;
	for (auto &it : m_list) {
		if (id == _id) {
			return it;
		}
		id++;
	}
	return m_list.back();
}

bool appl::BufferManager::exist(const std::string& _fileName) {
	for (auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		if (it->getFileName() == _fileName) {
			return true;
		}
	}
	return false;
}

void appl::BufferManager::open(const std::string& _fileName) {
	if (exist(_fileName) == true) {
		return;
	}
	if (get(_fileName, true) == nullptr) {
		return;
	}
	sendMultiCast(appl::MsgSelectNewFile, _fileName);
}

void appl::BufferManager::onReceiveMessage(const ewol::object::Message& _msg) {
	APPL_DEBUG("receive message !!! " << _msg);
}

ewol::object::Shared<appl::BufferManager> appl::BufferManager::keep() {
	ewol::object::Shared<appl::BufferManager> object = ewol::dynamic_pointer_cast<appl::BufferManager>(getManager().localKeep("???BufferManager???"));
	if (nullptr != object) {
		return object;
	}
	// this element create a new one every time ....
	EWOL_INFO("CREATE : appl::BufferManager: ???BufferManager???");
	object = ewol::object::makeShared(new appl::BufferManager());
	if (nullptr == object) {
		EWOL_ERROR("allocation error of a resource : ???BufferManager???");
		return nullptr;
	}
	getManager().localAdd(object);
	return object;
}

