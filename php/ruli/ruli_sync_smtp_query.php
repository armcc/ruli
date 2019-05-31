<?
	function perform_query($domain, $options) {

		echo "+++ launching SRV/SMTP query for: domain=$domain\n";	

		$srv_list = ruli_sync_smtp_query($domain, $options);
		if (!$srv_list) {
			echo "SMTP query failed for: domain=$domain\n";
			return;
		}

		print_r($srv_list);
	}

	perform_query("bocaaberta.com.br", 0);
	perform_query("kensingtonlabs.com", 0);
	perform_query("aol.com", 0);
	perform_query("bad.tld", 0);
?>
