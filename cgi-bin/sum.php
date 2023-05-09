<?php
echo "content-type: text/html\r\n";
echo "status: 200 OK\r\n\r\n";
echo "<html>";
echo "<head>";
echo "<title> hello cgi </title>";
echo "</head>";
echo "<body>";
echo "<h1>Our simple calculator</h1>";
$query_string = $_SERVER['QUERY_STRING'];
parse_str($query_string, $params);
$a = isset($params['a']) ? $params['a'] : '';
$b = isset($params['b']) ? $params['b'] : '';
echo "<h2>".$a." + ".$b." : ".strval((int)$a + (int)$b)."</h2>";
echo "</body>";
echo "</html>";
?>
