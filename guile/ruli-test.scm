; $Id: ruli-test.scm,v 1.3 2004/08/26 17:09:47 evertonm Exp $

; with optional argument
(ruli-sync-query "_http._tcp" "isc.org" -1 0)
(ruli-sync-smtp-query "isc.org" 0)

; without optional argument
(ruli-sync-query "_http._tcp" "isc.org" -1)
(ruli-sync-smtp-query "isc.org")

(define sample-srv-list 
  '(
    ( (target "host1.domain")
      (priority 0)
      (weight 10)
      (port 25)
      (addresses "1.1.1.1" "2.2.2.2") )
    ( (target "host2.domain")
      (priority 0)
      (weight 0)
      (port 80)
      (addresses "3.3.3.3" "4.4.4.4") )
  )
)

; activate readline support
(use-modules (ice-9 readline))
(activate-readline)

; write query result to file
(define result '())
(define out '())
(set! result (ruli-sync-smtp-query "aol.com"))
(set! out (open-output-file "result.txt"))
(write-line result out)
(close out)

