#ifndef __VARIABLE___H
# define __VARIABLE___H
# include <vector>
# include <string>

# include "Type.h"
# include "IObject.h"

namespace kpl
{
	class	Variable : public IObject
	{
        public:
            Variable(int size = 1024);
            Variable(const Type *, int size = 1024);
            Variable(const Type &, int size = 1024);
            Variable(const Variable&);
            ~Variable();
            Variable&   operator = (const Variable&);


            const Type& GetKPLType() const;
            void        SetType(const Type&);
            void        SetType(const Type*);
            char*       Value();

            // Get a string representation of the object
            virtual const std::string&  ToString() const;

            // Get the type of the object
            virtual const std::string&  GetType() const;

            // returns true if the given type and content are equal.
            virtual bool                Equals(const IObject &value) const;

            // create a new instance by making a deep copy of the current object data
            virtual IObject*            Clone() const;
        protected:

        private:
            std::string             _name;
            char                    *_buf;
            int                     _bufSize;
            Type                    *_type;
            std::string             _objectToStr;
            std::string             _objectType;
	};
};

#endif //__VARIABLE___H
