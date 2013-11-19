/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <ewol/renderer/eContext.h>
#include <appl/debug.h>
#include <appl/Gui/WorkerCloseFile.h>
#include <ewol/widget/meta/StdPopUp.h>

#undef __class__
#define __class__ "WorkerCloseFile"

const char* appl::WorkerCloseFile::eventCloseDone = "close-file-done";

static const char* s_saveAsValidate = "save-as-validate";
static const char* s_saveValidate = "save-validate";
static const char* s_closeValidate = "close-validate";
static const char* s_saveAsDone = "save-as-done";

appl::WorkerCloseFile::WorkerCloseFile(const std::string& _bufferName) :
  m_bufferName(_bufferName),
  m_buffer(NULL),
  m_worker(NULL),
  m_bufferManager(NULL) {
	addEventId(eventCloseDone);
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
		APPL_ERROR("Try to close an non-existant file :" << m_bufferName);
		autoDestroy();
		return;
	}
	m_buffer = m_bufferManager->get(m_bufferName);
	if (m_buffer == NULL) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		autoDestroy();
		return;
	}
	if (m_buffer->isModify() == false) {
		generateEventId(eventCloseDone);
		m_buffer->removeObject();
		return;
	}
	
	widget::StdPopUp* tmpPopUp = new widget::StdPopUp();
	if (tmpPopUp == NULL) {
		APPL_ERROR("Can not create a simple pop-up");
		return;
	}
	tmpPopUp->setTitle("<bold>Close un-saved file:</bold>");
	tmpPopUp->setComment("The file named : <i>\"" + m_buffer->getFileName() + "\"</i> is curently modify.   <br/>If you don't saves these modifications,<br/>they will be definitly lost...");
	ewol::Widget* bt = NULL;
	if (m_buffer->hasFileName() == true) {
		bt = tmpPopUp->addButton("Save", true);
		if (bt != NULL) {
			bt->registerOnEvent(this, widget::Button::eventPressed, s_saveValidate);
		}
	}
	bt = tmpPopUp->addButton("Save As", true);
	if (bt != NULL) {
		bt->registerOnEvent(this, widget::Button::eventPressed, s_saveAsValidate);
	}
	bt = tmpPopUp->addButton("Close", true);
	if (bt != NULL) {
		bt->registerOnEvent(this, widget::Button::eventPressed, s_closeValidate);
	}
	tmpPopUp->addButton("Cancel", true);
	tmpPopUp->setRemoveOnExternClick(true);
	ewol::Windows* tmpWindows = ewol::getContext().getWindows();
	if (tmpWindows == NULL) {
		APPL_ERROR("Error to get the windows.");
		autoDestroy();
		return;
	}
	tmpWindows->popUpWidgetPush(tmpPopUp);
}

appl::WorkerCloseFile::~WorkerCloseFile(void) {
	appl::BufferManager::release(m_bufferManager);
}

void appl::WorkerCloseFile::onReceiveMessage(const ewol::EMessage& _msg) {
	if (m_bufferManager == NULL) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	APPL_DEBUG("have message : " << _msg);
	if (_msg.getMessage() == s_saveAsValidate) {
		m_worker = new appl::WorkerSaveFile(m_bufferName);
		if (m_worker != NULL) {
			m_worker->registerOnEvent(this, appl::WorkerSaveFile::eventSaveDone, s_saveAsDone);
		}
	} else if (_msg.getMessage() == s_saveValidate) {
		if (m_buffer == NULL) {
			APPL_ERROR("Error to get the buffer : oldName=" << m_bufferName);
			autoDestroy();
			return;
		}
		if (m_buffer->storeFile() == false) {
			ewol::Windows* tmpWindows = ewol::getContext().getWindows();
			if (tmpWindows == NULL) {
				return;
			}
			tmpWindows->displayWarningMessage("We can not save the file : <br/><i>" + m_buffer->getFileName() + "</i>");
		} else {
			generateEventId(eventCloseDone);
		}
	} else if (    _msg.getMessage() == s_closeValidate
	            || _msg.getMessage() == s_saveAsDone) {
		if (m_buffer == NULL) {
			APPL_ERROR("Error to get the buffer : " << m_bufferName);
			autoDestroy();
			return;
		}
		generateEventId(eventCloseDone);
		m_buffer->removeObject();
	}
}

void appl::WorkerCloseFile::onObjectRemove(ewol::EObject* _removeObject) {
	if (_removeObject == m_worker) {
		m_worker = NULL;
		APPL_VERBOSE("AutoRemove After closing sub widget ...");
		autoDestroy();
	} else if (_removeObject == m_bufferManager) {
		m_bufferManager = NULL;
		autoDestroy();
	} else if (_removeObject == m_buffer) {
		m_buffer = NULL;
		autoDestroy();
	}
}

