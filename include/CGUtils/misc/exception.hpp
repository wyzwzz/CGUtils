//
// Created by wyz on 2022/2/17.
//
#pragma once
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>

namespace wzz::misc{

inline void print_exception(const std::exception& e, int level = 0) noexcept
{
    std::cerr<< std::string(level,' ') << "exception: "<<e.what()<<"\n";
    try{
        std::rethrow_if_nested(e);
    }
    catch (const std::exception& nested_exception)
    {
        print_exception(nested_exception,level+1);
    }
    catch (...)
    {
        std::cerr<<"unknown exception\n";
    }
}

inline std::string extract_exception_ptr(const std::exception_ptr& eptr)
{
    try{
        std::rethrow_exception(eptr);
    }
    catch (const std::exception& e)
    {
        return e.what();
    }
    catch (...)
    {
        return "unknown exception";
    }
}

}


