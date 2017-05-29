const API_GATEWAY = 'https://8lxxl2jvdd.execute-api.us-west-2.amazonaws.com/trackerapi/trackerpos'
const API_GATEWAY_KEY = 'aoHdwHI5SA1BL4USB79LJ8Z4fCFsKTdcaTLbW6QO';
const GOOGLE_MAPS_API_KEY = 'AIzaSyDtY7XcAta9D76108tUv_JIOOfCFvvx-uQ';

var map_Obj;
var map_Marker_Icon;
var map_Tracker_Marker;

var trackerPos_Lat;
var trackerPos_Long;

//
var infoBubble_State = false;

//
var handlebars_HTML;
var handlebars_Template;

// 
var trackerData_Status = '-- REST --';
var trackerData_Timestamp = '04/03/17 | 5:45pm';
var trackerData_BatteryLevel = '90%';
var trackerData_CellGain = '-120 dBm';





$(window).on('load', function()
{   
    show_TrackerLoader($('#tracker-loader-container'));
});




function initTrackerDB()
{
    $.ajax(API_GATEWAY,
        {
            method: 'GET',
            data : { /*foo : 'bar', bar : 'foo'*/ },
            dataType: "json",
            contentType: 'application/json',
            headers:{
                'x-api-key': API_GATEWAY_KEY,
        },

        success: process_TrackerInfo
    });
}






function process_TrackerInfo(data)
{
    console.log( data );
    
    handlebars_Template = Handlebars.getTemplate('tracker_info');
    handlebars_HTML = handlebars_Template({ 
                                            status : trackerData_Status, 
                                            longitude : data.longitude, 
                                            latitude : data.latitude, 
                                            battery_level : trackerData_BatteryLevel, 
                                            cell_gain : trackerData_CellGain, 
                                            timestamp : trackerData_Timestamp
                                          });

    initMap( data.longitude, data.latitude );
}




function initMap(aws_latitude, aws_longitude)
{
    setTimeout( function() { hide_TrackerLoader($('#tracker-loader-container')) }, 1200);

	trackerPos_Lat = aws_longitude;
	trackerPos_Long = aws_latitude;

	map_Obj = new google.maps.Map(document.getElementById( 'map' ), {
		center: {lat: Number(aws_longitude), lng: Number(aws_latitude)},
        zoom: 12,
        disableDefaultUI: true,
        styles: mapStyle_Monochrome_Dark,
        // scrollwheel: false,
        zoomControl: true
    });


    map_Marker_Icon = {
        url: '../../site_assets/images/icons/maps/map-pin.svg',
        scaledSize: new google.maps.Size(40, 40),
        origin: new google.maps.Point(0,0),
        // anchor: new google.maps.Point(50, 50) // anchor
    };


    var map_Tracker_Marker = new google.maps.Marker({
        position: {lat: Number(aws_longitude), lng: Number(aws_latitude)},
        map: map_Obj,
        icon: map_Marker_Icon,
      });


	infoBubble = new InfoBubble({
        map: map_Obj,
        content: handlebars_HTML,
        position: new google.maps.LatLng( aws_longitude, aws_latitude ),
        shadowStyle: 0,
        padding: 0,
        backgroundColor:'#f36c2b',
        borderRadius: 0,
        arrowSize: 15,
        borderWidth: 0,
        borderColor: '#f36c2b',
        disableAutoPan: true,
        offset: {top: -45, left: 0},
        hideCloseButton: true,
        arrowPosition: 50,
        backgroundClassName: 'info-bubble-details',
        arrowStyle: 0
	});

	//infoBubble.open();

    map_Tracker_Marker.addListener('click', infoBubble_ToggleShow );


    var mapOptions = {
                        styles: mapStyle_Monochrome_Dark
                      }

}




/*
map_Tracker_Marker.addListener('click', function()
{
    // mapObj.setZoom(19);
    // mapObj.setCenter(map_Tracker_Marker.getPosition());
    // mapObj.setOptions(mapOptions);
    // mapObj.setTilt(45);
    // mapObj.setMapTypeId('satellite');
    infoBubble.open();
});
*/






function infoBubble_ToggleShow()
{
    if( !infoBubble_State )
    {
        infoBubble.open();
        map_Obj.setCenter({ lat: trackerPos_Lat, lng: trackerPos_Long });

    } else {
        infoBubble.close();        

    }

    infoBubble_State = !infoBubble_State;
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










var mapStyle_Default = [];


var mapStyle_Monochrome_Dark = [
									{ "elementType": "geometry", "stylers": [{ "color": "#212121" }]},
									{ "elementType": "labels.icon", "stylers": [{ /*"visibility": "off"*/ }]},
									{ "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
									{ "elementType": "labels.text.stroke", "stylers": [{ "color": "#212121" }]},
									{ "featureType": "administrative", "elementType": "geometry", "stylers": [{ "color": "#666666" }]},
									{ "featureType": "administrative.country", "elementType": "labels.text.fill", "stylers": [{ "color": "#9e9e9e" }]},
									{ "featureType": "administrative.land_parcel", "stylers": [{ /*"visibility": "off"*/ }]},
									{ "featureType": "administrative.locality", "elementType": "labels.text.fill", "stylers": [{ "color": "#bdbdbd" }]},
									{ "featureType": "poi", "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
									{ "featureType": "poi.park", "elementType": "geometry", "stylers": [{ "color": "#181818" }]},
									{ "featureType": "poi.park", "elementType": "labels.text.fill", "stylers": [{ "color": "#616161" }]},
									{ "featureType": "poi.park", "elementType": "labels.text.stroke", "stylers": [{ "color": "#1b1b1b" }]},
									{ "featureType": "road", "elementType": "geometry.fill", "stylers": [{ "color": "#2c2c2c" }]},
									{ "featureType": "road", "elementType": "labels.text.fill", "stylers": [{ "color": "#8a8a8a" }]},
									{ "featureType": "road.arterial", "elementType": "geometry", "stylers": [{ "color": "#373737" }]},
									{ "featureType": "road.highway",  "elementType": "geometry", "stylers": [{ "color": "#3c3c3c" }]},
									{ "featureType": "road.highway.controlled_access", "elementType": "geometry", "stylers": [{ "color": "#4e4e4e" }]},
									{ "featureType": "road.local", "elementType": "labels.text.fill", "stylers": [{ "color": "#616161" }]},
									{ "featureType": "transit", "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
									{ "featureType": "water", "elementType": "geometry", "stylers": [{ "color": "#000000" }]},
									{ "featureType": "water", "elementType": "labels.text.fill", "stylers": [{ "color": "#3d3d3d" }]}
								];

var mapStyle_Monochrome_Light = [
									{ "elementType": "geometry", "stylers": [{ "color": "#f5f5f5" }]},
									{ "elementType": "labels.icon", "stylers": [{ /*"visibility": "off"*/ }]},
									{ "elementType": "labels.text.fill", "stylers": [{ "color": "#616161" }]},
									{ "elementType": "labels.text.stroke", "stylers": [{ "color": "#f5f5f5" }]},
									{ "featureType": "administrative.land_parcel", "elementType": "labels.text.fill", "stylers": [{ "color": "#bdbdbd" }]},
									{ "featureType": "poi", "elementType": "geometry", "stylers": [{ "color": "#eeeeee" }]},
									{ "featureType": "poi", "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
									{ "featureType": "poi.park", "elementType": "geometry", "stylers": [{ "color": "#e5e5e5" }]},
									{ "featureType": "poi.park", "elementType": "labels.text.fill", "stylers": [{ "color": "#9e9e9e" }]},
									{ "featureType": "road", "elementType": "geometry", "stylers": [{ "color": "#ffffff" }]},
									{ "featureType": "road.arterial", "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
									{ "featureType": "road.highway", "elementType": "geometry", "stylers": [{ "color": "#dadada" }]},
									{ "featureType": "road.highway", "elementType": "labels.text.fill", "stylers": [{ "color": "#616161" }]},
									{ "featureType": "road.local", "elementType": "labels.text.fill", "stylers": [{ "color": "#9e9e9e" }]},
									{ "featureType": "transit.line", "elementType": "geometry", "stylers": [{ "color": "#e5e5e5" }]},
									{ "featureType": "transit.station", "elementType": "geometry", "stylers": [{ "color": "#eeeeee" }]},
									{ "featureType": "water", "elementType": "geometry", "stylers": [{ "color": "#c9c9c9" }]},
									{ "featureType": "water", "elementType": "labels.text.fill", "stylers": [{ "color": "#9e9e9e" }]}
								]


var mapStyle_KTM = [
						{ "elementType": "geometry", "stylers": [{ "color": "#212121" }]},
						{ "elementType": "labels.icon", "stylers": [{ /*"visibility": "off"*/ }]},
						{ "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
						{ "elementType": "labels.text.stroke", "stylers": [{ "color": "#212121" }]},
						{ "featureType": "administrative", "elementType": "geometry", "stylers": [{ "color": "#757575" }]},
						{ "featureType": "administrative.country", "elementType": "labels.text.fill", "stylers": [{ "color": "#9e9e9e" }]},
						{ "featureType": "administrative.land_parcel", "stylers": [{ /*"visibility": "off"*/ }]},
						{ "featureType": "administrative.locality", "elementType": "labels.text.fill", "stylers": [{ "color": "#bdbdbd" }]},
						{ "featureType": "poi", "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
						{ "featureType": "poi.park", "elementType": "geometry", "stylers": [{ "color": "#181818" }]},
						{ "featureType": "poi.park", "elementType": "labels.text.fill", "stylers": [{ "color": "#616161" }]},
						{ "featureType": "poi.park", "elementType": "labels.text.stroke", "stylers": [{ "color": "#1b1b1b" }]},
						{ "featureType": "road", "elementType": "labels.text.fill", "stylers": [{ "color": "#f36c2b" }]},
						{ "featureType": "road.arterial", "elementType": "geometry", "stylers": [{ "color": "#373737" }]},
						{ "featureType": "road.arterial", "elementType": "geometry.fill", "stylers": [{ "color": "#784307" }]},
						{ "featureType": "road.highway", "elementType": "geometry", "stylers": [{ "color": "#3c3c3c" }]},
						{ "featureType": "road.highway", "elementType": "geometry.fill", "stylers": [{ "color": "#f36c2b" }]},
						{ "featureType": "road.highway", "elementType": "geometry.stroke", "stylers": [{ "color": "#f36c2b" }]},
						{ "featureType": "road.highway.controlled_access", "elementType": "geometry", "stylers": [{ "color": "#4e4e4e" }]},
						{ "featureType": "road.highway.controlled_access", "elementType": "geometry.fill", "stylers": [{ "color": "#f36c2b" }]},
						{ "featureType": "road.highway.controlled_access", "elementType": "geometry.stroke", "stylers": [{ "color": "#f36c2b" }]},
						{ "featureType": "road.local", "elementType": "labels.text.fill", "stylers": [{ "color": "#616161" }]},
						{ "featureType": "transit", "elementType": "labels.text.fill", "stylers": [{ "color": "#757575" }]},
						{ "featureType": "water", "elementType": "geometry", "stylers": [{ "color": "#000000" }]},
						{ "featureType": "water", "elementType": "labels.text.fill", "stylers": [{ "color": "#3d3d3d" }]}
					]