/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/object/Worker.h>
#include <appl/BufferManager.h>

namespace appl {
	class WorkerSaveFile : public ewol::object::Worker {
		public:
			esignal::Signal<> signalSaveDone;
			esignal::Signal<> signalAbort;
			eproperty::Value<std::string> propertyBufferName;
			eproperty::Value<bool> propertyForceSave;
		protected:
			WorkerSaveFile();
			void init();
		public:
			DECLARE_FACTORY(WorkerSaveFile);
			virtual ~WorkerSaveFile();
		private:
			ememory::SharedPtr<ewol::widget::FileChooser> m_chooser; //! pop-up element that is open...
			ememory::SharedPtr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // callback function
			void onCallbackSaveAsValidate(const std::string& _value);
			void onCallbackCancel();
	};
}

