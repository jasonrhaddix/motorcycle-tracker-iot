/********************************************************************************/
/* This script initiates SlickJS sliders on current page, parses copy in each 
   slide, and finds the target where the copy will show */ 
/********************************************************************************/

var carouselsArray = [];
var carouselsElementsArray = [];
var carouselCopyArray = [];
var carouselId = 0;


$( window ).on('load', function()
{
	carouselsElementsArray = document.getElementsByClassName( 'slick-carousel-source' );
	if( carouselsElementsArray.length > 0 ) init_SlickJS_Carousels();
})


function init_SlickJS_Carousels()
{
	$(".slick-carousel-source").on('init', compileCarouselDivs);

	$('.slick-carousel-source').slick({
		autoplay: true,
		infinite: true,
		arrows: true,
		autoplaySpeed: 5000,
		// nextArrow: $('.slick-carousel-source').closest('.row').find('.next-arrow'),
		// prevArrow: $('.slick-carousel-source').closest('.row').find('.prev-arrow')
		// dots: true,
		// appendDots:'.dot-container',
		/*customPaging : function(slider, i) {
	        var thumb = $(slider.$slides[i]).data('thumb');
	        return '<a><img src="'+thumb+'"></a>';
    	},*/
	});
	
}


function compileCarouselDivs( event )
{
	var carousel = event.currentTarget;
	carousel.setAttribute( "id", ["slick-carousel-"] + carouselId);

	var slick_slides = carousel.getElementsByClassName('slick-slide');
	var slick_copy_container = $(carousel).parent().find('.slick-carousel-copy').find('p');
	var slidesArray = [];
	
	for( var i = 0; i < slick_slides.length; ++i)
	{
		if( i > 0 && i < slick_slides.length-1 )
		{
			slick_slides[i].setAttribute("data-copy", slick_slides[i].innerHTML);
			slidesArray.push(  slick_slides[i] );
			carouselCopyArray.push( slick_slides[i].innerHTML );
		}

		slick_slides[i].innerHTML = "";
	}
	
	slick_copy_container[0].innerHTML = slick_slides[1].getAttribute( "data-copy" );

	var tempArray = [];
	tempArray["id"] = carouselId;
	tempArray["carousel"] = carousel;
	tempArray["slidesArray"] = slidesArray;
	tempArray["copyContainer"] = slick_copy_container;
	tempArray["copyList"] = carouselCopyArray;
	carouselsArray.push( tempArray );

	++carouselId;

}


$('.slick-carousel-source').on('beforeChange', function( event, slick, currentSlide, nextSlide )
{	
	var target_carousel = event.currentTarget.id;
	target_carousel = parseInt( target_carousel.split("-")[2]);

	var slick_carousel = carouselsArray[target_carousel].carousel;
	var copy_container = carouselsArray[target_carousel].copyContainer;
	var slideContainer = carouselsArray[target_carousel].slidesArray[nextSlide];

	copy_container[0].innerHTML = slideContainer.getAttribute( "data-copy" );

});
