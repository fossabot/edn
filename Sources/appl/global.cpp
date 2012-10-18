/**
 *******************************************************************************
 * @file tools_Globals.cpp
 * @brief Editeur De N'ours : Globals Values
 * @author Edouard DUPIN
 * @date 05/12/2010
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

#include <appl/global.h>
#include <ColorizeManager.h>
#include <appl/globalMsg.h>
#include <ewol/eObject/EObject.h>

#undef __class__
#define __class__	"globals"




erreurCode_te globals::init(void)
{
	erreurCode_te ret = ERR_NONE;
	

	return ret;
}



// -----------------------------------------------------------
static bool displayEOL = false;
bool globals::IsSetDisplayEndOfLine(void)
{
	return displayEOL;
}

void globals::SetDisplayEndOfLine(bool newVal)
{
	APPL_INFO("Set EndOfLine " << newVal);
	displayEOL = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
static bool displaySpaceChar = true;
bool globals::IsSetDisplaySpaceChar(void)
{
	return displaySpaceChar;
}

void globals::SetDisplaySpaceChar(bool newVal)
{
	APPL_INFO("Set SpaceChar " << newVal);
	displaySpaceChar = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}
// -----------------------------------------------------------
static bool displayTabChar = true;
bool globals::IsSetDisplayTabChar(void)
{
	return displayTabChar;
}

void globals::SetDisplayTabChar(bool newVal)
{
	APPL_INFO("Set SpaceChar " << newVal);
	displayTabChar = newVal;
	//ewol::widgetMessageMultiCast::Send(-1, ednMsgUserDisplayChange);
}

// -----------------------------------------------------------
static bool AutoIndent = true;
bool globals::IsSetAutoIndent(void)
{
	return AutoIndent;
}

void globals::SetAutoIndent(bool newVal)
{
	APPL_INFO("Set AutoIndent " << newVal);
	AutoIndent = newVal;
}

// -----------------------------------------------------------


bool globals::OrderTheBufferList(void)
{
	return true;
}

// -----------------------------------------------------------


int32_t globals::getNbColoneBorder(void)
{
	return 6;
}

int32_t globals::getNbLineBorder(void)
{
	return 3;
}


#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Spacer.h>
static const char * const l_changeIndentation = "edn-event-change-indentation";
static const char * const l_changeSpace       = "edn-event-change-spaces";
static const char * const l_changeTabulation  = "edn-event-change-tabulation";
static const char * const l_changeEndOfLine   = "edn-event-change-endOfLine";

globals::ParameterGlobalsGui::ParameterGlobalsGui(void) 
{
	ewol::CheckBox* myCheckbox = NULL;
	ewol::Spacer* mySpacer = NULL;
	
	mySpacer = new ewol::Spacer();
	if (NULL == mySpacer) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		mySpacer->SetExpendX(true);
		mySpacer->SetExpendY(true);
		SubWidgetAdd(mySpacer);
	}
	myCheckbox = new ewol::CheckBox("Automatic Indentation");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpendX(true);
		myCheckbox->SetValue(IsSetAutoIndent());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeIndentation);
		SubWidgetAdd(myCheckbox);
	}
	myCheckbox = new ewol::CheckBox("Display space char (' ')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpendX(true);
		myCheckbox->SetValue(IsSetDisplaySpaceChar());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeSpace);
		SubWidgetAdd(myCheckbox);
	}
	myCheckbox = new ewol::CheckBox("Display tabulation char ('\\t')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpendX(true);
		myCheckbox->SetValue(IsSetDisplayTabChar());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeTabulation);
		SubWidgetAdd(myCheckbox);
	}
	myCheckbox = new ewol::CheckBox("Display end of line ('\\n')");
	if (NULL == myCheckbox) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		myCheckbox->SetExpendX(true);
		myCheckbox->SetValue(IsSetDisplayEndOfLine());
		myCheckbox->RegisterOnEvent(this, ewolEventCheckBoxClicked, l_changeEndOfLine);
		SubWidgetAdd(myCheckbox);
	}
}

globals::ParameterGlobalsGui::~ParameterGlobalsGui(void) 
{
	
}


/**
 * @brief Receive a message from an other EObject with a specific eventId and data
 * @param[in] CallerObject Pointer on the EObject that information came from
 * @param[in] eventId Message registered by this class
 * @param[in] data Data registered by this class
 * @return ---
 */
void globals::ParameterGlobalsGui::OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data)
{
	ewol::SizerVert::OnReceiveMessage(CallerObject, eventId, data);
	
	if (eventId == l_changeEndOfLine) {
		if (data == "true") {
			SetDisplayEndOfLine(true);
		} else {
			SetDisplayEndOfLine(false);
		}
	} else if (eventId == l_changeIndentation) {
		if (data == "true") {
			SetAutoIndent(true);
		} else {
			SetAutoIndent(false);
		}
	} else if (eventId == l_changeSpace) {
		if (data == "true") {
			SetDisplaySpaceChar(true);
		} else {
			SetDisplaySpaceChar(false);
		}
	} else if (eventId == l_changeTabulation) {
		if (data == "true") {
			SetDisplayTabChar(true);
		} else {
			SetDisplayTabChar(false);
		}
	}
	
}

