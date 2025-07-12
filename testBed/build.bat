@ECHO OFF
SetLocal EnableDelayedExpansion

REM Build script for testbed

REM Collect all C files recursively
SET "cFilenames="
FOR /R %%f IN (*.c) DO (
    SET "cFilenames=!cFilenames! "%%f""
)

REM Set build variables
SET "assembly=testbed"
SET "compilerFlags=-g"
REM -Wall -Werror
SET "includeFlags=-Isrc -I../engine/src"
SET "linkerFlags=-L../bin -lengine"
SET "defines=-D_DEBUG"

ECHO Building "%assembly%"...

clang !cFilenames! %compilerFlags% -o "../bin/%assembly%.exe" %defines% %includeFlags% %linkerFlags%

IF ERRORLEVEL 1 (
    ECHO Build failed.
    EXIT /B 1
) ELSE (
    ECHO Build succeeded.
)
