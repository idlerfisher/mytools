#pragma once
#include <afxinet.h>

#define HTTP_RELEASE \
    if (pHttpConn) {pHttpConn->Close(); delete pHttpConn; pHttpConn = NULL;}\
    if (pHttpFile) {pHttpFile->Close(); delete pHttpFile; pHttpFile = NULL;}

//callback
typedef void (*CbHttpFinished)(bool bResult, const CString& strOut);
typedef void (*CbHttpProgress)(DWORD dwVal, DWORD dwMax);

typedef std::map<CString, CString> FORMDATAMAP;

class CHttpClient
{
public:
    CHttpClient();
    ~CHttpClient();

    void    RegisterCallBack(CbHttpProgress funCbHttpProgress = NULL, CbHttpFinished funCbHttpFinished = NULL);
    bool    HttpGet(const CString& strUrl, CString& strOut);
    bool    HttpPost(const CString& strUrl, const FORMDATAMAP& formDataMap, CString& strOut);
    bool    HttpPost(const CString& strUrl, const CString& strFormData, CString& strOut);
    bool    UploadFile(const CString& strUrl, const CString& strFilePath, const CString& strOut);
    bool    DownloadFile(const CString& strUrl, const CString& strPath, const CString& strName="");

    static int      HexToI(const char *s);
    static CString  UrlEncode(LPCTSTR szInStr);
    static CString  UrlDecode(LPCTSTR szInStr);
    static bool     Utf8ToMb(CString& mb, const CString& utf8);
    static CString  UrlEncodeJoinFormData(const FORMDATAMAP& data);
    static bool     MbToUtf8(CString& utf8, const CString& mb);

private:
    bool    UploadFileEx(CHttpFile* pHttpFile, const CString& strFilePath, CString& strOut);

private:
    CInternetSession m_internetSession;
    CbHttpFinished m_funHttpFinished;
    CbHttpProgress m_funHttpProgress;
};

class CAsyncHttpClient
{
public:
    CAsyncHttpClient()
    {
        Reset();
    }

    void Get(const CString& strUrl, CbHttpProgress funProgress = NULL, CbHttpFinished funFinished = NULL);
    void Post(const CString& strUrl, const FORMDATAMAP& formDataMap, CbHttpProgress funProgress = NULL, CbHttpFinished funFinished = NULL);
    void Post(const CString& strUrl, const CString& strFormData, CbHttpProgress funProgress = NULL, CbHttpFinished funFinished = NULL);
    void UploadFile(const CString& strUrl, const CString& strFilePath, CbHttpProgress funProgress = NULL, CbHttpFinished funFinished = NULL);
    void DownloadFile(const CString& strUrl, const CString& strPath, const CString& strName="", CbHttpProgress funProgress = NULL, CbHttpFinished funFinished = NULL);

private:
    static DWORD WINAPI CbGet(LPVOID lpParameter);
    static DWORD WINAPI CbPost(LPVOID lpParameter);
    static DWORD WINAPI CbUploadFile(LPVOID lpParameter);
    static DWORD WINAPI CbDownloadFile(LPVOID lpParameter);
    void Reset();

private:
    //以下变量用来保存线程需要用的数据
    CString m_strUrl;
    CString m_strFormData;
    CString m_strFilePath;
    CString m_strFileName;
    CbHttpFinished m_funHttpFinished;
    CbHttpProgress m_funHttpProgress;
};
