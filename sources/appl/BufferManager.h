/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__

#include <list>
#include <appl/Buffer.h>
#include <appl/globalMsg.h>
#include <ewol/widget/Widget.h>
#include <ewol/resource/Resource.h>

namespace appl {
	class BufferManager : public ewol::Resource {
		protected:
			BufferManager();
		public:
			virtual ~BufferManager();
		private:
			std::list<ewol::object::Owner<appl::Buffer>> m_list; // list of all buffer curently open
		public:
			/**
			 * @brief Get a specific buffer with his name (can create a new buffer).
			 * @param[in] _fileName Name of the file to open.
			 * @param[in] _createIfNeeded Create the buffer if not existed.
			 * @return a pointer on the buffer
			 */
			ewol::object::Shared<appl::Buffer> get(const std::string& _fileName, bool _createIfNeeded=false);
			/**
			 * @brief Load a specific file, event if it not existed:
			 * @param[in] _fileName Name of the file to open or create.
			 */
			void open(const std::string& _fileName);
			/**
			 * @brief Check if a buffer is already open.
			 * @param[in] _fileName name of the file.
			 * @return true if the buffer is already open.
			 */
			bool exist(const std::string& _fileName);
			/**
			 * @brief Get count of all buffer availlable.
			 * @return Number of buffer
			 */
			int32_t size() const {
				return m_list.size();
			}
			/**
			 * @brief Get a pointer on a buffer Id (never remember this ID!!!).
			 * @param[in] _id Number of buffer
			 * @return pointer on the buffer
			 */
			ewol::object::Shared<appl::Buffer> get(int32_t _id);
			/**
			 * @brief Create a new buffer empty.
			 * @return Created buffer or nullptr.
			 */
			ewol::object::Shared<appl::Buffer> createNewBuffer();
		private:
			ewol::object::Shared<appl::Buffer> m_bufferSelected;
		public:
			/**
			 * @brief Set the current buffer selected
			 * @param[in] _bufferSelected Pointer on the buffer selected
			 */
			void setBufferSelected(ewol::object::Shared<appl::Buffer> _bufferSelected);
			/**
			 * @brief Get the current buffer selected
			 * @return Pointer on the buffer selected
			 */
			ewol::object::Shared<appl::Buffer> getBufferSelected() {
				return m_bufferSelected;
			};
		public: // herited function
			void onReceiveMessage(const ewol::object::Message& _msg);
			void onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject);
		public: // resource manager
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or nullptr if an error occured.
			 */
			static ewol::object::Shared<appl::BufferManager> keep();
	};
};

#endif

