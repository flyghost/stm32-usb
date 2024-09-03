@echo off

cd /d %~dp0

REM 删除当前目录的文件夹及子目录的文件夹
for /f "delims=" %%a in ('dir /b/s/ad') do (
    REM 以下用于删除KEIL创建的文件夹
    if "%%~na"=="DebugConfig" rd /s /q "%%a" 2>nul
    if "%%~na"=="RTE" rd /s /q "%%a" 2>nul
    if "%%~na"=="STM324xG-EVAL_USBD-FS" rd /s /q "%%a" 2>nul
    if "%%~na"=="STM324xG-EVAL_USBD-HS" rd /s /q "%%a" 2>nul
    if "%%~na"=="STM324xG-EVAL_USBH-FS" rd /s /q "%%a" 2>nul
    if "%%~na"=="STM324xG-EVAL_USBH-HS" rd /s /q "%%a" 2>nul

)

del *.scvd /s
del JLinkLog.txt /s
del *.uvguix.* /s           
del *.lst /s

exit


