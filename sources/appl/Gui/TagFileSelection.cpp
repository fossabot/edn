/**
 *******************************************************************************
 * @file TagFileSelection.cpp
 * @brief Editeur De N'ours : Tags list selection to jump (sources)
 * @author Edouard DUPIN
 * @date 16/10/2012
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


/**
 *******************************************************************************
 * @file ewol/widget/meta/FileChooser.cpp
 * @brief ewol File chooser meta widget system (Sources)
 * @author Edouard DUPIN
 * @date 29/12/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <appl/Gui/TagFileSelection.h>
#include <ewol/widget/SizerHori.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/WidgetManager.h>
#include <etk/Vector.h>
#include <etk/tool.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Label.h>

#include <ewol/ewol.h>



#undef __class__
#define __class__	"TagFileSelection"


extern const char * const applEventctagsSelection           = "appl-event-ctags-validate";
extern const char * const applEventctagsCancel              = "appl-event-ctags-cancel";


appl::TagFileSelection::TagFileSelection(void)
{
	AddEventId(applEventctagsSelection);
	AddEventId(applEventctagsCancel);
	
	ewol::Label*      myWidgetTitle = NULL;
	ewol::Button*     myWidgetValidate = NULL;
	ewol::Button*     myWidgetCancel = NULL;
	
	ewol::SizerVert * mySizerVert = NULL;
	ewol::SizerHori * mySizerHori = NULL;
	ewol::Spacer *    mySpacer = NULL;
	#if defined(__TARGET_OS__Android)
		SetDisplayRatio(0.90);
	#elif defined(__TARGET_OS__Windows)
		SetDisplayRatio(0.80);
	#else
		SetDisplayRatio(0.80);
	#endif
	
	mySizerVert = new ewol::SizerVert();
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		mySizerVert->LockExpendContamination(true);
		// set it in the pop-up-system : 
		SubWidgetSet(mySizerVert);
		
		mySizerHori = new ewol::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new ewol::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			myWidgetValidate = new ewol::Button("Jump");
			if (NULL == myWidgetValidate) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myWidgetValidate->SetImage("icon/Load.svg");
				myWidgetValidate->RegisterOnEvent(this, ewolEventButtonPressed, applEventctagsSelection);
				mySizerHori->SubWidgetAdd(myWidgetValidate);
			}
			myWidgetCancel = new ewol::Button("Cancel");
			if (NULL == myWidgetCancel) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myWidgetCancel->SetImage("icon/Remove.svg");
				myWidgetCancel->RegisterOnEvent(this, ewolEventButtonPressed, applEventctagsCancel);
				mySizerHori->SubWidgetAdd(myWidgetCancel);
			}
		}
		m_listTag = new appl::TagFileList();
		if (NULL == m_listTag) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			m_listTag->RegisterOnEvent(this, applEventCtagsListValidate);
			m_listTag->RegisterOnEvent(this, applEventCtagsListSelect);
			m_listTag->RegisterOnEvent(this, applEventCtagsListUnSelect);
			m_listTag->SetExpendX(true);
			m_listTag->SetExpendY(true);
			m_listTag->SetFillX(true);
			m_listTag->SetFillY(true);
			mySizerVert->SubWidgetAdd(m_listTag);
		}
		
		myWidgetTitle = new ewol::Label("Ctags Jump Selection ...");
		if (NULL == myWidgetTitle) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(myWidgetTitle);
		}
	}
}


appl::TagFileSelection::~TagFileSelection(void)
{
	
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void appl::TagFileSelection::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	EWOL_INFO("ctags LIST ... : \"" << eventId << "\" ==> data=\"" << data << "\"" );
	if (eventId == applEventctagsSelection) {
		if (m_eventNamed!="") {
			GenerateEventId(applEventctagsSelection, m_eventNamed);
			//==> Auto remove ...
			AutoDestroy();
		}
	} else if (eventId == applEventCtagsListSelect) {
		m_eventNamed = data;
		
	} else if (eventId == applEventCtagsListUnSelect) {
		m_eventNamed = "";
	} else if (eventId == applEventCtagsListValidate) {
		GenerateEventId(applEventctagsSelection, data);
		//==> Auto remove ...
		AutoDestroy();
	} else if (eventId == applEventctagsCancel) {
		GenerateEventId(applEventctagsCancel, "");
		//==> Auto remove ...
		AutoDestroy();
	}
	return;
};


/**
 * @brief Add a Ctags item on the curent list
 * @param[in] file Compleate file name
 * @param[in] jump line id
 * @return ---
 */
void appl::TagFileSelection::AddCtagsNewItem(etk::UString file, int32_t line)
{
	if (NULL != m_listTag) {
		m_listTag->Add(file, line);
	}
}


/**
 * @brief Inform object that an other object is removed ...
 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
 * @note : Sub classes must call this class
 * @return ---
 */
void appl::TagFileSelection::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::PopUp::OnObjectRemove(m_listTag);
	// second step find if in all the elements ...
	if(removeObject == m_listTag) {
		m_listTag = NULL;
	}
}





