
// GLOBAL VARS
// var userIsMobile;

var nav_OpenState = false;
var header_nav;

/*// SOURCES
var main_nav;
var sub_nav;
var navBtns;
var imgUL;

var mobile_hamburger;
var mobile_nav;*/


init_Nav(); // set as async


function init_Nav()
{
	defineSources_Nav();
	addEventListeners_Nav();
}


function defineSources_Nav()
{
	header_nav = $( '.header-nav' )[0];
	header = $( 'header' )[0];
	header_btns = $( '.header-btn-container' )[0];
	// modal_close = $( '.modal-wrapper' )[0];

}



function addEventListeners_Nav()
{
	header_nav.addEventListener( 'click', nav_OpenClose );
	// modal_close.addEventListener( 'click', nav_OpenClose );

}


function nav_OpenClose(e)
{
	if(!nav_OpenState){
		$(header_nav).find( '#nav-hamburger' )[0].classList.add( 'openNav' );
		header.classList.add('openNav_Header');
		$(header_btns).css({display: 'none'});
	} else {
		$(header_nav).find( '#nav-hamburger' )[0].classList.remove( 'openNav' );
		header.classList.remove('openNav_Header');
		$(header_btns).css({display: 'block'});
	} 

	nav_OpenState = !nav_OpenState;
}



$('#nav-overlay').on('hidden.bs.modal', function () {
	$(header_nav).find( '#nav-hamburger' )[0].classList.remove( 'openNav' );
	header.classList.remove('openNav_Header');
	$(header_btns).css({display: 'block'});

	nav_OpenState = false;
})

