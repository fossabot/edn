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
			ewol::object::Signal<void> signalSaveDone;
		protected:
			WorkerSaveFile();
			void init(const std::string& _bufferName, bool _forceSaveAs=true);
		public:
			DECLARE_FACTORY(WorkerSaveFile);
			virtual ~WorkerSaveFile();
		private:
			std::string m_bufferName;
			std::shared_ptr<ewol::widget::FileChooser> m_chooser; //! pop-up element that is open...
			std::shared_ptr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
		public: // derived function
			virtual void onReceiveMessage(const ewol::object::Message& _msg);
	};
};

#endif
