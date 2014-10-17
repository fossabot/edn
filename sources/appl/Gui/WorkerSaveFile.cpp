/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/context/Context.h>
#include <appl/debug.h>
#include <appl/Gui/WorkerSaveFile.h>

#undef __class__
#define __class__ "WorkerSaveFile"


appl::WorkerSaveFile::WorkerSaveFile() :
  signalSaveDone(*this, "save-file-done"),
  signalAbort(*this, "save-file-abort") {
	addObjectType("appl::WorkerSaveFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::WorkerSaveFile::init(const std::string& _bufferName, bool _forceSaveAs) {
	ewol::object::Worker::init();
	m_bufferName = _bufferName;
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		destroy();
		return;
	}
	if (m_bufferName == "") {
		// need to find the curent file ...
		std::shared_ptr<appl::Buffer> tmpp = m_bufferManager->getBufferSelected();
		if (tmpp == nullptr) {
			APPL_ERROR("No selected buffer now ...");
			destroy();
			return;
		}
		m_bufferName = tmpp->getFileName();
	}
	if (m_bufferManager->exist(m_bufferName) == false) {
		APPL_ERROR("Try to save an non-existant file :" << m_bufferName);
		destroy();
		return;
	}
	std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(m_bufferName);
	if (tmpBuffer == nullptr) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		destroy();
		return;
	}
	if (_forceSaveAs == false) {
		if (tmpBuffer->hasFileName() == true) {
			tmpBuffer->storeFile();
			signalSaveDone.emit();
			destroy();
			return;
		}
	}
	m_chooser = ewol::widget::FileChooser::create();
	if (nullptr == m_chooser) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
		destroy();
		return;
	}
	m_chooser->setTitle("Save files As...");
	m_chooser->setValidateLabel("Save");
	etk::FSNode tmpName(m_bufferName);
	m_chooser->setFolder(tmpName.getNameFolder());
	m_chooser->setFileName(tmpName.getNameFile());
	std::shared_ptr<ewol::widget::Windows> tmpWindows = ewol::getContext().getWindows();
	if (tmpWindows == nullptr) {
		APPL_ERROR("Error to get the windows.");
		destroy();
		return;
	}
	tmpWindows->popUpWidgetPush(m_chooser);
	m_chooser->signalValidate.bind(shared_from_this(), &appl::WorkerSaveFile::onCallbackSaveAsValidate);
	m_chooser->signalCancel.bind(shared_from_this(), &appl::WorkerSaveFile::onCallbackCancel);
}

appl::WorkerSaveFile::~WorkerSaveFile() {
	APPL_ERROR("Remove Worker");
}

void appl::WorkerSaveFile::onCallbackCancel() {
	signalAbort.emit();
	destroy();
}

void appl::WorkerSaveFile::onCallbackSaveAsValidate(const std::string& _value) {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		destroy();
		return;
	}
	if (_value == "") {
		APPL_ERROR(" might be an error of the File chooser system...");
		destroy();
		return;
	}
	if (m_bufferManager->exist(m_bufferName) == false) {
		APPL_ERROR("Try to save an non-existant file :" << m_bufferName);
		destroy();
		return;
	}
	std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(m_bufferName);
	if (tmpBuffer == nullptr) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		destroy();
		return;
	}
	tmpBuffer->setFileName(_value);
	if (tmpBuffer->storeFile() == false) {
		std::shared_ptr<ewol::widget::Windows> tmpWindows = ewol::getContext().getWindows();
		if (tmpWindows == nullptr) {
			destroy();
			return;
		}
		tmpWindows->displayWarningMessage("We can not save the file : <br/><i>" + tmpBuffer->getFileName() + "</i>");
	} else {
		signalSaveDone.emit();
	}
	destroy();
}

