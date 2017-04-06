var mapObj;
var API = 'https://8lxxl2jvdd.execute-api.us-west-2.amazonaws.com/trackerapi/trackerpos'; // GATEWAy API URL
var API_KEY = 'aoHdwHI5SA1BL4USB79LJ8Z4fCFsKTdcaTLbW6QO'; // GATEWAY API KEY
var GOOGLE_API_KEY = 'AIzaSyDtY7XcAta9D76108tUv_JIOOfCFvvx-uQ'; // GOOGLE MAPS API key
var track = [];
var paths = [];


function initAWSAjax()
{
    $.ajax(API, {
        method: 'GET',
        data : { /*foo : 'bar', bar : 'foo'*/ },
        dataType: "json",
        contentType: 'application/json',
        headers:{
            'x-api-key': API_KEY,
            // "Access-Control-Allow-Origin" : "*",

        },
        success: processJSON
        // success: drawPaths
    });
       
}



function processJSON(data)
{
	/*
  console.log(data.longitude);
	console.log(data.latitude);
	console.log(data.timestamp);
  */
  
	initMap(data.longitude, data.latitude, data.timestamp);
}


function initMap(aws_latitude, aws_longitude, aws_timestamp) {

	var l = aws_longitude;
	var L = aws_latitude;

	mapObj = new google.maps.Map(document.getElementById('map'), {
		center: {lat: Number(l), lng: Number(L)},
		zoom: 12,
        disableDefaultUI: true,
        styles: mapStyle_Dark,
        // scrollwheel: false,
        zoomControl: true
	});


	infoBubble = new InfoBubble({
	    map: mapObj,
	    content: '<h1>HERE!</h1>',
	    position: new google.maps.LatLng( l, L ),
	    shadowStyle: 0,
	    padding: 0,
	    backgroundColor:'#f36c2b',
	    borderRadius: 0,
	    arrowSize: 15   ,
	    borderWidth: 0,
	    borderColor: '#f36c2b',
	    disableAutoPan: true,
	    hideCloseButton: true,
	    arrowPosition: 0,
	    backgroundClassName: 'info-bubble-details',
	    arrowStyle: 2
	});

	infoBubble.open();

}





var mapStyle_Dark = [
  {
    "elementType": "geometry",
    "stylers": [
      {
        "color": "#212121"
      }
    ]
  },
  {
    "elementType": "labels.icon",
    "stylers": [
      {
        "visibility": "off"
      }
    ]
  },
  {
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#757575"
      }
    ]
  },
  {
    "elementType": "labels.text.stroke",
    "stylers": [
      {
        "color": "#212121"
      }
    ]
  },
  {
    "featureType": "administrative",
    "elementType": "geometry",
    "stylers": [
      {
        "color": "#666666"
      }
    ]
  },
  {
    "featureType": "administrative.country",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#9e9e9e"
      }
    ]
  },
  {
    "featureType": "administrative.land_parcel",
    "stylers": [
      {
        // "visibility": "off"
      }
    ]
  },
  {
    "featureType": "administrative.locality",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#bdbdbd"
      }
    ]
  },
  {
    "featureType": "poi",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#757575"
      }
    ]
  },
  {
    "featureType": "poi.park",
    "elementType": "geometry",
    "stylers": [
      {
        "color": "#181818"
      }
    ]
  },
  {
    "featureType": "poi.park",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#616161"
      }
    ]
  },
  {
    "featureType": "poi.park",
    "elementType": "labels.text.stroke",
    "stylers": [
      {
        "color": "#1b1b1b"
      }
    ]
  },
  {
    "featureType": "road",
    "elementType": "geometry.fill",
    "stylers": [
      {
        "color": "#2c2c2c"
      }
    ]
  },
  {
    "featureType": "road",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#8a8a8a"
      }
    ]
  },
  {
    "featureType": "road.arterial",
    "elementType": "geometry",
    "stylers": [
      {
        "color": "#373737"
      }
    ]
  },
  {
    "featureType": "road.highway",
    "elementType": "geometry",
    "stylers": [
      {
        "color": "#3c3c3c"
      }
    ]
  },
  {
    "featureType": "road.highway.controlled_access",
    "elementType": "geometry",
    "stylers": [
      {
        "color": "#4e4e4e"
      }
    ]
  },
  {
    "featureType": "road.local",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#616161"
      }
    ]
  },
  {
    "featureType": "transit",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#757575"
      }
    ]
  },
  {
    "featureType": "water",
    "elementType": "geometry",
    "stylers": [
      {
        "color": "#000000"
      }
    ]
  },
  {
    "featureType": "water",
    "elementType": "labels.text.fill",
    "stylers": [
      {
        "color": "#3d3d3d"
      }
    ]
  }
];