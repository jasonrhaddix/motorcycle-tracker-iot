<!DOCTYPE html>
<html class="no-js" lang="en">
    <head>
        <meta charset="UTF-8" >
        <meta http-equiv="x-ua-compatible" content="ie=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, target-densityDpi=device-dpi, user-scalable=no">

        <!-- // Default Meta -->
        <meta property="description" content="">
        <meta property="keywords" content="">

        <!-- // Facebook, LinkedIn og Meta -->
        <meta property="og:title" content="" >
        <meta property="og:url" content="" >
        <meta property="og:description" content="">
        <!-- <meta property="og:image" content="site_assets/images/icons/app/img.jpg" > -->
        <meta property="og:type" content="website" >

        <title>KTM Tracker</title>
        
        <!-- Web App Manifest -->
        <!-- <link rel="manifest" href="manifest.json"> -->

        <!-- // APPLE TOUCH ICONS -->
        <!-- <link rel="apple-touch-icon" sizes="57x57" href="site_assets/images/icons/app/apple-icon-57x57.png">
        <link rel="apple-touch-icon" sizes="60x60" href="site_assets/images/icons/app/apple-icon-60x60.png">
        <link rel="apple-touch-icon" sizes="72x72" href="site_assets/images/icons/app/apple-icon-72x72.png">
        <link rel="apple-touch-icon" sizes="76x76" href="site_assets/images/icons/app/apple-icon-76x76.png">
        <link rel="apple-touch-icon" sizes="114x114" href="site_assets/images/icons/app/apple-icon-114x114.png">
        <link rel="apple-touch-icon" sizes="120x120" href="site_assets/images/icons/app/apple-icon-120x120.png">
        <link rel="apple-touch-icon" sizes="144x144" href="site_assets/images/icons/app/apple-icon-144x144.png">
        <link rel="apple-touch-icon" sizes="152x152" href="site_assets/images/icons/app/apple-icon-152x152.png">
        <link rel="apple-touch-icon" sizes="180x180" href="site_assets/images/icons/app/apple-icon-180x180.png"> -->

        <!-- // DEFAULT ICONS -->
        <!-- <link rel="icon" type="image/png" sizes="192x192"  href="site_assets/images/icons/app/android-icon-192x192.png">
        <link rel="icon" type="image/png" sizes="96x96" href="site_assets/images/icons/app/favicon-96x96.png">
        <link rel="icon" type="image/png" sizes="32x32" href="site_assets/images/icons/app/favicon-32x32.png">
        <link rel="icon" type="image/png" sizes="16x16" href="site_assets/images/icons/app/favicon-16x16.png"> -->
        
        <!-- // MS TILE ICON -->
        <!-- <meta name="msapplication-TileImage" content="site_assets/images/icons/app/ms-icon-144x144.png"> -->
        
        
        <!-- App Theme Color -->
        <meta name="msapplication-TileColor" content="#151515">
        <meta name="theme-color" content="#151515">
        <meta name="apple-mobile-web-app-status-bar-style" content="#151515">

        <script src="https://use.fontawesome.com/3ebcca6518.js"></script>

        <!-- Style Sheets -->
        <!-- <link rel="stylesheet" href="scripts/_libs/photo_sphere_viewer/photo-sphere-viewer.min.css">
        <link rel="stylesheet" href="scripts/_libs/slickjs/slick.css">
        <link rel="stylesheet" href="scripts/_libs/slickjs/slick-theme.css">
        <link rel="stylesheet" href="scripts/_libs/videojs/video-js.min.css"> -->
        <link rel="stylesheet" href="scripts/css_-noedit-/compressed/main.css">

        <!-- 
            ADD GOOGLE ANALYTICS HERE
         -->
        
    </head>
    <body class="page-home">
        <div class="modal fade nav-modal" id="nav-overlay" tabindex="-1" role="dialog" aria-labelledby="purchaseLabel" aria-hidden="true">
            <a class="modalCloseImg search-les-close-btn" href="#nav-overlay" data-dismiss="modal" title="Close"></a>
            <div class="vert-align-container">
                <div class="horz-align-container">
                    <div class="nav-container">
                        <div class="col-sm-12">
                            <div class="nav-list">
                                <ul>
                                    <li>
                                        <h2>
                                            <a href="">MAP OPTIONS</a>
                                        </h2>
                                        <div class="nav-list-items">
                                            <ul>
                                                <li><a href="">Community Event</a></li>
                                                <li><a href="">Community Managers</a></li>
                                                <li><a href="">Partnered Streamers</a></li>
                                                <li><a href="">Our Philosphy</a></li>
                                                <li><a href="">Our Games</a></li>
                                            </ul>    
                                        </div>
                                    </li>
                                    <li>
                                        <h2>
                                            <a href="">TRACKER CALLS</a>
                                        </h2>
                                        <div class="nav-list-items">
                                            <ul>
                                                <li><a href="">Community Event</a></li>
                                                <li><a href="">Community Managers</a></li>
                                                <li><a href="">Partnered Streamers</a></li>
                                                <li><a href="">Our Philosphy</a></li>
                                                <li><a href="">Our Games</a></li>
                                            </ul>    
                                        </div>
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <header id="header-main">
            <div class="header-content"> 
                <div class="header-nav" data-toggle="modal" data-target="#nav-overlay" style="z-index: 9999;">
                    <div id="nav-hamburger">
                        <span></span>
                        <span></span>
                        <span></span>   
                        <span></span>
                    </div>
                </div>
                <div class="header-logo">
                    <div>
                        <a href="index.php"><img class="svg" alt="KTM Tracker" src="site_assets/images/logos/app/header-logo.svg"></a>
                    </div>
                </div>
            </div>
        </header>


        <section class="container-fluid main-container">
            <div id="tracker-loader-container">
                <div class="tracker-loader">
                    <div class="loader">
                        <svg 
                            version="1.1"
                            id="loader-1"
                            xmlns="http://www.w3.org/2000/svg"
                            xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
                            width="40px" height="40px"
                            viewBox="0 0 50 50"
                            style="enable-background:new 0 0 50 50;"
                            xml:space="preserve">
                            <path 
                                fill="#000"
                                d="M43.935,25.145c0-10.318-8.364-18.683-18.683-18.683c-10.318,0-18.683,8.365-18.683,18.683h4.068c0-8.071,6.543-14.615,14.615-14.615c8.072,0,14.615,6.543,14.615,14.615H43.935z">
                            <animateTransform 
                                attributeType="xml"
                                attributeName="transform"
                                type="rotate"
                                from="0 25 25"
                                to="360 25 25"
                                dur="0.6s"
                                repeatCount="indefinite"/>
                            </path>
                        </svg>
                    </div>
                    <p>Loading tracker data...</p>
            </div>
            </div>
            <article id="map" class="map-container"></article>
        </section>


        <footer class="container-fluid footer-main">
        </footer>


        <script src="scripts/_libs/mobile_detect_js/mobile-detect.min.js"></script>
        <script>
            var md = new MobileDetect( window.navigator.userAgent );
            var _isMobile = ( md.mobile() != null) ? true : false;
        </script>

        <!-- API Scripts -->
        <script src="scripts/_libs/jquery/jquery.min.js"></script>
        <script src="scripts/_libs/bootstrap/js/bootstrap.min.js"></script>
        <script src="scripts/_libs/gsap/TweenMax.js"></script>
        <script src="scripts/_libs/modernizr/modernizr.js"></script>
        <script src="scripts/_libs/js_info_bubble/infobubble.js"></script>
        <script src="scripts/_libs/handlebars/handlebars.min.js"></script>
        <!-- <script src="scripts/_libs/videojs/video.min.js"></script> -->
        <!-- <script src="scripts/_libs/slickjs/slick.min.js"></script> -->

        <!-- Scripts -->
        <script src="scripts/js/utils/convert-svg.js"></script>
        <script src="scripts/js/google_maps_api.js"></script>
        <!-- <script src="scripts/js/window-resize.js"></script> -->
        <!-- <script src="scripts/js/modernizr-dom.js"></script> -->
        <!-- <script src="scripts/js/slick_carousel.js"></script> -->
        <!-- <script src="scripts/js/event_scroller.js"></script> -->
        <script defer src="scripts/js/navigation.js"></script>
        <script src="scripts/js/main.js"></script>

        <script async defer src="https://maps.googleapis.com/maps/api/js?key=AIzaSyDtY7XcAta9D76108tUv_JIOOfCFvvx-uQ&sensor=true&callback=initTrackerDB"></script>
</html>
