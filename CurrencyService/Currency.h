#pragma once

#include <vector>

namespace currency
{

//--------------------------------------------------------------------------------------------

#define OUTPUT_FILE_PATH "CurrencyServiceOutput"

//--------------------------------------------------------------------------------------------

const char* const ItemParamName[] = 
{ 
	"r030",
	"txt",
	"rate",
	"cc",
	"exchangedate",
};

const int ItemParamCount = sizeof(ItemParamName)/sizeof(char*);

//--------------------------------------------------------------------------------------------

class Item
{

public:

	//
	//
	Item() {}
	virtual ~Item() {}

	//
	//
	CString value(int index) const
	{
		if (index < 0 || index >= ItemParamCount)
		{
			return CString();
		}

		return m_value[index];
	}

	void setValue(int index, const CString& value)
	{
		if (index < 0 || index >= ItemParamCount)
		{
			return;
		}

		m_value[index] = value;
	}
	
private:

	CString m_value[ItemParamCount];
};

//--------------------------------------------------------------------------------------------

enum FormatType : int
{
	UNDEFINE = -1,
	JSON = 0,
	XML = 1,
	CSV = 2,
};

const int FormatTypeCount = 3;

//--------------------------------------------------------------------------------------------

class CBaseList : public CObject
{
public:
	CBaseList();
	virtual ~CBaseList();

	virtual BOOL load() = 0;
	virtual BOOL save() = 0;
		
	void copyData(const CBaseList& other);

	CString urlPath() const { return m_urlPath; }
	CString localPath() const { return m_localPath; }
	
protected:
		
	BOOL downloadFile(CString& fileData);
		
	FormatType m_type;
	
	CString m_urlPath;
	CString m_localPath;
	
	std::vector<Item> m_items;
};

//--------------------------------------------------------------------------------------------

class CJSONList : public CBaseList 
{
public:
	//
	//
	CJSONList();
	virtual ~CJSONList();
	
	//
	//
	virtual BOOL load() override;
	virtual BOOL save() override;
};

//--------------------------------------------------------------------------------------------

class CXMLList : public CBaseList 
{
public:
	//
	//
	CXMLList();
	virtual ~CXMLList();
	
	//
	//
	virtual BOOL load() override;
	virtual BOOL save() override;
};

//--------------------------------------------------------------------------------------------

class CCSVList : public CBaseList 
{
public:
	//
	//
	CCSVList();
	virtual ~CCSVList();
	
	//
	//
	virtual BOOL load() override;
	virtual BOOL save() override;
};

//--------------------------------------------------------------------------------------------

std::shared_ptr<CBaseList> create(currency::FormatType type);

//--------------------------------------------------------------------------------------------

}


