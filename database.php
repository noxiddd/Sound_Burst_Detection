<?php
session_start();

$servername = "localhost";
$username = "root";
$password = "root";
$dbname = "Sound_Burst_Detection";



/////////*****************VARIABLES*********************///
$ID=5;
$prev_ID=0;//stores the previous id of last event entered into the database
$dist_01=$_SESSION['dist_mod01'];//distance of gunshot from module 01
$dist_02=$_SESSION['dist_mod02'];//distance of gunshot from module 02
$dist_03=$_SESSION['dist_mod03'];//distance of gunshot from module 03
///////***********************************************///



// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);//establshes connection to databse
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
else
   {
       echo "<br><font color='white'>connection sucessful</font></br>";
   }
$date=date('d.m.y');
$time=date('h:i:s');

$sql = 'SELECT Event_ID FROM Gunshots_Recorded';
$result =$conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        $prev_ID=$row["Event_ID"];
    }
} else {
    echo "0 results";
}
/*
if ($result->num_rows > 0) {
    echo "<table><tr><th>Event_ID</th><th>Event_Date</th><th>Event_Time</th></tr>";
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<tr><td>".$row["Event_ID"]."</td><td>".$row["Event_Date"]." ".$row["Event_TIme"]." ".$row["Dist_Mod_01"]."</td></tr>";
    }
    echo "</table>";
} else {
    echo "0 results";
}*/
 // mysql_select_db('Sound_Burst_Detection');
/*  $retval = mysql_query( $sql, $conn );
 if(! $retval ) {
      die('Could not get data: ' . mysql_error());
   }

if($row = mysql_fetch_array($retval, MYSQL_ASSOC))
{
    echo "EventID:{$row[0]}";
}*/
$prev_ID=$prev_ID+1;//increments from last used ID
$sql = "INSERT INTO Gunshots_Recorded (Event_ID,Event_Date,Event_Time, Dist_Mod_01,Dist_Mod_02,Dist_Mod_03)
VALUES ($prev_ID,NOW(),NOW(),$dist_01,$dist_02,$dist_03)";

if ($conn->query($sql) === TRUE) {
    echo "<br><font color='white'>New record created successfully</font></br>";
} else {
    echo "<font color='white'>Error: " . $sql . "<br>" . $conn->error."</font>";
}

?>