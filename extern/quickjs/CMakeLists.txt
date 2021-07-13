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
# ***** END GPL LICENSE BLOCK *****

set(INC
  .
)

set(SRC
  cutils.c
  cutils.h
  libbf.c
  libbf.h
  libregexp.c
  libregexp.h
  libregexp-opcode.h
  libunicode.c
  libunicode.h
  libunicode-table.h
  list.h
  quickjs.h
  quickjs.c
  quickjs-atom.h
  quickjs-jsx.h
  #quickjs-libc.c
  #quickjs-libc.h
  quickjs-opcode.h
  quickjs-version.h
  win/dirent.h
  win/getopt.h
)

set(LIB
)

add_compile_definitions(JS_STRICT_NAN_BOXING)
add_compile_definitions(CONFIG_JSX)
#add_compile_definitions(CONFIG_STORAGE)
add_compile_definitions(CONFIG_BIGNUM)

blender_add_lib(extern_quickjs "${SRC}" "${INC}" "" "${LIB}")
