@echo off
@echo =======================���з���ǰ����
del /s /Q *.ncb
del /s /Q *.log
del /s /Q *.bak
del /s /Q *.ilk
del /s /Q *.pdb
del /s /Q *.obj
del /s /Q *.htm
del /s /Q *.o
del /s /Q *.idb
del /s /Q *.manifest
del /s /Q *.pch
del /s /Q *.dep
del /s /Q *.manifest.res
del /s /Q *.d
del /s /Q *.filepart
del /s /Q *.db
del /s /Q *.sdf
del /s /Q ps_framework.*.user
del /s /Q /a:h-s *.suo
@echo =======================��ʱ��ִ�� del /s /Q *.user
@echo =======================ɾ�� Debug �ļ���
for /f "delims=" %%a in ('dir /ad /s /b ".\" ^| "C:\Windows\System32\findstr.exe" "\Debug$"') do (
rd /s /q %%a
)
@echo =======================ɾ�� build �ļ���
for /f "delims=" %%a in ('dir /ad /s /b ".\" ^| "C:\Windows\System32\findstr.exe" "\build$"') do (
rd /s /q %%a
)
@echo =======================ɾ�� data �ļ���
for /f "delims=" %%a in ('dir /ad /s /b ".\" ^| "C:\Windows\System32\findstr.exe" "\data$"') do (
rd /s /q %%a
)
@echo =======================ɾ�� log �ļ���
for /f "delims=" %%a in ('dir /ad /s /b ".\" ^| "C:\Windows\System32\findstr.exe" "\log$"') do (
rd /s /q %%a
)
@echo =======================ɾ�� ipch �ļ���
for /f "delims=" %%a in ('dir /ad /s /b ".\" ^| "C:\Windows\System32\findstr.exe" "\ipch$"') do (
rd /s /q %%a
)
@echo =======================ɾ�� ������ include �� lib �ļ���
rd /s /q .\include
rd /s /q .\lib
@echo �������
pause
