<!-- ***************************************SOUND BUSRT DETECTOR*********************-->
<html>


<head>
<center> <font size="5">SOUND BURST LOCATION SYSTEM</font></center>

<?php
//gsd_01 gun shot detector 01 is the promary module
$gsd_01_x=$_GET["gsd_01_event_location_x"];
$gsd_01_y=$_GET["gsd_01_event_location_y"];
echo 'b '.$gsd_01_x." ".$gsd_01_y.'grumble';


//**************************************Database*********************************//
//	session_start();//wtf is this need to check it out

$user_name="*******";
$password="********";
$datbase="Sound_Burst_Detection"
$server="localhost";
$conn=new mysqli($server,$user_name,$password,$database);

if($conn->connect_error){//check if connecttion failed
  die("Connecttion Failed: ".$conn->connect_error);
}
echo "Connected Sucessfully";
//$evnt_time=$_SESSION[]
?>

<style>
table,th,td{border:1px solid black; border-collapse:collapse;}
</style>
</head>


</div>
  <table id="Gun_Detected">
    <tr><!--table roe tag -->
      	<th><!-- table header flag-->
            Event Location
        </th>
        <th>Event Time</th>
        <th>Event Date</th>
    </tr>
    <tr>
        <td> 77W 18N</td>
        <td> 13:34 GMT </td>
        <td> 11/11/2017</td>
    </tr>
</table>

<div>



<button onclick="addElementToTable()">press mee</button>
<script>
function addElementToTable()//dynamically  adds more fields to table 
   {
    var detection_table=document.getElementById("Gun_Detected");
    var row=detection_table.insertRow(2);
    var cell1=row.insertCell(0);
    var cell2=row.insertCell(1);
    var cell3=row.insertCell(2);
    cell1.innerHTML="null";//place holder value
    cell2.innerHTML="null";
    cell3.innerHTML="null";
   }

</script>
</body>
</html>

