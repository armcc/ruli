/*-GNU-GPL-BEGIN-*
RULI - Resolver User Layer Interface - Querying DNS SRV records
Copyright (C) 2004 Everton da Silva Marques

RULI is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

RULI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RULI; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
*-GNU-GPL-END-*/

/*
  $Id: lua_ruli.h,v 1.5 2004/10/26 18:19:24 evertonm Exp $
*/


#ifndef LUA_RULI_H
#define LUA_RULI_H


#include <lua.h>


int luaopen_ruli(lua_State *L);


#endif /* LUA_RULI_H */

