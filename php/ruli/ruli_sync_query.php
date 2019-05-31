<?
	function perform_query($service, $domain, $fallback_port, $options) {

		echo "+++ launching SRV query for: service=$service domain=$domain\n";	

		$srv_list = ruli_sync_query($service, $domain, $fallback_port, $options);
		if (!$srv_list) {
			echo "SRV query failed for: service=$service domain=$domain\n";
			return;
		}

		print_r($srv_list);
	}

	perform_query("_http._tcp", "bocaaberta.com.br", -1, 0);
	perform_query("_ftp._tcp", "aol.com", -1, 0);
	perform_query("_xxx._xxx", "bad.tld", -1, 0);
?>
