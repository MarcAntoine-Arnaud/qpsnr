%module qpsnr

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%include "stdExceptions.i"

%{
#include <qpsnr.hpp>
%}

namespace std {
  %template(OptionMap) map< string, string >;
}

%include <qpsnr.hpp>
