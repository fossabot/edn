/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#include <appl/debug.h>
#include <appl/global.h>
#include <appl/BufferManager.h>
#include <ewol/renderer/EObject.h>
#include <ewol/renderer/EObjectManager.h>

#undef __class__
#define __class__ "BufferManager"

appl::BufferManager::BufferManager(void) :
  ewol::Resource("???BufferManager???") {
	
}

appl::BufferManager::~BufferManager(void) {
	esize_t previousCount = m_list.size();
	for (esize_t iii = m_list.size()-1; iii >= 0 ; --iii) {
		if (m_list[iii] == NULL) {
			continue;
		}
		delete(m_list[iii]);
		if (previousCount == m_list.size()) {
			APPL_ERROR("Error in removing buffer !! ");
		}
		previousCount = m_list.size();
	}
	m_list.clear();
}

appl::Buffer* appl::BufferManager::get(const etk::UString& _fileName, bool _createIfNeeded) {
	for (esize_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii] == NULL) {
			continue;
		}

		if (m_list[iii]->getFileName() == _fileName) {
			return m_list[iii];
		}
	}
	if (_createIfNeeded == true) {
		appl::Buffer* tmp = new appl::Buffer();
		if (tmp == NULL) {
			APPL_ERROR("Can not allocate the Buffer class : " << _fileName);
			return NULL;
		}
		tmp->loadFile(_fileName);
		m_list.pushBack(tmp);
		return tmp;
	}
	return NULL;
}

void appl::BufferManager::onObjectRemove(ewol::EObject * _removeObject) {
	for (esize_t iii = 0; iii < m_list.size(); ++iii) {
		if (m_list[iii] != _removeObject) {
			continue;
		}
		m_list[iii] = NULL;
		m_list.remove(iii);
		return;
	}
}

void appl::BufferManager::open(const etk::UString& _fileName) {
	(void)get(_fileName, true);
	sendMultiCast(appl::MsgSelectNewFile, _fileName);
}

void appl::BufferManager::onReceiveMessage(const ewol::EMessage& _msg) {
	APPL_DEBUG("receive message !!! " << _msg);
}

appl::BufferManager* appl::BufferManager::keep(void) {
	//EWOL_INFO("KEEP : appl::GlyphPainting : file : \"" << _filename << "\"");
	appl::BufferManager* object = static_cast<appl::BufferManager*>(getManager().localKeep("???BufferManager???"));
	if (NULL != object) {
		return object;
	}
	// this element create a new one every time ....
	EWOL_INFO("CREATE : appl::BufferManager: ???BufferManager???");
	object = new appl::BufferManager();
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ???BufferManager???");
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void appl::BufferManager::release(appl::BufferManager*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::Resource* object2 = static_cast<ewol::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}



#ifdef QSGDQSDFGSDFGSDFGZS8DFGHD_sDFGSDFGDGT


class classBufferManager: public ewol::EObject {
	public:
		// Constructeur
		classBufferManager(void);
		~classBufferManager(void);
	public:
		virtual void onReceiveMessage(const ewol::EMessage& _msg);
	private:
		// return the ID of the buffer allocated
		// create a buffer with no element
		int32_t     create(void);
		// open curent filename
		int32_t     open(etk::FSNode &myFile);
		bool        remove(int32_t BufferID);
	public:
		int32_t     getSelected(void) { return m_idSelected;};
		//void        setSelected(int32_t id) {m_idSelected = id;};
		BufferText* get(int32_t BufferID);
		bool        exist(int32_t BufferID);
		bool        exist(etk::FSNode &myFile);
		int32_t     getId(etk::FSNode &myFile);
		// return the number of buffer (open in the past) if 5 buffer open and 4 close  == > return 5
		uint32_t    size(void);
		uint32_t    sizeOpen(void);
		int32_t     witchBuffer(int32_t iEmeElement);
		

	private:
		
		etk::Vector<BufferText*> listBuffer;  //!< element List of the char Elements
		
		void        removeAll(void);          //!< remove all buffer
		int32_t     m_idSelected;
};


// Constructeur
classBufferManager::classBufferManager(void)
{
	m_idSelected = -1;
	RegisterMultiCast(ednMsgGuiNew);
	RegisterMultiCast(ednMsgOpenFile);
	RegisterMultiCast(ednMsgGuiClose);
	RegisterMultiCast(ednMsgGuiSave);
	RegisterMultiCast(ednMsgCodeViewSelectedId);
	RegisterMultiCast(ednMsgBufferId);
}

classBufferManager::~classBufferManager(void)
{
	//clean All Buffer
	APPL_INFO("~classBufferManager::removeAll();");
	removeAll();
	// clear The list of Buffer
	APPL_INFO("~classBufferManager::listBuffer.clear();");
	listBuffer.clear();
}


void classBufferManager::onReceiveMessage(const ewol::EMessage& _msg)
{
	ewol::EObject::onReceiveMessage(_msg);
	
	if (_msg.getMessage() == ednMsgBufferId) {
		// select a new buffer ID :
		if (_msg.getData() == "") {
			APPL_ERROR("Request select buffer ID = \"\" ");
		} else {
			int32_t newID = -1;
			sscanf(_msg.getData().c_str(), "%d", &newID);
			if(true == Exist(newID)) {
				m_idSelected = newID;
			} else {
				m_idSelected = -1;
				APPL_ERROR("Request a non existant ID : " << newID << " reset to -1...");
			}
		}
	} else if (_msg.getMessage() == ednMsgGuiNew) {
		int32_t newOne = Create();
		if (-1 != newOne) {
			m_idSelected = newOne;
			SendMultiCast(ednMsgBufferId, m_idSelected);
			SendMultiCast(ednMsgBufferListChange);
		}
	} else if (_msg.getMessage() == ednMsgOpenFile) {
		if (_msg.getData() != "" ) {
			etk::FSNode myFile(_msg.getData());
			if (myFile.getNodeType() == etk::FSN_FILE) {
				APPL_DEBUG("request open file = \"" << _msg.getData() << "\" ?= \"" << myFile << "\"");
				int32_t newOne = open(myFile);
				if (-1 != newOne) {
					m_idSelected = newOne;
					SendMultiCast(ednMsgBufferId, m_idSelected);
					SendMultiCast(ednMsgBufferListChange);
				} else {
					// TODO : notify user that we can not open the request file...
					APPL_ERROR("Can not open the file : \"" << myFile << "\"");
				}
			} else {
				APPL_ERROR("Request to open an Unknox element file : " << myFile << " type:" << myFile.getNodeType());
			}
		}
	} else if (_msg.getMessage() == ednMsgGuiSave) {
		if (_msg.getData() == "") {
			APPL_ERROR("Null data for close file ... ");
		} else {
			if (_msg.getData() == "current") {
				// Check buffer existence
				if(true == Exist(m_idSelected)) {
					// If no name  == > request a Gui display ...
					if (get(m_idSelected)->haveName() == false) {
						SendMultiCast(ednMsgGuiSaveAs, "current");
					} else {
						get(m_idSelected)->Save();
					}
				}
			} else {
				int32_t newId;
				sscanf(_msg.getData().c_str(), "%d", &newId);
				if (false == Exist(newId)) {
					APPL_ERROR("Request a save As with a non existant ID=" << newId);
				} else {
					// If no name  == > request a Gui display ...
					if (get(newId)->haveName() == false) {
						SendMultiCast(ednMsgGuiSaveAs, newId);
					} else {
						get(m_idSelected)->Save();
					}
				}
				SendMultiCast(ednMsgBufferState, "saved");
			}
		}
	} else if (_msg.getMessage() == ednMsgGuiClose) {
		if (_msg.getData() == "") {
			APPL_ERROR("Null data for close file ... ");
		} else {
			if (_msg.getData() == "All") {
				
			} else {
				int32_t closeID = -1;
				if (_msg.getData() == "current") {
					closeID = m_idSelected;
					APPL_DEBUG("Close specific buffer ID" << closeID);
				} else {
					// close specific buffer ...
					sscanf(_msg.getData().c_str(), "%d", &closeID);
					APPL_DEBUG("Close specific buffer ID="<< closeID);
				}
				if(true == Exist(closeID)) {
					// get the new display buffer 
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
							for(int32_t ii=closeID+1; ii < listBuffer.size(); ii++) {
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
					// remove requested buffer
					remove(closeID);
					SendMultiCast(ednMsgBufferListChange);
				} else {
					APPL_ERROR("Request close of a non existant ID : " << closeID);
				}
			}
		}
	} else if (_msg.getMessage() == ednMsgCodeViewSelectedId) {
		//Change the selected buffer
		if (_msg.getData() == "") {
			APPL_ERROR("Null data for changing buffer ID file ... ");
		} else {
			int32_t newId;
			sscanf(_msg.getData().c_str(), "%d", &newId);
			if (true == Exist(newId)) {
				m_idSelected = newId;
			} else {
				APPL_ERROR("code biew request the selection of an non -existant buffer  == > reset to -1");
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
				// If no name  == > request a Gui display ...
				if (get(dataID)->haveName() == false) {
					SendMessage(APPL_MSG__GUI_SHOW_SAVE_AS, dataID);
				} else {
					get(dataID)->Save();
				}
			}
			break;
	}
	*/
}

void classBufferManager::removeAll(void) {
	int32_t i;
	for (i=0; i<listBuffer.size(); i++) {
		remove(i);
	}
	SendMultiCast(ednMsgGuiClose, "All");
}



/**
 * @brief Create a new buffer with no name and empty
 * @return The ID of the curent buffer where the file is loaded
 */
int32_t	classBufferManager::create(void) {
	// allocate a new Buffer
	BufferText *myBuffer = new BufferText();
	// add at the list of element
	listBuffer.pushBack(myBuffer);
	int32_t basicID = listBuffer.size() - 1;
	return basicID;
}


/**
 * @brief open a file with the name set in parameters
 * @param[in] filename curent filename
 * @return The ID of the curent buffer where the file is loaded
 * @todo : check if this file is not curently open and return the old ID
 */
int32_t classBufferManager::open(etk::FSNode &myFile) {
	if (false == exist(myFile)) {
		// allocate a new Buffer
		BufferText *myBuffer = new BufferText(myFile);
		// add at the list of element
		listBuffer.pushBack(myBuffer);
		return listBuffer.size() - 1;
	} else {
		// the buffer already existed  == > we open it ...
		return getId(myFile);
	}
}

BufferText * classBufferManager::get(int32_t BufferID) {
	// possible special case : -1;
	if (-1 >= BufferID) {
		return NULL;
	}
	// check if the Buffer existed
	if (BufferID < listBuffer.size()) {
		// check if the buffer already existed
		if (NULL != listBuffer[BufferID]) {
			return listBuffer[BufferID];
		} else {
			APPL_ERROR("non existing Buffer " << BufferID);
		}
	} else {
		APPL_ERROR("call an non existing Buffer number too hight : " << BufferID << " > " << listBuffer.size());
	}
	return NULL;
}

bool classBufferManager::exist(int32_t BufferID) {
	if (-1 >= BufferID) {
		return false;
	}
	// check if the Buffer existed
	if (BufferID < listBuffer.size()) {
		// check if the buffer already existed
		if (NULL != listBuffer[BufferID]) {
			return true;
		}
	}
	return false;
}

bool classBufferManager::exist(etk::FSNode &myFile) {
	if (-1 == getId(myFile)) {
		return false;
	}
	return true;
}

int32_t classBufferManager::getId(etk::FSNode &myFile) {
	int32_t iii;
	// check if the Buffer existed
	for (iii=0; iii < listBuffer.size(); iii++) {
		// check if the buffer already existed
		if (NULL != listBuffer[iii]) {
			if ( listBuffer[iii]->getFileName() == myFile) {
				return iii;
			}
		}
	}
	return -1;
}


// return the number of buffer (open in the past) if 5 buffer open and 4 close  == > return 5
uint32_t classBufferManager::size(void) {
	return listBuffer.size();
}

// nb of opens file Now ...
uint32_t classBufferManager::sizeOpen(void) {
	uint32_t jjj = 0;
	// check if the Buffer existed
	for (int32_t iii=0; iii<listBuffer.size(); iii++) {
		// check if the buffer already existed
		if (NULL != listBuffer[iii]) {
			jjj++;
		}
	}
	return jjj;
}

bool classBufferManager::remove(int32_t BufferID) {
	if (-1 >= BufferID) {
		return false;
	}
	// check if the Buffer existed
	if (BufferID < listBuffer.size()) {
		// check if the buffer already existed
		if (NULL != listBuffer[BufferID]) {
			// TODO : Check if it saved...
			/*
			if (false == isSaved(BufferID) ) {
				APPL_INFO("Buffer " << BufferID << " : Not Saved", BufferID);
			}
			*/
			// Delete the Buffer
			delete( listBuffer[BufferID] );
			listBuffer[BufferID] = NULL;
			/*
			ewol::widgetMessageMultiCast::Send(getWidgetId(), ednMsgBufferListChange);
			*/
			return true;
		} else {
			APPL_INFO("non existing Buffer " << BufferID);
			return false;
		}
	} else {
		APPL_INFO("call an non existing Buffer number too hight : " << BufferID << " > " << listBuffer.size());
		return false;
	}
}

/**
 * @brief to get the element 14 in the buffer
 */
int32_t classBufferManager::witchBuffer(int32_t iEmeElement) {
	int32_t i;
	for (i=0; i<listBuffer.size(); i++) {
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


appl::Buffer* get(const etk::UString& _filename);
appl::Buffer* get(esize_t _bufferID);
esize_t size(void):


#endif
