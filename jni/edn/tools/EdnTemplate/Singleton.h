/**
 *******************************************************************************
 * @file Singleton.h
 * @brief Editeur De N'ours : singleton system class
 * @author Edouard DUPIN
 * @date 04/12/2010
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

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>
using namespace std;


/////////////////////////////////////////////////////////////////////////////
//
// Singleton - modèle Singleton applicable à n'importe quelle classe.
//
/////////////////////////////////////////////////////////////////////////////

template <typename T>
class Singleton
{
	protected:
		// Constructeur/destructeur
		Singleton() { }
		~Singleton() { /*std::cout << "destroying singleton." << std::endl;*/ }

	public:
		// Interface publique
		static T *getInstance()
		{
			if (NULL == _singleton)
			{
				/*std::cout << "C: Singleton	 | creating singleton." << std::endl;*/
				_singleton = new T;
			}
			/*
			else
			{
				std::cout << "C: Singleton	 | singleton already created!" << std::endl;
			}
			*/

			return (static_cast<T*> (_singleton));
		}

		static void kill()
		{
			if (NULL != _singleton)
			{
				delete _singleton;
				_singleton = NULL;
			}
		}

	private:
	// Unique instance
		static T *_singleton;
};

template <typename T>
		T *Singleton<T>::_singleton = NULL;

#endif


