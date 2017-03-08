<html>
<style>
table {
    font-family: arial, sans-serif;
    border-collapse: collapse;
    width: 100%;
}

td, th {
    border: 1px solid #dddddd;
    text-align: left;
    padding: 8px;
}

tr:nth-child(even) {
    background-color: #dddddd;
}
</style>
<?php
$servername = "localhost";
$username = "root";
$password = "root";
$dbname = "Sound_Burst_Detection";

$conn = new mysqli($servername, $username, $password, $dbname);//establshes connection to databse
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
else
   {
       echo "<br><font color='cyan'>connection sucessful</font></br>";
   }

$sql = 'SELECT Event_ID, Location, Event_Date, Event_Time FROM Location';
$result =$conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
    echo "<table style='width:100%'><tr><th>Event_ID</th><th>Event_Date</th><th>Event_Time</th><th>Distance_Mod_01</th><th>Distance_Mod_02</th><th>Distance_Mod_03</th></tr>";
    while($row = $result->fetch_assoc()) {
        echo "<tr><td><center>".$row["Event_ID"]."</center></td><td><center>".$row["Event_Date"]."</center></td><td><center>".$row["Event_Time"]."</center></td><td><center>".$row["Dist_Mod_01"]."</center></td><td><center>".$row["Dist_Mod_02"]."</center></td><td><center>".$row["Dist_Mod_03"]."</center></td><td>"."</td></tr>";
    }
     echo "</table>";
} else {
    echo "0 results";
}   
?>

?>
</html>