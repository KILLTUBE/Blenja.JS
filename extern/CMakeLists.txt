# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The Original Code is Copyright (C) 2006, Blender Foundation
# All rights reserved.
# ***** END GPL LICENSE BLOCK *****

# Libs that adhere to strict flags
add_subdirectory(curve_fit_nd)

# Otherwise we get warnings here that we cant fix in external projects
remove_strict_flags()

# Not a strict flag, but noisy for code we don't maintain
if(CMAKE_COMPILER_IS_GNUCC)
  remove_cc_flag(
    "-Wmisleading-indentation"
  )
endif()


add_subdirectory(rangetree)
add_subdirectory(wcwidth)

if(WITH_BULLET)
  if(NOT WITH_SYSTEM_BULLET)
    add_subdirectory(bullet2)
  endif()
endif()

if(WITH_DRACO)
  add_subdirectory(draco)
endif()

if(WITH_BINRELOC)
  add_subdirectory(binreloc)
endif()

if(NOT WITH_SYSTEM_GLEW)
  if(WITH_GLEW_ES)
    add_subdirectory(glew-es)
  else()
    add_subdirectory(glew)
  endif()
endif()

if(WITH_LZO AND NOT WITH_SYSTEM_LZO)
  add_subdirectory(lzo)
endif()

if(WITH_LZMA)
  add_subdirectory(lzma)
endif()

if(WITH_CYCLES OR WITH_COMPOSITOR OR WITH_OPENSUBDIV)
  add_subdirectory(clew)
  if(WITH_CUDA_DYNLOAD)
    add_subdirectory(cuew)
  endif()
endif()

if(WITH_GHOST_X11 AND WITH_GHOST_XDND)
  add_subdirectory(xdnd)
endif()

if(WITH_LIBMV)
  add_subdirectory(ceres)
endif()

if(WITH_LIBMV OR WITH_GTESTS OR (WITH_CYCLES AND WITH_CYCLES_LOGGING))
  if(NOT WITH_SYSTEM_GFLAGS)
    add_subdirectory(gflags)
  endif()
  add_subdirectory(glog)
endif()

if(WITH_GTESTS)
  add_subdirectory(gtest)
  add_subdirectory(gmock)
endif()

if(WITH_SDL AND WITH_SDL_DYNLOAD)
  add_subdirectory(sdlew)
endif()

if(WITH_AUDASPACE AND NOT WITH_SYSTEM_AUDASPACE)
  set(AUDASPACE_CMAKE_CFG ${CMAKE_CURRENT_SOURCE_DIR}/audaspace/blender_config.cmake)
  add_subdirectory(audaspace)
endif()

if(WITH_QUADRIFLOW)
  set(QUADRIFLOW_CMAKE_CFG ${CMAKE_CURRENT_SOURCE_DIR}/quadriflow/blender_config.cmake)
  add_subdirectory(quadriflow)
endif()

if(WITH_MOD_FLUID)
  add_subdirectory(mantaflow)
endif()

if (WITH_COMPOSITOR)
	add_subdirectory(smaa_areatex)
endif()

add_subdirectory(duktape)
add_subdirectory(quickjs)
