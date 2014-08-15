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

appl::WorkerSaveAllFile::WorkerSaveAllFile() {
	addObjectType("appl::WorkerSaveAllFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::WorkerSaveAllFile::init() {
	ewol::Object::init();
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	// List all current open file :
	for (int32_t iii=0; iii<m_bufferManager->size(); ++iii) {
		std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(iii);
		if (tmpBuffer == nullptr) {
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
	m_worker = appl::WorkerSaveFile::create(m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	m_worker->registerOnEvent(shared_from_this(), appl::WorkerSaveFile::eventSaveDone, s_saveAsDone);
}

appl::WorkerSaveAllFile::~WorkerSaveAllFile() {
	
}

void appl::WorkerSaveAllFile::onReceiveMessage(const ewol::object::Message& _msg) {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (_msg.getMessage() == s_saveAsDone) {
		if (m_bufferNameList.size() == 0) {
			autoDestroy();
			return;
		}
		// create the worker :
		m_worker = appl::WorkerSaveFile::create(m_bufferNameList.front());
		// remove first element :
		m_bufferNameList.erase(m_bufferNameList.begin());
		if (m_bufferNameList.size() == 0) {
			autoDestroy();
			return;
		}
		m_worker->registerOnEvent(shared_from_this(), appl::WorkerSaveFile::eventSaveDone, s_saveAsDone);
	}
}

