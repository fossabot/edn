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

const char* appl::WorkerCloseFile::eventCloseDone = "close-file-done";

static const char* s_saveAsValidate = "save-as-validate";
static const char* s_saveValidate = "save-validate";
static const char* s_closeValidate = "close-validate";
static const char* s_saveAsDone = "save-as-done";

appl::WorkerCloseFile::WorkerCloseFile(const std::string& _bufferName) :
  m_bufferName(_bufferName),
  m_buffer(nullptr),
  m_worker(nullptr),
  m_bufferManager(nullptr) {
	addObjectType("appl::WorkerCloseFile");
	addEventId(eventCloseDone);
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	if (m_bufferManager == nullptr) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	if (m_bufferName == "") {
		// need to find the curent file ...
		ewol::object::Shared<appl::Buffer> tmpp = m_bufferManager->getBufferSelected();
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
		generateEventId(eventCloseDone);
		m_buffer->removeObject();
		return;
	}
	
	ewol::object::Shared<ewol::widget::StdPopUp> tmpPopUp = ewol::object::makeShared(new ewol::widget::StdPopUp());
	if (tmpPopUp == nullptr) {
		APPL_ERROR("Can not create a simple pop-up");
		return;
	}
	tmpPopUp->setTitle("<bold>Close un-saved file:</bold>");
	tmpPopUp->setComment("The file named : <i>\"" + m_buffer->getFileName() + "\"</i> is curently modify.   <br/>If you don't saves these modifications,<br/>they will be definitly lost...");
	ewol::object::Shared<ewol::Widget> bt = nullptr;
	if (m_buffer->hasFileName() == true) {
		bt = tmpPopUp->addButton("Save", true);
		if (bt != nullptr) {
			bt->registerOnEvent(this, "pressed", s_saveValidate);
		}
	}
	bt = tmpPopUp->addButton("Save As", true);
	if (bt != nullptr) {
		bt->registerOnEvent(this, "pressed", s_saveAsValidate);
	}
	bt = tmpPopUp->addButton("Close", true);
	if (bt != nullptr) {
		bt->registerOnEvent(this, "pressed", s_closeValidate);
	}
	tmpPopUp->addButton("Cancel", true);
	tmpPopUp->setRemoveOnExternClick(true);
	ewol::object::Shared<ewol::widget::Windows> tmpWindows = ewol::getContext().getWindows();
	if (tmpWindows == nullptr) {
		APPL_ERROR("Error to get the windows.");
		autoDestroy();
		return;
	}
	tmpWindows->popUpWidgetPush(tmpPopUp);
}

appl::WorkerCloseFile::~WorkerCloseFile() {
	
}

void appl::WorkerCloseFile::onReceiveMessage(const ewol::object::Message& _msg) {
	if (m_bufferManager == nullptr) {
		// nothing to do in this case ==> can do nothing ...
		return;
	}
	APPL_DEBUG("have message : " << _msg);
	if (_msg.getMessage() == s_saveAsValidate) {
		m_worker = ewol::object::makeShared(new appl::WorkerSaveFile(m_bufferName));
		if (m_worker != nullptr) {
			m_worker->registerOnEvent(this, appl::WorkerSaveFile::eventSaveDone, s_saveAsDone);
		}
	} else if (_msg.getMessage() == s_saveValidate) {
		if (m_buffer == nullptr) {
			APPL_ERROR("Error to get the buffer : oldName=" << m_bufferName);
			autoDestroy();
			return;
		}
		if (m_buffer->storeFile() == false) {
			ewol::object::Shared<ewol::widget::Windows> tmpWindows = ewol::getContext().getWindows();
			if (tmpWindows == nullptr) {
				return;
			}
			tmpWindows->displayWarningMessage("We can not save the file : <br/><i>" + m_buffer->getFileName() + "</i>");
		} else {
			generateEventId(eventCloseDone);
		}
	} else if (    _msg.getMessage() == s_closeValidate
	            || _msg.getMessage() == s_saveAsDone) {
		if (m_buffer == nullptr) {
			APPL_ERROR("Error to get the buffer : " << m_bufferName);
			autoDestroy();
			return;
		}
		generateEventId(eventCloseDone);
		m_buffer->removeObject();
	}
}

void appl::WorkerCloseFile::onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject) {
	if (_removeObject == m_worker) {
		m_worker = nullptr;
		APPL_VERBOSE("AutoRemove After closing sub widget ...");
		autoDestroy();
	} else if (_removeObject == m_bufferManager) {
		m_bufferManager = nullptr;
		autoDestroy();
	} else if (_removeObject == m_buffer) {
		m_buffer = nullptr;
		autoDestroy();
	}
}

