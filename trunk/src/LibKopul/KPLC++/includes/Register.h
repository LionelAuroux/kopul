/* 
 * File:   Register.h
 * Author: deveza
 *
 * Created on August 4, 2010, 12:14 AM
 */

#ifndef _REGISTER_H
# define _REGISTER_H
# include <vector>

# include "FunctionList.h"
# include "Type.h"

namespace kpl
{
    typedef char** stream;
    typedef int fd;
    typedef int socket;

    class Register
    {
    public:
        Register();
        Register(const Register&);
        ~Register();
        Register&                           operator = (const Register&);
        void                                Add(const Type&);
        void                                Add(const Type*);
        friend Register&                    operator << (Register&, const Type&);
        friend Register&                    operator << (Register&, const Type*);
        void                                Clear();
        void                                Dump(MODE mode = MEMORY_MODE) const;
        FunctionList<int (*)(stream, ...)> *CompileInMemoryMode() const;
        FunctionList<int (*)(fd, ...)>     *CompileInFileMode() const;
        FunctionList<int (*)(socket, ...)> *CompileInSocketMode() const;
        const std::string&                  GetRegisterName() const;
        void                                SetRegisterName(const std::string&);

    private:
        static bool                     _llvmIsInit;
        std::vector<Type *>             _listType;
        std::string                     _moduleName;
    };
};

#endif	/* _REGISTER_H */

