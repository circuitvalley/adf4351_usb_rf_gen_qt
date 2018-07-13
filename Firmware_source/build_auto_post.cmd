setlocal EnableDelayedExpansion
:: get project name
for %%i in ("%cd%") do set "project=%%~nxi"
:: read build number 
for /f "delims=;" %%i in (build-number.txt) do set bn=%%i
:: increase build number
set /a bn=%bn%+1
:: write new build number
echo %bn%; > build-number.txt
echo Current Build: %bn%
::delete old hex file if available 
if %bn% GTR 1 (
set /a tmp=%bn%-1
 del dist\default\production\%project%_build_!tmp!.hex
)
:: copy new hex file,, if you have configuration name other than default , put name here insted off default 
 copy dist\default\production\%project%.production.hex dist\default\production\%project%_build_%bn%.hex