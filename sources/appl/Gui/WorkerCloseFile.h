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
	class WorkerCloseFile : public ewol::EObject {
		public:
			// note : if == "" ==> current ...
			WorkerCloseFile(const std::string& _bufferName);
			virtual ~WorkerCloseFile(void);
		private:
			std::string m_bufferName;
			appl::WorkerSaveFile* m_worker; //! sub-worker element...
			appl::BufferManager* m_bufferManager; //!< handle on the buffer manager
		public: // derived function
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			virtual void onObjectRemove(ewol::EObject * _removeObject);
	};
};

#endif