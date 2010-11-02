/* 
 * File:   IComposedType.h
 * Author: deveza
 *
 * Created on October 29, 2010, 2:40 AM
 */

#ifndef _ICOMPOSEDTYPE_H
# define _ICOMPOSEDTYPE_H
# include "Type.h"

namespace   kpl
{
    class   IComposedType
    {
    public:
        virtual ~IComposedType(){}
        virtual int GetNbType() const = 0;
        virtual const Type  &GetTypeN(int i) const = 0;
    };
}

#endif	/* _ICOMPOSEDTYPE_H */

