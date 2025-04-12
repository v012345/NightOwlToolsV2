import os
if __name__ == '__main__':
    project_path = os.path.abspath( "application" )
    project_name = os.path.basename( project_path )
    build_xml = os.path.join( project_path, "build.xml" )
    os.system( 'C:\\work\\tools\\android_sdk\\tools\\android.bat update project --target 2 --path C:\\work\\BlackMoon\\Closers.android\\application --name application' )
    