/*-GNU-GPL-BEGIN-*
RULI - Resolver User Layer Interface - Querying DNS SRV records
Copyright (C) 2003 Everton da Silva Marques

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
  $Id: guile_ruli.c,v 1.6 2004/10/13 18:32:50 evertonm Exp $
 */


#include <assert.h>

#include "guile_ruli.h"

#include <ruli.h>
#include <string.h>


extern char *gh_scm2newstr(SCM str, size_t *lenp);


const char * const RULI_SYNC_QUERY      = "ruli-sync-query";
const char * const RULI_SYNC_SMTP_QUERY = "ruli-sync-smtp-query";
const char * const RULI_SYNC_HTTP_QUERY = "ruli-sync-http-query";

const char * const SYMB_TIMEOUT     = "timeout";
const char * const SYMB_UNAVAILABLE = "unavailable";
const char * const SYMB_SRV_CODE    = "srv-code";
const char * const SYMB_RCODE       = "rcode";

const char * const SYMB_TARGET      = "target";
const char * const SYMB_PRIORITY    = "priority";
const char * const SYMB_WEIGHT      = "weight";
const char * const SYMB_PORT        = "port";
const char * const SYMB_ADDRESSES   = "addresses";

void ruli_guile_init(void)
{
  scm_c_define_gsubr(RULI_SYNC_QUERY,      3, 1, 0, scm_ruli_sync_query);
  scm_c_define_gsubr(RULI_SYNC_SMTP_QUERY, 1, 1, 0, scm_ruli_sync_smtp_query);
  scm_c_define_gsubr(RULI_SYNC_HTTP_QUERY, 1, 2, 0, scm_ruli_sync_http_query);
}

static SCM scan_sync_srv_list(ruli_sync_t *sync_query)
{
  SCM result = SCM_EOL;
  int srv_list_size;
  const ruli_list_t *srv_list;
  int srv_code;
  int i;

  SCM symb_target;
  SCM symb_priority;
  SCM symb_weight;
  SCM symb_port;
  SCM symb_addresses;

  assert(sync_query);

  srv_code = ruli_sync_srv_code(sync_query);
  assert(srv_code != RULI_SRV_CODE_VOID);

  /* if lookup times out, return 'timeout' symbol */
  if (srv_code == RULI_SRV_CODE_ALARM)
    return scm_string_to_symbol(scm_mem2string(SYMB_TIMEOUT, strlen(SYMB_TIMEOUT)));

  /* if service is not available, return 'unavailable' symbol */
  if (srv_code == RULI_SRV_CODE_UNAVAILABLE)
    return scm_string_to_symbol(scm_mem2string(SYMB_UNAVAILABLE, strlen(SYMB_UNAVAILABLE)));

  /* if srv error code, return (srv-code <srv-code> <srv-message>) */
  if (srv_code) {
    int rcode;
    const char *srv_msg = ruli_srv_errstr(srv_code);
    SCM symb_srv_code = 
      scm_string_to_symbol(scm_mem2string(SYMB_SRV_CODE, strlen(SYMB_SRV_CODE)));
    SCM s_srv_code = scm_int2num(srv_code);
    SCM s_srv_msg = scm_mem2string(srv_msg, strlen(srv_msg));

    result = scm_append(scm_list_2(result, scm_list_1(scm_list_3(symb_srv_code, s_srv_code, s_srv_msg))));

    /* if rcode, return (rcode <rcode>) */
    rcode = ruli_sync_rcode(sync_query);
    if (rcode) {
      SCM symb_rcode = 
        scm_string_to_symbol(scm_mem2string(SYMB_RCODE, strlen(SYMB_RCODE)));
      SCM s_rcode = scm_int2num(rcode);
      result = scm_append(scm_list_2(result, scm_list_1(scm_list_2(symb_rcode, s_rcode))));
    }

    return result;
  }

  srv_list = ruli_sync_srv_list(sync_query);
  assert(srv_list);

  srv_list_size = ruli_list_size(srv_list);
  assert(srv_list_size >= 0);
  if (srv_list_size < 1)
    return SCM_EOL;

  symb_target    = scm_string_to_symbol(scm_mem2string(SYMB_TARGET, strlen(SYMB_TARGET)));
  symb_priority  = scm_string_to_symbol(scm_mem2string(SYMB_PRIORITY, strlen(SYMB_PRIORITY)));
  symb_weight    = scm_string_to_symbol(scm_mem2string(SYMB_WEIGHT, strlen(SYMB_WEIGHT)));
  symb_port      = scm_string_to_symbol(scm_mem2string(SYMB_PORT, strlen(SYMB_PORT)));
  symb_addresses = scm_string_to_symbol(scm_mem2string(SYMB_ADDRESSES, strlen(SYMB_ADDRESSES)));

  /*
   * Scan list of SRV records
   */
  for (i = 0; i < srv_list_size; ++i) {
    ruli_srv_entry_t *entry = (ruli_srv_entry_t *) ruli_list_get(srv_list, i);
    ruli_list_t      *addr_list = &entry->addr_list;
    int              addr_list_size = ruli_list_size(addr_list);
    int              j;
    SCM              s_srv = SCM_EOL;
    SCM              s_addr_list = scm_list_1(symb_addresses);

    /*
     * target host
     */
    {
      char txt_dname_buf[RULI_LIMIT_DNAME_TEXT_BUFSZ];
      int  txt_dname_len;
      SCM  s_target;

      if (ruli_dname_decode(txt_dname_buf, RULI_LIMIT_DNAME_TEXT_BUFSZ,
                            &txt_dname_len,
                            entry->target, entry->target_len))
        continue;

      s_target = scm_list_2(symb_target, scm_mem2string(txt_dname_buf, txt_dname_len));
      s_srv = scm_append(scm_list_2(s_srv, scm_list_1(s_target)));
    }

    /*
     * priority, weight, port
     */
    s_srv = scm_append(scm_list_2(s_srv, scm_list_1(
      scm_list_2(symb_priority, scm_int2num(entry->priority))
      )));

    s_srv = scm_append(scm_list_2(s_srv, scm_list_1(
      scm_list_2(symb_weight, scm_int2num(entry->weight))
      )));

    s_srv = scm_append(scm_list_2(s_srv, scm_list_1(
      scm_list_2(symb_port, scm_int2num(entry->port))
      )));

    /*
     * addresses
     */
    for (j = 0; j < addr_list_size; ++j) {
      char buf[40];
      ruli_addr_t *addr = (ruli_addr_t *) ruli_list_get(addr_list, j);
      int len = ruli_addr_snprint(buf, sizeof(buf), addr);
      assert(len > 0);
      assert(len < sizeof(buf));
      s_addr_list = scm_append(scm_list_2(s_addr_list, 
					  scm_list_1(scm_mem2string(buf, len))
					  ));
    }

    s_srv = scm_append(scm_list_2(s_srv, scm_list_1(s_addr_list)));

    result = scm_append(scm_list_2(result, scm_list_1(s_srv)));

  } /* scan srv records */

  return result;
}

SCM scm_ruli_sync_query(SCM s_service, SCM s_domain, SCM s_fallback_port,
			SCM s_options)
{
  SCM result = SCM_EOL;

  char   *service;
  size_t service_len;
  char   *domain;
  size_t domain_len;
  int    fallback_port;
  int    options;

  SCM_ASSERT(SCM_STRINGP(s_service), s_service, SCM_ARG1, RULI_SYNC_QUERY);
  SCM_ASSERT(SCM_STRINGP(s_domain), s_domain, SCM_ARG2, RULI_SYNC_QUERY);
  SCM_ASSERT(SCM_NUMBERP(s_fallback_port), s_fallback_port, SCM_ARG3, RULI_SYNC_QUERY);

  service       = gh_scm2newstr(s_service, &service_len);
  domain        = gh_scm2newstr(s_domain, &domain_len);
  fallback_port = SCM_INUM(s_fallback_port);
  options       = SCM_UNBNDP(s_options) ? 0 : SCM_INUM(s_options);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_query(service, domain,
                                 fallback_port, options);
    if (!sync_query)
      goto exit;

    result = scan_sync_srv_list(sync_query);

    ruli_sync_delete(sync_query);
  }

exit:
  free(service);
  free(domain);
  return result;
}

SCM scm_ruli_sync_smtp_query(SCM s_domain, SCM s_options)
{
  SCM result = SCM_EOL;

  char   *domain;
  size_t domain_len;
  int    options;

  SCM_ASSERT(SCM_STRINGP(s_domain), s_domain, SCM_ARG1, RULI_SYNC_SMTP_QUERY);

  domain  = gh_scm2newstr(s_domain, &domain_len);
  options = SCM_UNBNDP(s_options) ? 0 : SCM_INUM(s_options);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_smtp_query(domain, options);
    if (!sync_query)
      goto exit;

    result = scan_sync_srv_list(sync_query);

    ruli_sync_delete(sync_query);
  }

exit:
  free(domain);
  return result;
}

SCM scm_ruli_sync_http_query(SCM s_domain, SCM s_force_port, SCM s_options)
{
  SCM result = SCM_EOL;

  char   *domain;
  size_t domain_len;
  int    force_port;
  int    options;

  SCM_ASSERT(SCM_STRINGP(s_domain), s_domain, SCM_ARG1, RULI_SYNC_SMTP_QUERY);

  domain     = gh_scm2newstr(s_domain, &domain_len);
  force_port = SCM_UNBNDP(s_force_port) ? -1 : SCM_INUM(s_force_port);
  options    = SCM_UNBNDP(s_options) ? 0 : SCM_INUM(s_options);

  {
    ruli_sync_t *sync_query;

    sync_query = ruli_sync_http_query(domain, force_port, options);
    if (!sync_query)
      goto exit;

    result = scan_sync_srv_list(sync_query);

    ruli_sync_delete(sync_query);
  }

exit:
  free(domain);
  return result;
}

