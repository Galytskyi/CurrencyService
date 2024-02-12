// Service.cpp : implementation file
//

#include "stdafx.h"

#include "Service.h"
#include "CurrencyService.h"
#include "Options.h"

//--------------------------------------------------------------------------------------------

CLog CService::m_log;

SERVICE_STATUS CService::m_serviceStatus;
SERVICE_STATUS_HANDLE CService::m_serviceStatusHandle = NULL;

HANDLE CService::m_serviceControlEvent = NULL;

RunServiceParam CService::m_runServiceParam;
BOOL CService::m_bStopService = FALSE;	

//--------------------------------------------------------------------------------------------

CService::CService()
{
}

//--------------------------------------------------------------------------------------------

CService::~CService()
{
}

//--------------------------------------------------------------------------------------------

BOOL CService::install()
{
	//
	//
	SC_HANDLE serviceControlManager = OpenSCManager(0, 0,  SC_MANAGER_CREATE_SERVICE);
	if (!serviceControlManager)
	{
		m_log.write("Service Control Manager is not availble");
		return FALSE;
	}
		
	//
	//
 	char path[ _MAX_PATH + 1 ];
	if (!GetModuleFileName( 0, path, sizeof(path)/sizeof(path[0]) ))
	{
		m_log.write("Servive Path is unknown");
		return FALSE;
	}
		
	//
	//
	SC_HANDLE service = CreateService(
		serviceControlManager,
		SERVICE_NAME, 
		SERVICE_NAME,
		SERVICE_ALL_ACCESS, 
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
		SERVICE_AUTO_START, 
		SERVICE_ERROR_IGNORE, 
		path,
		0, 0, 0, 0, 0 );

	//
	//
	BOOL result = FALSE;

    if (service)
	{
		result = TRUE;
		m_log.write("Service Installed Successfully");
		CloseServiceHandle(service);
	}
	else
	{
		if(GetLastError() == ERROR_SERVICE_EXISTS)
		{
			result = TRUE;
			m_log.write("Service is already installed");
		}
		else
		{
			CString strError;
			strError.Format("Service Was not Installed Successfully. Error Code %d", GetLastError());
			m_log.write(strError);
		}
	}
	 
	return result;
}

//--------------------------------------------------------------------------------------------

BOOL CService::uninstall()
{
	//
	//
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );
	if (!serviceControlManager)
	{
		m_log.write("Service Control Manager is not available");
		return FALSE;
	}
	
	//
	//
	SC_HANDLE service = OpenService( serviceControlManager, SERVICE_NAME, SERVICE_QUERY_STATUS | DELETE );
	if (!service)
	{
		m_log.write("Service is not available");
		CloseServiceHandle(serviceControlManager);
		return FALSE;
	}

	//
	//
	BOOL result = FALSE;
	SERVICE_STATUS serviceStatus;
		
	if (QueryServiceStatus(service, &serviceStatus))
	{
		if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
		{
			if(DeleteService(service))
			{
				m_log.write("Service Removed Successfully\n");
				result = TRUE;
			}
			else
			{
				DWORD dwError = GetLastError();

				switch(dwError)
				{
					case ERROR_ACCESS_DENIED:
						m_log.write("Error: Access Denied While trying to Remove MyService");
						break;

					case ERROR_INVALID_HANDLE:
						m_log.write("Error: Handle invalid while trying to Remove Service");
						break;

					case ERROR_SERVICE_MARKED_FOR_DELETE:
						m_log.write("Error: Service already marked for deletion");
						break;

					default:
						CString strError;
						strError.Format("Code error %d: %d", dwError);
						m_log.write(strError);
						break;
				}
			}
		}
		else
		{
			m_log.write("Service is still Running");
		}
	}

	CloseServiceHandle(service);
	CloseServiceHandle(serviceControlManager);
	
	return result;
}

//--------------------------------------------------------------------------------------------

BOOL CService::start()
{
	//
	//
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );
	if (!serviceControlManager)
	{
		m_log.write("Service Control Manager is not available");
		return FALSE;
	}

	//
	//
	SC_HANDLE service = OpenService( serviceControlManager, SERVICE_NAME, SERVICE_QUERY_STATUS | SERVICE_START);
	if (!service)
	{
		m_log.write("Service is not available");
		CloseServiceHandle(serviceControlManager);
		return FALSE;
	}

	BOOL result = FALSE;
	SERVICE_STATUS serviceStatus;

	if (QueryServiceStatus(service, &serviceStatus))
	{
		if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
		{
			result = StartService(service, 0, NULL);
		}
	}

	CloseServiceHandle(service);
	CloseServiceHandle(serviceControlManager);
	
	return result;
}

//--------------------------------------------------------------------------------------------

BOOL CService::stop()
{
	//
	//
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );
	if (!serviceControlManager)
	{
		m_log.write("Service Control Manager is not available");
		return FALSE;
	}

	//
	//
	SC_HANDLE service = OpenService( serviceControlManager, SERVICE_NAME, SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (!service)
	{
		m_log.write("Service is not available");
		CloseServiceHandle(serviceControlManager);
		return FALSE;
	}

	BOOL result = FALSE;
	SERVICE_STATUS serviceStatus;

	if (QueryServiceStatus(service, &serviceStatus))
	{
		if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			result = ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus);
		}
	}

	CloseServiceHandle(service);
	CloseServiceHandle(serviceControlManager);
	
	return result;
}

//--------------------------------------------------------------------------------------------

BOOL CService::userCotnrol()
{
	//
	//
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );
	if (!serviceControlManager)
	{
		m_log.write("Service Control Manager is not available");
		return FALSE;
	}

	//
	//
	SC_HANDLE service = OpenService( serviceControlManager, SERVICE_NAME, SERVICE_QUERY_STATUS | SERVICE_USER_DEFINED_CONTROL);
	if (!service)
	{
		m_log.write("Service is not available");
		CloseServiceHandle(serviceControlManager);
		return FALSE;
	}

	BOOL result = FALSE;
	SERVICE_STATUS serviceStatus;

	if (QueryServiceStatus(service, &serviceStatus))
	{
		result = ControlService(service, USER_DEFINED_CONTROL, &serviceStatus);
	}

	CloseServiceHandle(service);
	CloseServiceHandle(serviceControlManager);
	
	return result;
}


//--------------------------------------------------------------------------------------------

void CService::run()
{
	// start service
	//
	SERVICE_TABLE_ENTRY serviceTable[] =
	{
 		{ SERVICE_NAME, &CService::ServiceMain },
		{ 0, 0 }
	};

	StartServiceCtrlDispatcher(serviceTable);
}

//--------------------------------------------------------------------------------------------

DWORD CService::status(DWORD& dwStatus) const
{
	//
	//
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );
	if (!serviceControlManager)
	{
		m_log.write("Service Control Manager is not available");
		return FALSE;
	}
	
	//
	//
	SC_HANDLE service = OpenService( serviceControlManager, SERVICE_NAME, SERVICE_QUERY_STATUS );
	if (!service)
	{
		m_log.write("Service is not available");
		CloseServiceHandle(serviceControlManager);
		return FALSE;
	}

	//
	//
	SERVICE_STATUS serviceStatus;

	//#define SERVICE_STOPPED                        0x00000001
	//#define SERVICE_START_PENDING                  0x00000002
	//#define SERVICE_STOP_PENDING                   0x00000003
	//#define SERVICE_RUNNING                        0x00000004
	//#define SERVICE_CONTINUE_PENDING               0x00000005
	//#define SERVICE_PAUSE_PENDING                  0x00000006
	//#define SERVICE_PAUSED                         0x00000007

		
	if (QueryServiceStatus(service, &serviceStatus))
	{
		dwStatus = serviceStatus.dwCurrentState;
	}
	else
	{
		return FALSE;
	}
	
	CloseServiceHandle(service);
	CloseServiceHandle(serviceControlManager);

	return TRUE;
}

//--------------------------------------------------------------------------------------------

void WINAPI CService::ServiceMain( DWORD /*argc*/, TCHAR* /*argv*/[] )
{
	// Register handler
	//
	m_serviceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, &CService::ServiceControlHandler);
	if (!m_serviceStatusHandle)
	{
		return;
	}
	
	// initialise service status
	//
	m_serviceStatus.dwServiceType = SERVICE_WIN32;
	m_serviceStatus.dwCurrentState = SERVICE_STOPPED;
	m_serviceStatus.dwControlsAccepted = 0;
	m_serviceStatus.dwWin32ExitCode = NO_ERROR;
	m_serviceStatus.dwServiceSpecificExitCode = NO_ERROR;
	m_serviceStatus.dwCheckPoint = 0;
	m_serviceStatus.dwWaitHint = 0;

	// service is starting
	//
	m_serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);

	// Create the Controlling Event here
	//
	m_serviceControlEvent = CreateEvent( 0, FALSE, FALSE, 0 );

	// Service running
	//
	m_serviceStatus.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
	m_serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);

	// begin load data from internet and save to local file
	//
	updateFiles();

	// wait for stop of service 
	//
	/////////////////////////////////////////////////////
	WaitForSingleObject(m_serviceControlEvent, INFINITE );
	/////////////////////////////////////////////////////

	// service was stopped
	//
	m_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
	SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);

	// do cleanup here
	//
	CloseHandle(m_serviceControlEvent);
	m_serviceControlEvent = NULL;

	// service is now stopped
	//
	m_serviceStatus.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
	m_serviceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);
}

//--------------------------------------------------------------------------------------------

void WINAPI CService::ServiceControlHandler(DWORD controlCode)
{
	CString strHead = CString("Service ");

	switch(controlCode)
	{
		case SERVICE_CONTROL_INTERROGATE:
			m_log.write(strHead + "Interrogate");
			break;

		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:

			m_log.write(strHead + "is stoping");

			m_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);
    
			m_bStopService = TRUE;
		
			SetEvent(m_serviceControlEvent);
			return;

		case SERVICE_CONTROL_PAUSE:
			m_log.write(strHead + "Pause");
			break;

		case SERVICE_CONTROL_CONTINUE:
			m_log.write(strHead + "Continue");
			break;

		default:
			if (controlCode >= 128 && controlCode <= 255)
			{
				// user defined control code
				//
				CString strCode;
				strCode.Format(strHead + "ControlCode: %d", controlCode);
				m_log.write(strCode);
			}
			else
			{
				// unrecognised control code
				//
				CString strCode;
				strCode.Format(strHead + "Unrecognised controlCode: %d", controlCode);
				m_log.write(strCode);
			}
			break;
	}

	SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);
}

//--------------------------------------------------------------------------------------------

void CService::updateFiles()
{
	m_log.write("Service Started");

	// check cofiguration
	//
	if (!m_runServiceParam.load())
	{
		m_log.write("Config file was not loaded");
		return;
	}

	if (!m_runServiceParam.isValid())
	{
		m_log.write("Config file is not valid");
		return;
	}

	using namespace currency;

	// set files 
	//
	std::shared_ptr<CBaseList> sourlist = create(m_runServiceParam.sourType());
	std::shared_ptr<CBaseList> destlist = create(m_runServiceParam.destType());

	if (sourlist == NULL || destlist == NULL)
	{
		return;
	}

	// set timeout for period to read and write currency file 
	// sec = tick_count * 100 ms
	//
	if (m_runServiceParam.timeout() <= 0)
	{
		return;
	}

	int tick_count = m_runServiceParam.timeout() * 10;
	int tick_step = 100; // 100 ms

	// begin to load internet-file and save to local-file
	//
	while (!m_bStopService)
	{
		// download data file with currency from internet
		//
		if (!sourlist->load())
		{
			m_log.write("Error: file with currency from internet was not loaded");
			Sleep(tick_step);
			continue;
		}
		m_log.write("File was downloaded from: " + sourlist->urlPath());

		// copy data from internet-file to local-file to
		//
		destlist->copyData(*sourlist.get());
		
		// save data file with currency to local file
		//
		if (!destlist->save())
		{
			m_log.write("Error: local file with currency was not saved");
			Sleep(tick_step);
			continue;
		}
		m_log.write("File was saved to: " + destlist->localPath());
						
		// timeout to next read and write currency file 
		//
		for(int t = 0; t < tick_count; t++)
		{
			if (m_bStopService)
			{
				break;
			}

			Sleep(tick_step);
		}
	}

	m_log.write("Service stoped");
}

//--------------------------------------------------------------------------------------------