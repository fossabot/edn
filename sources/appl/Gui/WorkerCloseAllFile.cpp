/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <ewol/context/Context.h>
#include <appl/debug.h>
#include <appl/Gui/WorkerCloseAllFile.h>

appl::WorkerCloseAllFile::WorkerCloseAllFile() {
	addObjectType("appl::WorkerCloseAllFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::WorkerCloseAllFile::init() {
	ewol::object::Worker::init();
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		destroy();
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
		destroy();
		return;
	}
	// create the worker :
	m_worker = appl::WorkerCloseFile::create();
	m_worker->signalCloseDone.connect(shared_from_this(), &appl::WorkerCloseAllFile::onCallbackCloseDone);
	m_worker->startAction(m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		destroy();
		return;
	}
}

appl::WorkerCloseAllFile::~WorkerCloseAllFile() {
	APPL_ERROR("Remove Worker");
}

void appl::WorkerCloseAllFile::onCallbackCloseDone() {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (m_bufferNameList.size() == 0) {
		destroy();
		return;
	}
	// create the worker :
	m_worker = appl::WorkerCloseFile::create();
	m_worker->signalCloseDone.connect(shared_from_this(), &appl::WorkerCloseAllFile::onCallbackCloseDone);
	m_worker->startAction(m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		destroy();
		return;
	}
}

