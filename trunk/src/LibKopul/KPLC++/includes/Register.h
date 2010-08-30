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
# include "Container.h"

namespace kpl
{
    typedef char** stream;
    typedef int fd;
    typedef int socket;

    class Register : public Container<Type>
    {
    public:
        Register();
        Register(const Register&);
        ~Register();
        Register&                           operator = (const Register&);
        void                                Dump(MODE mode = MEMORY_MODE) const;
        FunctionList<int (*)(stream, ...)> *CompileInMemoryMode() const;
        FunctionList<int (*)(fd, ...)>     *CompileInFileMode() const;
        FunctionList<int (*)(socket, ...)> *CompileInSocketMode() const;
        const std::string&                  GetRegisterName() const;
        void                                SetRegisterName(const std::string&);

    private:
        static bool                     _llvmIsInit;
        std::string                     _moduleName;
    };
};

#endif	/* _REGISTER_H */

