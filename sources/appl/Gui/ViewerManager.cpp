/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <appl/debug.h>
#include <appl/global.h>
#include <appl/Gui/ViewerManager.h>
#include <appl/Gui/TextViewer.h>
#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <gale/resource/Manager.h>

appl::ViewerManager::ViewerManager() {
	addObjectType("appl::ViewerManager");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

appl::ViewerManager::~ViewerManager() {
	
}

ememory::SharedPtr<appl::TextViewer> appl::ViewerManager::getViewerSelected() {
	return m_viewer.lock();
}

bool appl::ViewerManager::isLastSelected(const ememory::SharedPtr<appl::TextViewer>& _viewer) {
	return m_viewer.lock() == _viewer;
}

void appl::ViewerManager::setViewerSelected(const ememory::SharedPtr<appl::TextViewer>& _viewer, const ememory::SharedPtr<appl::Buffer>& _buffer) {
	if (m_viewer.lock() == _viewer) {
		return;
	}
	m_viewer = _viewer;
	if (m_bufferManager != nullptr) {
		//m_bufferManager->setBufferSelected(_buffer);
	}
}
