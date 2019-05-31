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
  $Id: lua_ruli.c,v 1.9 2004/10/26 18:19:24 evertonm Exp $
*/


#include <assert.h>
#include <ruli.h>
#include <lauxlib.h>

#include "lua_ruli.h"


static void scan_sync_srv_list(lua_State *L, int result_index, ruli_sync_t *sync_query)
{
  ruli_list_t *srv_list;
  int         srv_list_size;
  int         srv_code;
  int         i;

  assert(sync_query);

  srv_code = ruli_sync_srv_code(sync_query);
  assert(srv_code != RULI_SRV_CODE_VOID);

  if (srv_code == RULI_SRV_CODE_ALARM) {
    return;
  }

  if (srv_code == RULI_SRV_CODE_UNAVAILABLE) {
    return;
  }

  if (srv_code) {
    int rcode = ruli_sync_rcode(sync_query);
    if (rcode) {
      return;
    }

    return;
  }

  srv_list = ruli_sync_srv_list(sync_query);
  assert(srv_list);

  srv_list_size = ruli_list_size(srv_list);
  assert(srv_list_size >= 0);
  if (srv_list_size < 1) {
    return;
  }

  /*
   * Scan list of SRV records
   */
  for (i = 0; i < srv_list_size; ++i) {
    ruli_srv_entry_t *entry = (ruli_srv_entry_t *) ruli_list_get(srv_list, i);
    ruli_list_t      *addr_list = &entry->addr_list;
    int              addr_list_size = ruli_list_size(addr_list);
    int              j;
    int              srv_index;
    int              addr_index;

    /*
     * target host
     */
    {
      char txt_dname_buf[RULI_LIMIT_DNAME_TEXT_BUFSZ];
      int  txt_dname_len;

      if (ruli_dname_decode(txt_dname_buf, RULI_LIMIT_DNAME_TEXT_BUFSZ,
                            &txt_dname_len,
                            entry->target, entry->target_len))
        continue;

      /* create srv table */
      lua_newtable(L);
      srv_index = lua_gettop(L);

      /* add srv record to result table */
      lua_pushvalue(L, srv_index);         /* value=srv table */
      lua_rawseti(L, result_index, i + 1); /* key=i+1, store at result table, pop value */

      /* add target=>hostname to srv table */
      lua_pushstring(L, "target");                      /* key */
      lua_pushlstring(L, txt_dname_buf, txt_dname_len); /* value=hostname */
      lua_settable(L, srv_index);                       /* store at srv table, pop key/value */
    }

    /*
     * priority
     */
    lua_pushstring(L, "priority");      /* key */
    lua_pushnumber(L, entry->priority); /* value */
    lua_settable(L, srv_index);         /* store at srv table, pop key/value */

    /*
     * weight
     */
    lua_pushstring(L, "weight");      /* key */
    lua_pushnumber(L, entry->weight); /* value */
    lua_settable(L, srv_index);       /* store at srv table, pop key/value */

    /*
     * port
     */
    lua_pushstring(L, "port");      /* key */
    lua_pushnumber(L, entry->port); /* value */
    lua_settable(L, srv_index);     /* store at srv table, pop key/value */

    /*
     * addresses
     */

    /* create addr table */
    lua_newtable(L);
    addr_index = lua_gettop(L);

    /* add addr table to srv table */
    lua_pushstring(L, "addresses"); /* key */
    lua_pushvalue(L, addr_index);   /* value=addr table */
    lua_settable(L, srv_index);     /* store at srv table, pop key/value */

    for (j = 0; j < addr_list_size; ++j) {
      char buf[40];
      ruli_addr_t *addr = (ruli_addr_t *) ruli_list_get(addr_list, j);
      int len = ruli_addr_snprint(buf, sizeof(buf), addr);

      assert(len > 0);
      assert(len < sizeof(buf));

      /* add to addr table */
      lua_pushstring(L, buf);            /* value=addr string */
      lua_rawseti(L, addr_index, j + 1); /* key=j+1, store at addr table, pop value */
    }

    lua_pop(L, 2); /* remove srv and addr tables from stack */

  } /* scan srv records */

}

static int lua_ruli_sync_query(lua_State *L) 
{
  const char *service;
  const char *domain;
  int        fallback_port;
  long       options;
  int        result_index;

  lua_newtable(L);
  result_index = lua_gettop(L);

  service       = lua_tostring(L, 1);
  domain        = lua_tostring(L, 2);
  fallback_port = lua_tonumber(L, 3);
  options       = lua_tonumber(L, 4);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_query(service, domain,
                                 fallback_port, options);
    if (!sync_query)
      goto exit;

    scan_sync_srv_list(L, result_index, sync_query);

    ruli_sync_delete(sync_query);
  }

exit:
  return 1; /* number of results */
}

static int lua_ruli_sync_smtp_query(lua_State *L) 
{
  const char *domain;
  long       options;
  int        result_index;

  lua_newtable(L);
  result_index = lua_gettop(L);

  domain        = lua_tostring(L, 1);
  options       = lua_tonumber(L, 2);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_smtp_query(domain, options);
    if (!sync_query)
      goto exit;

    scan_sync_srv_list(L, result_index, sync_query);

    ruli_sync_delete(sync_query);
  }

exit:
  return 1; /* number of results */
}

static int lua_ruli_sync_http_query(lua_State *L) 
{
  const char *domain;
  int        force_port;
  long       options;
  int        result_index;

  lua_newtable(L);
  result_index = lua_gettop(L);

  domain     = lua_tostring(L, 1);
  force_port = lua_tonumber(L, 2);
  options    = lua_tonumber(L, 3);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_http_query(domain, force_port, options);
    if (!sync_query)
      goto exit;

    scan_sync_srv_list(L, result_index, sync_query);

    ruli_sync_delete(sync_query);
  }

exit:
  return 1; /* number of results */
}

static const struct luaL_reg ruli_calls[] = {
  {"ruli_sync_query",      lua_ruli_sync_query},
  {"ruli_sync_smtp_query", lua_ruli_sync_smtp_query},
  {"ruli_sync_http_query", lua_ruli_sync_http_query},
  {NULL, NULL}
};

int luaopen_ruli(lua_State *L) 
{
  luaL_openlib(L, "ruli", ruli_calls, 0);
  return 1;
}
