/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__

#include <appl/Debug.h>
#include <appl/globalMsg.h>

#include <CodeView.h>
#include <BufferView.h>
#include <BufferManager.h>
#include <ewol/widget/Label.h>

class MainWindows : public ewol::Windows
{
	private:
		int32_t m_currentSavingAsIdBuffer;
		widget::Label* m_widgetLabelFileName;
	public:
		// Constructeur
		MainWindows(void);
		~MainWindows(void);
		// Derived function
		const char * const GetObjectType(void) { return "MainWindows"; };
		// Derived function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		// Derived function
		virtual void OnObjectRemove(ewol::EObject * removeObject);
};

#define EDN_CAST_MAIN_WINDOWS(curentPointer) EWOL_CAST(TYPE_EOBJECT_EDN_MAIN_WINDOWS,MainWindows,curentPointer)

#endif


