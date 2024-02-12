#pragma once

//--------------------------------------------------------------------------------------------

#define LOG_FILE_NAME "CurrencyServiceLog.txt"

//--------------------------------------------------------------------------------------------

class CLog : public CObject
{
public:
	CLog();
	virtual ~CLog();

	static void write(const CString& msg);

private:

	static CString m_path;
};

//--------------------------------------------------------------------------------------------


