/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__

#include <appl/debug.h>
#include <appl/globalMsg.h>

#include <appl/Gui/BufferView.h>
#include <appl/BufferManager.h>
#include <ewol/widget/Label.h>

class MainWindows : public ewol::Windows {
	private:
		int32_t m_currentSavingAsIdBuffer;
		widget::Label* m_widgetLabelFileName;
	public:
		// Constructeur
		MainWindows(void);
		~MainWindows(void);
	public: // Derived function
		virtual const char * const getObjectType(void) { return "MainWindows"; };
		virtual void onReceiveMessage(const ewol::EMessage& _msg);
		virtual void onObjectRemove(ewol::EObject * _removeObject);
};


#endif


