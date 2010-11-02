/* 
 * File:   Container.h
 * Author: deveza
 *
 * Created on August 24, 2010, 3:28 PM
 */

#ifndef _CONTAINER_H
# define _CONTAINER_H
# include <vector>

# include "IObject.h"

namespace kpl
{

    template <typename T>
    class Container
    {
    public:
        Container();
        Container(const Container& orig);
        ~Container();
        Container&          operator = (const Container&);
        void                Add(const T&);
        void                Add(const T*);
        Container&          operator << (const T&);
        Container&          operator << (const T*);
        void                Clear();
        unsigned int        Size();
        T*                  operator[](unsigned int i);
    protected:
        std::vector<T*>      _list;

    };

};
#endif	/* _CONTAINER_H */

