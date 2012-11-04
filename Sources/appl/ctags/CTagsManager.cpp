/**
 *******************************************************************************
 * @file CTagsManager.cpp
 * @brief Editeur De N'ours : Ctags manager : acces to the ctags file (Sources)
 * @author Edouard DUPIN
 * @date 15/07/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <appl/Debug.h>
#include <appl/global.h>
#include <CTagsManager.h>
#include <BufferManager.h>
#include <ewol/eObject/EObject.h>
#include <ewol/widget/meta/FileChooser.h>
#include <appl/Gui/TagFileSelection.h>

// TODO : The line ID is no more stored in the file system (FSNode) ...

#undef  __class__
#define __class__    "CTagsManager"

class CTagsManager: public ewol::EObject
{
	public:
		// Constructeur
		CTagsManager(void);
		~CTagsManager(void);
		
		/**
		 * @brief Get the current Object type of the EObject
		 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
		 * @param[in] objectType type description
		 * @return true if the object is compatible, otherwise false
		 */
		const char * const GetObjectType(void)
		{
			return "CTagsManager";
		};
		/**
		 * @brief Receive a message from an other EObject with a specific eventId and data
		 * @param[in] CallerObject Pointer on the EObject that information came from
		 * @param[in] eventId Message registered by this class
		 * @param[in] data Data registered by this class
		 * @return ---
		 */
		void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		
		int32_t                     m_currentSelectedID;
		void                        LoadTagFile(void);
		int32_t                     MultipleJump(void);
		void                        JumpTo(void);
		void                        PrintTag(const tagEntry *entry);
		etk::UString                GetFolder(etk::UString &inputString);
		etk::UString                m_tagFolderBase;
		etk::UString                m_tagFilename;
		tagFile *                   m_ctagFile;
		// history system
		int32_t                     m_historyPos;
		etk::Vector<etk::FSNode*>   m_historyList;
		void                        RegisterHistory(void);
};

static CTagsManager* s_elementPointer = NULL;
void cTagsManager::Init(void)
{
	if (NULL != s_elementPointer) {
		s_elementPointer = NULL;
		EWOL_WARNING("Ctags manager already instanciate ... ==> restart IT (can have memory leek ...)");
	}
	s_elementPointer = new CTagsManager();
	if (NULL == s_elementPointer) {
		EWOL_ERROR("Ctags manager error to instanciate ...");
	}
}
void cTagsManager::UnInit(void)
{
	if (NULL != s_elementPointer) {
		delete(s_elementPointer);
		s_elementPointer = NULL;
	} else {
		EWOL_ERROR("Ctags manager not instanciate ... ==> can not remove it ...");
	}
}



/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
CTagsManager::CTagsManager(void)
{
	m_tagFilename = "";
	m_tagFolderBase = "";
	m_ctagFile = NULL;
	m_historyPos = 0;
	RegisterMultiCast(ednMsgGuiCtags);
	RegisterMultiCast(ednMsgBufferId);
	RegisterMultiCast(ednMsgCtagsLoadFile);
	EWOL_INFO("Ctags manager (INIT)");
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
CTagsManager::~CTagsManager(void)
{
	EWOL_INFO("Ctags manager (Un-INIT)");
	if(0 != m_historyList.Size()) {
		for (int32_t iii=0; iii< m_historyList.Size(); iii++) {
			delete(m_historyList[iii]);
		}
		m_historyList.Clear();
	}
}

const char * ednEventPopUpCtagsLoadFile = "edn-event-load-ctags";

void CTagsManager::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	//EWOL_INFO("ctags manager event ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventId == ednMsgBufferId) {
		//m_currentSelectedID = dataID;
	} else if(    eventId == ednEventPopUpCtagsLoadFile
	           || eventId == ednMsgCtagsLoadFile) {
		// open the new one :
		etk::FSNode tmpFilename = data;
		m_tagFilename = tmpFilename.GetNameFile();
		m_tagFolderBase = tmpFilename.GetNameFolder();
		APPL_DEBUG("Receive load Ctags file : " << m_tagFolderBase << "/" << m_tagFilename << " ");
		LoadTagFile();
	} else if (eventId == ednMsgGuiCtags) {
		if (data == "Load") {
			APPL_INFO("Request opening ctag file");
			ewol::FileChooser* tmpWidget = new ewol::FileChooser();
			if (NULL == tmpWidget) {
				APPL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				tmpWidget->SetTitle("Open Exuberant Ctags File");
				tmpWidget->SetValidateLabel("Open");
				PopUpWidgetPush(tmpWidget);
				tmpWidget->RegisterOnEvent(this, ewolEventFileChooserValidate, ednEventPopUpCtagsLoadFile);
			}
		} else if (data == "ReLoad") {
			APPL_INFO("Request re-load ctag file");
			LoadTagFile();
		} else if (data == "Jump") {
			JumpTo();
		} else if (data == "Back") {
			if (m_historyList.Size() > 0) {
				int32_t id = m_historyList.Size()-1;
				SendMultiCast(ednMsgOpenFile, m_historyList[id]->GetName() );
				SendMultiCast(ednMsgGuiGotoLine, 0);// TODO : m_historyList[id]->GetLineNumber());
				// Remove element ....
				delete(m_historyList[id]);
				m_historyList[id]=NULL;
				m_historyList.PopBack();
			}
		} else {
			
		}
	} else if (eventId == applEventctagsSelection) {
		// save the current file in the history
		RegisterHistory();
		// parse the input data
		char tmp[4096];
		int32_t lineID;
		sscanf(data.c_str(), "%d:%s", &lineID, tmp);
		// generate envents
		SendMultiCast(ednMsgOpenFile, tmp);
		SendMultiCast(ednMsgGuiGotoLine, lineID - 1);
	}
}


void CTagsManager::LoadTagFile(void)
{
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


void CTagsManager::RegisterHistory(void)
{
	APPL_INFO("save curent filename and position : ");
	int32_t currentSelected = BufferManager::GetSelected();
	Buffer* tmpBuf = BufferManager::Get(currentSelected);
	if (NULL != tmpBuf) {
		etk::FSNode * bufferFilename = new etk::FSNode();
		*bufferFilename = tmpBuf->GetFileName();
		// TODO : bufferFilename->SetLineNumber(tmpBuf->GetCurrentLine());
		m_historyList.PushBack(bufferFilename);
	}
}


void CTagsManager::JumpTo(void)
{
	if (NULL != m_ctagFile) {
		// get the middle button of the clipboard ==> represent the current selection ...
		etk::UString data = ewol::clipBoard::Get(ewol::clipBoard::CLIPBOARD_SELECTION);
		APPL_DEBUG("clipboard data : \"" << data << "\"");
		if (data.Size() == 0) {
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
			PrintTag(&entry);
			
			if (tagsFindNext (m_ctagFile, &entry) == TagSuccess) {
				APPL_INFO("Multiple file destination ...");
				appl::TagFileSelection* tmpWidget = new appl::TagFileSelection();
				if (NULL == tmpWidget) {
					APPL_ERROR("Can not allocate widget ==> display might be in error");
				} else {
					tmpWidget->AddCtagsNewItem(myfile.GetName(), lineID);
					do {
						tmpFile = m_tagFolderBase + "/" + entry.file;
						myfile = tmpFile;
						lineID = entry.address.lineNumber;
						PrintTag(&entry);
						tmpWidget->AddCtagsNewItem(myfile.GetName(), lineID);
					} while (tagsFindNext (m_ctagFile, &entry) == TagSuccess);
					PopUpWidgetPush(tmpWidget);
					tmpWidget->RegisterOnEvent(this, applEventctagsSelection);
				}
			} else {
				RegisterHistory();
				APPL_INFO(" OPEN the TAG file Destination : " << tmpFile );
				SendMultiCast(ednMsgOpenFile, myfile.GetName());
				SendMultiCast(ednMsgGuiGotoLine, lineID - 1);
			}
		} else {
			APPL_INFO("no tag find ...");
		}
	}
}


void CTagsManager::PrintTag(const tagEntry *entry)
{
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

