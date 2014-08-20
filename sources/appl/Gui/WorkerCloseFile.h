/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __WORKER_CLOSE_FILE_H__
#define __WORKER_CLOSE_FILE_H__

#include <ewol/widget/meta/FileChooser.h>
#include <appl/BufferManager.h>
#include <appl/Gui/WorkerSaveFile.h>

namespace appl {
	class WorkerCloseFile : public ewol::Object {
		public:
			ewol::object::Signal<void> signalCloseDone;
		protected:
			// note : if == "" ==> current ...
			WorkerCloseFile();
			void init(const std::string& _bufferName);
		public:
			DECLARE_FACTORY(WorkerCloseFile);
			virtual ~WorkerCloseFile();
		private:
			std::string m_bufferName;
			std::shared_ptr<appl::Buffer> m_buffer; //!< reference on the buffer (when rename, we have no more reference on the buffer
			std::shared_ptr<appl::WorkerSaveFile> m_worker; //! sub-worker element...
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // derived function
			virtual void onReceiveMessage(const ewol::object::Message& _msg);
	};
};

#endif