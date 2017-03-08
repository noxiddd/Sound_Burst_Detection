<html>
<style>
table {
    font-family: arial, sans-serif;
    border-collapse: collapse;
    width: 100%;
    color:brown;
}

td, th {
    border: 1px solid #dddddd;
    text-align: left;
    padding: 8px;
    

}
th{
  background-color: red;
}

tr:nth-child(even) {
    background-color: #d00dff;
}
</style>
<style style="text/css">
body {
  background-image: url("images/fina.png");
  background-position: 50% 50%;
  background-repeat: repeat;
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
       echo "<br><font color='green'>connection sucessful</font></br>";
   }
echo "<center><h1><font color='black'>GUNSHOT EVENTS</font></h1></center>";
$sql = 'SELECT Event_ID,Event_Date,Event_Time,Dist_Mod_01,Dist_Mod_02,Dist_Mod_03 FROM Gunshots_Recorded';
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


</html>