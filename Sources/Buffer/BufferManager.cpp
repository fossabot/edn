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


#include "tools_debug.h"
#include "tools_globals.h"
#include "BufferManager.h"
//#include "MsgBroadcast.h"

#undef __class__
#define __class__	"BufferManager"

// Constructeur
/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
BufferManager::BufferManager(void) : MsgBroadcast("Buffer Manager", EDN_CAT_BUFFER_MANAGER)
{
	// nothing to do ...
	BufferNotExiste = new BufferEmpty();
	m_idSelected = -1;
}

/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
BufferManager::~BufferManager(void)
{
	//clean All Buffer
	EDN_INFO("~BufferManager::RemoveAll();");
	RemoveAll();
	// clear The list of Buffer
	EDN_INFO("~BufferManager::listBuffer.Clear();");
	listBuffer.Clear();
	EDN_INFO("~BufferManager::delete(BufferNotExiste);");
	delete(BufferNotExiste);
}


void BufferManager::OnMessage(int32_t id, int32_t dataID)
{
	switch (id)
	{
		case EDN_MSG__BUFFER_CHANGE_CURRENT:
			m_idSelected = dataID;
			break;
		case EDN_MSG__NEW:
			{
				int32_t newOne = Create();
				if (-1 != newOne) {
					SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, newOne);
				}
			}
			break;
		case EDN_MSG__BUFF_ID_CLOSE:
			// Check buffer existence
			if(true == Exist(dataID)) {
				// Get the new display buffer 
				if (m_idSelected == dataID) {
					// Try previous buffer
					int32_t destBuffer = -1;
					for(int32_t ii=dataID-1; ii >= 0; ii--) {
						if (true == Exist(ii) ) {
							destBuffer = ii;
							break;
						}
					}
					//EDN_DEBUG("new buffer selected : ?? " << destBuffer);
					// try next buffer
					if (-1 == destBuffer) {
						for(int32_t ii=dataID+1; ii < listBuffer.Size(); ii++) {
							if (true == Exist(ii) ) {
								destBuffer = ii;
								break;
							}
						}
					}
					//EDN_DEBUG("new buffer selected : ?? " << destBuffer);
					// set it to the currect display
					SendMessage(EDN_MSG__CURRENT_CHANGE_BUFFER_ID, destBuffer);
				}
				//EDN_DEBUG("Remove : " << dataID);
				// Remove requested buffer
				Remove(dataID);
			}
			break;
		case EDN_MSG__BUFF_ID_SAVE:
			// Check buffer existence
			if(true == Exist(dataID)) {
				// If no name ==> request a Gui display ...
				if (Get(dataID)->HaveName() == false) {
					SendMessage(EDN_MSG__GUI_SHOW_SAVE_AS, dataID);
				} else {
					Get(dataID)->Save();
				}
			}
			break;
	}
}







/**
 * @brief Remove all buffer opened
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
void BufferManager::RemoveAll(void)
{
	int32_t i;
	for (i=0; i<listBuffer.Size(); i++) {
		Remove(i);
	}
	SendMessage(EDN_MSG__BUFFER_REMOVE_ALL);
}



/**
 * @brief Create a new buffer with no name and empty
 *
 * @param[in,out] ---
 *
 * @return The ID of the curent buffer where the file is loaded
 *
 */
int32_t	BufferManager::Create(void)
{
	// allocate a new Buffer
	Buffer *myBuffer = new BufferText();
	// Add at the list of element
	listBuffer.PushBack(myBuffer);
	int32_t basicID = listBuffer.Size() - 1;
	SendMessage(EDN_MSG__BUFFER_ADD, basicID);
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
int32_t BufferManager::Open(Edn::File &myFile)
{
	// TODO : Check here if the file is already open ==> and display it if needed
	// allocate a new Buffer
	Buffer *myBuffer = new BufferText(myFile);
	// Add at the list of element
	listBuffer.PushBack(myBuffer);
	int32_t basicID = listBuffer.Size() - 1;
	SendMessage(EDN_MSG__BUFFER_ADD, basicID);
	return basicID;
}



Buffer * BufferManager::Get(int32_t BufferID)
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
			EDN_ERROR("non existing Buffer " << BufferID);
		}
	} else {
		EDN_ERROR("call an non existing Buffer number too hight : " << BufferID << " > " << listBuffer.Size());
	}
	return BufferNotExiste;
}


bool BufferManager::Exist(int32_t BufferID)
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


bool BufferManager::Exist(Edn::File &myFile )
{
	if (-1 == GetId(myFile)) {
		return false;
	}
	return true;
}


int32_t BufferManager::GetId(Edn::File &myFile)
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
uint32_t BufferManager::Size(void)
{
	return listBuffer.Size();
}


/**
 * @brief
 *
 * @param[in,out] ---
 *
 * @return ---
 *
 */
bool BufferManager::Remove(int32_t BufferID)
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
				EDN_INFO("Buffer " << BufferID << " : Not Saved", BufferID);
			}
			*/
			// Delete the Buffer
			delete( listBuffer[BufferID] );
			listBuffer[BufferID] = NULL;
			SendMessage(EDN_MSG__BUFFER_REMOVE, BufferID);
			return true;
		} else {
			EDN_INFO("non existing Buffer " << BufferID);
			return false;
		}
	} else {
		EDN_INFO("call an non existing Buffer number too hight : " << BufferID << " > " << listBuffer.Size());
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
int32_t BufferManager::WitchBuffer(int32_t iEmeElement)
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






