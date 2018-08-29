/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once

#include <appl/debug.hpp>
#include <appl/BufferManager.hpp>
#include <appl/globalMsg.hpp>
#include <ewol/widget/TreeView.hpp>
#include <ewol/widget/Windows.hpp>
#include <etk/FlatTree.hpp>

namespace appl {
	class TreeElement {
		public:
			TreeElement(const etk::String& _nodeName, bool _isFolder = false, bool _isExpand=false):
			  m_nodeName(_nodeName),
			  m_isFolder(_isFolder),
			  m_buffer(null),
			  m_isSelected(false),
			  m_isExpand(_isExpand) {
				
			}
			etk::String m_nodeName; // must be here ==> the buffer is optionnal..
			bool m_isFolder;
			ememory::SharedPtr<appl::Buffer> m_buffer;
			bool m_isSelected = false;
			bool m_isExpand = false;
	};
	namespace widget {
		
		class BufferTree;
		using BufferTreeShared = ememory::SharedPtr<BufferTree>;
		using BufferTreeWeak = ememory::WeakPtr<BufferTree>;
		
		class BufferTree : public ewol::widget::TreeView {
			private:
				ememory::SharedPtr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
			private:
				ememory::SharedPtr<appl::GlyphPainting> m_paintingProperties; //!< element painting property
				int32_t m_colorBackground1;
				int32_t m_colorBackground2;
				int32_t m_colorBackgroundHide;
				int32_t m_colorBackgroundSelect;
				int32_t m_colorTextNormal;
				int32_t m_colorTextModify;
				int32_t m_colorTextNotOpen;
			private:
				int32_t m_selectedIdRequested;
				int32_t m_selectedID;
				void updateFlatTree();
				void generateFlatTree();
				ememory::SharedPtr<etk::TreeNode<TreeElement>> m_tree;
				etk::FlatTree<TreeElement> m_flatTree;
				ememory::SharedPtr<appl::Buffer> m_selection;
			protected:
				// Constructeur
				BufferTree();
				void init() override;
			public:
				DECLARE_FACTORY(BufferTree);
				virtual ~BufferTree();
			private:
				bool m_openOrderMode; //!< true if the order is the opening order mode, otherwise, Alphabetic order
			protected:
				// function call to display the list :
				etk::Color<> getBasicBG() override;
				void removeAllElement();
				// Derived function
				ivec2 getMatrixSize() const override;
				fluorine::Variant getData(int32_t _role, const ivec2& _pos) override;
				bool onItemEvent(const ewol::event::Input& _event, const ivec2& _pos, const vec2& _mousePosition) override;
				void onItemExpandEvent(const ivec2& _pos) override;
			private: //callback function:
				void onCallbackChangeName();
				void onCallbackIsSave();
				void onCallbackIsModify();
				void onNewBuffer(const ememory::SharedPtr<appl::Buffer>& _buffer);
				void onSelectBuffer(const ememory::SharedPtr<appl::Buffer>& _buffer);
				void onRemoveBuffer(const ememory::SharedPtr<appl::Buffer>& _buffer);
				void calculateMinMaxSize() override;
		};
	}
}
