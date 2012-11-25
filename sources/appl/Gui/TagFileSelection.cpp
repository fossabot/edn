/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
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
	
	widget::Label*      myWidgetTitle = NULL;
	widget::Button*     myWidgetValidate = NULL;
	widget::Button*     myWidgetCancel = NULL;
	
	widget::SizerVert * mySizerVert = NULL;
	widget::SizerHori * mySizerHori = NULL;
	widget::Spacer *    mySpacer = NULL;
	#if defined(__TARGET_OS__Android)
		SetDisplayRatio(0.90);
	#elif defined(__TARGET_OS__Windows)
		SetDisplayRatio(0.80);
	#else
		SetDisplayRatio(0.80);
	#endif
	
	mySizerVert = new widget::SizerVert();
	if (NULL == mySizerVert) {
		EWOL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		mySizerVert->LockExpendContamination(true);
		// set it in the pop-up-system : 
		SubWidgetSet(mySizerVert);
		
		mySizerHori = new widget::SizerHori();
		if (NULL == mySizerHori) {
			EWOL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			mySizerVert->SubWidgetAdd(mySizerHori);
			mySpacer = new widget::Spacer();
			if (NULL == mySpacer) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				mySpacer->SetExpendX(true);
				mySizerHori->SubWidgetAdd(mySpacer);
			}
			myWidgetValidate = new widget::Button("Jump");
			if (NULL == myWidgetValidate) {
				EWOL_ERROR("Can not allocate widget ==> display might be in error");
			} else {
				myWidgetValidate->SetImage("icon/Load.svg");
				myWidgetValidate->RegisterOnEvent(this, ewolEventButtonPressed, applEventctagsSelection);
				mySizerHori->SubWidgetAdd(myWidgetValidate);
			}
			myWidgetCancel = new widget::Button("Cancel");
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
		
		myWidgetTitle = new widget::Label("Ctags Jump Selection ...");
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
	widget::PopUp::OnObjectRemove(m_listTag);
	// second step find if in all the elements ...
	if(removeObject == m_listTag) {
		m_listTag = NULL;
	}
}





