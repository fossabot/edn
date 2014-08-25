/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/context/Context.h>
#include <appl/debug.h>
#include <appl/Gui/WorkerCloseFile.h>
#include <ewol/widget/meta/StdPopUp.h>

#undef __class__
#define __class__ "WorkerCloseFile"

appl::WorkerCloseFile::WorkerCloseFile() :
  signalCloseDone(*this, "close-file-done"),
  m_buffer(nullptr),
  m_worker(nullptr),
  m_bufferManager(nullptr) {
	addObjectType("appl::WorkerCloseFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::WorkerCloseFile::init(const std::string& _bufferName) {
	ewol::Object::init();
	m_bufferName = _bufferName;
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	if (m_bufferName == "") {
		// need to find the curent file ...
		std::shared_ptr<appl::Buffer> tmpp = m_bufferManager->getBufferSelected();
		if (tmpp == nullptr) {
			APPL_ERROR("No selected buffer now ...");
			autoDestroy();
			return;
		}
		m_bufferName = tmpp->getFileName();
	}
	if (m_bufferManager->exist(m_bufferName) == false) {
		APPL_ERROR("Try to close an non-existant file :" << m_bufferName);
		autoDestroy();
		return;
	}
	m_buffer = m_bufferManager->get(m_bufferName);
	if (m_buffer == nullptr) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		autoDestroy();
		return;
	}
	if (m_buffer->isModify() == false) {
		signalCloseDone.emit();
		m_buffer->destroy();
		return;
	}
	
	std::shared_ptr<ewol::widget::StdPopUp> tmpPopUp = ewol::widget::StdPopUp::create();
	if (tmpPopUp == nullptr) {
		APPL_ERROR("Can not create a simple pop-up");
		return;
	}
	tmpPopUp->setTitle("<bold>Close un-saved file:</bold>");
	tmpPopUp->setComment("The file named : <i>\"" + m_buffer->getFileName() + "\"</i> is curently modify.   <br/>If you don't saves these modifications,<br/>they will be definitly lost...");
	std::shared_ptr<ewol::widget::Button> bt = nullptr;
	if (m_buffer->hasFileName() == true) {
		bt = tmpPopUp->addButton("Save", true);
		if (bt != nullptr) {
			bt->signalPressed.bind(shared_from_this(), &appl::WorkerCloseFile::onCallbackSaveValidate);
		}
	}
	bt = tmpPopUp->addButton("Save As", true);
	if (bt != nullptr) {
		bt->signalPressed.bind(shared_from_this(), &appl::WorkerCloseFile::onCallbackSaveAsValidate);
	}
	bt = tmpPopUp->addButton("Close", true);
	if (bt != nullptr) {
		bt->signalPressed.bind(shared_from_this(), &appl::WorkerCloseFile::onCallbackClose);
	}
	tmpPopUp->addButton("Cancel", true);
	tmpPopUp->setRemoveOnExternClick(true);
	std::shared_ptr<ewol::widget::Windows> tmpWindows = ewol::getContext().getWindows();
	if (tmpWindows == nullptr) {
		APPL_ERROR("Error to get the windows.");
		autoDestroy();
		return;
	}
	tmpWindows->popUpWidgetPush(tmpPopUp);
}

appl::WorkerCloseFile::~WorkerCloseFile() {
	
}


void appl::WorkerCloseFile::onCallbackSaveAsValidate() {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	m_worker = appl::WorkerSaveFile::create(m_bufferName);
	if (m_worker != nullptr) {
		m_worker->signalSaveDone.bind(shared_from_this(), &appl::WorkerCloseFile::onCallbackClose);
	}
}

void appl::WorkerCloseFile::onCallbackSaveValidate() {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (m_buffer == nullptr) {
		APPL_ERROR("Error to get the buffer : oldName=" << m_bufferName);
		autoDestroy();
		return;
	}
	if (m_buffer->storeFile() == false) {
		std::shared_ptr<ewol::widget::Windows> tmpWindows = ewol::getContext().getWindows();
		if (tmpWindows == nullptr) {
			return;
		}
		tmpWindows->displayWarningMessage("We can not save the file : <br/><i>" + m_buffer->getFileName() + "</i>");
	} else {
		signalCloseDone.emit();
	}
}

void appl::WorkerCloseFile::onCallbackClose() {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (m_buffer == nullptr) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		autoDestroy();
		return;
	}
	signalCloseDone.emit();
	m_buffer->destroy();
	m_buffer.reset();
}

