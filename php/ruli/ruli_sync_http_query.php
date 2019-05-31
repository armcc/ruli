<?
	function perform_query($domain, $force_port, $options) {

		echo "+++ launching SRV/HTTP query for: domain=$domain\n";	

		$srv_list = ruli_sync_http_query($domain, $force_port, $options);
		if (!$srv_list) {
			echo "HTTP query failed for: domain=$domain\n";
			return;
		}

		print_r($srv_list);
	}

	perform_query("registro.br", -1, 0);
	perform_query("registro.br", 80, 0);
	perform_query("gnu.org", -1, 0);
	perform_query("bad.tld", -1, 0);
?>
