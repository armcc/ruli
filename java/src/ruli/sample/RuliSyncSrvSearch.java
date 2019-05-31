
package ruli.sample;

import java.util.Map;

import ruli.RuliSync;
import ruli.RuliUtil;

public class RuliSyncSrvSearch {
    
    private static void usage() {
	System.err.println("usage:   RuliSyncSrvSearch service domain [fallbackPort [options]]");
	System.err.println("example: RuliSyncSrvSearch _http._tcp web-domain.tld");
	System.exit(1);
    }

    public static void main(String[] args) {

	if ((args.length < 2) || (args.length > 4))
	    usage();

	String service = args[0];
	String domain = args[1];
	int fallbackPort = (args.length > 2) ? Integer.parseInt(args[2]) : -1;
	int options = (args.length > 3) ? Integer.parseInt(args[3]) : 0;

	Map[] srvList = RuliSync.srvQuery(service, domain, 
					  fallbackPort, options);

	System.out.println(service + "." + domain);

	RuliUtil.dumpSrvList(System.out, srvList);
    }

}
