/**
 *******************************************************************************
 * @file EdnTree.h
 * @brief Editeur De N'ours : Basic tree in a vector For none expensive tree (template)
 * @author Edouard DUPIN
 * @date 04/04/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */
#ifndef __EDN_TREE_H__
#define __EDN_TREE_H__

#undef __class__
#define __class__	"EdnTreeElement"

template < class T > class EdnTreeElement
{
	public:
		EdnTreeElement(int32_t parent, int32_t id, T &maNouvelleClass) : elementPtr(NULL), m_parent(parent), m_id(id)
		{
			elementPtr = new T(maNouvelleClass);
		};
		~EdnTreeElement(void)
		{
			delete(elementPtr);
		};
		EdnTreeElement(const EdnTreeElement &mustCopy ) : elementPtr(NULL), m_parent(mustCopy.m_parent), m_id(mustCopy.m_id)
		{
			elementPtr = new T(*mustCopy.elementPtr);
		}
		int32_t	GetParrent(void) { return m_parent;};
		int32_t	GetId(void) { return m_id;};
		T*		GetPtr() const 
		{
			return elementPtr;
		};

	private:
		T* elementPtr;		// pointer on the curent element
		int32_t m_parent;
		int32_t m_id;
};

#undef __class__
#define __class__	"EdnTree"

#define ROOT_NODE_ID	(-1)

template < class T > class EdnTree 
{
	public: 
		// constructeur et destructeur
		EdnTree(void)
		{
			m_LastId = 0;
		};
		
		~EdnTree(void)
		{
			
		};
		// Common function ...
		int32_t GetNumberNode(void)
		{
			return m_listElement.size();
		};
		
		int32_t GetDepth(void)
		{
			// TODO : ...
			return 0;
		};
		//!< add an element in the tree
		int32_t Add(T &maNouvelleClass, int32_t parent=-1)
		{
			if (true == CheckPresenceParrent(parent)) {
				// create the local element
				EdnTreeElement<T> nouvelElement(parent, m_LastId, maNouvelleClass);
				// add it in the list
				m_listElement.push_back(nouvelElement);
				// increment the ID of the element
				m_LastId++;
				// Add is Ok, in theory ...
				return m_LastId-1;
			}
			return -2;
		};
		
		bool Remove(int32_t id)
		{
			return false;
		};
		
		bool Clear(void)
		{
			return false;
		};
		
		T* Get(int32_t id)
		{
			// try to find ID
			int32_t realID = FindElementWithId(id);
			// when we find it, check it
			if (0 > realID || realID >= m_LastId) {
				return NULL;
			}
			// Return the element :
			return m_listElement[realID].GetPtr();
		};
		
		std::vector<int32_t> GetListSubNode( int32_t parentId = ROOT_NODE_ID)
		{
			std::vector<int32_t> res;
			int32_t i;
			for (i=0; i<(int32_t)m_listElement.size(); i++) {
				if (m_listElement[i].GetParrent() == parentId) {
					// Add the element ID in the list ...
					res.push_back(m_listElement[i].GetId());
				}
			}
			return res;
		};
		
		std::vector<int32_t> Root(void)
		{
			return GetListSubNode(ROOT_NODE_ID);
		};
		
		
	private:
		int32_t								m_LastId;
		std::vector< EdnTreeElement<T> >	m_listElement;		//!< list of element...
		
		bool CheckPresence(int32_t id)
		{
			int32_t i;
			for (i=0; i<(int32_t)m_listElement.size(); i++) {
				if (m_listElement[i].GetId() == id) {
					return true;
				}
			}
			return false;
		};
		
		bool CheckPresenceParrent(int32_t parrentId)
		{
			if (ROOT_NODE_ID == parrentId) {
				return true;
			}
			return CheckPresence(parrentId);
		};
		
		int32_t	FindElementWithId(int32_t id)
		{
			int32_t i;
			for (i=0; i<(int32_t)m_listElement.size(); i++) {
				if (m_listElement[i].GetId() == id) {
					return i;
				}
			}
			return ROOT_NODE_ID;
		};
};



#undef __class__
#define __class__	NULL

#endif

