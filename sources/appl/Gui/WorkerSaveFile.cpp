/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/renderer/eContext.h>
#include <appl/debug.h>
#include <appl/Gui/WorkerSaveFile.h>

#undef __class__
#define __class__ "WorkerSaveFile"

static const char* appl::WorkerSaveFile::eventSaveDone = "save-file-done";

static const char* s_saveAsValidate = "save-as-validate";

appl::WorkerSaveFile::WorkerSaveFile(const std::string& _bufferName, bool _forceSaveAs) :
  m_bufferName(_bufferName),
  m_chooser(NULL),
  m_bufferManager(NULL) {
	addEventId(eventSaveDone);
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	if (m_bufferManager == NULL) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	// TODO : if "" ==> it is current buffer selected ...
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
	m_chooser = new widget::FileChooser();
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
	ewol::Windows* tmpWindows = ewol::getContext().getWindows();
	if (tmpWindows == NULL) {
		APPL_ERROR("Error to get the windows.");
		autoDestroy();
		return;
	}
	tmpWindows->popUpWidgetPush(m_chooser);
	m_chooser->registerOnEvent(this, widget::FileChooser::eventValidate, s_saveAsValidate);
}

appl::WorkerSaveFile::~WorkerSaveFile(void) {
	appl::BufferManager::release(m_bufferManager);
}

void appl::WorkerSaveFile::onReceiveMessage(const ewol::EMessage& _msg) {
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
			ewol::Windows* tmpWindows = ewol::getContext().getWindows();
			if (tmpWindows == NULL) {
				return;
			}
			tmpWindows->displayWarningMessage("We can not save the file : <br/><i>" + tmpBuffer->getFileName() + "</i>");
		}
	}
}

void appl::WorkerSaveFile::onObjectRemove(ewol::EObject* _removeObject) {
	if (_removeObject == m_chooser) {
		m_chooser = NULL;
		APPL_VERBOSE("AutoRemove After closing sub widget ...");
		autoDestroy();
	} else if (_removeObject == m_bufferManager) {
		m_bufferManager = NULL;
		autoDestroy();
	}
}

