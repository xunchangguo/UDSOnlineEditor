/ OEGOffice.cpp : Implementation of COEGOffice
#include "stdafx.h"
#include "OEGOffice.h"
#include "base64.h"
#include <string>

// COEGOffice

STDMETHODIMP COEGOffice::Test(void)
{
	MessageBox(L"Test()", L"", MB_OK);
	return S_OK;
}

#import "C:\\Program Files\\Common Files\\Microsoft Shared\\OFFICE12\\MSO.DLL" 
#import "C:\\Program Files\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.OLB"
#import "c:\\Program Files\\Microsoft Office\\Office12\\MSWORD.OLB" rename("FindText","_FindText"), rename("ExitWindows","_ExitWindows")   
//using namespace Word;

#import "..\\UDSOnlineEditor\\net.dll" no_namespace
bool Upload(BSTR docName,BSTR uploadUrl){

	bool bOK = false;

//	CoInitialize(NULL);
	CLSID clsid;
	CLSIDFromProgID(OLESTR("YRExchange.HttpRequest"),&clsid);
	CComPtr<IHttpRequest> httpRequest;
	httpRequest.CoCreateInstance(clsid);
	//MessageBox(GetForegroundWindow(),L"¿ªÊ¼ÉÏ´«ÎÄ¼þ","ÒÚéÅ¿Ø¼þÌáÊ¾",MB_OK);
	if (httpRequest!=NULL)
	{
		httpRequest->put_Method(L"POST");
		httpRequest->put_Url(uploadUrl);
		httpRequest->put_PostFile(docName);
        httpRequest->put_ShowRequestProgress(true);
        httpRequest->put_SaveResponseToFile(false);
        
		IHttpResponsePtr httpResponse = httpRequest->Send();
		if(httpResponse != NULL){
			if(httpResponse->GetStatusCode()==200) {
				//MessageBox(GetForegroundWindow(),L"±£´æÎÄ¼þµ½·þÎñÆ÷³É¹¦","ÒÚéÅ¿Ø¼þÌáÊ¾",MB_OK);
			}
			bOK = true;
			httpResponse.Release();
		} else {
			//MessageBox(NULL,L"ÉÏ´«ÎÄ¼þÊ§°Ü£¬ÇëÁªÏµÔËÎ¬ÈËÔ±","ÒÚéÅ¿Ø¼þ´íÎóÌáÊ¾",MB_OK);
		}
	}else{
		//MessageBox(NULL,L"ÉÏ´«¿Ø¼þµ÷ÓÃÊ§°Ü£¬ÇëÖØÐÂ×¢²á","ÒÚéÅ¿Ø¼þ´íÎóÌáÊ¾",MB_OK);
	}
	
//	CoUninitialize();	

	return bOK;
}

bool Download(BSTR downloadUrl, BSTR docName){

	bool bOK = false;
    // MessageBox(NULL, L"enter Download()", L"", MB_OK);
    // MessageBox(NULL, downloadUrl, L"downloadUrl", MB_OK);
    // MessageBox(NULL, docName, L"docName", MB_OK);
//	CoInitialize(NULL);
	CLSID clsid;
	CLSIDFromProgID(OLESTR("YRExchange.HttpRequest"),&clsid);
	CComPtr<IHttpRequest> httpRequest;
	httpRequest.CoCreateInstance(clsid);
	//MessageBox(GetForegroundWindow(),L"¿ªÊ¼ÉÏ´«ÎÄ¼þ","ÒÚéÅ¿Ø¼þÌáÊ¾",MB_OK);
	if (httpRequest!=NULL)
	{
		httpRequest->put_Method(L"POST");
		httpRequest->put_Url(downloadUrl);
		httpRequest->put_ResponseFile(docName);
        httpRequest->put_ShowRequestProgress(true);
        httpRequest->put_SaveResponseToFile(true);
        
		IHttpResponsePtr httpResponse = httpRequest->Send();
		if(httpResponse != NULL){
            //MessageBox(NULL, L"httpResponse != NULL", L"", MB_OK);
			if(httpResponse->GetStatusCode()==200) {
				//MessageBox(GetForegroundWindow(),L"±£´æÎÄ¼þµ½·þÎñÆ÷³É¹¦","ÒÚéÅ¿Ø¼þÌáÊ¾",MB_OK);
			}
			bOK = true;
			httpResponse.Release();
		} else {
            MessageBox(NULL, L"httpResponse == NULL", L"", MB_OK);
			//MessageBox(NULL,L"ÉÏ´«ÎÄ¼þÊ§°Ü£¬ÇëÁªÏµÔËÎ¬ÈËÔ±","ÒÚéÅ¿Ø¼þ´íÎóÌáÊ¾",MB_OK);
		}
	}else{
		//MessageBox(NULL,L"ÉÏ´«¿Ø¼þµ÷ÓÃÊ§°Ü£¬ÇëÖØÐÂ×¢²á","ÒÚéÅ¿Ø¼þ´íÎóÌáÊ¾",MB_OK);
	}
	
//	CoUninitialize();	

	return bOK;
}


// COEGOffice

_FramerControl* COEGOffice::GetFramerControl()
{
	IID iid = IID__FramerControl;
	_FramerControl *pFramerCtrl = NULL;
	this->GetDlgControl(IDC_FRAMERCONTROL1, iid, (void**)&pFramerCtrl);
	return pFramerCtrl;	
}

IUnknown* COEGOffice::GetUnknownCP()
{
	IUnknown* pUnknownCP = NULL;
	_FramerControl* pFramerCtrl = GetFramerControl();	
	pFramerCtrl->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnknownCP));
	pFramerCtrl->Release();

	return pUnknownCP;
}

IDispatch* COEGOffice::GetActiveDocument()
{
	IDispatch* pDispActiveDocument = NULL;

	_FramerControl* pFramerCtrl = GetFramerControl();	
	pFramerCtrl->get_ActiveDocument(&pDispActiveDocument);
	return pDispActiveDocument;
}


extern HANDLE v_hPrivateHeap;
#define CHECK_NULL_RETURN(v, e) if ((v) == NULL) return (e)
#define E_WIN32_LASTERROR        (0x80070000 | GetLastError()) // Assured Error with last Win32 code

void* DsoMemAlloc(DWORD cbSize)
{
    CHECK_NULL_RETURN(v_hPrivateHeap, NULL);
    return HeapAlloc(v_hPrivateHeap, HEAP_ZERO_MEMORY, cbSize);
}

void DsoMemFree(LPVOID ptr)
{
    if ((v_hPrivateHeap) && (ptr))
        HeapFree(v_hPrivateHeap, 0, ptr);
}

HRESULT DsoConvertToMBCSEx(LPCWSTR pwszUnicodeString, DWORD cbUniLen, LPSTR pszMbcsString, DWORD cbMbcsLen, UINT uiCodePage)
{
	DWORD cbRet;
	UINT iCode = CP_ACP;

	if (IsValidCodePage(uiCodePage))
		iCode = uiCodePage;

	CHECK_NULL_RETURN(pszMbcsString,     E_POINTER);
	pszMbcsString[0] = L'\0';

	CHECK_NULL_RETURN(pwszUnicodeString, E_POINTER);
	CHECK_NULL_RETURN(cbMbcsLen,         E_INVALIDARG);
	CHECK_NULL_RETURN(cbUniLen,          E_INVALIDARG);

	cbRet = WideCharToMultiByte(iCode, 0, pwszUnicodeString, -1, pszMbcsString, cbMbcsLen, NULL, NULL);
	if (cbRet == 0)	return E_WIN32_LASTERROR;

	pszMbcsString[cbRet] = '\0';
	return S_OK;
}

LPSTR DsoConvertToMBCS(LPCWSTR pwszUnicodeString, UINT& cbnew)
{
	LPSTR psz = NULL;
	UINT cblen;

    if ((pwszUnicodeString) && 
        ((cblen = lstrlenW(pwszUnicodeString)) > 0))
	{
		cbnew = ((cblen + 1) * sizeof(WCHAR));
		if ((psz = (LPSTR)DsoMemAlloc(cbnew)) != NULL) 
		{
			if (FAILED(DsoConvertToMBCSEx(pwszUnicodeString, cblen, psz, cbnew, GetACP())))
            {
			    DsoMemFree(psz); psz = NULL;
            }
		}
	}

	return psz;
}

HRESULT DsoConvertToUnicodeEx(LPCSTR pszMbcsString, DWORD cbMbcsLen, LPWSTR pwszUnicode, DWORD cbUniLen, UINT uiCodePage)
{
	DWORD cbRet;
	UINT iCode = CP_ACP;

	if (IsValidCodePage(uiCodePage))
		iCode = uiCodePage;

	CHECK_NULL_RETURN(pwszUnicode,    E_POINTER);
	pwszUnicode[0] = L'\0';

	CHECK_NULL_RETURN(pszMbcsString,  E_POINTER);
	CHECK_NULL_RETURN(cbMbcsLen,      E_INVALIDARG);
	CHECK_NULL_RETURN(cbUniLen,       E_INVALIDARG);

	cbRet = MultiByteToWideChar(iCode, 0, pszMbcsString, cbMbcsLen, pwszUnicode, cbUniLen);
	if (cbRet == 0)	return E_WIN32_LASTERROR;

	pwszUnicode[cbRet] = L'\0';
	return S_OK;
}

LPWSTR DsoConvertToLPWSTR(LPCSTR pszMbcsString)
{
	LPWSTR pwsz = NULL;
	UINT cblen, cbnew;

	if ((pszMbcsString) && 
        ((cblen = lstrlenA(pszMbcsString)) > 0))
	{
		cbnew = ((cblen + 1) * sizeof(WCHAR));
		if ((pwsz = (LPWSTR)DsoMemAlloc(cbnew)) != NULL) 
		{
			if (FAILED(DsoConvertToUnicodeEx(pszMbcsString, cblen, pwsz, cbnew, GetACP())))
            {
			    DsoMemFree(pwsz);
                pwsz = NULL;
            }
		}
	}

	return pwsz;
}

BSTR DsoConvertToBSTR(LPCSTR pszMbcsString)
{
	BSTR bstr = NULL;
    LPWSTR pwsz = DsoConvertToLPWSTR(pszMbcsString);
	if (pwsz)
	{
	    bstr = SysAllocString(pwsz);
	    DsoMemFree(pwsz);
    }
	return bstr;
}

CComBSTR GetDocFileType(const CComBSTR& bstrFileName)
{
	CComBSTR bstrFileType = L"";
	HRESULT hr;
	hr = StgIsStorageFile(bstrFileName);
	if ( SUCCEEDED(hr) ){
		//MessageBox(NULL, L"Is Storage File", L"", MB_OK);
		// Open the template for read access only...
		IStorage *pstgTemplate = NULL;
        hr = StgOpenStorage(bstrFileName, NULL, 
            //(STGM_READ | STGM_SHARE_DENY_WRITE | STGM_TRANSACTED),
			(STGM_READ | STGM_SHARE_EXCLUSIVE),
             NULL, 0, &pstgTemplate);
		//char buff[100];
		//itoa((int)hr, buff, 16);
		//CComBSTR bstrError = buff;
		//MessageBox(NULL, bstrError, L"", MB_OK);
		if ( SUCCEEDED(hr) ){
//		MessageBox(NULL, L"2", L"", MB_OK);
			 // We get the CLSID from the template...
			CLSID clsid;
			hr = ReadClassStg(pstgTemplate, &clsid);
			if ( SUCCEEDED(hr) ){
//		MessageBox(NULL, L"3", L"", MB_OK);
				BSTR  dddd;
				StringFromCLSID(clsid,&dddd);
//		MessageBox(NULL, CComBSTR(dddd), L"", MB_OK);

				char * pstrNameTemp1 = NULL;
				LPWSTR  pstrNameTemp2;
				if ((dddd) && (SysStringLen(dddd) > 0)){
					pstrNameTemp2 = SysAllocString(dddd);
					UINT cbnew = 0;
					pstrNameTemp1 = DsoConvertToMBCS(pstrNameTemp2, cbnew);
				}

				if(0 == strcmp(pstrNameTemp1,"{00020906-0000-0000-C000-000000000046}")
					|| 0 == strcmp(pstrNameTemp1,"{F4754C9B-64F5-4B40-8AF4-679732AC0607}"))
					bstrFileType = L"doc";
				else if(0 == strcmp(pstrNameTemp1,"{00020820-0000-0000-C000-000000000046}")
					|| 0 == strcmp(pstrNameTemp1,"{00020830-0000-0000-C000-000000000046}"))
					bstrFileType = L"xls";
				else if(0 == strcmp(pstrNameTemp1,"{64818D10-4F9B-11CF-86EA-00AA00B929E8}")
					|| 0== strcmp(pstrNameTemp1,"{64818D11-4F9B-11CF-86EA-00AA00B929E8}")
					|| 0== strcmp(pstrNameTemp1,"{CF4F55F4-8F87-4D47-80BB-5808164BB3F8}")
					)
					bstrFileType = L"ppt";
				DsoMemFree(pstrNameTemp1);
			}
//		MessageBox(NULL, bstrFileType, L"", MB_OK);

		} else {
            MessageBox(NULL, L"StgOpenStorage() fail!", L"", MB_OK);
        }
        
        if ( pstgTemplate ){
            pstgTemplate->Release();
            pstgTemplate = NULL;
        }
	} else {
		MessageBox(NULL, L"Is NOT Storage File", L"", MB_OK);
    }
    
	return bstrFileType;
}

STDMETHODIMP COEGOffice::OpenDocument(BSTR filename, VARIANT_BOOL bReadonly, VARIANT_BOOL* opened)
{
	CComBSTR bstrFileType = GetDocFileType(filename);

	//CComVariant varProgId(VT_ERROR);
	CComVariant varProgId(VT_ERROR);
	if ( bstrFileType.Length() >0 )
		varProgId = bstrFileType;
	//MessageBox(bstrFileType, L"", MB_OK);
	_FramerControl* pFramerControl = GetFramerControl();
	CComVariant varDocument(filename);
	CComVariant varReadOnly(bReadonly);
	CComVariant varWebUsername(L"");
	CComVariant varWebPassword(L"");
	HRESULT hr = pFramerControl->Open(varDocument, varReadOnly, varProgId, varWebUsername, varWebPassword);
	if ( SUCCEEDED(hr) )
		*opened = TRUE;
	else
		*opened = FALSE;
	return hr;
}

STDMETHODIMP COEGOffice::CloseDocument(VARIANT_BOOL* closed)
{
	_FramerControl* pFramerControl = GetFramerControl();
	HRESULT hr = pFramerControl->Close();
	if ( SUCCEEDED(hr) )
		*closed = TRUE;
	else
		*closed = FALSE;
	return hr;
}

STDMETHODIMP COEGOffice::SaveDocument(VARIANT_BOOL* saved)
{
	_FramerControl* pFramerControl = GetFramerControl();
	CComVariant varSaveAsDocument;
	CComVariant varOverwriteExisting(TRUE);
	CComVariant varWebUsername(L"");
	CComVariant varWebPassword(L"");
	HRESULT hr = pFramerControl->Save(varSaveAsDocument, varOverwriteExisting, varWebUsername, varWebPassword);
	if ( SUCCEEDED(hr) )
		*saved = TRUE;
	else
		*saved = FALSE;
	return hr;
}

STDMETHODIMP COEGOffice::get_ShowRevisions(VARIANT_BOOL* pVal)
{
	IDispatch* pDispActiveDocument = GetActiveDocument();
	if ( pDispActiveDocument != NULL ){
		Word::_DocumentPtr doc(pDispActiveDocument);
		if ( doc != NULL )
			*pVal = doc->GetShowRevisions();
	}

	return S_OK;
}

STDMETHODIMP COEGOffice::put_ShowRevisions(VARIANT_BOOL newVal)
{
	IDispatch* pDispActiveDocument = GetActiveDocument();
	if ( pDispActiveDocument != NULL ){
		Word::_DocumentPtr doc(pDispActiveDocument);
		if ( doc != NULL )		
			doc->PutShowRevisions(newVal);
	}

	return S_OK;
}

STDMETHODIMP COEGOffice::get_ShowComments(VARIANT_BOOL* pVal)
{
	IDispatch* pDispActiveDocument = GetActiveDocument();
	if ( pDispActiveDocument != NULL ){
		Word::_DocumentPtr doc(pDispActiveDocument);
		if ( doc != NULL ){	
			Word::ViewPtr view = doc;
			if ( view != NULL ){
				*pVal = view->GetShowComments();
			}
		}
	}

	return S_OK;
}

STDMETHODIMP COEGOffice::put_ShowComments(VARIANT_BOOL newVal)
{
	IDispatch* pDispActiveDocument = GetActiveDocument();
	if ( pDispActiveDocument != NULL ){
		Word::_DocumentPtr doc(pDispActiveDocument);
		if ( doc != NULL ){
			Word::ViewPtr view = doc;
			if ( view != NULL ){
				view->PutShowComments(newVal);
			}
		}
	}

	return S_OK;
}

STDMETHODIMP COEGOffice::UploadToUDS(BSTR uploadURL, VARIANT_BOOL* uploaded)
{
    _FramerControl* pFramerCtrl = GetFramerControl();
    BSTR documentFullName;
    pFramerCtrl->get_DocumentFullName(&documentFullName);

    //MessageBox(CComBSTR(documentFullName), L"documentFullName", MB_OK);

    IDispatch* pApplication = NULL;
    //pFramerCtrl->get_GetApplication(&pApplication);
    if ( pApplication != NULL ){
        // Word::_ApplicationPtr wordApp(pApplication);
        // if ( wordApp != NULL ){
        //     MessageBox(L"Is Word", L"", MB_OK);
        // } else {
        //     MessageBox(L"Is Not Word", L"", MB_OK);
        // }
    }
    
	IDispatch* pDispActiveDocument = GetActiveDocument();
	if ( pDispActiveDocument != NULL ){
		if ( Upload(documentFullName, uploadURL) ){
			MessageBox(L"Upload success!");
		} else {
			MessageBox(L"Upload failure!");
		}
	}

	return S_OK;
}

STDMETHODIMP COEGOffice::OpenFromUDS(BSTR downloadURL, BSTR fileName, VARIANT_BOOL* downloaded)
{
    CComBSTR docFileName = L"c:\\test234.doc";

    char szTempPath[1024];
    ::GetTempPathA(1024, szTempPath);
    char szTempLongPath[1024];
    ::GetLongPathNameA(szTempPath, szTempLongPath, 1024);

    GUID guid;
    HRESULT hr = ::CoCreateGuid(&guid);
    char buf[64] = {0};
    _snprintf(buf, sizeof(buf)
              , "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
              , guid.Data1
              , guid.Data2
              , guid.Data3
              , guid.Data4[0], guid.Data4[1]
              , guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
              , guid.Data4[6], guid.Data4[7]
        );
    std::string strTempPath(szTempLongPath);
    std::string strGuid(buf);
    std::string strDocFileName = strTempPath + strGuid;
    
    docFileName = CComBSTR(strDocFileName.c_str());
    docFileName.Append(L".");
    docFileName.Append(CComBSTR(fileName));
    //MessageBox(docFileName, L"", MB_OK);

    if ( Download(downloadURL, docFileName) ){
        IDispatch* pDispActiveDocument = GetActiveDocument();
        if ( pDispActiveDocument != NULL ){
            //Word::_DocumentPtr doc(pDispActiveDocument);
            VARIANT_BOOL bClosed = false;
            CloseDocument(&bClosed);
        }
    
        VARIANT_BOOL bOpened = false;
        OpenDocument(docFileName, false, &bOpened);
        //MessageBox(L"Download success!");
    } else {
			MessageBox(L"Download failure!");
	}

	return S_OK;
}

CComBSTR ToBase64(const CComBSTR& bstrText)
{
	UINT nLen = 0;
	unsigned char* pszText = (unsigned char*)DsoConvertToMBCS(bstrText, nLen);
	std::string strText = base64_encode(pszText, nLen);
	CComBSTR bstrRet = DsoConvertToBSTR(strText.c_str());
	DsoMemFree(pszText);
	return bstrRet;
}

CComBSTR FromBase64(const CComBSTR& bstrText)
{
	UINT nLen = 0;
	char* pszText = DsoConvertToMBCS(bstrText, nLen);
	std::string strRet = base64_decode(std::string(pszText));
	CComBSTR bstrRet = DsoConvertToBSTR(strRet.c_str());
	DsoMemFree(pszText);
	return bstrRet;
}

STDMETHODIMP COEGOffice::get_DocComments(BSTR* pVal)
{
	CComBSTR bstrComments = L"{\n\t'comments':[";

	IDispatch* pDispActiveDocument = GetActiveDocument();
	if ( pDispActiveDocument != NULL ){
		Word::_DocumentPtr doc(pDispActiveDocument);
		if ( doc != NULL ){
			Word::CommentsPtr comments = doc->GetComments();
			
			size_t nComments = comments->GetCount();
			for ( size_t i = 0 ; i < nComments; i++ ){
				Word::CommentPtr comment = comments->Item(i + 1);
				bstrComments += L"{\n";

				// 批注内容 
				Word::RangePtr r_commentRange = comment->GetRange();
				CComBSTR bstrCommentContext = BSTR(r_commentRange->GetText());

				//CComBSTR bstrCommentContext64 = ToBase64(bstrCommentContext);
				//CComBSTR bstrOrig = FromBase64(bstrCommentContext64);
				//MessageBox(bstrCommentContext64, L"Base64", MB_OK);
				//MessageBox(bstrOrig, L"Orig", MB_OK);

				bstrComments += L"\t\t'comment': ";
				bstrComments += L"'";
				bstrComments += bstrCommentContext + L"',\n";

				// 批注原文
				Word::RangePtr r_commentScope = comment->GetScope();
				CComBSTR bstrCommentScope = BSTR(r_commentScope->GetText());

				bstrComments += L"\t\t'text': ";
				bstrComments += L"'";
				bstrComments += bstrCommentScope + L"'\n";
				if ( i < nComments - 1)
					bstrComments += L"\t\t},";
				else
					bstrComments += L"\t\t}";
			}
			bstrComments += L"]\n}\n";
			*pVal = ::SysAllocString(bstrComments);
		}
	}

	return S_OK;
}

#define wdNoRevision 0
#define wdRevisionInsert 1
#define wdRevisionDelete 2

STDMETHODIMP COEGOffice::get_DocRevisions(BSTR* pVal)
{
	CComBSTR bstrRevisions = L"{\n\t'revisions':[";

	IDispatch* pDispActiveDocument = GetActiveDocument();
	if ( pDispActiveDocument != NULL ){
		Word::_DocumentPtr doc(pDispActiveDocument);

		if ( doc != NULL ){
			Word::RevisionsPtr revisions = doc->GetRevisions();
			size_t nRevisions = revisions->GetCount();
			for ( size_t i = 0 ; i < nRevisions; i++ ){
				Word::RevisionPtr revision = revisions->Item(i + 1);

				// 修订内容 和 修订类型
				Word::RangePtr r_revisionRange = revision->GetRange();
				CComBSTR bstrRevisionContext = BSTR(r_revisionRange->GetText());
				int revisionType = revision->GetType();

				bstrRevisions  += L"{\n";

				CComBSTR bstrRevisionType = L"Unknown";
				switch ( revisionType ){
				case wdRevisionInsert:
				{
					bstrRevisionType = L"Insert";
				} break;
				case wdRevisionDelete:
				{
					bstrRevisionType = L"Delete";
				} break;
				}

				bstrRevisions += L"\t\t'type': ";
				bstrRevisions += L"'";
				bstrRevisions += bstrRevisionType + L"',\n";

				bstrRevisions += L"\t\t'context': ";
				bstrRevisions += L"'";
				bstrRevisions += bstrRevisionContext + L"'\n";

				if ( i < nRevisions - 1)
					bstrRevisions += L"\t\t},";
				else
					bstrRevisions += L"\t\t}";
			}
			bstrRevisions += L"]\n}\n";
			*pVal = ::SysAllocString(bstrRevisions);
		}
	}
	return S_OK;
}

HRESULT __stdcall COEGOffice::OnDocumentClosedFramercontrol1()
{
    DeleteFile(m_bstrFileName);
    
	return 0;
}

HRESULT __stdcall COEGOffice::BeforeDocumentClosedFramercontrol1(LPDISPATCH Document, BOOL* Cancel)
{
    _FramerControl* pFramerCtrl = GetFramerControl();
    BSTR documentFullName;
    pFramerCtrl->get_DocumentFullName(&documentFullName);
    m_bstrFileName = documentFullName;
    
	return 0;
}
