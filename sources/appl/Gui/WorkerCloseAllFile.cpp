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

appl::WorkerCloseAllFile::WorkerCloseAllFile() {
	addObjectType("appl::WorkerCloseAllFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::WorkerCloseAllFile::init() {
	ewol::Object::init();
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	// List all current open file :
	for (int64_t iii=m_bufferManager->size()-1; iii>=0; --iii) {
		std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(iii);
		if (tmpBuffer == nullptr) {
			continue;
		}
		if (tmpBuffer->isModify() == false) {
			APPL_TODO("destroy object");
			tmpBuffer->destroy();
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
	m_worker = appl::WorkerCloseFile::create(m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	m_worker->signalCloseDone.bind(shared_from_this(), &appl::WorkerCloseAllFile::onCallbackCloseDone);
}

appl::WorkerCloseAllFile::~WorkerCloseAllFile() {
	
}

void appl::WorkerCloseAllFile::onCallbackCloseDone() {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	// create the worker :
	m_worker = appl::WorkerCloseFile::create(m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		autoDestroy();
		return;
	}
	m_worker->signalCloseDone.bind(shared_from_this(), &appl::WorkerCloseAllFile::onCallbackCloseDone);
}

