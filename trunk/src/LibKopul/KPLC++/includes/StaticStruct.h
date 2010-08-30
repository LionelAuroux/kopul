#ifndef __STRUCT___H
# define __STRUCT___H
# include <string>
# include <vector>

# include "StaticType.h"
# include "Container.h"

namespace kpl
{
	class	StaticStruct : public StaticType, public Container<StaticType>
	{
	public:
            StaticStruct();
            StaticStruct(const StaticStruct&);
            ~StaticStruct();
            StaticStruct&                     operator = (const StaticStruct&);
            const std::vector<StaticType *>&  GetListType() const;
            int                               GetSize() const;
            int                               GetSizeInOctet() const;

            // Get a string representation of the object
            virtual const std::string&                ToString() const;

            // Get the type of the object
            virtual const std::string&                GetType() const;

            // returns true if the given type and content are equal.
            virtual bool                              Equals(const IObject &value) const;

            // create a new instance by making a deep copy of the current object data
            virtual IObject*                          Clone() const;

        protected:
            const llvm::Type*                 GetLLVMType() const;

        private:
            std::string                       _objectToStr;
            std::string                       _objectType;
	};
};

#endif //__STRUCT___H
