/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <etk/types.h>
#include <etk/os/FSNode.h>
#include <appl/debug.h>
#include <appl/global.h>
#include <appl/BufferManager.h>
#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <esignal/Signal.h>
#include <gale/resource/Manager.h>

appl::BufferManager::BufferManager() :
  signalNewBuffer(this, "new-buffer", ""),
  signalSelectFile(this, "select-buffer", ""),
  signalTextSelectionChange(this, "text-selection-change", ""),
  signalRemoveBuffer(this, "remove-buffer", "") {
	addObjectType("appl::BufferManager");
}

appl::BufferManager::~BufferManager() {
	m_list.clear();
}


ememory::SharedPtr<appl::Buffer> appl::BufferManager::createNewBuffer() {
	ememory::SharedPtr<appl::Buffer> tmp = appl::Buffer::create();
	if (tmp == nullptr) {
		APPL_ERROR("Can not allocate the Buffer (empty).");
		return nullptr;
	}
	tmp->setParent(ewol::Object::sharedFromThis());
	m_list.push_back(tmp);
	APPL_INFO("Create a new Buffer");
	signalNewBuffer.emit(tmp->getFileName());
	APPL_INFO("Create a new Buffer (done)");
	APPL_INFO("select Buffer");
	signalSelectFile.emit(tmp->getFileName());
	APPL_INFO("select Buffer (done)");
	return tmp;
}

ememory::SharedPtr<appl::Buffer> appl::BufferManager::get(const std::string& _fileName, bool _createIfNeeded) {
	APPL_INFO("get('" << _fileName << "'," << _createIfNeeded << ")");
	for (auto &it : m_list) {
		if (it == nullptr) {
			continue;
		}
		if (it->getFileName() == _fileName) {
			return it;
		}
	}
	if (_createIfNeeded == true) {
		if (etk::FSNodeGetType(_fileName) == etk::typeNode_folder) {
			APPL_WARNING("try open a folder : " << _fileName);
			APPL_CRITICAL("plop");
			return nullptr;
		}
		ememory::SharedPtr<appl::Buffer> tmp = appl::Buffer::create();
		if (tmp == nullptr) {
			APPL_ERROR("Can not allocate the Buffer class : " << _fileName);
			return nullptr;
		}
		tmp->setParent(ewol::Object::sharedFromThis());
		tmp->loadFile(_fileName);
		m_list.push_back(tmp);
		APPL_INFO("Creata a open Buffer");
		signalNewBuffer.emit(tmp->getFileName());
		APPL_INFO("Creata a open Buffer (done)");
		return tmp;
	}
	return nullptr;
}

void appl::BufferManager::setBufferSelected(ememory::SharedPtr<appl::Buffer> _bufferSelected) {
	m_bufferSelected = _bufferSelected;
	if (m_bufferSelected == nullptr) {
		APPL_ERROR("select a NULL buffer ...");
		propertySetOnWidgetNamed("appl-widget-display-name", "value", "---");
		return;
	}
	APPL_INFO("Set buffer selected");
	//signalSelectFile.emit(m_bufferSelected->getName());
	//propertySetOnWidgetNamed("appl-widget-display-name", "value", m_bufferSelected->getName());
	APPL_INFO("Set buffer selected (done)");
}

ememory::SharedPtr<appl::Buffer> appl::BufferManager::get(int32_t _id) {
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
		APPL_WARNING(" the element '" << _fileName << "' already exist ... just reselect it ...");
		signalSelectFile.emit(_fileName);
		propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName(_fileName));
		return;
	}
	if (get(_fileName, true) == nullptr) {
		APPL_ERROR("Error get '" << _fileName << "' ... ");
		return;
	}
	signalSelectFile.emit(_fileName);
	propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName(_fileName));
}

void appl::BufferManager::requestDestroyFromChild(const ememory::SharedPtr<Object>& _child) {
	APPL_WARNING("Buffer request a close...");
	bool find = false;
	int32_t newValue = -1;
	auto it = m_list.begin();
	while(it != m_list.end()) {
		if (*it == nullptr) {
			it = m_list.erase(it);
			continue;
		}
		if (*it == _child) {
			it = m_list.erase(it);
			find = true;
			break;
		}
		newValue++;
		++it;
	}
	if (find == true) {
		signalRemoveBuffer.emit(ememory::dynamicPointerCast<appl::Buffer>(_child));
	}
	if (m_bufferSelected == _child) {
		if (    it != m_list.end()
		     && *it != nullptr) {
			APPL_VERBOSE("Remove buffer select new one");
			signalSelectFile.emit((*it)->getFileName());
			propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName((*it)->getFileName()));
			APPL_VERBOSE("Remove buffer select new one (done)");
			return;
		}
		if (    m_list.size() != 0
		     && m_list.back() != nullptr) {
			APPL_VERBOSE("Remove buffer select new one (last)");
			signalSelectFile.emit(m_list.back()->getFileName());
			propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName(m_list.back()->getFileName()));
			APPL_VERBOSE("Remove buffer select new one (done)");
			return;
		}
		signalSelectFile.emit("");
		propertySetOnWidgetNamed("appl-widget-display-name", "value", "---");
		m_bufferSelected = nullptr;
	}
}

