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
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>

#undef __class__
#define __class__ "ViewerManager"

appl::ViewerManager::ViewerManager(void) :
  ewol::Resource("???ViewerManager???"),
  m_viewer(NULL) {
	addObjectType("appl::ViewerManager");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
}

appl::ViewerManager::~ViewerManager(void) {
	appl::BufferManager::release(m_bufferManager);
}

void appl::ViewerManager::setViewerSelected(appl::TextViewer* _viewer, appl::Buffer* _buffer) {
	if (m_viewer == _viewer) {
		return;
	}
	m_viewer = _viewer;
	if (m_bufferManager != NULL) {
		m_bufferManager->setBufferSelected(_buffer);
	}
}

void appl::ViewerManager::onReceiveMessage(const ewol::EMessage& _msg) {
	APPL_DEBUG("receive message !!! " << _msg);
}

void appl::ViewerManager::onObjectRemove(ewol::EObject* _removeObject) {
	ewol::Resource:: onObjectRemove(_removeObject);
	if (_removeObject == m_viewer) {
		m_viewer = NULL;
		return;
	}
}

appl::ViewerManager* appl::ViewerManager::keep(void) {
	//EWOL_INFO("KEEP : appl::GlyphPainting : file : \"" << _filename << "\"");
	appl::ViewerManager* object = static_cast<appl::ViewerManager*>(getManager().localKeep("???ViewerManager???"));
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	EWOL_INFO("CREATE : appl::ViewerManager: ???ViewerManager???");
	object = new appl::ViewerManager();
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ???ViewerManager???");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void appl::ViewerManager::release(appl::ViewerManager*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}
