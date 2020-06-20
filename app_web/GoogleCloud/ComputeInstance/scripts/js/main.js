var differed_objs = [];


/* Wait until site is loaded */
$(window).on('load', function()
{
	// init_NotificationBtns();
	
	/*
	differed_objs = document.getElementsByClassName( 'differed-loading' );
	if( differed_objs.length > 0 ) init_ShowDifferedObejcts();
	*/
	
	if( _isMobile ) init_MobileOnlyElements();

	window.addEventListener( 'resize', window_OnResize, false );
});


/*
function init_NotificationBtns()
{
	notification_btns = document.getElementsByClassName('notification-request');

	for( var i = 0; i < notification_btns.length; ++i )
	{
		notification_btns[i].addEventListener( 'click', notification_RequestPermission );
	}
}

function notification_RequestPermission()
{
	Notification.requestPermission();
}*/



function window_OnResize()
{	
	// 
}



/********************************************************************************/
/* Global script to find the 'differed-loading' class which has display:none set
   and added display block as inline style. This supersedes the previous class and
   show the content that has that class*/ 
/********************************************************************************/
/*
function init_ShowDifferedObejcts()
{
	for( var i = 0; i < differed_objs.length; ++i )
	{
		var obj = differed_objs[i];
		$(obj).attr('style', 'display: block !important');
	}
}
*/
/********************************************************************************/
/**/
/********************************************************************************/




/********************************************************************************/
/* Show MOBILE ONLY elements */ 
/********************************************************************************/
/*
function init_MobileOnlyElements()
{
	mobile_only_dom = document.getElementsByClassName( 'mobile-only' );
	for( var i = 0; i < mobile_only_dom.length; ++i)
	{
		$(mobile_only_dom[i]).css({ display: 'block' });
	}
}
*/
/********************************************************************************/
/**/
/********************************************************************************/




/********************************************************************************/
/* index.html Transit sections submit on ENTER KEY */ 
/********************************************************************************/
/*
$("input").keypress( function(event) {
	if (event.which == 13) {
        event.preventDefault();
        // CODE
    }
});
*/