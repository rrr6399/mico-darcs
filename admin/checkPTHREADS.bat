
@echo Checking and building (if necessary) PThreads for %1 exception type
set VER=2

@if NOT EXIST win32-bin\pthread%1%VER%.dll goto print1
@if NOT EXIST win32-bin\lib\pthread%1%VER%.lib goto print2

@echo DLLs and LIBs found. PThreads usage is enabled

@goto end

:print1
@echo The file win32-bin\pthread%1%VER%.dll was not found
goto build

:print2
@echo  The file win32-bin\lib\pthread%1%VER%.lib was not found
goto build

:build
	@cd contrib\pthreads

	@rem Adapt the line below for your VSTUDIO version
	@rem or use devenv instead of nmake to include manifests automatically in vc8


	@IF NOT EXIST "%VS80COMNTOOLS%vsvars32.bat" (
		@nmake clean %1
	) ELSE (
		@devenv.exe pthread.sln /Build %2
	)

	@copy pthread%1%VER%.dll ..\..\win32-bin
	@copy pthread%1%VER%.lib ..\..\win32-bin\lib
	@cd ..\..

@echo #AUTOMATICALLY GENERATED FILE. DO NOT MODIFY > makevars.win32.pth
@echo #Library files were installed in win32-bin and win32-bin\lib. >> makevars.win32.pth
@echo #>> makevars.win32.pth

@echo HAVE_PTHREADS = 1 >> makevars.win32.pth
@echo HAVE_THREADS = 1 >> makevars.win32.pth
@echo #>> makevars.win32.pth
@echo #Exception model>> makevars.win32.pth
@echo HAVE_PTHREADS_%1 = 1 >> makevars.win32.pth


:end
@