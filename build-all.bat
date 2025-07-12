@ECHO OFF
SetLocal EnableDelayedExpansion

REM Build Everything

ECHO Building everything...

PUSHD engine
CALL build.bat
IF !ERRORLEVEL! NEQ 0 (
    ECHO  Engine build failed with error code: !ERRORLEVEL!
    POPD
    EXIT /B !ERRORLEVEL!
)
POPD

PUSHD testbed
CALL build.bat
IF !ERRORLEVEL! NEQ 0 (
    ECHO  Testbed build failed with error code: !ERRORLEVEL!
    POPD
    EXIT /B !ERRORLEVEL!
)
POPD

ECHO  All assemblies built successfully.
