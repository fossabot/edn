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

appl::ViewerManager::ViewerManager() :
  ewol::Resource("???ViewerManager???"),
  m_viewer(nullptr) {
	addObjectType("appl::ViewerManager");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
}

appl::ViewerManager::~ViewerManager() {
	
}

bool appl::ViewerManager::isLastSelected(const ewol::object::Shared<appl::TextViewer>& _viewer) {
	return m_viewer == _viewer;
}

void appl::ViewerManager::setViewerSelected(const ewol::object::Shared<appl::TextViewer>& _viewer, const ewol::object::Shared<appl::Buffer>& _buffer) {
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

void appl::ViewerManager::onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject) {
	ewol::Resource:: onObjectRemove(_removeObject);
	if (_removeObject == m_viewer) {
		m_viewer.reset();
		return;
	}
}

ewol::object::Shared<appl::ViewerManager> appl::ViewerManager::keep() {
	//EWOL_INFO("KEEP : appl::GlyphPainting : file : \"" << _filename << "\"");
	ewol::object::Shared<appl::ViewerManager> object = ewol::dynamic_pointer_cast<appl::ViewerManager>(getManager().localKeep("???ViewerManager???"));
	if (nullptr != object) {
		return object;
	}
	// this element create a new one every time ....
	EWOL_INFO("CREATE : appl::ViewerManager: ???ViewerManager???");
	object = ewol::object::makeShared(new appl::ViewerManager());
	if (nullptr == object) {
		EWOL_ERROR("allocation error of a resource : ???ViewerManager???");
		return nullptr;
	}
	getManager().localAdd(object);
	return object;
}
