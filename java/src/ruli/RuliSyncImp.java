
package ruli;

import java.util.Map;

class RuliSyncImp {

    static native Map[] srvQuery(String service, String domain, 
				 int fallbackPort, int options);

    static native Map[] smtpQuery(String domain, int options);

    static native Map[] httpQuery(String domain, int forcePort, 
				  int options);

    static {
	System.loadLibrary("java-ruli");
    }
}
