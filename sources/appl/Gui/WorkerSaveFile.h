/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __WORKER_SAVE_FILE_H__
#define __WORKER_SAVE_FILE_H__

#include <ewol/widget/meta/FileChooser.h>
#include <ewol/object/Worker.h>
#include <appl/BufferManager.h>

namespace appl {
	class WorkerSaveFile : public ewol::object::Worker {
		public:
			esignal::ISignal<> signalSaveDone;
			esignal::ISignal<> signalAbort;
			eproperty::Value<std::string> propertyBufferName;
			eproperty::Value<bool> propertyForceSave;
		protected:
			WorkerSaveFile();
			void init();
		public:
			DECLARE_FACTORY(WorkerSaveFile);
			virtual ~WorkerSaveFile();
		private:
			std::shared_ptr<ewol::widget::FileChooser> m_chooser; //! pop-up element that is open...
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // callback function
			void onCallbackSaveAsValidate(const std::string& _value);
			void onCallbackCancel();
	};
};

#endif
