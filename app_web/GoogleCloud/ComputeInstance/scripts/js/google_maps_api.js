
//
const API_GATEWAY = 'https://8lxxl2jvdd.execute-api.us-west-2.amazonaws.com/trackerapi/trackerpos'
const API_GATEWAY_KEY = 'aoHdwHI5SA1BL4USB79LJ8Z4fCFsKTdcaTLbW6QO';
const GOOGLE_MAPS_API_KEY = 'AIzaSyDtY7XcAta9D76108tUv_JIOOfCFvvx-uQ';

//
var handlebars_HTML;
var handlebars_Template;


var trackerData_Status = 'REST';
var trackerData_Timestamp = '12/03/17 | 5:45pm';





$(window).on('load', function()
{   
    show_TrackerLoader($('#tracker-loader-container'));
});





function initTrackerDB()
{
    $.ajax(API_GATEWAY, {
        method: 'GET',
        data : { /*foo : 'bar', bar : 'foo'*/ },
        dataType: "json",
        contentType: 'application/json',
        headers:{
            'x-api-key': API_GATEWAY_KEY,
            // "Access-Control-Allow-Origin" : "*",
        },

        success: process_TrackerInfo
    });
}





function process_TrackerInfo(data)
{
    handlebars_Template = Handlebars.getTemplate('tracker_info');
    handlebars_HTML = handlebars_Template({ status : trackerData_Status, longitude : data.longitude, latitude : data.latitude, timestamp : trackerData_Timestamp });

    initMap( data.longitude, data.latitude, data.timestamp );
}





function initMap(aws_latitude, aws_longitude, aws_timestamp)
{
    setTimeout( function()
    {
        hide_TrackerLoader($('#tracker-loader-container'));
    }, 1200)


	var l = aws_longitude;
	var L = aws_latitude;

	mapObj = new google.maps.Map(document.getElementById( 'map' ), {
		center: {lat: Number(l), lng: Number(L)},
        zoom: 12,
        disableDefaultUI: true,
        // styles: mapStyle_Dark,
        // scrollwheel: false,
        zoomControl: true
    });


    var markerIcon = {
        url: '../../site_assets/images/icons/maps/map-pin.svg',
        scaledSize: new google.maps.Size(40, 40), // scaled size
        origin: new google.maps.Point(0,0), // origin
        // anchor: new google.maps.Point(50, 50) // anchor
    };


    var trackerMarker = new google.maps.Marker({
        position: {lat: Number(l), lng: Number(L)},
        map: mapObj,
        icon: markerIcon,
      });


	infoBubble = new InfoBubble({
        map: mapObj,
        content: handlebars_HTML,
        position: new google.maps.LatLng( l, L ),
        shadowStyle: 0,
        padding: 0,
        backgroundColor:'#f36c2b',
        borderRadius: 0,
        arrowSize: 0,
        borderWidth: 0,
        borderColor: '#f36c2b',
        disableAutoPan: true,
        offset: {top: -45, left: -110},
        // hideCloseButton: true,
        arrowPosition: 0,
        backgroundClassName: 'info-bubble-details',
        // pixelOffset: new google.maps.Size(-130, -120),
        arrowStyle: 2
	});

	//infoBubble.open();

    trackerMarker.addListener('click', function()
    {
        // mapObj.setZoom(19);
        // mapObj.setCenter(trackerMarker.getPosition());
        // mapObj.setOptions(mapOptions);
        // mapObj.setTilt(45);
        mapObj.setMapTypeId('satellite');
        infoBubble.open();
      });

    var mapOptions = 
    {
      styles: mapStyle_Dark
    }

}





function show_TrackerLoader(obj)
{
    $(obj).css({display: 'block'});
    TweenMax.to( obj, 0.5, { opacity:1, ease:Power4.easeOut});
}


function hide_TrackerLoader(obj)
{
    TweenMax.to( obj, 0.5, { opacity:0, ease:Power4.easeOut, onComplete: function()
    {
        $(obj).css({display: 'none'});
        
    }});
}




        
Handlebars.getTemplate = function(name) {
    if (Handlebars.templates === undefined || Handlebars.templates[name] === undefined) {
        $.ajax({
            url : 'scripts/js/templates/handlebars/' + name + '.handlebars',
            success : function(data) {
                if (Handlebars.templates === undefined) {
                    Handlebars.templates = {};
                }
                Handlebars.templates[name] = Handlebars.compile(data);
            },
            async : false
        });
    }
    return Handlebars.templates[name];
};





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
        // "visibility": "off"
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
