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
       /* var UpperLeft  = {lat: 18.00, lng: -76.0};
        var UpperRight  = {lat: 18.00, lng: -77};
        var LowerLeft = {lat: 17.00, lng: -76.00};
        var LowerRight = {lat: 17.00, lng: -77.00};*/

        var map = new google.maps.Map(document.getElementById('map'), {
          zoom: 20,
          center: uluru,
          mapTypeId: google.maps.MapTypeId.HYBRID
        });

        var places = [
       {
         lat: lati+0.0003,
         lng: longi+0.0003,
         image: 'marker1',
         title: '0',
         club: 'Fort Wayne Time Corners'
       },
       {
         lat: lati+0.0006,
         lng: longi+0.0006,
         image: 'marker2',
         title: '1',
         club: 'Roanoke'
        },
          {
         lat: lati+0.0009,
         lng: longi+0.0009,
         image: 'marker3',
         title: '1',
         club: 'Roanokes'
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
            north: 18.0054+latShift,
            south: 18.0043000+latShift,
            east: -76.7481692+lonShift,
            west: -76.7496+lonShift
          }
        });
     function addPlace( place ) {
        var content = '<strong>Club name: ' + place.club + '</strong>';
        var infowindow = new google.maps.InfoWindow({
            content: content
        });

        var image = '/js/markers/' + place.image + '.png';
        var latlng = new google.maps.LatLng( place.lat, place.lng );
        var marker = new google.maps.Marker({
            position: latlng,
            title: place.title,
        
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
