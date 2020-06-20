
/*
	live-page
*/


var pageLoaded;
var _screenSize;
var _screenWidth = $(document).width();


$(window).on('load', function()
{
	/*pageLoaded =  document.getElementsByTagName('body')[0].classList[0];

	switch( pageLoaded )
	{
		case "home-page" :
			// initResize_HomePage();  // Load Home Page
			break;

		case "live-page" :
			initResize_LivePage(); // Load Live Page
			break;

		default :
			// initResize_Default();
	}*/

});




/*
function initResize_Default()
{
	this.window.addEventListener( 'resize', function()
	{
		medieQueries_Init();
	});	
}





function initResize_LivePage()
{
	var services_container = document.getElementsByClassName('services-container')[0];
	var services_content = document.getElementsByClassName('services-content')[0];
	var today_container = document.getElementsByClassName('today-container')[0];
	var today_content_container = document.getElementsByClassName('today-content-container')[0];
	var info_section_array = document.getElementsByClassName('info-section');


	medieQueries_Init();


	if( _screenSize == 'xl' || _screenSize == 'lg' || _screenSize == 'md')
	{
		$(services_content).css({ height: $(today_container).height() - 45 });

	} else {

		$(services_content).css({ height: 'auto' });

	}


	this.window.addEventListener( 'resize', function()
	{
		medieQueries_Init();

		$(services_content).find(".weather-icon")
		$(services_content).find(".parking-icon")

		if( _screenSize == 'xl' || _screenSize == 'lg' || _screenSize == 'md')
		{
			$(services_content).css({ height: $(today_container).height() - 45 });

		} else {

			$(services_content).css({ height: 'auto' });

		}
	});	
	
	// $(services_content).find(".weather-icon").css({height:'100%'});
	// $(services_content).find(".weather-icon").css({height:$(services_content).find(".weather-icon").height()});
}*/





function medieQueries_Init()
{

	_screenWidth = $(window).width();

	var mqls = [
			    window.matchMedia( 'screen and (max-width: 379px)' ),                         // xxs
			    window.matchMedia( 'screen and (min-width: 380px) and (max-width: 579px)' ),  // xs
			    window.matchMedia( 'screen and (min-width: 580px) and (max-width: 767px)' ),  // sm
			    window.matchMedia( 'screen and (min-width: 768px) and (max-width: 990px)' ),  // md
			    window.matchMedia( 'screen and (min-width: 991px) and (max-width: 1199px)' ), // lg
			    window.matchMedia( 'screen and (min-width: 1200px)' )                         // xl
				]


	for ( var i = 0; i < mqls.length; ++i ){
	    mediaqueryresponse( mqls[i] );
	}


	function mediaqueryresponse( mql ){
	    if ( mqls[0].matches ){
	    	_screenSize = 'xxs';
	    }
	    else if ( mqls[1].matches ){  
	    	_screenSize = 'xs';
	    }
	    else if ( mqls[2].matches ){
	    	_screenSize = 'sm';
	    }
	    else if ( mqls[3].matches ){ 
	    	_screenSize = 'md';
	    }
	    else if ( mqls[4].matches ){ 
	    	_screenSize = 'lg';
	    }
	    else if ( mqls[5].matches ){ 
	    	_screenSize = 'xl';
	    }
	    else{
	    	console.log( "WINDOW RESIZE | Media Queries | No size found")
	    }

	}

}