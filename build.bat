@echo off
setlocal EnableDelayedExpansion

echo [INFO] Starting build process...

REM Set color codes for different message types
set "INFO=[92m[INFO][0m"
set "WARN=[93m[WARN][0m"
set "ERROR=[91m[ERROR][0m"
set "TRACE=[94m[TRACE][0m"

REM Check TCC
echo !TRACE! Checking for TCC compiler...
if not exist "tcc" (
    echo !ERROR! TCC folder not found!
    echo !INFO! Please extract tcc.zip to the tcc folder
    pause
    exit /b 1
)

REM Check SDL3 directory structure
echo !TRACE! Checking SDL3 directory structure...
if not exist "lib" (
    echo !WARN! lib directory not found, creating directory structure...
    mkdir "lib\SDL3\bin"
    mkdir "lib\SDL3\include\SDL3"
    echo !INFO! Please copy:
    echo    - SDL3.dll to lib\SDL3\bin
    echo    - SDL3 headers to lib\SDL3\include\SDL3
    pause
    exit /b 2
)

REM Check SDL3 files
echo !TRACE! Checking SDL3 files...
set "_missing_files="

if not exist "lib\SDL3\bin\SDL3.dll" (
    set "_missing_files=1"
    echo !ERROR! SDL3.dll not found in lib\SDL3\bin
)

if not exist "lib\SDL3\include\SDL3\SDL.h" (
    set "_missing_files=1"
    echo !ERROR! SDL.h not found in lib\SDL3\include\SDL3
)

if defined _missing_files (
    echo !ERROR! Missing required SDL3 files
    echo !INFO! Please ensure all SDL3 files are in their correct locations
    pause
    exit /b 3
)

REM Create output directories
echo !TRACE! Creating output directories...
if not exist "bin" (
    echo !INFO! Creating bin directory...
    mkdir "bin"
)

echo !TRACE! Setting up TCC path...
set "PATH=%CD%\tcc;%PATH%"

echo !INFO! Starting compilation...
echo !TRACE! Compiling with verbose output...

REM Copy SDL3.dll to current directory for linking
copy "lib\SDL3\bin\SDL3.dll" "tcc\" > nul

REM Save current error level
set "_prev_errorlevel=0"

REM Compile resources
windres resources.rc -O coff -o resources.res

REM Add resources.res to your compilation command
tcc -v -o "bin/rebuild_and_resist.exe" ^
    "-I./include" ^
    "-I./include/data" ^
    "-I./lib/SDL3/include" ^
    "-I./src" ^
    "-DSDL_MAIN_HANDLED" ^
    "-DUSE_MATH_DEFINES" ^
    "src/main.c" ^
    "src/game.c" ^
    "src/player.c" ^
    "src/building.c" ^
    "src/building_data.c" ^
    "src/enemy.c" ^
    "src/enemy_data.c" ^
    "src/wave_data.c" ^
    "src/renderer.c" ^
    "src/resource_manager.c" ^
    "src/survivor.c" ^
    "src/client_settings.c" ^
    "src/input.c" ^
    "src/wave.c" ^
    "src/text.c" ^
    "src/building_manager.c" ^
    "src/enemy_manager.c" ^
    "src/survivor_manager.c" ^
    "lib/SDL3/bin/SDL3.dll" ^
    "tcc/lib/msvcrt.def" ^
    "tcc/lib/kernel32.def" ^
    "tcc/lib/gdi32.def" ^
    "tcc/lib/user32.def" ^
    resources.res

set "_compile_error=%errorlevel%"

if !_compile_error! neq 0 (
    echo !ERROR! Compilation failed with error code !_compile_error!
    if !_compile_error! equ 1 (
        echo !INFO! This might be a syntax error
    ) else if !_compile_error! equ 2 (
        echo !INFO! This might be a linker error
    ) else (
        echo !INFO! Unknown error occurred
    )
    pause
    exit /b 4
)

echo !INFO! Compilation successful
echo !TRACE! Copying SDL3.dll to bin directory...

copy "lib\SDL3\bin\SDL3.dll" "bin\" > nul
if errorlevel 1 (
    echo !ERROR! Failed to copy SDL3.dll to bin directory
    pause
    exit /b 5
)

echo !INFO! Build completed successfully!
echo !INFO! You can now run: bin\rebuild_and_resist.exe
echo.
echo !TRACE! Build process completed with no errors
pause

endlocal 