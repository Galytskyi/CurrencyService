#pragma once

#include "winsvc.h"

#include "Currency.h"
#include "Log.h"
#include "Options.h"

//--------------------------------------------------------------------------------------------

#define SERVICE_NAME "CurrencyService"

//--------------------------------------------------------------------------------------------

#define USER_DEFINED_CONTROL 130

//--------------------------------------------------------------------------------------------

class CService : public CObject
{
public:
	//
	//
	CService();
	virtual ~CService();

	// control
	//
	BOOL install();
	BOOL uninstall();

	BOOL start();
	BOOL stop();
	
	BOOL userCotnrol();
	
	// run
	//
	void run();

	DWORD status(DWORD& dwStatus) const;
	
private:

	// service log
	//
	static CLog m_log;

	// service main
	// 
	static SERVICE_STATUS m_serviceStatus;
	static SERVICE_STATUS_HANDLE m_serviceStatusHandle;
	static void WINAPI ServiceMain( DWORD /*argc*/, TCHAR* /*argv*/[] );

	// service handler
	//
	static HANDLE m_serviceControlEvent;
	static void WINAPI ServiceControlHandler(DWORD controlCode);
	
	// download data file with currency from internet and save to local file
	//
	static RunServiceParam m_runServiceParam;
	static BOOL m_bStopService;	
	static void updateFiles();
};

//--------------------------------------------------------------------------------------------

