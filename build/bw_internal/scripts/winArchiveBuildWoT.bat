@echo off
rem set JOB_NAME=test
set ARCHIVE_DIR=\\bbwindows07\BuildArchive\%JOB_NAME%
set COPY_PATTERN=*.exe *.pdb *.dll *.xml *.dle *.mll *.pyd
set ROBO_OPTS=/S /PURGE /NP /MT /XD .svn ^& IF %ERRORLEVEL% GEQ 4 GOTO END

IF %ERRORLEVEL% NEQ 0 GOTO FAILED

echo Copying build to %ARCHIVE_DIR%
call robocopy.exe ..\..\game  %ARCHIVE_DIR%\game      %COPY_PATTERN% %ROBO_OPTS%
EXIT /B 0
GOTO END

:FAILED
echo Build failed, skipping archiving.

:END