/*
 * _reversi.cpp
 *
 *  Created on: 2014年12月31日
 *      Author: jadesoul
 */

#include <python.h>
#include <boost/python.hpp>

char const* greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
}
