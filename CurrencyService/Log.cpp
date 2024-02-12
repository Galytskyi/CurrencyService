// Log.cpp : implementation file
//

#include "stdafx.h"

#include "CurrencyService.h"
#include "Service.h"
#include "Log.h"
#include "Options.h"

//--------------------------------------------------------------------------------------------

CString CLog::m_path;	

//--------------------------------------------------------------------------------------------

CLog::CLog()
{
	m_path = currentDir() + CString(LOG_FILE_NAME);
}

//--------------------------------------------------------------------------------------------

CLog::~CLog()
{
}

//--------------------------------------------------------------------------------------------

void CLog::write(const CString& msg)
{
	if (m_path.IsEmpty())
	{
		return;
	}

	FILE *fileHandle = fopen(m_path, "a");
	if (!fileHandle)
	{
		return;
	}

	CString strMsg;

	// get time msg
	//
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	strMsg.Format(	"%02d:%02d:%04d %02d:%02d:%02d: " + msg + "\n", 
					SystemTime.wDay, SystemTime.wMonth, SystemTime.wYear,  
					SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

	// write msg
	//

	fprintf(fileHandle, strMsg);
	fclose(fileHandle);
	
	// print 
	//
	std::cout << strMsg << std::endl;
}

//--------------------------------------------------------------------------------------------
