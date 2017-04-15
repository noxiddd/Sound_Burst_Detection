<!DOCTYPE html>
<html>
  <head>
    <style>
      #map {
        height: 800px;
        width: 100%;
       }
    </style>
  </head>
  <body>

  <div class=buttons>
<br><button id="back" class="float-left submit-button" >BACK</button></br>
</div>
    <!--PHP Section-->
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
$sql = 'SELECT Dist_Mod_01,Dist_Mod_02,Dist_Mod_03 FROM Gunshots_Recorded';
$result =$conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
   
    while($row = $result->fetch_assoc()) 
    {
        echo "<script> var dist01 = ". json_encode($row["Dist_Mod_01"])."</script>";
        echo "<script> var dist02 = ". json_encode($row["Dist_Mod_02"])."</script>";
        echo "<script> var dist03 = ". json_encode($row["Dist_Mod_03"])."</script>";
    }
   
} else {
    echo "0 results";
}   
?>














     <!--PHP Section-->

    <h3>Gunshot Location</h3>
    <div id="map"></div>
    <script>
      function initMap() {
        var lati=18.0047367;
        var longi=-76.7491686;
        var uluru = {lat: lati, lng: longi};
        var uluru2 = {lat: 17.0047367, lng: -76.7491686};
        var latShift=-0.0004;
        var lonShift=-0.0004;
        var x11,x22,x33,x; //x values 
        var y11,y22,y33,y;//y values
        var r11,r22,r33;//distance of source from each module//subscript
        var A,B,C,D,E,F;//temp variables
        var x00,y00;
        var x,y;
        var boundNorthLat=18.0054+latShift;
        var boundSouthLat=18.0043000+latShift;
        var boundWestLon=-76.7495+lonShift;
        var boundEastLon=-76.7480+lonShift;

        
        x11=longi-0.0003;
        y11=boundNorthLat;
        x22=boundWestLon;
        y22=boundSouthLat;
        x33=boundEastLon;
        y33=boundSouthLat;
          
        x00=longi-0.0003;
        y00=18.0054+latShift-0.0005;  
          
       
        r11=dist01/10000;//Math.sqrt( (x00-x11)*(x00-x11) + (y00-y11)*(y00-y11)  );
	      r22=dist02/10000;//Math.sqrt( (x00-x22)*(x00-x22) + (y00-y22)*(y00-y22)  );
      	r33=dist03/10000;//Math.sqrt( (x00-x33)*(x00-x33) + (y00-y33)*(y00-y33)  );
       
       console.log("r11: ");
       console.log(r11);  
       console.log("r22: ");
       console.log(r22);  
       console.log("r33: ");
       console.log(r33);  
      
       A=(-2.0*x11)+(2.0*x22);
	     B=(-2.0*y11)+(2.0*y22);
	     C=(r11*r11)-(r22*r22)-(x11*x11)+(x22*x22)-(y11*y11)+(y22*y22);
	     D=(-2.0*x22)+(2.0*x33);
	     E=(-2.0*y22)+(2.0*y33);
	     F=(r22*r22)-(r33*r33)-(x22*x22)+(x33*x33)-(y22*y22)+(y33*y33);

       /* var UpperLeft  = {lat: 18.00, lng: -76.0};
        var UpperRight  = {lat: 18.00, lng: -77};
        var LowerLeft = {lat: 17.00, lng: -76.00};
        var LowerRight = {lat: 17.00, lng: -77.00};*/
        y=(C*D-F*A)/(B*D-E*A);
      	x=(C*E-F*B)/(A*E-D*B);

        console.log("x: ");
       console.log(x);
       console.log("y: ");
       console.log(y);    

        var map = new google.maps.Map(document.getElementById('map'), {
          zoom: 20,
          center: uluru,
          mapTypeId: google.maps.MapTypeId.HYBRID
        });

        var places = [
       {
         lat: y11,
         lng: x11,
         image: 'M01',
         title: 'M_01',
         item: 'Module 01',
         animation: google.maps.Animation.DROP

       },
       {
         lat: y22,
         lng: x22,
         image: 'M02',
         title: 'M_02',
         item: 'Module 02',
         animation: google.maps.Animation.DROP
        },
          {
         lat: y33,
         lng: x33,
         image: 'M03',
         title: 'M_03',
         item: 'Module 03',
         animation: google.maps.Animation.DROP
        },
            {
         lat:y,
         lng: x,
         image: 'Gunshot',
         title: 'GunshotSource',
         item: 'Gun',
         animation: google.maps.Animation.BOUNCE
        }
       ];


        
      for( var i = 0;  i < places.length;  i++ ) {
            addPlace( places[i] );
          }


        /*var marker = new google.maps.Marker({
          position: uluru,uluru2,
          map: map,
          animation: google.maps.Animation.BOUNCE,
          title: 'Hello World!'
         // mapTypeId:google.maps.MapTypeId.SATELLITE
        });*/


        
        var rectangle = new google.maps.Rectangle({
          strokeColor: '#FF0000',
          strokeOpacity: 0.8,
          strokeWeight: 2,
          fillColor: '#FF00FF',
          fillOpacity: 0.35,
          map: map,
          bounds: {
            north: boundNorthLat,
            south: boundSouthLat,
            east: boundEastLon,
            west: boundWestLon
          }
        });


     function addPlace( place ) {
        var content = '<strong>Marker: ' + place.item + '</strong>';
        var infowindow = new google.maps.InfoWindow({
            content: content
        });

        var image = '/js/markers/' + place.image + '.png';
        var latlng = new google.maps.LatLng( place.lat, place.lng );

        var marker = new google.maps.Marker({
            position: latlng,
            title: place.title,
            animation: place.animation
        });

        marker.setMap( map );

        google.maps.event.addListener( marker, 'click', function() {
            infowindow.open( map, marker );
        });
    }  

      }
      
    </script>
    <script async defer
    src="https://maps.googleapis.com/maps/api/js?key=AIzaSyArMr4qeHSwAx8nCmnN-AGfRik0Z8yVDSI&callback=initMap">
    </script>
  </body>
<script type="text/javascript">
    document.getElementById("back").onclick = function () {
        location.href = "homepage.php";
    };  
</script>




</html>
