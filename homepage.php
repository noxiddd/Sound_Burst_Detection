<hmtl>
<head>
<center> 
<h1>Sound Burst Detection</h1>
<!-- Styles -->	
<style>
.col{
  color:red;
      width:200px;
      height:200px;
}
 .col:hover {
      color:blue;
     }
</style>  
<style style="text/css">
body {
  background-image: url("images/sound.jpg");
  background-position: 50% 50%;
  background-repeat: repeat;
}
</style>
<script type="javascript">
alert("you...."

</script>



</head>

<br><img src="images/finalyeargif.gif" alt="Test" WIDTH=600 HEIGHT=300> </center></br>

<button id="button_table" class="float-left submit-button" >Retrieve Data</button>

<center>
<div class=col>
<?php
echo "<br>POST</br> ";
echo $_POST[Temp];
echo "<br>GET</br>";
echo $_GET[Temp];
?>
</div>
</center>

<br>
<?php include 'database.php';
session_start();
$_SESSION['dist_mod01']=$_GET[distMod01];
$_SESSION['dist_mod02']=$_GET[distMod02];
$_SESSION['dist_mod03']=$_GET[distMod03];
?>
</br>

<script type="text/javascript">
    document.getElementById("button_table").onclick = function () {
        location.href = "table.php";
    };
</script>
   
</html> 
