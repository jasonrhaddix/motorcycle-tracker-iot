/********************************************************************************/
/* MODERNIZR CHECKS */
/********************************************************************************/

$( window ).on('load', function()
{	
	// modernizr_geo_objs = document.getElementsByClassName( 'modernizr-geolocation' );
	// if( Modernizr.geolocation && modernizr_geo_objs.length > 0 ) init_GeoObjs();

});



/* Shows DOM elements with class '.modernizr-geolocation' and unhides them.
   DOM elements hidden with CSS (display:none) */
function init_GeoObjs()
{
	for( var i = 0; i < modernizr_geo_objs.length; ++i ) modernizr_geo_objs[i].style.display = 'block';

}
