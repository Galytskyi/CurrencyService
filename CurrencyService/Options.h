#pragma once

#include "Currency.h"

//--------------------------------------------------------------------------------------------

#define OPTIONS_FILE_NAME "Config.ini"

//--------------------------------------------------------------------------------------------

const char* const rsoParam[] = 
{ 
	"SourceType",
	"DestionType",
	"Timeout",
};

const int rsoParamCount = sizeof(rsoParam)/sizeof(char*);

enum rsoParamT : int
{
	sourType = 0,
	destType = 1,
	timeout = 2
};

//--------------------------------------------------------------------------------------------

class RunServiceParam : public CObject
{
public:
	//
	//
	RunServiceParam();
	virtual ~RunServiceParam();

	//
	//
	BOOL isValid() const;
	
	//
	//
	BOOL load();

	// getters
	//
	currency::FormatType sourType() const { return m_sourType; }
	currency::FormatType destType() const { return m_destType; }
	int timeout() const { return m_timeout; } // sec 
		
private:

	CString m_path; // options file
	BOOL init(const CString& path);
		
	currency::FormatType m_sourType;
	currency::FormatType m_destType;
	int m_timeout; // sec 
};

//--------------------------------------------------------------------------------------------

CString currentDir();

//--------------------------------------------------------------------------------------------


