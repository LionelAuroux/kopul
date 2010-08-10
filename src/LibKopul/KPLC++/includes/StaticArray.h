#ifndef __STATICARRAY___H
# define __STATICARRAY___H
# include "StaticType.h"

namespace kpl
{
	class	StaticArray : public StaticType
	{
	public:
            StaticArray(const StaticType& type, int size);
            StaticArray(const StaticArray&);
            ~StaticArray();
            StaticArray&        operator = (const StaticArray&);

            int             GetSize() const;
            // Get a string representation of the object
            const std::string&  ToString() const;

            // Get the type of the object
            const std::string&  GetType() const;

            // returns true if the given type and content are equal.
            bool                Equals(const IObject &value) const;

            // create a new instance by making a deep copy of the current object data
            IObject*            Clone() const;

        protected:
            const llvm::Type*   GetLLVMType() const;

        private:
            StaticType*         _staticType;
            int                 _size;
            std::string         _objectToStr;
            std::string         _objectType;
	};
};

#endif //__STATICARRAY___H
