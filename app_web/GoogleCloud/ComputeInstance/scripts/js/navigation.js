
// GLOBAL VARS
// var userIsMobile;

var mobileNav_OpenState = false;


// SOURCES
var main_nav;
var sub_nav;
var navBtns;
var imgUL;

var mobile_hamburger;
var mobile_nav;


init_Nav(); // set as async

// $( window ).on( 'load', function()
// {
// 	init_Nav();
// });



function init_Nav()
{
	defineSources_Nav();
	addEventListeners_Nav();
}



function defineSources_Nav()
{
	main_nav = document.getElementById( 'header-main' );
	sub_nav = document.getElementById( 'home-sub-nav' );
	sub_nav_ul = $(sub_nav).find('.img-sub-nav > ul');

	mobile_hamburger = document.getElementById( 'nav-hamburger' );
	mobile_nav = document.getElementById( 'mobile-nav-overlay-container' );
}



function addEventListeners_Nav()
{
	mobile_hamburger.addEventListener( 'click', mobileNav_Open );
	
	if( !_isMobile )
	{
		sub_nav.addEventListener( 'mouseover',navOverlay_Open );
		sub_nav.addEventListener( 'mouseout',navOverlay_Close );
		
		navBtns = $('.nav-main a').toArray();
		imgUL = $(sub_nav).find('.img-sub-nav > ul').toArray();

		for( var i = 0; i < navBtns.length; ++i ) 
		{
			var element = navBtns[i];

			element.dom = imgUL[i];
	    	element.addEventListener( 'mouseover', navOverlay_Open );
	    	element.addEventListener( 'mouseover', navOverlay_ShowImages );
			element.addEventListener( 'mouseout', navOverlay_Close );
		};
	}
}



/********************************************************************************/
/* Mobile Nav */
/********************************************************************************/
function navOverlay_Open( event )
{
	event.preventDefault();
	$(sub_nav).addClass('showSubNav');
}


function navOverlay_Close( event )
{
	event.preventDefault();
	$(sub_nav).removeClass('showSubNav');
}


function navOverlay_ShowImages( event )
{
	event.preventDefault();
	// console.log( event.target.dom );

	for( var i = 0; i < navBtns.length; ++i )
	{
		// navBtns[i].dom.style.opacity = 0;
		navBtns[i].dom.style.display = 'none';
	}

	event.target.dom.style.display = 'block';
	// event.target.dom.opacity = 1;

}



/********************************************************************************/
/* Mobile Nav */
/********************************************************************************/
function mobileNav_Open(e)
{
	if(!mobileNav_OpenState){
		mobile_nav.classList.add( 'showMobileNav' );
		mobile_hamburger.classList.add( 'openNav' );
	} else {
		mobile_nav.classList.remove( 'showMobileNav' );
		mobile_hamburger.classList.remove( 'openNav' );
	} 

	mobileNav_OpenState = !mobileNav_OpenState;

}
