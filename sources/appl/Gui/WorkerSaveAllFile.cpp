/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#include <ewol/context/Context.hpp>
#include <appl/debug.hpp>
#include <appl/Gui/WorkerSaveAllFile.hpp>

appl::WorkerSaveAllFile::WorkerSaveAllFile() {
	addObjectType("appl::WorkerSaveAllFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::WorkerSaveAllFile::init() {
	ewol::object::Worker::init();
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		destroy();
		return;
	}
	// List all current open file :
	for (auto &it : *m_bufferManager) {
		if (it == nullptr) {
			continue;
		}
		if (it->isModify() == false) {
			continue;
		}
		if (it->hasFileName() == false) {
			m_bufferNameList.pushBack(it->getFileName());
		} else {
			it->storeFile();
		}
	}
	// checkif an element has something to do in the queue
	if (m_bufferNameList.size() == 0) {
		destroy();
		return;
	}
	// create the worker :
	m_worker = appl::WorkerSaveFile::create("buffer-name", m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		destroy();
		return;
	}
	m_worker->signalSaveDone.connect(sharedFromThis(), &appl::WorkerSaveAllFile::onCallbackSaveAsDone);
}

appl::WorkerSaveAllFile::~WorkerSaveAllFile() {
	APPL_ERROR("Remove Worker");
}

void appl::WorkerSaveAllFile::onCallbackSaveAsDone() {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (m_bufferNameList.size() == 0) {
		destroy();
		return;
	}
	// create the worker :
	m_worker = appl::WorkerSaveFile::create("buffer-name", m_bufferNameList.front());
	// remove first element :
	m_bufferNameList.erase(m_bufferNameList.begin());
	if (m_bufferNameList.size() == 0) {
		destroy();
		return;
	}
	m_worker->signalSaveDone.connect(sharedFromThis(), &appl::WorkerSaveAllFile::onCallbackSaveAsDone);
}

