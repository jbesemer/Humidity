set timestamp=%DATE:/=-%@%TIME::=.%
set timestamp=%timestamp: =%
set ZIPNAME=Projects_Humidity From Home %timestamp%.zip
set ZIPPER=C:\Program Files\7-Zip\7z.exe
set DEST=C:\Users\jb\Google Drive\Software
set PLAYERBIN=C:\Program Files (x86)\SoundPlayer\
set SOUNDS=C:\Lib\Sounds\

"%PLAYERBIN%Player.exe" "%SOUNDS%DRIP.WAV"

del "%ZIPNAME%" || goto :error
"%ZIPPER%" a "%ZIPNAME%" . -xr!.svn -xr!*.zip || goto :error
copy "%ZIPNAME%" "%DEST%" || goto :error
del "%ZIPNAME%" || goto :error

"%PLAYERBIN%Player.exe" "%SOUNDS%DING.WAV"

pause
goto :exit

:error
"%PLAYERBIN%Player.exe" "%SOUNDS%BUZZ.WAV"
