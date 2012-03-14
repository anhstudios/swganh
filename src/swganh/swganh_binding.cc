
/*
 This file is part of SWGANH. For more information, visit http://swganh.com

 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "swganh_binding.h"
#include <boost/python.hpp>

using namespace boost::python;
using namespace std;

void exportObject();
void exportTangible();
void exportCreature();
void exportPlayer();
void exportObjectController();
void exportOutOfBand();
void exportVec3();
void exportQuat();
void exportSWGANHKernel();
void exportCombatService();

BOOST_PYTHON_MODULE(py_swganh) {
    docstring_options local_docstring_options(true, true, false);

    exportObject();
    exportTangible();
    exportCreature();
    exportPlayer();
    exportObjectController();
    exportVec3();
    exportQuat();
    exportOutOfBand();
    exportSWGANHKernel();
    exportCombatService();
}