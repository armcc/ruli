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
  $Id: ruli-guile.c,v 1.3 2004/08/15 06:21:45 evertonm Exp $
 */

#include <stdlib.h>

#include "guile_ruli.h"

static void inner_main(void *closure, int argc, char **argv)
{
    ruli_guile_init();
    scm_shell(argc, argv);
}

int main(int argc, char **argv)
{
  scm_boot_guile(argc, argv, inner_main, 0);
  exit(0); /* UNREACHEABLE */
}

