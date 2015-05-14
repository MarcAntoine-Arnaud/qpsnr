/*
*	qpsnr (C) 2010 E. Oriani, ema <AT> fastwebnet <DOT> it
*
*	This file is part of qpsnr.
*
*	qpsnr is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	qpsnr is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with qpsnr.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _MT_H_
#define _MT_H_

#include <list>
#include <vector>
#include <errno.h>
#include <exception>
#include <string>

#include <iostream>

namespace mt {

	class mt_exception : public std::exception
	{
	public:
		mt_exception(const std::string& what)
			: _what(what)
		{
		}

		virtual const char* what() const throw()
		{
			return _what.c_str();
		}

		~mt_exception() throw ()
		{
		}
	private:
		const std::string	_what;
	};

}

#endif
