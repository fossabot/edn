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
			static const char* eventCloseDone;
		public:
			// note : if == "" ==> current ...
			WorkerCloseFile(const std::string& _bufferName);
			virtual ~WorkerCloseFile(void);
		private:
			std::string m_bufferName;
			appl::Buffer* m_buffer; //!< reference on the buffer (when rename, we have no more reference on the buffer
			appl::WorkerSaveFile* m_worker; //! sub-worker element...
			appl::BufferManager* m_bufferManager; //!< handle on the buffer manager
		public: // derived function
			virtual void onReceiveMessage(const ewol::object::Message& _msg);
			virtual void onObjectRemove(ewol::Object * _removeObject);
	};
};

#endif