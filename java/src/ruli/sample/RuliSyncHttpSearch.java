
package ruli.sample;

import java.util.Map;

import ruli.RuliSync;
import ruli.RuliUtil;

public class RuliSyncHttpSearch {
    
    private static void usage() {
	System.err.println("usage:   RuliSyncHttpSearch domain [forcePort [options]]");
	System.err.println("example: RuliSyncHttpSearch web-domain.tld 80");
	System.exit(1);
    }

    public static void main(String[] args) {

	if ((args.length < 1) || (args.length > 3))
	    usage();

	String domain = args[0];
	int forcePort = (args.length > 1) ? Integer.parseInt(args[1]) : -1;
	int options = (args.length > 2) ? Integer.parseInt(args[2]) : 0;

	Map[] srvList = RuliSync.httpQuery(domain, forcePort, options);

	System.out.print(domain);
	if (forcePort > 0)
	    System.out.print(":" + forcePort);
        System.out.println();

	RuliUtil.dumpSrvList(System.out, srvList);
    }

}
