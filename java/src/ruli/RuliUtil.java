
package ruli;

import java.util.Map;
import java.io.PrintStream;

public class RuliUtil {

    public static void dumpSrvList(PrintStream out, Map[] srvList) {

	for (int i = 0; i < srvList.length; ++i) {
	    Map srv = srvList[i];
	    String target = (String) srv.get("target");
	    int priority = ((Integer) srv.get("priority")).intValue();
	    int weight = ((Integer) srv.get("weight")).intValue();
	    int port = ((Integer) srv.get("port")).intValue();

	    out.print("  target=" + target + " priority=" + priority + " weight=" + weight + " port=" + port + " addresses=");
	    
	    String[] addrList = (String[]) srv.get("addresses");
	    for (int j = 0; j < addrList.length; ++j)
		out.print(addrList[j] + " ");
	    
	    out.println();
	}

    }
}
