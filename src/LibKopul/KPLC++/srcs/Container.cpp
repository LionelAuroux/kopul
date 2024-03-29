/* 
 * File:   Container.cpp
 * Author: deveza
 * 
 * Created on August 24, 2010, 3:28 PM
 */

#include <exception>

#include "Type.h"
#include "Container.h"
#include "StaticType.h"
#include "Switch.h"

using namespace kpl;

template <typename T>
Container<T>::Container()
{

}

template <typename T>
Container<T>::Container(const Container<T>& orig)
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
Container<T>&       Container<T>::operator = (const Container<T>& old)
{
    for (unsigned int i = 0; i < old._list.size(); ++i)
        this->Add(old._list[i]);
    std::cout << this->_list.size() << "  " << old._list.size() << std::endl;
    return (*this);
}

template <typename T>
void                Container<T>::Add(const T& toAdd)
{
    this->_list.push_back(reinterpret_cast<T *>(reinterpret_cast<const IObject&>(toAdd).Clone()));
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
    return (*this);
}

template <typename T>
Container<T>&   Container<T>::operator << (const T* toAdd)
{
    this->Add(*toAdd);
    return (*this);
}

template <typename T>
unsigned int        Container<T>::Size()
{
    return (this->_list.size());
}

template <typename T>
T*                  Container<T>::operator[](unsigned int i)
{
    if (i > this->_list.size())
        throw std::exception();
    return (this->_list[i]);
}

template class Container<Type>;
template class Container<StaticType>;
template class Container<SwitchCondition>;