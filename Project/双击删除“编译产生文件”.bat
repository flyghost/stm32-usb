@echo off

cd /d %~dp0

REM ɾ����ǰĿ¼���ļ��м���Ŀ¼���ļ���
for /f "delims=" %%a in ('dir /b/s/ad') do (
    REM ��������ɾ��KEIL�������ļ���
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


