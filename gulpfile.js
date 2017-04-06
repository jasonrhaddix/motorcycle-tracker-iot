/********************************************************************************************************/
/* GULP FLOW */
/* When the 'gulp' command is called in the bash window, GULP with initiate workflow below */
/********************************************************************************************************/
/*
  1. Modernizr - scrubs files in app directory for Modernizr references and compiles a custom library
     build which is written to the 'scripts/_libs/modernizr' directory.

  2. CopyLibs - pulls library scripts from '/node_modules' and puts them in their respective 
     'scripts/_libs/' directory. This allows for libraries to be easily update via npm.

  3. Watch - The watch task includes .html .scss .css .js files and looks for change.
  
  4. PHP - php server spins up.

  5. Browser-Sync - initiates a local web server using PHP IP port and will reload the browsers once a file is 
     changed on the watch list.
*/



/********************************************************************************************************/
/* Module includes */
/********************************************************************************************************/
var gulp              = require( 'gulp' ),                     // base GULP module
  log                 = require( 'log-utils' ),                // module for CLI output logging
  gulp_modernizr      = require( 'gulp-modernizr' ),           // Modernizr module for feature detection
  gulp_sass           = require( 'gulp-sass' ),                // SASS module for compiling to CSS 
  gulp_uglifycss      = require( 'gulp-uglifycss' ),           // uglifies CSS
  gulp_uglify         = require( 'gulp-uglify' ),              // uglifies JS
  gulp_beautifycss    = require( 'gulp-cssbeautify' ),         // beautifies CSS
  sourcemaps          = require( 'gulp-sourcemaps' ),          // module for routing browser CSS inspection back to SASS
  // gulp_csso           = require( 'gulp-csso' ),                // CSS optimizer and minifier (unused)
  autoprefixer        = require( 'gulp-autoprefixer' ),        // module for adding vendor prefixes to CSS
  project_size        = require( 'gulp-size' ),                // module for outputting CSS file size to CLI (show on CSS build)
  gulp_sftp           = require( 'gulp-sftp' ),                // allows for CLI upload to SFTP server (NOT FOR PRODUCTION - exposes login credentials)
  gulp_changed        = require( 'gulp-changed'),              // module for comparing files from src and dest directories (used as GULP pipe for filetering)
  gulp_zip            = require( 'gulp-zip' ),                 // module for zipping files (used for archiving entire repo - used GIT LFS)
  gulp_date           = require( 'gulp-date-rev' ),            // module for prefixing dates to files (used for prefixing date to ZIP archive)
  gulp_list           = require( 'gulp-filenamelist' ),        // module for listing all files in a folder and outputs to txt/json
  php_server          = require( 'gulp-connect-php'),          // PHP Webserver
  browser_sync        = require( 'browser-sync' ).create(),    // Browser-Sync module for create a local web client and synchronizing broswers
  reload              = browser_sync.reload;  
  

/********************************************************************************************************/
/* App directories */
/********************************************************************************************************/
var devDir              = 'app_web/GoogleCloud/ComputeInstance',
    srcDir              = 'app_web/GoogleCloud/ComputeInstance',
    distDir             = '_dist/'; // directory used for SHA1 comparison for upload to dev server //



/********************************************************************************************************/
/* Module options variables */
/********************************************************************************************************/
var modernizrOptions    = require( './modernizr-config.json' ),

    archiveOptions      = 'archive.zip',
  
    gulpSassOptions     = { outputStyle: 'compressed' },
  
    autoprefixerOptions = { browsers: ['last 2 versions'], cascade: false },
  
    sftpOptions_DEV     = { host: '', 
                            user: '',
                            pass: '',
                            remotePath: '',
                            callback: cloneToDist_DEV
                           },

    sftpOptions_LIVE    = { host: '', 
                            user: '',
                            pass: '',
                            remotePath: '',
                            callback: cloneToDist_LIVE
                          },
  
  fileListOptions       = { outputFileName: 'app_files.txt',
                            separator: '\n',
                            includeRelativePath: true,
                          };




/********************************************************************************/
// Copy Libraries from Node Modules
/********************************************************************************/
gulp.task( 'copylibs', function()
{
    // JS NODES
    gulp.src([ 'node_modules/jquery/dist/jquery.min.js' ]).pipe( gulp.dest( devDir + '/scripts/_libs/jquery' ));
    gulp.src([ 'node_modules/bootstrap-sass/assets/javascripts/bootstrap.min.js' ]).pipe( gulp.dest( devDir + '/scripts/_libs/bootstrap/js' ));
    gulp.src([ 'node_modules/mobile-detect/mobile-detect.min.js' ]).pipe( gulp.dest( devDir + '/scripts/_libs/mobile_detect_js' ));
    gulp.src([ 'node_modules/js-info-bubble/src/infobubble-compiled.js' ]).pipe( gulp.dest( devDir + '/scripts/_libs/js_info_bubble' ));
});


/********************************************************************************/
// Copy Libraries from Node Modules
/********************************************************************************/
gulp.task( 'modernizr', function()
{  
  log.ok( log.green( "Initializing | MODERNIZR..." ));
  return gulp.src( srcDir + '/scripts/js/*' )
      .pipe( gulp_modernizr( modernizrOptions ) )
      .pipe( gulp_uglify())
      .pipe( gulp.dest( devDir + '/scripts/_libs/modernizr' ))
});


/********************************************************************************/
// Initialize HTML folder to watch
/********************************************************************************/
gulp.task( 'html', function()
{
  return gulp.src( srcDir + '/**/*.html');
});


/********************************************************************************/
// Initialize PHP folder to watch
/********************************************************************************/
gulp.task( 'php', function()
{
  return gulp.src( srcDir + '/**/*.php');
});


/********************************************************************************/
// Initialize JS folder to watch
/********************************************************************************/
gulp.task( 'js', function()
{
  return gulp.src( srcDir + '/scripts/js/**/*.js');
});


/********************************************************************************/
// Initialize CSS folder to watch
/********************************************************************************/
gulp.task( 'css', function()
{
  return gulp.src( srcDir + '/scripts/css_-noedit/compressed/*.css');
});


/********************************************************************************/
//  SASS to CSS | Sourcemaps + autoprefixer(unused) + compressed CSS mode
/********************************************************************************/
gulp.task( 'sass', function()
{
  log.ok( log.green( "Building | CSS..." ));
  return gulp.src( srcDir + '/scripts/scss/**/*.scss' )
    .pipe( sourcemaps.init())
    .pipe( gulp_sass( gulpSassOptions ).on( 'error', gulp_sass.logError ))
    .pipe( autoprefixer( autoprefixerOptions ))
    // .pipe( gulp_csso({ restructure: false }))
    .pipe( sourcemaps.write('./maps'))
    .pipe( project_size())
    .pipe( gulp.dest( devDir + '/scripts/css_-noedit-/compressed' ))
});


/********************************************************************************/
// Initialize 'watch' for live-reload
/********************************************************************************/
gulp.task( 'watch', function() {
  // log.ok( log.green( "Initializing | GULP WATCH..." ));
  gulp.watch( srcDir + '/**/*.html', ['html']).on('change', reload);
  gulp.watch( srcDir + '/scripts/scss/**/*.scss', ['sass'] );
  gulp.watch( devDir + '/scripts/css_-noedit-/compressed/*.css').on('change', reload);
  gulp.watch( devDir + '/scripts/js/**/*.js').on('change', reload);
  gulp.watch( srcDir + '/**/*.php', [reload]); 
});


/********************************************************************************/
// Browser-sync with PHP | NOT WORKING
/********************************************************************************/
gulp.task('php_server', function() {
    php_server.server({ base: devDir , port: 8010, keepalive: true});
});

gulp.task('browser-sync',['php_server'], function() {
    browser_sync.init({
        proxy: '127.0.0.1:8010',
        port: 3000,
        open: true,
        notify: false
    });
});


/********************************************************************************/
// Start Gulp | use 'gulp'
/********************************************************************************/
gulp.task( 'default', [ 'modernizr', 'copylibs', 'watch', 'browser-sync' /*'php'*/ ]);









/********************************************************************************************************/
/* GULP utilities (used in CLI and are outside of normal GULP flow) */
/********************************************************************************************************/
/********************************************************************************/
// Zips up app and uses Git LFS to store the archive | Usage: 'gulp archive'
/********************************************************************************/
gulp.task( 'archive', function()
{
  log.ok( log.green( "Building | ARCHIVE.ZIP..." ));
    return gulp.src( devDir + '/**/*' )
      .pipe( gulp_zip( archiveOptions ))
      .pipe( gulp_date( archiveOptions ))
      .pipe( gulp.dest( './_archive' ))
});


/********************************************************************************/
// Generate a text file with a list of application files | Usage: 'gulp listfiles'
/********************************************************************************/

gulp.task( 'listfiles', function()
{
    log.ok( log.green( "Building | FILES LIST..." ));
    return gulp.src( devDir + '/**/*.*' )
      .pipe( gulp_list( fileListOptions ))
      .pipe( gulp.dest( './' ))
});


/********************************************************************************/
// SFTP upload to server | Usage: 'gulp upload' - DEV
/********************************************************************************/
gulp.task( 'upload', function()
{
  log.ok( log.green( "Initializing | SFTP UPLOAD..." ));
    return gulp.src( devDir + '/**/*' )
      .pipe( gulp_changed( distDir + "/dev/", { hasChanged: gulp_changed.compareSha1Digest } ))
      .pipe( gulp_sftp( sftpOptions_DEV ))
});

function cloneToDist_DEV()
{
  return gulp.src( devDir + '**/*' )
    .pipe( gulp_changed( distDir + "/dev/", { hasChanged: gulp_changed.compareSha1Digest } ))
    .pipe( gulp.dest( distDir + "/dev/" ));
}


/********************************************************************************/
// SFTP upload to server | Usage: 'gulp upload live' - LIVE
/********************************************************************************/
gulp.task( 'upload-live', function()
{
  log.ok( log.green( "Initializing | SFTP UPLOAD..." ));
    return gulp.src( devDir + '**/*' )
      .pipe( gulp_changed( distDir + "/live/", { hasChanged: gulp_changed.compareSha1Digest } ))
      .pipe( gulp_sftp( sftpOptions_LIVE ))
});

function cloneToDist_LIVE()
{
  return gulp.src( devDir + '**/*' )
    .pipe( gulp_changed( distDir + "/live/", { hasChanged: gulp_changed.compareSha1Digest } ))
    .pipe( gulp.dest( distDir + "/live/" ));
}

/********************************************************************************/
// CSS Beautify | Usage: 'gulp css-b'
/********************************************************************************/
gulp.task( 'css-b', function()
{
    return gulp.src( devDir + 'scripts/css_-noedit-/compressed/**/*.css')
        .pipe( gulp_beautifycss({
            indent: '  ',
            openbrace: 'separate-line',
            autosemicolon: true
        }))
        .pipe( gulp.dest( devDir + '/scripts/css_-noedit-/uncompressed' ));
});


/********************************************************************************/
// CSS Uglify | Usage: 'gulp css-u'
/********************************************************************************/
gulp.task( 'css-u', function()
{
  return gulp.src( devDir +  '/scripts/css_-noedit-/uncompressed/**/*.css')
    .pipe( gulp_uglifycss({
        "maxLineLen": 80,
      "uglyComments": true
      }))
      .pipe( gulp.dest( devDir + '/scripts/css_-noedit-/compressed' ));
});