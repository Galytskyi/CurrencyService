// Options.cpp : implementation file
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>

#include "CurrencyService.h"
#include "Options.h"

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

RunServiceParam::RunServiceParam() 
	: m_sourType (currency::FormatType::UNDEFINE)
	, m_destType (currency::FormatType::UNDEFINE)
	, m_timeout (0) // sec 
{
}

//--------------------------------------------------------------------------------------------

RunServiceParam::~RunServiceParam()
{
}

//--------------------------------------------------------------------------------------------

BOOL RunServiceParam::isValid() const
{
	using namespace currency;

	if (m_sourType == FormatType::UNDEFINE || m_destType == FormatType::UNDEFINE)
	{
		return FALSE;
	}

	if (m_timeout <= 0)
	{
		return FALSE;
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------------

BOOL RunServiceParam::load()
{
	using namespace std;

	m_path = currentDir() + OPTIONS_FILE_NAME;

	// check : is option file exist ?
	//
	ifstream file(m_path);
	if(file.fail())
	{
		init(m_path);
	}

	// read params from option file;
	//
	string line;
	
	ifstream in(m_path); // окрываем файл для чтения
	if (!in.is_open())
	{
		return FALSE;
	}

	while (getline(in, line))
	{
		for (int p = 0; p < rsoParamCount; p++)
		{
			basic_string <char>::size_type start, end; 
			start = line.find(rsoParam[p], 0);
			end = line.find(":", 0);

			if (start == string::npos || end == string::npos)
				continue;

			string name = line.substr(0, end);
			string param = line.substr(end+1, line.length());
						
			// SourceType
			//
			if (!name.compare(rsoParam[rsoParamT::sourType]))
			{
				if (!param.compare("json"))
					m_sourType = currency::FormatType::JSON;

				if (!param.compare("xml"))
					m_sourType = currency::FormatType::XML;

				if (!param.compare("csv"))
					m_sourType = currency::FormatType::CSV;
			}

			// DestionType
			//
			if (!name.compare(rsoParam[rsoParamT::destType]))
			{
				if (!param.compare("json"))
					m_destType = currency::FormatType::JSON;

				if (!param.compare("xml"))
					m_destType = currency::FormatType::XML;

				if (!param.compare("csv"))
					m_destType = currency::FormatType::CSV;
			}
			
			// Timeout
			//
			if (!name.compare(rsoParam[rsoParamT::timeout]))
			{
				m_timeout = atoi(param.c_str());
			}
		}
	}
	
	in.close();
	
	return TRUE;
}

//--------------------------------------------------------------------------------------------

BOOL RunServiceParam::init(const CString& path)
{
	if (path.IsEmpty())
	{
		return FALSE;
	}

	std::ofstream out(path);
	if (!out.is_open())
	{
		return FALSE;
	}

	out << rsoParam[rsoParamT::sourType] << ":xml" << std::endl;
	out << rsoParam[rsoParamT::destType] << ":xml" << std::endl;
	out << rsoParam[rsoParamT::timeout] << ":1" << std::endl;

	out.close();
	
	return TRUE;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

CString currentDir()
{
	WCHAR path[MAX_PATH];
    DWORD size = GetModuleFileNameW(NULL, path, MAX_PATH);

	int sp=sizeof(path);
    while (sp!=0) 
	{
        if(path[sp-1]=='\\') 
		{
            path[sp]='\0';
            break;
        }
        sp--;
    }

	return CString(path);
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

