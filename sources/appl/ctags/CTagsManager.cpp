/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <CTagsManager.h>
#include <BufferManager.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/eContext.h>
#include <ewol/widget/meta/FileChooser.h>
#include <appl/Gui/TagFileSelection.h>

// TODO : The line ID is no more stored in the file system (FSNode) ...

#undef  __class__
#define __class__ "CTagsManager"

class CTagsManager: public ewol::EObject {
	public:
		// Constructeur
		CTagsManager(void);
		~CTagsManager(void);
		
		const char * const getObjectType(void) {
			return "CTagsManager";
		};
		void onReceiveMessage(const ewol::EMessage& _msg);
		
		int32_t                     m_currentSelectedID;
		void                        loadTagFile(void);
		int32_t                     multipleJump(void);
		void                        jumpTo(void);
		void                        printTag(const tagEntry *entry);
		etk::UString                getFolder(etk::UString &inputString);
		etk::UString                m_tagFolderBase;
		etk::UString                m_tagFilename;
		tagFile *                   m_ctagFile;
		// history system
		int32_t                     m_historyPos;
		etk::Vector<etk::FSNode*>   m_historyList;
		void                        registerHistory(void);
};

static CTagsManager* s_elementPointer = NULL;
void cTagsManager::init(void) {
	if (NULL != s_elementPointer) {
		s_elementPointer = NULL;
		EWOL_WARNING("Ctags manager already instanciate ...  == > restart IT (can have memory leek ...)");
	}
	s_elementPointer = new CTagsManager();
	if (NULL == s_elementPointer) {
		EWOL_ERROR("Ctags manager error to instanciate ...");
	}
}
void cTagsManager::unInit(void) {
	if (NULL != s_elementPointer) {
		delete(s_elementPointer);
		s_elementPointer = NULL;
	} else {
		EWOL_ERROR("Ctags manager not instanciate ...  == > can not remove it ...");
	}
}

CTagsManager::CTagsManager(void) {
	m_tagFilename = "";
	m_tagFolderBase = "";
	m_ctagFile = NULL;
	m_historyPos = 0;
	registerMultiCast(ednMsgGuiCtags);
	registerMultiCast(ednMsgBufferId);
	registerMultiCast(ednMsgCtagsLoadFile);
	EWOL_INFO("Ctags manager (INIT)");
}

CTagsManager::~CTagsManager(void) {
	EWOL_INFO("Ctags manager (Un-INIT)");
	if(0 != m_historyList.size()) {
		for (int32_t iii=0; iii< m_historyList.size(); iii++) {
			delete(m_historyList[iii]);
		}
		m_historyList.clear();
	}
}

const char * ednEventPopUpCtagsLoadFile = "edn-event-load-ctags";

void CTagsManager::onReceiveMessage(const ewol::EMessage& _msg) {
	//EWOL_INFO("ctags manager event ... : \"" << eventId << "\"  == > data=\"" << data << "\"" );
	if (_msg.getMessage() == ednMsgBufferId) {
		//m_currentSelectedID = dataID;
	} else if(    _msg.getMessage() == ednEventPopUpCtagsLoadFile
	           || _msg.getMessage() == ednMsgCtagsLoadFile) {
		// open the new one :
		etk::FSNode tmpFilename = _msg.getData();
		m_tagFilename = tmpFilename.getNameFile();
		m_tagFolderBase = tmpFilename.getNameFolder();
		APPL_DEBUG("Receive load Ctags file : " << m_tagFolderBase << "/" << m_tagFilename << " ");
		loadTagFile();
	} else if (_msg.getMessage() == ednMsgGuiCtags) {
		if (_msg.getData() == "Load") {
			APPL_INFO("Request opening ctag file");
			widget::FileChooser* tmpWidget = new widget::FileChooser();
			if (NULL == tmpWidget) {
				APPL_ERROR("Can not allocate widget  == > display might be in error");
			} else {
				tmpWidget->setTitle("Open Exuberant Ctags file");
				tmpWidget->setValidateLabel("Open");
				ewol::getContext().getWindows()->popUpWidgetPush(tmpWidget);
				tmpWidget->registerOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpCtagsLoadFile);
			}
		} else if (_msg.getData() == "ReLoad") {
			APPL_INFO("Request re-load ctag file");
			loadTagFile();
		} else if (_msg.getData() == "Jump") {
			jumpTo();
		} else if (_msg.getData() == "Back") {
			if (m_historyList.size() > 0) {
				int32_t id = m_historyList.size()-1;
				sendMultiCast(ednMsgOpenFile, m_historyList[id]->getName() );
				sendMultiCast(ednMsgGuiGotoLine, 0);// TODO : m_historyList[id]->getLineNumber());
				// remove element ....
				delete(m_historyList[id]);
				m_historyList[id]=NULL;
				m_historyList.popBack();
			}
		} else {
			
		}
	} else if (_msg.getMessage() == applEventctagsSelection) {
		// save the current file in the history
		registerHistory();
		// parse the input data
		char tmp[4096];
		int32_t lineID;
		sscanf(_msg.getData().c_str(), "%d:%s", &lineID, tmp);
		// generate envents
		sendMultiCast(ednMsgOpenFile, tmp);
		sendMultiCast(ednMsgGuiGotoLine, lineID - 1);
	}
}


void CTagsManager::loadTagFile(void) {
	tagFileInfo info;
	// close previous tag file
	if (NULL != m_ctagFile) {
		tagsClose(m_ctagFile);
		m_ctagFile = NULL;
	}
	if (m_tagFilename == "") {
		return;
	}
	// load (open) the tag file : 
	APPL_INFO("try to open tag file : " << m_tagFilename);
	m_ctagFile = tagsOpen(m_tagFilename.c_str(), &info);
	if (NULL != m_ctagFile) {
		APPL_INFO("open exuberant Ctags file is OK ...");
	} else {
		APPL_INFO("Error to open ctags file ...");
	}
}

void CTagsManager::registerHistory(void) {
	APPL_INFO("save curent filename and position : ");
	int32_t currentSelected = BufferManager::getSelected();
	/*
	BufferText* tmpBuf = BufferManager::get(currentSelected);
	if (NULL != tmpBuf) {
		etk::FSNode * bufferFilename = new etk::FSNode();
		*bufferFilename = tmpBuf->getFileName();
		// TODO : bufferFilename->setLineNumber(tmpBuf->getCurrentLine());
		m_historyList.pushBack(bufferFilename);
	}
	*/
}

void CTagsManager::jumpTo(void) {
	if (NULL != m_ctagFile) {
		// get the middle button of the clipboard  == > represent the current selection ...
		etk::UString data = ewol::clipBoard::get(ewol::clipBoard::clipboardSelection);
		APPL_DEBUG("clipboard data : \"" << data << "\"");
		if (data.size() == 0) {
			APPL_INFO("No current selection");
		}
		tagEntry entry;
		APPL_INFO("try to find the tag : " << data);
		if (tagsFind (m_ctagFile, &entry, data.c_str(), 0) == TagSuccess) {
			tagEntry entrySave = entry;
			int32_t numberOfTags = 0;
			
			// For all tags : Save in an internal Structure :
			etk::UString tmpFile(m_tagFolderBase + "/" + entry.file);
			etk::FSNode myfile(tmpFile);
			int32_t lineID = entry.address.lineNumber;
			printTag(&entry);
			
			if (tagsFindNext (m_ctagFile, &entry) == TagSuccess) {
				APPL_INFO("Multiple file destination ...");
				appl::TagFileSelection* tmpWidget = new appl::TagFileSelection();
				if (NULL == tmpWidget) {
					APPL_ERROR("Can not allocate widget  == > display might be in error");
				} else {
					tmpWidget->addCtagsNewItem(myfile.getName(), lineID);
					do {
						tmpFile = m_tagFolderBase + "/" + entry.file;
						myfile = tmpFile;
						lineID = entry.address.lineNumber;
						printTag(&entry);
						tmpWidget->addCtagsNewItem(myfile.getName(), lineID);
					} while (tagsFindNext (m_ctagFile, &entry) == TagSuccess);
					ewol::getContext().getWindows()->popUpWidgetPush(tmpWidget);
					tmpWidget->registerOnEvent(this, applEventctagsSelection);
				}
			} else {
				registerHistory();
				APPL_INFO(" OPEN the TAG file Destination : " << tmpFile );
				sendMultiCast(ednMsgOpenFile, myfile.getName());
				sendMultiCast(ednMsgGuiGotoLine, lineID - 1);
			}
		} else {
			APPL_INFO("no tag find ...");
		}
	}
}


void CTagsManager::printTag(const tagEntry *entry) {
	#if 1
		APPL_INFO("find Tag file : name=\"" << entry->name << "\" in file=\"" << entry->file 
			<< "\" at line="<< (int32_t)entry->address.lineNumber);
	#else
		int i;
		APPL_INFO("find Tag file : name=\"" << entry->name << "\" in file=\"" << entry->file 
			<< "\" pattern=\"" << entry->address.pattern 
			<< "\" at line="<< (int32_t)entry->address.lineNumber);
		
		APPL_INFO("Extention field : ");
		if (entry->kind != NULL  &&  entry->kind [0] != '\0') {
			APPL_INFO("        kind : " << entry->kind);
		}
		if (entry->fileScope) {
			APPL_INFO("        file : ");
		}
		for (i = 0  ;  i < entry->fields.count  ;  ++i) {
			APPL_INFO("               " << entry->fields.list[i].key << ":" << entry->fields.list[i].value );
		}
	#endif
}

