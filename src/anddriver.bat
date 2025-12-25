@echo off

set input=%~1

rem === Copy files to desired location ===
copy "%cwd%" "%input%" /Y

exit /b 0