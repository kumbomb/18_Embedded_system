<script language='javascript'>
  window.setTimeout('window.location.reload()',1000);
</script>

<?php
$connect = mysqli_connect("localhost", "root", "root")
   or die("Connect Fail: " . mysqli_error());
echo "Connect Success!";
mysqli_select_db($connect, "term_project") or die("Select DB Fail!");

$query = "select time, lightness, temp from term order by time";
$result = mysqli_query($connect, $query) or die("Query Fail: " . mysql_error());

echo "<table>\n";

echo "<tr><td>Time</td><td>Lightness</td><td>Temp</td>";

while ($line = mysqli_fetch_array($result,MYSQLI_ASSOC)) {
   echo "\t<tr>\n";
   foreach ($line as $col_value) {
       echo "\t\t<td>$col_value</td>\n";
   }
   echo "\t</tr>\n";
}

echo "</table>\n";

mysqli_free_result($result);

mysqli_close($connect);
?>
