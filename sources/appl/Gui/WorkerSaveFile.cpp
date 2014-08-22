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

static const char* s_saveAsValidate = "save-as-validate";

appl::WorkerSaveFile::WorkerSaveFile() :
  signalSaveDone(*this, "save-file-done") {
	addObjectType("appl::WorkerSaveFile");
	// load buffer manager:
	m_bufferManager = appl::BufferManager::create();
}

void appl::WorkerSaveFile::init(const std::string& _bufferName, bool _forceSaveAs) {
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
		APPL_ERROR("Try to save an non-existant file :" << m_bufferName);
		autoDestroy();
		return;
	}
	std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(m_bufferName);
	if (tmpBuffer == nullptr) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		autoDestroy();
		return;
	}
	if (_forceSaveAs == false) {
		if (tmpBuffer->hasFileName() == true) {
			tmpBuffer->storeFile();
			signalSaveDone.emit();
			autoDestroy();
			return;
		}
	}
	m_chooser = ewol::widget::FileChooser::create();
	if (nullptr == m_chooser) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
		autoDestroy();
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
		autoDestroy();
		return;
	}
	tmpWindows->popUpWidgetPush(m_chooser);
	m_chooser->registerOnEvent(shared_from_this(), "validate", s_saveAsValidate);
}

appl::WorkerSaveFile::~WorkerSaveFile() {
	
}

void appl::WorkerSaveFile::onReceiveMessage(const ewol::object::Message& _msg) {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	if (_msg.getMessage() == s_saveAsValidate) {
		if (_msg.getData() == "") {
			APPL_ERROR(" might be an error of the File chooser system...");
			return;
		}
		if (m_bufferManager->exist(m_bufferName) == false) {
			APPL_ERROR("Try to save an non-existant file :" << m_bufferName);
			return;
		}
		std::shared_ptr<appl::Buffer> tmpBuffer = m_bufferManager->get(m_bufferName);
		if (tmpBuffer == nullptr) {
			APPL_ERROR("Error to get the buffer : " << m_bufferName);
			return;
		}
		tmpBuffer->setFileName(_msg.getData());
		if (tmpBuffer->storeFile() == false) {
			std::shared_ptr<ewol::widget::Windows> tmpWindows = ewol::getContext().getWindows();
			if (tmpWindows == nullptr) {
				return;
			}
			tmpWindows->displayWarningMessage("We can not save the file : <br/><i>" + tmpBuffer->getFileName() + "</i>");
		} else {
			signalSaveDone.emit();
		}
	}
}

