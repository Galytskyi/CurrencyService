// CurrencyService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CurrencyService.h"
#include "Service.h"

//--------------------------------------------------------------------------------------------
// libs

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "Chilkat\\libs\\ChilkatRelDll.lib")

//--------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------------------
// service cmd

#define CMD_SERVICE_INSTALL "install"
#define	CMD_SERVICE_UNINSTALL "uninstall"

#define	CMD_SERVICE_START "start"
#define	CMD_SERVICE_STOP "stop"

#define	CMD_SERVICE_USER_CTRL "user_control"

//--------------------------------------------------------------------------------------------
// main

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	CService service;

	// analyze params of command line
	//
	if ( argc > 1 && !lstrcmpi( argv[1], CMD_SERVICE_INSTALL ) )
	{
		if (service.install())
		{
			std::cout << "Installed" << std::endl;
		}
	}
	else if ( argc > 1 && !lstrcmpi( argv[1], CMD_SERVICE_UNINSTALL ) )
	{
		if (service.uninstall())
		{
			std::cout << "Uninstalled" << std::endl;
		}
	}
	else if ( argc > 1 && !lstrcmpi( argv[1], CMD_SERVICE_START ) )
	{
		if (service.start())
		{
			std::cout << "Started" << std::endl;
		}
	}
	else if ( argc > 1 && !lstrcmpi( argv[1], CMD_SERVICE_STOP ) )
	{
		if (service.stop())
		{
			std::cout << "Stopped" << std::endl;
		}
	}
	else if ( argc > 1 && !lstrcmpi( argv[1], CMD_SERVICE_USER_CTRL ) )
	{
		if (service.userCotnrol())
		{
			std::cout << "User controled" << std::endl;
		}
	}
	else
	{
		DWORD dwStatus = 0;
		BOOL result = service.status(dwStatus);
		
		service.run();
	}

	std::cout << "Check log file" << std::endl;

	//
	//
	return nRetCode;
}

//--------------------------------------------------------------------------------------------