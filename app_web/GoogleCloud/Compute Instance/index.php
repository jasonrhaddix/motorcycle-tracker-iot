<!DOCTYPE html>
<html>
  <head>
    <title>Where's My Bike</title>
    <meta name="viewport" content="initial-scale=1.0">
    <meta charset="utf-8">
    <style>
      html, body {
        height: 100%;
        margin: 0;
        padding: 0;
      }
      #map {
        height: 100%;
      }
    </style>
    <script
  src="https://code.jquery.com/jquery-2.2.4.min.js"
  integrity="sha256-BbhdlvQf/xTY9gja0Dq3HiwQF8LaCRTXxZKRutelT44="
  crossorigin="anonymous"></script>
    <!-- <script src="scripts/php/create_one.php"></script> -->
    <!-- <script src="scripts/_libs/jquery/jquery.min.js"></script> -->
  </head>
  <body>
    <div id="map"></div>
    <script>
      var map;
      var API = 'https://8lxxl2jvdd.execute-api.us-west-2.amazonaws.com/trackerapi/tracker-pos'; // GATEWAy API URL
      var API_KEY = 'aoHdwHI5SA1BL4USB79LJ8Z4fCFsKTdcaTLbW6QO'; // GATEWAY API KEY
      var GOOGLE_API_KEY = 'AIzaSyDtY7XcAta9D76108tUv_JIOOfCFvvx-uQ'; // GOOGLE MAPS API key
      var track = [];
      var paths = [];

        function animateCircle(line) {
            var count = 0;
            window.setInterval(function() {
              count = (count + 1) % 200;

              var icons = line.get('icons');
              icons[0].offset = (count / 2) + '%';
              line.set('icons', icons);
          }, 500);
        }

        $.ajax(API, {
            method: 'GET',
            data: {
                p: 24*3600,
                t: 'G'
            },
            headers:{
                'x-api-key': API_KEY
            },
            success: drawPaths
        });

        function drawPaths(data, status, jqXHR) {
            var latlng;
            var ts = 0;
            var avglatlng = {lat:0,lng:0};
            var points = [];
            var pointCount = 0;

            var path;
            var color=0;
            var colorString;
            var lineSymbol;

            data.forEach(function(coord) {
                colorString = getColor(color);
                if (coord.t-ts>30*60*1000 && ts!==0) {
                    track.push(points);
                    lineSymbol = {
                        path: google.maps.SymbolPath.FORWARD_CLOSED_ARROW,
                        scale: 5,
                        strokeColor: colorString,
                        strokeWeight: 2,
                        strokeOpacity: 1.0,
                        fillColor: 'white',
                        fillOpacity: 1.0
                    };
                    path = new google.maps.Polyline({
                        path: points,
                        geodesic: true,
                        strokeColor: colorString,
                        strokeOpacity: 1.0,
                        strokeWeight: 4,
                        icons: [{
                          icon: lineSymbol,
                          offset: '100%'
                        }]
                      });
                    if (points.length>5) {
                        animateCircle(path);
                        path.setMap(map);
    //                    runSnapToRoad(path.getPath());
                        paths.push(path);
                        color++;
                    }
                    points = [];
                    // keep last point for continuous line
                    points.push(latlng);

                }
                ts = coord.t;
                latlng = {
                        lat: coord.l,
                        lng: coord.L
                };
                avglatlng.lat += coord.l;
                avglatlng.lng += coord.L;
                points.push(latlng);
                pointCount++;
                var marker = new google.maps.Marker({
                    position: latlng,
                    title: (new Date(coord.t)).toTimeString(),
                    map: map,
                    icon: {
                        path: google.maps.SymbolPath.CIRCLE,
                        scale: 2,
                        strokeWeight: 1,
                        strokeColor: 'white',
                        strokeOpacity: 1.0,
                        fillColor: 'black',
                        fillOpacity: 1.0,
                        zIndex:9
                    }
                  });
            });
            colorString = getColor(color);

            if (points.length>0) {
                track.push(points);

                lineSymbol = {
                    path: google.maps.SymbolPath.FORWARD_CLOSED_ARROW,
                    strokeColor: colorString,
                    scale: 5,
                    strokeWeight: 2,
                    strokeOpacity: 1.0,
                    fillColor: 'white',
                    fillOpacity: 1.0
                };

                path = new google.maps.Polyline({
                    path: points,
                    geodesic: true,
                    strokeColor: colorString,
                    strokeOpacity: 1.0,
                    strokeWeight: 4,
                    icons: [{
                      icon: lineSymbol,
                      offset: '100%'
                    }]
                  });
                animateCircle(path);
                path.setMap(map);
//                    runSnapToRoad(path.getPath());
                paths.push(path);
            }
            if (pointCount>0) {
                avglatlng.lat /= pointCount;
                avglatlng.lng /= pointCount;
                map.setCenter(avglatlng);
            }
            // last known points
            var marker = new google.maps.Marker({
                position: latlng,
                title: new Date(ts).toTimeString(),
                map: map,
                icon: {
                    path: google.maps.SymbolPath.CIRCLE,
                    scale: 5,
                    strokeColor: 'red',
                    zIndex:10
                }
              });
        }

        function getColor(color) {
            var initColor = 0xff1e00;
            return '#'+new String("000000"+(Math.random(0x1000000)).toString(16)).substr(-6,6);
        }
        
        function runSnapToRoad(path) {
          var pathValues = [];
          for (var i = 0; i < path.getLength(); i++) {
            pathValues.push(path.getAt(i).toUrlValue());

            if (pathValues.length>90) {
                $.get('https://roads.googleapis.com/v1/snapToRoads', {
                interpolate: true,
                key: GOOGLE_API_KEY,
                path: pathValues.join('|')
              }, function(data) {
                processSnapToRoadResponse(data);
                drawSnappedPolyline();
              });
              pathValues = [];
            }
          }
          if (pathValues.length>0) {
            $.get('https://roads.googleapis.com/v1/snapToRoads', {
                interpolate: true,
                key: GOOGLE_API_KEY,
                path: pathValues.join('|')
              }, function(data) {
                processSnapToRoadResponse(data);
                drawSnappedPolyline();
              });
          }
        }

        // Store snapped polyline returned by the snap-to-road method.
        function processSnapToRoadResponse(data) {
          snappedCoordinates = [];
          for (var i = 0; i < data.snappedPoints.length; i++) {
            var latlng = new google.maps.LatLng(
                data.snappedPoints[i].location.latitude,
                data.snappedPoints[i].location.longitude);
            snappedCoordinates.push(latlng);
          }
        }

        // Draws the snapped polyline (after processing snap-to-road response).
        function drawSnappedPolyline() {
          var snappedPolyline = new google.maps.Polyline({
            path: snappedCoordinates,
            strokeColor: 'yellow',
            strokeWeight: 6,
            strokeOpacity: 1.0
          });

          snappedPolyline.setMap(map);
        }

      function initMap() {
        map = new google.maps.Map(document.getElementById('map'), {
          center: {lat: 33.773016, lng: -118.149690},
          zoom: 13
        });
      }
    </script>
    <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyDtY7XcAta9D76108tUv_JIOOfCFvvx-uQ&callback=initMap"
    async defer></script>
  </body>
</html>