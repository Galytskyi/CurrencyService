CurrencyService
===============

CurrencyService is a service for downloading a list of exchange rates of the National Bank of Ukraine, and saving this list to a file on the local computer


CurrencyService provides the following functionality
----------------------------------------------------

- Using the API (https://bank.gov.ua/en/open-data/api-dev), the `CurrencyService` receives lists of currencies and their exchange rates in `JSON`, `XML` or` CSV` format (from the Internet). Then automatically saves this data in a file on the local computer in the `JSON`, `XML` or` CSV` formats.
- The service performs this operation with a specified period of time (default value one time per second).
- All events and errors occurring during the working of the service are recorded in the log: `CurrencyServiceLog.txt`.
- To configure the service, use the config file: `Config.ini`.


Base Commands
-------------

	CurrencyService install
	CurrencyService uninstall

	CurrencyService start
	CurrencyService user_control
	CurrencyService stop

Configuration
-------------

To configure the service, use the settings file `Config.ini`.
The Config.ini file contains three parameters, sample contents:

    SourceType:json
    DestionType:xml
    Timeout:1

`SourceType` parameter – specifies the format for obtaining a list of currencies from the Internet. Can take one of the following string values:

	json
	xml
	csv

`DestionType` parameter – specifies the format for saving the list of currencies to a file on the local computer. Can take one of the following string values:

	json
	xml
	csv

`Timeout` parameter – specifies the time period (in seconds) after which the lists of currencies will be updated. Can take any int value greater than zero.

In order for the settings to take effect, after editing `Config.ini` file, you must restart the service; to do this, use the commands:

     CurrencyService start
     CurrencyService stop

Log
---

- While the service is running, as well as installing the service, starting the service, stopping the service, and uninstalling the service, all events will be recorded in the log. 
- Any errors that occur during the working of the service are also recorded in the log.

Example of the log:

	11:02:2024 21:17:17: Service Installed Successfully
	11:02:2024 21:17:23: Service Started
	11:02:2024 21:17:23: File was downloaded from: https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange
	11:02:2024 21:17:23: File was saved to: D:\CurrencyServiceOutput.xml
	11:02:2024 21:17:24: File was downloaded from: https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange
	11:02:2024 21:17:24: File was saved to: D:\CurrencyServiceOutput.xml
	11:02:2024 21:17:25: File was downloaded from: https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange
	11:02:2024 21:17:25: File was saved to: D:\CurrencyServiceOutput.xml
	11:02:2024 21:17:26: Service ControlCode: 130
	11:02:2024 21:17:26: File was downloaded from: https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange
	11:02:2024 21:17:26: File was saved to: D:\CurrencyServiceOutput.xml
	11:02:2024 21:17:28: File was downloaded from: https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange
	11:02:2024 21:17:28: File was saved to: D:\CurrencyServiceOutput.xml
	11:02:2024 21:17:29: File was downloaded from: https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange
	11:02:2024 21:17:29: File was saved to: D:\CurrencyServiceOutput.xml
	11:02:2024 21:17:30: Service is stoping
	11:02:2024 21:17:30: Service stoped
	11:02:2024 21:17:40: Service Removed Successfully

Documentation and Used libraries
--------------------------------
During development `Chilkat` library was used. This library is working with transformation of formats: `JSON`, `XML` or` CSV`.

- `JSON` <https://www.example-code.com/mfc/json.asp>
- `XML` <https://www.example-code.com/mfc/xml.asp>
- `CSV` <https://www.example-code.com/mfc/csv.asp>

