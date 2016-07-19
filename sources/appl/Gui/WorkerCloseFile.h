/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
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
			ememory::SharedPtr<appl::Buffer> m_buffer; //!< reference on the buffer (when rename, we have no more reference on the buffer
			ememory::SharedPtr<appl::WorkerSaveFile> m_worker; //! sub-worker element...
			ememory::SharedPtr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // callback Functions
			void onCallbackSaveAsValidate();
			void onCallbackSaveValidate();
			void onCallbackClose();
			void onCallbackCancel();
	};
}

