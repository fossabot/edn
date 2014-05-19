/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/context/Context.h>
#include <appl/debug.h>
#include <appl/Gui/WorkerCloseAllFile.h>

#undef __class__
#define __class__ "WorkerCloseAllFile"

static const char* s_closeDone = "close-done";

appl::WorkerCloseAllFile::WorkerCloseAllFile() :
  m_worker(NULL),
  m_bufferManager(NULL) {
	addObjectType("appl::WorkerCloseAllFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	if (m_bufferManager == NULL) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	// List all current open file :
	for (int64_t iii=m_bufferManager->size()-1; iii>=0; --iii) {
		appl::Buffer* tmpBuffer = m_bufferManager->get(iii);
		if (tmpBuffer == NULL) {
			continue;
		}
		if (tmpBuffer->isModify() == false) {
			tmpBuffer->removeObject();
			continue;
		}
		m_bufferNameList.push_back(tmpBuffer->getFileName());
	}
	// checkif an element has something to do in the queue
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	// create the worker :
	m_worker = new appl::WorkerCloseFile(m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	m_worker->registerOnEvent(this, appl::WorkerCloseFile::eventCloseDone, s_closeDone);
}

appl::WorkerCloseAllFile::~WorkerCloseAllFile() {
	appl::BufferManager::release(m_bufferManager);
}

void appl::WorkerCloseAllFile::onReceiveMessage(const ewol::object::Message& _msg) {
	if (m_bufferManager == NULL) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (_msg.getMessage() == s_closeDone) {
		if (m_bufferNameList.size() == 0) {
			autoDestroy();
			return;
		}
		// create the worker :
		m_worker = new appl::WorkerCloseFile(m_bufferNameList.front());
		// remove first element :
		m_bufferNameList.erase(m_bufferNameList.begin());
		if (m_bufferNameList.size() == 0) {
			autoDestroy();
			return;
		}
		m_worker->registerOnEvent(this, appl::WorkerCloseFile::eventCloseDone, s_closeDone);
	}
}

void appl::WorkerCloseAllFile::onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject) {
	if (_removeObject == m_worker) {
		m_worker = NULL;
		APPL_VERBOSE("AutoRemove After saving sub widget ...");
		autoDestroy();
	} else if (_removeObject == m_bufferManager) {
		m_bufferManager = NULL;
		autoDestroy();
	}
}

