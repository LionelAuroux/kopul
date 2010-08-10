/* 
 * File:   IObject.h
 * Author: deveza
 *
 * Created on August 4, 2010, 2:15 AM
 */

#ifndef _IOBJECT_H
#define	_IOBJECT_H
# include <string>

namespace   kpl
{
    class   IObject
    {
    public:
        virtual ~IObject(){}

        // Get a string representation of the object
        virtual const std::string&  ToString() const = 0;

        // Get the type of the object
        virtual const std::string&  GetType() const = 0;

        // returns true if the given type and content are equal.
        virtual bool                Equals(const IObject &value) const = 0;

        // create a new instance by making a deep copy of the current object data
        virtual IObject*            Clone() const = 0;
    };
};

#endif	/* _IOBJECT_H */

