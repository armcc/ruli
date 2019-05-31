
package ruli;

import java.util.Map;

public class RuliSync {

    public static Map[] srvQuery(String service, String domain, 
				 int fallbackPort, int options) {
	return RuliSyncImp.srvQuery(service, domain, fallbackPort, options);
    }

    public static Map[] smtpQuery(String domain, int options) {
	return RuliSyncImp.smtpQuery(domain, options);
    }

    public static Map[] httpQuery(String domain, int forcePort, int options) {
	return RuliSyncImp.httpQuery(domain, forcePort, options);
    }
}
