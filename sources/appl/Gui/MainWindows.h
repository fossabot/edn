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
#include <appl/BufferManager.h>

class MainWindows : public ewol::widget::Windows {
	private:
		std::shared_ptr<ewol::widget::Label> m_widgetLabelFileName;
	protected:
		// Constructeur
		MainWindows();
		void init();
	public:
		DECLARE_FACTORY(MainWindows);
		virtual ~MainWindows();
	private:
		std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		/**
		 * @brief Display a pop-up to the select the name of the file.
		 * @param[in] _buffer Buffer that might be saved with a new name.
		 */
		void saveAsPopUp(const std::shared_ptr<appl::Buffer>& _buffer);
		/**
		 * @brief Display a pop-up to the user to confirm wat he want to do when he close a file not saved.
		 * @param[in] _buffer Buffer that might be close.
		 */
		void closeNotSavedFile(const std::shared_ptr<appl::Buffer>& _buffer);
	public: // Derived function
		virtual void onReceiveMessage(const ewol::object::Message& _msg);
};


#endif


