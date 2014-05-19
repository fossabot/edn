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
#include <appl/BufferManager.h>

namespace appl {
	class WorkerSaveFile : public ewol::Object {
		public:
			static const char* eventSaveDone;
		public:
			WorkerSaveFile(const std::string& _bufferName, bool _forceSaveAs=true);
			virtual ~WorkerSaveFile();
		private:
			std::string m_bufferName;
			ewol::object::Shared<ewol::widget::FileChooser> m_chooser; //! pop-up element that is open...
			appl::BufferManager* m_bufferManager; //!< handle on the buffer manager
		public: // derived function
			virtual void onReceiveMessage(const ewol::object::Message& _msg);
			virtual void onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject);
	};
};

#endif
