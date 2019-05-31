; RULI - Resolver User Layer Interface - Querying DNS SRV records
; Copyright (C) 2004 Everton da Silva Marques
;
; RULI is free software; you can redistribute it and/or modify it
; under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2, or (at your option)
; any later version.
;
; RULI is distributed in the hope that it will be useful, but WITHOUT
; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
; License for more details.
; 
; You should have received a copy of the GNU General Public License
; along with RULI; see the file COPYING.  If not, write to the Free
; Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
; 02111-1307, USA.
;
; $Id: ruli.scm,v 1.4 2005/01/04 17:16:22 evertonm Exp $
;
; This is the Guile autoload file for the RULI extension.
;
; INSTALLATION:
;
; 1) Compile the library 'libguile-ruli.so'.
;
;    See the file README for details.
;
; 2) Put libguile-ruli.so in the dynamic loader path.
;
;    Under Linux, see the file /etc/ld.so.conf
;    or the LD_LIBRARY_PATH environment variable.
;
; 3) Put this file (ruli.scm) in the Guile autoload path.
;
;    See the GUILE_LOAD_PATH environment variable.
;
;    Run this shell command to find out the current
;    autoload path:
;
;    guile -c '(write %load-path) (newline)'
;
; 4) In the Guile interpreter, you can load the ruli module with:
;
;    (use-modules (ruli))
;
; USAGE:
;
; (ruli-sync-query service domain fallback_port [options])
;
; (ruli-sync-smtp-query domain [options])
;
; Possible results are:
;
;   1) Symbol 'timeout'
;   2) Symbol 'unavailable'
;   3) List of errors: ( (srv-code <number>) [(rcode <number>)] )
;   4) List of SRV records, possibly empty, as in this example:
;
;      (
;       ( 
;        (target "host1.domain")
;        (priority 0)
;        (weight 10)
;        (port 25)
;        (addresses "1.1.1.1" "2.2.2.2") 
;       )
;       (
;        (target "host2.domain")
;        (priority 0)
;        (weight 0)
;        (port 80)
;        (addresses "3.3.3.3" "4.4.4.4") 
;       )
;      )
;
; EXAMPLES:
;
; (ruli-sync-query "_http._tcp" "web-domain.tld" -1)
;
; (ruli-sync-smtp-query "mail-domain.tld")


; Module name
;
(define-module (ruli))

; Public symbols
;
(export ruli-sync-query)
(export ruli-sync-smtp-query)
(export ruli-sync-http-query)

; Initialize module from library (extension)
;
(load-extension "libguile-ruli" "ruli_guile_init")


; EOF

