/* 
 * File:   Register.h
 * Author: deveza
 *
 * Created on August 4, 2010, 12:14 AM
 */

#ifndef _REGISTER_H
# define _REGISTER_H
# include "Type.h"
# include <vector>

namespace kpl
{
    class Register
    {
    public:
        Register(MODE mode = MEMORY_MODE);
        Register(const Register&);
        ~Register();
        Register&           operator = (const Register&);
        void                Add(const Type&);
        void                Add(const Type*);
        friend Register&    operator << (Register&, const Type&);
        friend Register&    operator << (Register&, const Type*);
        void                Clear();
        void                Dump();
        void                Compile();
        const std::string&  GetRegisterName() const;
        void                SetRegisterName(const std::string&);

    private:
        static bool         _llvmIsInit;
        std::vector<Type *> _listType;
        std::string         _moduleName;
      	llvm::Module*       _module;
        MODE                _mode;
    };
};

#endif	/* _REGISTER_H */

