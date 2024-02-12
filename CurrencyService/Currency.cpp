// Currency.cpp : implementation file
//

#include "stdafx.h"
#include "afxinet.h"

#include <iostream>
#include <fstream>

#include "CurrencyService.h"
#include "Currency.h"
#include "Options.h"

#include "Chilkat\include\CkJsonArray.h"
#include "Chilkat\include\CkJsonObject.h"
#include "Chilkat\include\CkXml.h"
#include "Chilkat\include\CkCsv.h"

namespace currency
{

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

CBaseList::CBaseList() : m_type(FormatType::UNDEFINE) 
{
}

//--------------------------------------------------------------------------------------------

CBaseList::~CBaseList()
{ 
}

//--------------------------------------------------------------------------------------------

BOOL CBaseList::downloadFile(CString& fileData)
{
	fileData.Empty();
		
	// check URL
	//
	if (m_urlPath.IsEmpty())
	{
		return FALSE;
	}

	// load file from internet
	//
	CInternetSession session;
	CInternetFile* file = NULL;

	try
	{
		// try to open URL
		//
		file = dynamic_cast<CInternetFile*>(session.OpenURL(m_urlPath)); 
		if (file)
		{
			// load data
			//
			const int tmpBuffSize = 4096;
			char tmpBuff[tmpBuffSize + 1];
			
			UINT bytes = 0;
			do
			{
				bytes = file->Read(tmpBuff, tmpBuffSize);
				if(bytes)
				{
					tmpBuff[bytes] = 0;
					fileData += CA2T(tmpBuff);
				}
			} while(bytes);

			file->Close();
			delete file;
		}
		else 
		{
			return FALSE;	// invalid server
		}
	} 
	catch(...)					
	{
		// Error !!!
		// Check internet connection or ... 
		return FALSE;
	}
		   
	return TRUE;
}

//--------------------------------------------------------------------------------------------

void CBaseList::copyData(const CBaseList& other)
{
	if (this == &other)
		return;

	m_items = other.m_items;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

CJSONList::CJSONList() 
{
	m_type = FormatType::JSON;
	m_urlPath = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?json";
	m_localPath = currentDir() + OUTPUT_FILE_PATH + CString(".json");
}

//--------------------------------------------------------------------------------------------

CJSONList::~CJSONList()
{ 
}

//--------------------------------------------------------------------------------------------

BOOL CJSONList::load()
{
	CString fileData;

	// download data from internet
	//
	if (!downloadFile(fileData))
	{
		return FALSE;
	}

	if (fileData.IsEmpty())
	{
		return FALSE;
	}

	m_items.clear();

	// fill m_items 
	//
	CkJsonArray jsonArray;
    jsonArray.Load(fileData);

	int count = jsonArray.get_Size();
    for (int i = 0; i < count; i++)  
	{
		Item item;

        CkJsonObject *jsonObj = jsonArray.ObjectAt(i);
        
		for(int p = 0; p < ItemParamCount; p++)
		{
			item.setValue(p, jsonObj->stringOf(ItemParamName[p]));
		}

        delete jsonObj;
		
		m_items.push_back(item);
    }
			   
	return TRUE;
}

//--------------------------------------------------------------------------------------------

BOOL CJSONList::save()
{
	if (m_localPath.IsEmpty())
	{
		return FALSE;
	}

	CkJsonObject json;

	json.AddArrayAt(-1,"exchange");

	// add the array for arrayJson
	CkJsonArray *arrayJson = json.ArrayAt(json.get_Size() - 1);	
		
	int count = m_items.size();
	for(int i = 0; i < count; i++)
	{
		// append a new/empty ojbect to the end of the arrayJson.
		arrayJson->AddObjectAt(-1); 

		// get the object that was just appended
		CkJsonObject *jsonCurrency = arrayJson->ObjectAt(arrayJson->get_Size() - 1); 

		for(int p = 0; p < ItemParamCount; p++)
		{
			jsonCurrency->AddStringAt(-1, ItemParamName[p], m_items[i].value(p));
		}

		delete jsonCurrency;
	}
	
	delete arrayJson;
	
	json.put_EmitCompact(false);
	
	std::ofstream out(m_localPath);
	if (!out.is_open())
	{
		return FALSE;
	}
	
	out << json.emit();
	out.close();
	
	return TRUE;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

CXMLList::CXMLList() 
{
	m_type = FormatType::XML;
	m_urlPath = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange";
	m_localPath = currentDir() + OUTPUT_FILE_PATH + CString(".xml");
}

//--------------------------------------------------------------------------------------------

CXMLList::~CXMLList()
{ 
}

//--------------------------------------------------------------------------------------------

BOOL CXMLList::load()
{
	CString fileData;

	// download data from internet
	//
	if (!downloadFile(fileData))
	{
		return FALSE;
	}

	if (fileData.IsEmpty())
	{
		return FALSE;
	}

	m_items.clear();

	// fill m_items 
	//
	CkXml xml;

	BOOL success = xml.LoadXml(fileData);

	int count = xml.get_NumChildren() - 1;

	for (int i = 0; i <= count; i++) 
	{
		CkXml *currencyObj = xml.GetChild(i);

		Item item;

		int scount = currencyObj->get_NumChildren() - 1;
		for (int si = 0; si <= scount; si++) 
		{
			CkXml *subCurrencyObj = currencyObj->GetChild(si);

			for(int p = 0; p < ItemParamCount; p++)
			{
				if (!strcmp(subCurrencyObj->tag(), ItemParamName[p]))
				{
					item.setValue(p, subCurrencyObj->content());
				}
			}

			delete subCurrencyObj;
		}
				
		delete currencyObj;

		m_items.push_back(item);
	}
		   
	return TRUE;
}

//--------------------------------------------------------------------------------------------

BOOL CXMLList::save()
{
	if (m_localPath.IsEmpty())
	{
		return FALSE;
	}

	CkXml xml;

	xml.put_Tag("exchange");

	int count = m_items.size();
	for(int i = 0; i < count; i++)
	{
		CkXml *childNode = 0;
		childNode = xml.NewChild("currency","");

		for(int p = 0; p < ItemParamCount; p++)
		{
			childNode->NewChild2(ItemParamName[p], m_items[i].value(p));
		}

		delete childNode;
	}

	std::ofstream out(m_localPath);
	if (!out.is_open())
	{
		return FALSE;
	}
	
	out << xml.getXml();
	out.close();

	return TRUE;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

CCSVList::CCSVList() 
{
	m_type = FormatType::XML;
	m_urlPath = CString();
	m_localPath = currentDir() + OUTPUT_FILE_PATH + CString(".csv");
}

//--------------------------------------------------------------------------------------------

CCSVList::~CCSVList()
{ 
}

//--------------------------------------------------------------------------------------------

BOOL CCSVList::load()
{
	CString fileData;

	if (!downloadFile(fileData))
	{
		return FALSE;
	}

	if (fileData.IsEmpty())
	{
		return FALSE;
	}

	m_items.clear();

	return TRUE;
}

//--------------------------------------------------------------------------------------------

BOOL CCSVList::save()
{
	if (m_localPath.IsEmpty())
	{
		return FALSE;
	}

	CkCsv csv;

	// create data
	//
	int count = m_items.size();
	for(int i = 0; i < count; i++)
	{
		for(int p = 0; p < ItemParamCount; p++)
		{
			csv.SetCell(i, p, m_items[i].value(p));
		}
	}

	// save data CSV to file:
	//
    BOOL result = csv.SaveFile(m_localPath);
    if (!result) 
	{
		return FALSE;
    }

	return TRUE;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

std::shared_ptr<CBaseList> create(currency::FormatType type)
{
	std::shared_ptr<CBaseList> ptr;

	switch(type)
	{
		case currency::FormatType::JSON: ptr = std::make_shared<CJSONList>(); break;
		case currency::FormatType::XML: ptr = std::make_shared<CXMLList>(); break;
		case currency::FormatType::CSV: ptr = std::make_shared<CCSVList>(); break;
	}

	return ptr;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

}