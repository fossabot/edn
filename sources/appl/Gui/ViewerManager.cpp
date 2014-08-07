/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/Gui/ViewerManager.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <ewol/resource/Manager.h>

#undef __class__
#define __class__ "ViewerManager"

appl::ViewerManager::ViewerManager() {
	addObjectType("appl::ViewerManager");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::ViewerManager::init(const std::string& _uniqueName) {
  ewol::Resource::init(_uniqueName);
}

appl::ViewerManager::~ViewerManager() {
	
}

bool appl::ViewerManager::isLastSelected(const std::shared_ptr<appl::TextViewer>& _viewer) {
	return m_viewer == _viewer;
}

void appl::ViewerManager::setViewerSelected(const std::shared_ptr<appl::TextViewer>& _viewer, const std::shared_ptr<appl::Buffer>& _buffer) {
	if (m_viewer == _viewer) {
		return;
	}
	m_viewer = _viewer;
	if (m_bufferManager != nullptr) {
		m_bufferManager->setBufferSelected(_buffer);
	}
}

void appl::ViewerManager::onReceiveMessage(const ewol::object::Message& _msg) {
	APPL_DEBUG("receive message !!! " << _msg);
}

void appl::ViewerManager::onObjectRemove(const std::shared_ptr<ewol::Object>& _removeObject) {
	ewol::Resource:: onObjectRemove(_removeObject);
	if (_removeObject == m_viewer) {
		m_viewer.reset();
		return;
	}
}
