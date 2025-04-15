/*! \file
    \brief Исключения для marty::format. Стиль именов - snake_case, так как мы наследуемся от стандартной библиотеки
 */

#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
#include "utils.h"

//----------------------------------------------------------------------------
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <string>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// #include "format/exceptions.h"
// marty::format::
namespace marty{
namespace format{

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(cls, clsBase)                                                                    \
             class cls : public clsBase                                                                                       \
             {                                                                                                                \
             public:                                                                                                          \
                                                                                                                              \
                 cls(const std::string &what_arg, std::size_t offs=std::size_t(-1)) : clsBase(what_arg, offs) {}              \
                 cls(const char* what_arg       , std::size_t offs=std::size_t(-1)) : clsBase(std::string(what_arg), offs) {} \
                 cls(const cls &e)                                                  : clsBase(e) {}                           \
                 cls& operator=(const cls& e)     { clsBase::operator=(e); return *this; }                                    \
             }


//----------------------------------------------------------------------------
class base_error : public std::runtime_error
{
    typedef std::runtime_error base_exception_type;

    std::size_t m_offset = std::size_t(-1);


public:

    base_error(const std::string &what_arg, std::size_t offs=std::size_t(-1))     : base_exception_type(what_arg), m_offset(offs) {}
    base_error(const char* what_arg       , std::size_t offs=std::size_t(-1))     : base_exception_type(std::string(what_arg)), m_offset(offs) {}
    base_error(const base_error &e)                                               : base_exception_type(e), m_offset(e.m_offset) {}
    base_error& operator=(const base_error& e)                                    { base_exception_type::operator=(e); m_offset = e.m_offset; return *this; }

    std::size_t offset() const
    {
        return m_offset;
    }

};


MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(unexpected_format_end, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(unexpected_symbol, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(named_argid_not_supported, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(argid_out_of_range, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(argid_not_found, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(argid_already_exist, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(invalid_argument_type, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(filter_as_value_error, base_error);
MARTY_FORMAT_DECLARE_EXCEPTION_CLASS(unknown_value_filter, base_error);


    // std::size_t getSymbolOffset() const
    // {
    //     return symbolOffset;
    // }



// class base_error : public std::runtime_error // exception
// {
//  
//   typedef std::runtime_error base_exception_t;
//  
// public:
//  
//     base_error(const std::string& what_arg) : base_exception_t(what_arg             ) {}
//     base_error(const char* what_arg       ) : base_exception_t(std::string(what_arg)) {}
//  
//     base_error(const base_error &e)
//     : base_exception_t(e)
//     {}
//  
//     base_error& operator=(const base_error& e) = delete;
//  
// }; // class base_error

#if 0
MARTY_MEM_DECLARE_EXCEPTION_CLASS(base_error, std::runtime_error);
    MARTY_MEM_DECLARE_EXCEPTION_CLASS(invalid_value, base_error);
    MARTY_MEM_DECLARE_EXCEPTION_CLASS(unaligned_iterator, base_error);
    MARTY_MEM_DECLARE_EXCEPTION_CLASS(invalid_address_difference, base_error);
    MARTY_MEM_DECLARE_EXCEPTION_CLASS(incompatible_address_pointers, base_error);
    MARTY_MEM_DECLARE_EXCEPTION_CLASS(memory_access_error, base_error);
        MARTY_MEM_DECLARE_EXCEPTION_CLASS(access_denied           , memory_access_error);
        MARTY_MEM_DECLARE_EXCEPTION_CLASS(unassigned_memory_access, memory_access_error);
        MARTY_MEM_DECLARE_EXCEPTION_CLASS(unaligned_memory_access , memory_access_error);
        MARTY_MEM_DECLARE_EXCEPTION_CLASS(address_wrap            , memory_access_error);
        MARTY_MEM_DECLARE_EXCEPTION_CLASS(memory_fill_error       , memory_access_error);


inline
std::string getDefaultMemoryAccessErrorMessage(MemoryAccessResultCode rc)
{
    switch(rc)
    {
        case MemoryAccessResultCode::invalid               : return "MemoryAccessResultCode::invalid";
        case MemoryAccessResultCode::accessGranted         : return "access granted";
        case MemoryAccessResultCode::accessDenied          : return "access denied";
        case MemoryAccessResultCode::unassignedMemoryAccess: return "unassigned memory access";
        case MemoryAccessResultCode::unalignedMemoryAccess : return "unaligned memory access";
        case MemoryAccessResultCode::addressWrap           : return "address/offset wrap occured";
        case MemoryAccessResultCode::memoryFillError       : return "memory fill error";
        default: return "unknown MemoryAccessResultCode";
    }
}

inline
std::string getMemoryAccessErrorMessage(MemoryAccessResultCode rc, const std::string &customMsg=std::string())
{
    return !customMsg.empty() ? customMsg : getDefaultMemoryAccessErrorMessage(rc);
}

// Кидает исключение, соответствующее коду ошибки. Если ошибки нет - не кидает
inline
void throwMemoryAccessError(MemoryAccessResultCode rc, const std::string &msg=std::string(), std::string msgExtra=std::string())
{
    if (!msgExtra.empty())
        msgExtra = ": " + msgExtra;
    switch(rc)
    {
        case MemoryAccessResultCode::accessGranted         : break;

        case MemoryAccessResultCode::invalid               : throw invalid_value           (getMemoryAccessErrorMessage(rc, msg)+msgExtra);
        case MemoryAccessResultCode::accessDenied          : throw access_denied           (getMemoryAccessErrorMessage(rc, msg)+msgExtra);
        case MemoryAccessResultCode::unassignedMemoryAccess: throw unassigned_memory_access(getMemoryAccessErrorMessage(rc, msg)+msgExtra);
        case MemoryAccessResultCode::unalignedMemoryAccess : throw unassigned_memory_access(getMemoryAccessErrorMessage(rc, msg)+msgExtra);
        case MemoryAccessResultCode::addressWrap           : throw address_wrap            (getMemoryAccessErrorMessage(rc, msg)+msgExtra);
        case MemoryAccessResultCode::memoryFillError       : throw memory_fill_error       (getMemoryAccessErrorMessage(rc, msg)+msgExtra);

        default: return;
    }
}

inline
void throwMemoryAccessError(MemoryAccessResultCode rc, const char *msg)
{
    throwMemoryAccessError(rc, std::string(msg));
}

//----------------------------------------------------------------------------
#endif


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/exceptions.h"


