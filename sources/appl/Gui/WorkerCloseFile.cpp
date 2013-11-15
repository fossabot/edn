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

#undef __class__
#define __class__ "WorkerCloseFile"

static const char* s_saveAsValidate = "save-as-validate";
static const char* s_saveValidate = "save-validate";
static const char* s_closeValidate = "close-validate";

appl::WorkerCloseFile::WorkerCloseFile(const std::string& _bufferName) :
  m_closeAfter(_close),
  m_bufferName(_bufferName),
  m_worker(NULL),
  m_bufferManager(NULL) {
	// load buffer manager:
	m_bufferManager = appl::BufferManager::keep();
	
	if (m_bufferManager == NULL) {
		APPL_ERROR("can not call unexistant buffer manager ... ");
		autoDestroy();
		return;
	}
	if (m_bufferManager->exist(m_bufferName) == false) {
		APPL_ERROR("Try to close an non-existant file :" << m_bufferName);
		autoDestroy();
		return;
	}
	appl::Buffer* tmpBuffer = m_bufferManager->get(m_bufferName);
	if (tmpBuffer == NULL) {
		APPL_ERROR("Error to get the buffer : " << m_bufferName);
		autoDestroy();
		return;
	}
	if (tmpBuffer->isModify() == false) {
		tmpBuffer->removeObject();
		// TODO : Send message ...
		
		autoDestroy();
		return;
	}
	
	widget::StdPopUp* tmpPopUp = new widget::StdPopUp();
	if (tmpPopUp == NULL) {
		APPL_ERROR("Can not create a simple pop-up");
		return;
	}
	tmpPopUp->setTitle("<bold>Close un-saved file:</bold>");
	tmpPopUp->setComment("The file named : <i>\"" + tmpBuffer->getFileName() + "\"</i> is curently modify.   <br/>If you don't saves these modifications,<br/>they will be definitly lost...");
	ewol::Widget* bt = NULL;
	if (tmpBuffer->hasFileName() == true) {
		bt = tmpPopUp->addButton("Save", true);
		if (bt != NULL) {
			bt->registerOnEvent(this, widget::Button::eventPressed, s_saveValidate, tmpBuffer->getFileName());
		}
	}
	bt = tmpPopUp->addButton("Save As", true);
	if (bt != NULL) {
		bt->registerOnEvent(this, widget::Button::eventPressed, s_saveAsValidate, tmpBuffer->getFileName());
	}
	bt = tmpPopUp->addButton("Close", true);
	if (bt != NULL) {
		bt->registerOnEvent(this, widget::Button::eventPressed, s_closeValidate, tmpBuffer->getFileName());
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
		
	} else if (_msg.getMessage() == s_saveValidate) {
		
	} else if (_msg.getMessage() == s_closeValidate) {
		
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
	}
}

