#ifndef __VARIABLE___H
# define __VARIABLE___H
# include <vector>
# include <string>

# include "Value.h"

namespace kpl
{
	class	Variable : public Value
	{
        public:
            Variable(const std::string &name, const StaticType *);
            Variable(const std::string &name, const StaticType &);
            Variable(const std::string &name, const Type *, int sizeInBytes = 8192);
            Variable(const std::string &name, const Type &, int sizeInBytes = 8192);
            Variable(const Variable&);
            ~Variable();

            char*               operator * ();
            char*               GetValue();

            template <typename T>
            T                   Get() const
            {
                return (*((T *)this->_value));
            }

            template <typename T>
            void                Set(T value)
            {
                *((T *)this->_value) = value;
            }

            void                Set(const void *value, int sizeInBytes);

            void                SetVariableName(const std::string &variableName);
            const std::string   &GetVariableName() const;

            llvm::Value         *GetLLVMValue(llvm::BasicBlock *) const;

            // Get a string representation of the object
            const std::string&  ToString() const;

            // Get the type of the object
            const std::string&  GetType() const;

            // returns true if the given type and content are equal.
            bool                Equals(const IObject &value) const;

            // create a new instance by making a deep copy of the current object data
            IObject*            Clone() const;
        protected:

        private:
            std::string             _variableName;
            std::string             _objectToStr;
            std::string             _objectType;
	};
};

#endif //__VARIABLE___H
