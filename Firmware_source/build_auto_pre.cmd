setlocal EnableDelayedExpansion
:: get project name
for %%i in ("%cd%") do set "project=%%~nxi"
:: see if build number file is there 
if NOT exist build-number.txt echo 0; > build-number.txt
:: Read last build number
for /f "delims=;" %%i in (build-number.txt) do set bn=%%i
:: increase build number 
set /a bn=%bn%+1
:: Create a header file with build number
echo /************************************************** > build_number.h
echo * This file is autogenrated by build number script  >> build_number.h
echo **************************************************/ >> build_number.h
echo. >> build_number.h
echo #ifndef BUILDNUMBER_H >> build_number.h
echo #define BUILDNUMBER_H >> build_number.h
echo. >> build_number.h 
echo #define BUILD_NUMBER %bn% >> build_number.h
echo. >> build_number.h
echo #endif >> build_number.h