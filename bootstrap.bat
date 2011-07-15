@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: build_server.bat
::
:: Build's the project and produces a directory of all necesary executables and
:: dll's needed to run an instance of the server.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: initialize build variables
set "BUILD_DIRECTORY=build"
set "PROJECT_BASE=%~dp0"

:: this script accepts a directory in which to build as it's only parameter
if "%~1" NEQ "" (
    if not exist "%~1" (        
        echo Passed an invalid directory to build in!
	    echo.
	    echo Create the directory you want to build in, then rerun this script.
	    exit /b 1    
    )    
    
    set "BUILD_DIRECTORY=%~1"
)

:: prepare a directory to build the project and enter it
if exist "%BUILD_DIRECTORY%" (
    for /F %%i in ('dir /b "%BUILD_DIRECTORY%\*.*"') do (            
        echo Attempting to build in an existing, non-empty directory!
	    echo.
	    echo Please clear out the contents of the build directory and
	    echo run this script again.
	    exit /b 1    
    )
)

if not exist "%BUILD_DIRECTORY%" (
    cmake -E make_directory "%BUILD_DIRECTORY%"
)

cd "%BUILD_DIRECTORY%"

cmake -G "Visual Studio 10" -DENABLE_TEST_REPORT=ON %PROJECT_BASE%
cmake --build .