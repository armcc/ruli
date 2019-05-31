
package ruli.sample;

import java.util.Map;

import ruli.RuliSync;
import ruli.RuliUtil;

public class RuliSyncSmtpSearch {
    
    private static void usage() {
	System.err.println("usage:   RuliSyncSmtpSearch domain [options]");
	System.err.println("example: RuliSyncSmtpSearch mail-domain.tld");
	System.exit(1);
    }

    public static void main(String[] args) {

	if ((args.length < 1) || (args.length > 2))
	    usage();

	String domain = args[0];
	int options = (args.length > 1) ? Integer.parseInt(args[1]) : 0;

	Map[] srvList = RuliSync.smtpQuery(domain, options);

	System.out.println(domain);

	RuliUtil.dumpSrvList(System.out, srvList);
    }

}
