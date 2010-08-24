/* 
 * File:   Container.cpp
 * Author: deveza
 * 
 * Created on August 24, 2010, 3:28 PM
 */

#include "Container.h"

using namespace kpl;

template <typename T>
Container<T>::Container()
{

}

template <typename T>
Container<T>::Container(const Container& orig)
{
    for (unsigned int i = 0; i < orig._list.size(); ++i)
        this->Add(orig._list[i]);
}

template <typename T>
Container<T>::~Container()
{
    this->Clear();
}

template <typename T>
Container<T>&          Container<T>::operator = (const Container& old)
{
    for (unsigned int i = 0; i < old._list.size(); ++i)
        this->Add(old._list[i]);
    return (*this);
}

template <typename T>
void                Container<T>::Add(const T& toAdd)
{
    this->_list.push_back(static_cast<T *>(static_cast<const IObject&>(toAdd).Clone()));
}

template <typename T>
void                Container<T>::Add(const T* toAdd)
{
    this->Add(*toAdd);
}

template <typename T>
void                Container<T>::Clear()
{
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        delete (this->_list[i]);
    this->_list.clear();
}

template <typename T>
Container<T>&   Container<T>::operator << (const T& toAdd)
{
    this->Add(toAdd);
}

template <typename T>
Container<T>&   Container<T>::operator << (const T* toAdd)
{
    this->Add(*toAdd);
}