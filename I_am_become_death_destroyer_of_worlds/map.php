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
    <h3>Gunshot Location</h3>
    <div id="map"></div>
    <script>
      function initMap() {
        var uluru = {lat: 18.0047367, lng: -76.7491686};
        var map = new google.maps.Map(document.getElementById('map'), {
          zoom: 21,
          center: uluru,
          mapTypeId: google.maps.MapTypeId.HYBRID
        });
        var marker = new google.maps.Marker({
          position: uluru,
          map: map
         // mapTypeId:google.maps.MapTypeId.SATELLITE
        });
        

      }
    </script>
    <script async defer
    src="https://maps.googleapis.com/maps/api/js?key=AIzaSyArMr4qeHSwAx8nCmnN-AGfRik0Z8yVDSI&callback=initMap">
    </script>
  </body>
</html>