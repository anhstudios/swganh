/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#include "clock.h"

#include <cassert>
#include <ctime>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#if _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#else
#include <sys/time.h>
#endif

namespace anh {

Clock::Clock()
{
    stored_time_ = local_time();
#if _WIN32
    timeBeginPeriod(1);
#endif
}
Clock::~Clock()
{
}
char* Clock::current_date_time_string()
{
    time_t ltime;
    time( &ltime);
    return ctime(&ltime);
}

uint64_t Clock::global_time() const
{
    return local_time();
}

uint64_t Clock::local_time() const
{
#if _WIN32
	return 0;
    return timeGetTime();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
	return (tv.tv_sec*1000)+(tv.tv_usec / 1000);
#endif
}

} // anh