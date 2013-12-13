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

const char* appl::WorkerSaveFile::eventSaveDone = "save-file-done";

static const char* s_saveAsValidate = "save-as-validate";

appl::WorkerSaveFile::WorkerSaveFile(const std::string& _bufferName, bool _forceSaveAs) :
  m_bufferName(_bufferName),
  m_chooser(NULL),
  m_bufferManager(NULL) {
	addObjectType("appl::WorkerSaveFile");
	addEventId(eventSaveDone);
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	if (m_bufferManager == NULL) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	if (m_bufferName == "") {
		// need to find the curent file ...
		appl::Buffer* tmpp = m_bufferManager->getBufferSelected();
		if (tmpp == NULL) {
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
	appl::Buffer* tmpBuffer = m_bufferManager->get(m_bufferName);
	if (tmpBuffer == NULL) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		autoDestroy();
		return;
	}
	if (_forceSaveAs == false) {
		if (tmpBuffer->hasFileName() == true) {
			tmpBuffer->storeFile();
			generateEventId(eventSaveDone);
			autoDestroy();
			return;
		}
	}
	m_chooser = new ewol::widget::FileChooser();
	if (NULL == m_chooser) {
		APPL_ERROR("Can not allocate widget  == > display might be in error");
		autoDestroy();
		return;
	}
	m_chooser->setTitle("Save files As...");
	m_chooser->setValidateLabel("Save");
	etk::FSNode tmpName(m_bufferName);
	m_chooser->setFolder(tmpName.getNameFolder());
	m_chooser->setFileName(tmpName.getNameFile());
	ewol::widget::Windows* tmpWindows = ewol::getContext().getWindows();
	if (tmpWindows == NULL) {
		APPL_ERROR("Error to get the windows.");
		autoDestroy();
		return;
	}
	tmpWindows->popUpWidgetPush(m_chooser);
	m_chooser->registerOnEvent(this, ewol::widget::FileChooser::eventValidate, s_saveAsValidate);
}

appl::WorkerSaveFile::~WorkerSaveFile(void) {
	appl::BufferManager::release(m_bufferManager);
}

void appl::WorkerSaveFile::onReceiveMessage(const ewol::object::Message& _msg) {
	if (m_bufferManager == NULL) {
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
		appl::Buffer* tmpBuffer = m_bufferManager->get(m_bufferName);
		if (tmpBuffer == NULL) {
			APPL_ERROR("Error to get the buffer : " << m_bufferName);
			return;
		}
		tmpBuffer->setFileName(_msg.getData());
		if (tmpBuffer->storeFile() == false) {
			ewol::widget::Windows* tmpWindows = ewol::getContext().getWindows();
			if (tmpWindows == NULL) {
				return;
			}
			tmpWindows->displayWarningMessage("We can not save the file : <br/><i>" + tmpBuffer->getFileName() + "</i>");
		} else {
			generateEventId(eventSaveDone);
		}
	}
}

void appl::WorkerSaveFile::onObjectRemove(ewol::Object* _removeObject) {
	if (_removeObject == m_chooser) {
		m_chooser = NULL;
		APPL_VERBOSE("AutoRemove After closing sub widget ...");
		autoDestroy();
	} else if (_removeObject == m_bufferManager) {
		m_bufferManager = NULL;
		autoDestroy();
	}
}

