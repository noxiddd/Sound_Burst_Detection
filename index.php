<html>
<head><title>Sound Burst Detection System</title>
<link rel="shortcut icon" type="image/x-icon" href="hydro-4.png" />

<!-- Styles -->	
<style style="text/css">
body {
  background-image: url("images/sound.jpg");
  background-position: 50% 50%;
  background-repeat: repeat;
}
</style>
<script type="javascript">
alert("you....")
</script>

</head>

<body bgcolor="#00abff">
<center>
<img src="images/sound2.jpg" alt="Test" WIDTH=500 HEIGHT=400></center>
<?php
//echo "Sound Burst Detection System";

?>

<center>
<font color="white">Sound Burst Detection System</font>
<form action="index.php" method="post">
UserName: <input type="text" name="username">
PassWord: <input type="text" name="password">
<input type="submit" value="Enter">
</form>
</center>



<?php
$credentials=false;
$once="0";
if($_POST['username']=="root" && $_POST['password']=="root")
{
 header("Location: homepage.php");//goes to homepage webpage
 $credentials=true;
 $once=1;
}
else if($_POST['username']!="Hydro" && $_POST['password']!="password" && $once=0)
{
 $once=1;
 $credentials=false;
}


if($credentials==false && $once==0)
{
  echo "INPUT CREDENTIALS";
  $once==1;
}
else if($credentials==false && $once==1)
{
  echo "INCORRECT CREDENTIALS";
}
?>



</body></html>