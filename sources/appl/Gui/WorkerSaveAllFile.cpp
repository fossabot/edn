/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/context/Context.h>
#include <appl/debug.h>
#include <appl/Gui/WorkerSaveAllFile.h>

#undef __class__
#define __class__ "WorkerSaveAllFile"

static const char* s_saveAsDone = "save-as-done";

appl::WorkerSaveAllFile::WorkerSaveAllFile() :
  m_worker(NULL),
  m_bufferManager(NULL) {
	addObjectType("appl::WorkerSaveAllFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	if (m_bufferManager == NULL) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	// List all current open file :
	for (size_t iii=0; iii<m_bufferManager->size(); ++iii) {
		ewol::object::Shared<appl::Buffer> tmpBuffer = m_bufferManager->get(iii);
		if (tmpBuffer == NULL) {
			continue;
		}
		if (tmpBuffer->isModify() == false) {
			continue;
		}
		if (tmpBuffer->hasFileName() == false) {
			m_bufferNameList.push_back(tmpBuffer->getFileName());
		} else {
			tmpBuffer->storeFile();
		}
	}
	// checkif an element has something to do in the queue
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	// create the worker :
	m_worker = ewol::object::makeShared(new appl::WorkerSaveFile(m_bufferNameList.front()));
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	m_worker->registerOnEvent(this, appl::WorkerSaveFile::eventSaveDone, s_saveAsDone);
}

appl::WorkerSaveAllFile::~WorkerSaveAllFile() {
	
}

void appl::WorkerSaveAllFile::onReceiveMessage(const ewol::object::Message& _msg) {
	if (m_bufferManager == NULL) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (_msg.getMessage() == s_saveAsDone) {
		if (m_bufferNameList.size() == 0) {
			autoDestroy();
			return;
		}
		// create the worker :
		m_worker = new appl::WorkerSaveFile(m_bufferNameList.front());
		// remove first element :
		m_bufferNameList.erase(m_bufferNameList.begin());
		if (m_bufferNameList.size() == 0) {
			autoDestroy();
			return;
		}
		m_worker->registerOnEvent(this, appl::WorkerSaveFile::eventSaveDone, s_saveAsDone);
	}
}

void appl::WorkerSaveAllFile::onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject) {
	if (_removeObject == m_worker) {
		m_worker = NULL;
		APPL_VERBOSE("AutoRemove After saving sub widget ...");
		autoDestroy();
	} else if (_removeObject == m_bufferManager) {
		m_bufferManager = NULL;
		autoDestroy();
	}
}

