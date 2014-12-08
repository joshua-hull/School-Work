<?php
	$hostname = "fireflydatabase.joshuahull.net";
	$port = "5432";
	$database = "fireflyproject"
	$user = "fireflyuser";
	$password = "FireFlyProject!1";

	function database_insert($time, $count, $latitude, $longitude, $habitat, $luma, $comment){
		$connection = pg_connect("host=$hostname port=$PORT dbname=$database user=$user password=$password");
		if(!$connection){
			http_response_code(500);
			exit;
		}

		$result = pg_query($connection, "INSERT INTO entries (time, count, latitude, longitude, habitat, luma, comment) VALUES ('$time', '$count', '$latitude', '$longitude', '$habitat', '$luma', '$comment')");
		if(!$result){
			http_response_code(500);
			exit;
		}
	}
?>