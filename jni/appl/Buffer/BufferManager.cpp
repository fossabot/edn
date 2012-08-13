/**
 *******************************************************************************
 * @file BufferManager.cpp
 * @brief Editeur De N'ours : Text Buffer manager (sources)
 * @author Edouard DUPIN
 * @date 08/12/2010
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
#include <BufferManager.h>
#include <ewol/EObject.h>
#include <ewol/EObjectManager.h>

#undef __class__
#define __class__	"classBufferManager"

class classBufferManager: public ewol::EObject
{
	public:
		// Constructeur
		classBufferManager(void);
		~classBufferManager(void);
		/**
		 * @brief Get the current Object type of the EObject
		 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
		 * @param[in] objectType type description
		 * @return true if the object is compatible, otherwise false
		 */
		const char * const GetObjectType(void)
		{
			return "ApplBufferManager";
		}
	public:
		/**
		 * @brief Receive a message from an other EObject with a specific eventId and data
		 * @param[in] CallerObject Pointer on the EObject that information came from
		 * @param[in] eventId Message registered by this class
		 * @param[in] data Data registered by this class
		 * @return ---
		 */
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
	private:
		// return the ID of the buffer allocated
		// create a buffer with no element
		int32_t     Create(void);
		// open curent filename
		int32_t     Open(etk::File &myFile);
		bool        Remove(int32_t BufferID);
	public:
		int32_t     GetSelected(void) { return m_idSelected;};
		//void        SetSelected(int32_t id) {m_idSelected = id;};
		Buffer *    Get(int32_t BufferID);
		bool        Exist(int32_t BufferID);
		bool        Exist(etk::File &myFile);
		int32_t     GetId(etk::File &myFile);
		// return the number of buffer (open in the past) if 5 buffer open and 4 close ==> return 5
		uint32_t    Size(void);
		uint32_t    SizeOpen(void);
		int32_t     WitchBuffer(int32_t iEmeElement);
		

	private:
		
		etk::VectorType<Buffer*> listBuffer;  //!< element List of the char Elements
		
		void        RemoveAll(void);          //!< remove all buffer
		int32_t     m_idSelected;
		Buffer *    BufferNotExiste;          //!< When an error arrive in get buffer we return the Error buffer (not writable)
};


// Constructeur
/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
classBufferManager::classBufferManager(void)
{
	// nothing to do ...
	BufferNotExiste = new BufferEmpty();
	m_idSelected = -1;
	RegisterMultiCast(ednMsgGuiNew);
	RegisterMultiCast(ednMsgOpenFile);
	RegisterMultiCast(ednMsgGuiClose);
	RegisterMultiCast(ednMsgGuiSave);
	RegisterMultiCast(ednMsgCodeViewSelectedId);
	RegisterMultiCast(ednMsgBufferId);
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
classBufferManager::~classBufferManager(void)
{
	//clean All Buffer
	APPL_INFO("~classBufferManager::RemoveAll();");
	RemoveAll();
	// clear The list of Buffer
	APPL_INFO("~classBufferManager::listBuffer.Clear();");
	listBuffer.Clear();
	APPL_INFO("~classBufferManager::delete(BufferNotExiste);");
	delete(BufferNotExiste);
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void classBufferManager::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	ewol::EObject::OnReceiveMessage(CallerObject, eventId, data);
	
	if (eventId == ednMsgBufferId) {
		// select a new buffer ID :
		if (data == "") {
			APPL_ERROR("Request select buffer ID = \"\" ");
		} else {
			int32_t newID = -1;
			sscanf(data.Utf8Data(), "%d", &newID);
			if(true == Exist(newID)) {
				m_idSelected = newID;
			} else {
				m_idSelected = -1;
				APPL_ERROR("Request a non existant ID : " << newID << " reset to -1...");
			}
		}
	} else if (eventId == ednMsgGuiNew) {
		int32_t newOne = Create();
		if (-1 != newOne) {
			m_idSelected = newOne;
			SendMultiCast(ednMsgBufferId, m_idSelected);
			SendMultiCast(ednMsgBufferListChange);
		}
	} else if (eventId == ednMsgOpenFile) {
		if (data != "" ) {
			etk::File myFile(data, etk::FILE_TYPE_DIRECT);
			int32_t newOne = Open(myFile);
			if (-1 != newOne) {
				m_idSelected = newOne;
				SendMultiCast(ednMsgBufferId, m_idSelected);
				SendMultiCast(ednMsgBufferListChange);
			} else {
				// TODO : notify user that we can not open the request file...
				APPL_ERROR("Can not open the file : \"" << myFile << "\"");
			}
		}
	} else if (eventId == ednMsgGuiSave) {
		if (data == "") {
			APPL_ERROR("Null data for close file ... ");
		} else {
			if (data == "current") {
				// Check buffer existence
				if(true == Exist(m_idSelected)) {
					// If no name ==> request a Gui display ...
					if (Get(m_idSelected)->HaveName() == false) {
						SendMultiCast(ednMsgGuiSaveAs, "current");
					} else {
						Get(m_idSelected)->Save();
					}
				}
			} else {
				int32_t newId;
				sscanf(data.Utf8Data(), "%d", &newId);
				if (false == Exist(newId)) {
					APPL_ERROR("Request a save As with a non existant ID=" << newId);
				} else {
					// If no name ==> request a Gui display ...
					if (Get(newId)->HaveName() == false) {
						SendMultiCast(ednMsgGuiSaveAs, newId);
					} else {
						Get(m_idSelected)->Save();
					}
				}
				SendMultiCast(ednMsgBufferState, "saved");
			}
		}
	} else if (eventId == ednMsgGuiClose) {
		if (data == "") {
			APPL_ERROR("Null data for close file ... ");
		} else {
			if (data == "All") {
				
			} else {
				int32_t closeID = -1;
				if (data == "current") {
					closeID = m_idSelected;
					APPL_DEBUG("Close specific buffer ID" << closeID);
				} else {
					// close specific buffer ...
					sscanf(data.Utf8Data(), "%d", &closeID);
					APPL_DEBUG("Close specific buffer ID="<< closeID);
				}
				if(true == Exist(closeID)) {
					// Get the new display buffer 
					if (m_idSelected == closeID) {
						// Try previous buffer
						int32_t destBuffer = -1;
						for(int32_t ii=closeID-1; ii >= 0; ii--) {
							if (true == Exist(ii) ) {
								destBuffer = ii;
								break;
							}
						}
						// try next buffer
						if (-1 == destBuffer) {
							for(int32_t ii=closeID+1; ii < listBuffer.Size(); ii++) {
								if (true == Exist(ii) ) {
									destBuffer = ii;
									break;
								}
							}
						}
						// set it to the currect display
						m_idSelected = destBuffer;
						SendMultiCast(ednMsgBufferId, destBuffer);
					}
					// Remove requested buffer
					Remove(closeID);
					SendMultiCast(ednMsgBufferListChange);
				} else {
					APPL_ERROR("Request Close of a non existant ID : " << closeID);
				}
			}
		}
	} else if (eventId == ednMsgCodeViewSelectedId) {
		//Change the selected buffer
		if (data == "") {
			APPL_ERROR("Null data for changing buffer ID file ... ");
		} else {
			int32_t newId;
			sscanf(data.Utf8Data(), "%d", &newId);
			if (true == Exist(newId)) {
				m_idSelected = newId;
			} else {
				APPL_ERROR("code biew request the selection of an non -existant buffer ==> reset to -1");
				m_idSelected = -1;
			}
			SendMultiCast(ednMsgBufferId, m_idSelected);
			SendMultiCast(ednMsgBufferListChange);
		}
	}
	/*
	switch (id)
	{
			// Check buffer existence
			if(true == Exist(dataID)) {
				// If no name ==> request a Gui display ...
				if (Get(dataID)->HaveName() == false) {
					SendMessage(APPL_MSG__GUI_SHOW_SAVE_AS, dataID);
				} else {
					Get(dataID)->Save();
				}
			}
			break;
	}
	*/
}







/**
 * @brief Remove all buffer opened
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void classBufferManager::RemoveAll(void)
{
	int32_t i;
	for (i=0; i<listBuffer.Size(); i++) {
		Remove(i);
	}
	SendMultiCast(ednMsgGuiClose, "All");
}



/**
 * @brief Create a new buffer with no name and empty
 *
 * @param[in,out] ---
 *
 * @return The ID of the curent buffer where the file is loaded
 *
 */
int32_t	classBufferManager::Create(void)
{
	// allocate a new Buffer
	Buffer *myBuffer = new BufferText();
	// Add at the list of element
	listBuffer.PushBack(myBuffer);
	int32_t basicID = listBuffer.Size() - 1;
	return basicID;
}


/**
 * @brief open a file with the name set in parameters
 *
 * @param[in]		filename			curent filename
 *
 * @return The ID of the curent buffer where the file is loaded
 *
 * @todo : check if this file is not curently open and return the old ID
 *
 */
int32_t classBufferManager::Open(etk::File &myFile)
{
	if (false == Exist(myFile)) {
		// allocate a new Buffer
		Buffer *myBuffer = new BufferText(myFile);
		// Add at the list of element
		listBuffer.PushBack(myBuffer);
		return listBuffer.Size() - 1;
	} else {
		// the buffer already existed ==> we open it ...
		return GetId(myFile);
	}
}



Buffer * classBufferManager::Get(int32_t BufferID)
{
	// possible special case : -1;
	if (-1 >= BufferID) {
		return BufferNotExiste;
	}
	// check if the Buffer existed
	if (BufferID < listBuffer.Size()) {
		// check if the buffer already existed
		if (NULL != listBuffer[BufferID]) {
			return listBuffer[BufferID];
		} else {
			APPL_ERROR("non existing Buffer " << BufferID);
		}
	} else {
		APPL_ERROR("call an non existing Buffer number too hight : " << BufferID << " > " << listBuffer.Size());
	}
	return BufferNotExiste;
}


bool classBufferManager::Exist(int32_t BufferID)
{
	if (-1 >= BufferID) {
		return false;
	}
	// check if the Buffer existed
	if (BufferID < listBuffer.Size()) {
		// check if the buffer already existed
		if (NULL != listBuffer[BufferID]) {
			return true;
		}
	}
	return false;
}


bool classBufferManager::Exist(etk::File &myFile )
{
	if (-1 == GetId(myFile)) {
		return false;
	}
	return true;
}


int32_t classBufferManager::GetId(etk::File &myFile)
{
	int32_t iii;
	// check if the Buffer existed
	for (iii=0; iii < listBuffer.Size(); iii++) {
		// check if the buffer already existed
		if (NULL != listBuffer[iii]) {
			if ( listBuffer[iii]->GetFileName() == myFile) {
				return iii;
			}
		}
	}
	return -1;
}


// return the number of buffer (open in the past) if 5 buffer open and 4 close ==> return 5
uint32_t classBufferManager::Size(void)
{
	return listBuffer.Size();
}

// nb of opens file Now ...
uint32_t classBufferManager::SizeOpen(void)
{
	uint32_t jjj = 0;
	// check if the Buffer existed
	for (int32_t iii=0; iii<listBuffer.Size(); iii++) {
		// check if the buffer already existed
		if (NULL != listBuffer[iii]) {
			jjj++;
		}
	}
	return jjj;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
bool classBufferManager::Remove(int32_t BufferID)
{
	if (-1 >= BufferID) {
		return false;
	}
	// check if the Buffer existed
	if (BufferID < listBuffer.Size()) {
		// check if the buffer already existed
		if (NULL != listBuffer[BufferID]) {
			// TODO : Check if it saved...
			/*
			if (false == IsSaved(BufferID) ) {
				APPL_INFO("Buffer " << BufferID << " : Not Saved", BufferID);
			}
			*/
			// Delete the Buffer
			delete( listBuffer[BufferID] );
			listBuffer[BufferID] = NULL;
			/*
			ewol::widgetMessageMultiCast::Send(GetWidgetId(), ednMsgBufferListChange);
			*/
			return true;
		} else {
			APPL_INFO("non existing Buffer " << BufferID);
			return false;
		}
	} else {
		APPL_INFO("call an non existing Buffer number too hight : " << BufferID << " > " << listBuffer.Size());
		return false;
	}
}

/**
 * @brief to get the element 14 in the buffer 
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
int32_t classBufferManager::WitchBuffer(int32_t iEmeElement)
{
	int32_t i;
	for (i=0; i<listBuffer.Size(); i++) {
		if (NULL != listBuffer[i]) {
			iEmeElement--;
			// find the element : 
			if (0 == iEmeElement) {
				return i;
			}
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Namespace part : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static classBufferManager * localManager = NULL;

void BufferManager::Init(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> already exist, just unlink the previous ...");
		localManager = NULL;
	}
	localManager = new classBufferManager();
	
	if (NULL == localManager) {
		EWOL_CRITICAL("Allocation of classBufferManager not done ...");
	}
}

void BufferManager::UnInit(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return;
	}
	delete(localManager);
	localManager = NULL;
}

int32_t BufferManager::GetSelected(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return -1;
	}
	return localManager->GetSelected();
}

Buffer * BufferManager::Get(int32_t BufferID)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return NULL;
	}
	return localManager->Get(BufferID);
}

bool BufferManager::Exist(int32_t BufferID)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return false;
	}
	return localManager->Exist(BufferID);
}

bool BufferManager::Exist(etk::File &myFile)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return false;
	}
	return localManager->Exist(myFile);
}

int32_t BufferManager::GetId(etk::File &myFile)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return -1;
	}
	return localManager->GetId(myFile);
}

uint32_t BufferManager::Size(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return 0;
	}
	return localManager->Size();
}

uint32_t BufferManager::SizeOpen(void)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return 0;
	}
	return localManager->SizeOpen();
}

int32_t BufferManager::WitchBuffer(int32_t iEmeElement)
{
	if (NULL == localManager) {
		EWOL_ERROR("classBufferManager ==> request UnInit, but does not exist ...");
		return -1;
	}
	return localManager->WitchBuffer(iEmeElement);
}


