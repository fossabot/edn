/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __WORKER_CLOSE_FILE_H__
#define __WORKER_CLOSE_FILE_H__

#include <ewol/object/Worker.h>
#include <ewol/widget/meta/FileChooser.h>
#include <appl/BufferManager.h>
#include <appl/Gui/WorkerSaveFile.h>

namespace appl {
	class WorkerCloseFile : public ewol::object::Worker {
		public:
			esignal::ISignal<> signalCloseDone;
			esignal::ISignal<> signalAbort;
		protected:
			// note : if == "" ==> current ...
			WorkerCloseFile();
			void init();
		public:
			DECLARE_FACTORY(WorkerCloseFile);
			virtual ~WorkerCloseFile();
		public:
			/**
			 * @brief Action to do 
			 */
			void startAction(const std::string& _bufferName);
		private:
			std::string m_bufferName;
			std::shared_ptr<appl::Buffer> m_buffer; //!< reference on the buffer (when rename, we have no more reference on the buffer
			std::shared_ptr<appl::WorkerSaveFile> m_worker; //! sub-worker element...
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // callback Functions
			void onCallbackSaveAsValidate();
			void onCallbackSaveValidate();
			void onCallbackClose();
			void onCallbackCancel();
	};
};

#endif