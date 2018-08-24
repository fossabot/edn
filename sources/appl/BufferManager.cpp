/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <etk/types.hpp>
#include <etk/os/FSNode.hpp>
#include <appl/debug.hpp>
#include <appl/global.hpp>
#include <appl/BufferManager.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/object/Manager.hpp>
#include <esignal/Signal.hpp>
#include <gale/resource/Manager.hpp>

appl::BufferManager::BufferManager() :
  signalNewBuffer(this, "new-buffer", ""),
  signalSelectFile(this, "select-buffer", ""),
  signalTextSelectionChange(this, "text-selection-change", ""),
  signalRemoveBuffer(this, "remove-buffer", ""),
  signalSelectBuffer(this, "select-buffer-2", ""),
  signalNewBuffer2(this, "new-buffer-2", "") {
	addObjectType("appl::BufferManager");
}

appl::BufferManager::~BufferManager() {
	m_list.clear();
}


ememory::SharedPtr<appl::Buffer> appl::BufferManager::createNewBuffer() {
	ememory::SharedPtr<appl::Buffer> tmp = appl::Buffer::create();
	if (tmp == null) {
		APPL_ERROR("Can not allocate the Buffer (empty).");
		return null;
	}
	tmp->setParent(ewol::Object::sharedFromThis());
	m_list.pushBack(tmp);
	APPL_INFO("Create a new Buffer");
	signalNewBuffer.emit(tmp->getFileName());
	signalNewBuffer2.emit(tmp);
	APPL_INFO("Create a new Buffer (done)");
	APPL_INFO("select Buffer");
	signalSelectFile.emit(tmp->getFileName());
	signalSelectBuffer.emit(tmp);
	APPL_INFO("select Buffer (done)");
	return tmp;
}

ememory::SharedPtr<appl::Buffer> appl::BufferManager::get(const etk::String& _fileName, bool _createIfNeeded) {
	APPL_INFO("get('" << _fileName << "'," << _createIfNeeded << ")");
	for (auto &it : m_list) {
		if (it == null) {
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
			return null;
		}
		ememory::SharedPtr<appl::Buffer> tmp = appl::Buffer::create();
		if (tmp == null) {
			APPL_ERROR("Can not allocate the Buffer class : " << _fileName);
			return null;
		}
		tmp->setParent(ewol::Object::sharedFromThis());
		tmp->loadFile(_fileName);
		m_list.pushBack(tmp);
		APPL_INFO("Creata a open Buffer");
		signalNewBuffer.emit(tmp->getFileName());
		signalNewBuffer2.emit(tmp);
		APPL_INFO("Creata a open Buffer (done)");
		return tmp;
	}
	return null;
}

void appl::BufferManager::setBufferSelected(ememory::SharedPtr<appl::Buffer> _bufferSelected) {
	m_bufferSelected = _bufferSelected;
	if (m_bufferSelected == null) {
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

bool appl::BufferManager::exist(const etk::String& _fileName) {
	APPL_WARNING(" Check if buffer exist: '" << _fileName << "'");
	for (auto &it : m_list) {
		if (it == null) {
			continue;
		}
		APPL_WARNING("     =?= '" << it->getFileName() << "'");
		if (it->getFileName() == _fileName) {
			APPL_WARNING("         ==> FIND...");
			return true;
		}
	}
	APPL_WARNING("     ==> NOT FOUND...");
	return false;
}

void appl::BufferManager::open(const etk::String& _fileName) {
	etk::FSNode file(_fileName);
	etk::String fileName = file.getName();
	if (exist(fileName) == true) {
		APPL_WARNING(" the element '" << fileName << "' already exist ... just reselect it ...");
		signalSelectFile.emit(fileName);
		signalSelectBuffer.emit(get(fileName));
		propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName(fileName));
		return;
	}
	if (get(fileName, true) == null) {
		APPL_ERROR("Error get '" << fileName << "' ... ");
		return;
	}
	signalSelectFile.emit(fileName);
	signalSelectBuffer.emit(get(fileName));
	propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName(fileName));
}

void appl::BufferManager::requestDestroyFromChild(const ememory::SharedPtr<Object>& _child) {
	APPL_WARNING("Buffer request a close...");
	bool find = false;
	int32_t newValue = -1;
	auto it = m_list.begin();
	while(it != m_list.end()) {
		if (*it == null) {
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
		     && *it != null) {
			APPL_VERBOSE("Remove buffer select new one");
			signalSelectFile.emit((*it)->getFileName());
			signalSelectBuffer.emit(*it);
			propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName((*it)->getFileName()));
			APPL_VERBOSE("Remove buffer select new one (done)");
			return;
		}
		if (    m_list.size() != 0
		     && m_list.back() != null) {
			APPL_VERBOSE("Remove buffer select new one (last)");
			signalSelectFile.emit(m_list.back()->getFileName());
			signalSelectBuffer.emit(m_list.back());
			propertySetOnWidgetNamed("appl-widget-display-name", "value", etk::FSNodeGetRealName(m_list.back()->getFileName()));
			APPL_VERBOSE("Remove buffer select new one (done)");
			return;
		}
		signalSelectFile.emit("");
		signalSelectBuffer.emit(null);
		propertySetOnWidgetNamed("appl-widget-display-name", "value", "---");
		m_bufferSelected = null;
	}
}

