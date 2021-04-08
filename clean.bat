@echo off

rem execute setenv.bat to setup environment
call %PrjRootDir%\ide\adapt\setenv.bat

echo Purge repository by resetting all files to repository state
echo   git clean -xfd
echo --------------------------------------------------

rem check if git.exe is part of PATH-variable
WHERE /Q git.exe
IF NOT ERRORLEVEL 1 (
	git.exe clean -xfd %PrjRootDir%
) ELSE (
	rem check if git.exe exists in setenv_host variable
	WHERE /Q "%DIR_GIT_BIN_LOCAL%:git.exe"
	IF NOT ERRORLEVEL 1 (
		rem try executing git anyway
		"%DIR_GIT_BIN_LOCAL%\git.exe" clean -xfd %PrjRootDir%
	) ELSE (
		ECHO git.exe not part of PATH or DIR_GIT_BIN_LOCAL variable. check setenv_host.bat
		EXIT /B 1
	)
)
echo --------------------------------------------------
exit /b