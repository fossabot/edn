/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once

#include <list>
#include <appl/Buffer.h>
#include <appl/globalMsg.h>
#include <ewol/widget/Widget.h>
#include <gale/resource/Resource.h>

namespace appl {
	class BufferManager;
	using BufferManagerShared = ememory::SharedPtr<appl::BufferManager>;
	using BufferManagerWeak = ememory::WeakPtr<appl::BufferManager>;
	// TODO: This is a service ...
	class BufferManager : public ewol::Object {
		public:
			esignal::ISignal<std::string> signalNewBuffer;
			esignal::ISignal<std::string> signalSelectFile;
			esignal::ISignal<> signalTextSelectionChange;
			esignal::ISignal<ememory::SharedPtr<appl::Buffer>> signalRemoveBuffer;
		protected:
			BufferManager();
		public:
			DECLARE_SINGLE_FACTORY(BufferManager, "???Buffer_Manager???");
			virtual ~BufferManager();
		private:
			std::list<ememory::SharedPtr<appl::Buffer>> m_list; // list of all buffer curently open
		public:
			/**
			 * @brief Get a specific buffer with his name (can create a new buffer).
			 * @param[in] _fileName Name of the file to open.
			 * @param[in] _createIfNeeded Create the buffer if not existed.
			 * @return a pointer on the buffer
			 */
			ememory::SharedPtr<appl::Buffer> get(const std::string& _fileName, bool _createIfNeeded=false);
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
			ememory::SharedPtr<appl::Buffer> get(int32_t _id);
			/**
			 * @brief Create a new buffer empty.
			 * @return Created buffer or nullptr.
			 */
			ememory::SharedPtr<appl::Buffer> createNewBuffer();
		private:
			ememory::SharedPtr<appl::Buffer> m_bufferSelected;
		public:
			/**
			 * @brief Set the current buffer selected
			 * @param[in] _bufferSelected Pointer on the buffer selected
			 */
			void setBufferSelected(ememory::SharedPtr<appl::Buffer> _bufferSelected);
			/**
			 * @brief Get the current buffer selected
			 * @return Pointer on the buffer selected
			 */
			ememory::SharedPtr<appl::Buffer> getBufferSelected() {
				return m_bufferSelected;
			};
		private:
			void requestDestroyFromChild(const ememory::SharedPtr<Object>& _child);
		public:
			// generic iterators:
			std::list<ememory::SharedPtr<appl::Buffer>>::const_iterator begin() const {
				return m_list.begin();
			}
			std::list<ememory::SharedPtr<appl::Buffer>>::const_iterator end() const {
				return m_list.end();
			}
			std::list<ememory::SharedPtr<appl::Buffer>>::iterator begin() {
				return m_list.begin();
			}
			std::list<ememory::SharedPtr<appl::Buffer>>::iterator end() {
				return m_list.end();
			}
	};
}


