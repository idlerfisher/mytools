#include "StdAfx.h"
#include "HttpClient.h"

CHttpClient::CHttpClient() : m_internetSession("Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; )", 1, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_DONT_CACHE)
{
    m_funHttpFinished = NULL;
    m_funHttpProgress = NULL;
}

CHttpClient::~CHttpClient()
{
    m_internetSession.Close();
}

void CHttpClient::RegisterCallBack(CbHttpProgress funCbHttpProgress/* = NULL*/, CbHttpFinished funCbHttpFinished/* = NULL*/)
{
    m_funHttpProgress = funCbHttpProgress;
    m_funHttpFinished = funCbHttpFinished;
}

bool CHttpClient::HttpGet(const CString& strUrl, CString& strOut)
{
    DWORD dwServiceType;
    CString strServer;
    CString strObject;
    INTERNET_PORT nPort;
    CHttpConnection* pHttpConn = NULL;
    CHttpFile* pHttpFile = NULL;

    try
    {
        do
        {
            if (FALSE == AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
            {
                ASSERT(0);
                break;
            }

            pHttpConn = m_internetSession.GetHttpConnection(strServer, nPort);
            if (NULL == pHttpConn)
            {
                ASSERT(pHttpConn);
                break;
            }
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_RELOAD);
            if (NULL == pHttpFile)
            {
                ASSERT(pHttpFile);
                break;
            }
            if (FALSE == pHttpFile->SendRequest())
            {
                break;
            }

            DWORD dwRet;
            pHttpFile->QueryInfoStatusCode(dwRet);
            if (HTTP_STATUS_OK != dwRet)
            {
                break;
            }

            strOut.Empty();
            char szBuf[1024];
            DWORD dwRead = 0;
            while (dwRead = pHttpFile->Read(szBuf, sizeof(szBuf)))
            {
                strOut += CString(szBuf, dwRead);
            }

            if (m_funHttpFinished)
            {
                m_funHttpFinished(true, CString(strOut));
            }
            HTTP_RELEASE;
            return true;
        } while (0);
    }
    catch (CException*)
    {
        if (m_funHttpFinished)
        {
            m_funHttpFinished(false, "");
        }
        HTTP_RELEASE;
        return false;
    }

    if (m_funHttpFinished)
    {
        m_funHttpFinished(false, "");
    }
    HTTP_RELEASE;
    return false;
}

//m_pHttpFile->AddRequestHeaders("Accept: *,*/*");
//m_pHttpFile->AddRequestHeaders("Accept-Language: zh-cn");
//m_pHttpFile->AddRequestHeaders("Content-Type: Application/x-www-form-urlencoded");
//m_pHttpFile->AddRequestHeaders("Accept-Encoding: gzip, deflate");

bool CHttpClient::HttpPost(const CString& strUrl, const FORMDATAMAP& formDataMap, CString& strOut)
{
    return HttpPost(strUrl, UrlEncodeJoinFormData(formDataMap), strOut);
}

bool CHttpClient::HttpPost(const CString& strUrl, const CString& strFormData, CString& strOut)
{
    DWORD dwServiceType;
    CString strServer;
    CString strObject;
    INTERNET_PORT nPort;
    CHttpConnection* pHttpConn = NULL;
    CHttpFile* pHttpFile = NULL;

    try
    {
        do 
        {
            if (FALSE == AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
            {
                ASSERT(0);
                break;
            }
            pHttpConn = m_internetSession.GetHttpConnection(strServer, nPort);
            if (NULL == pHttpConn)
            {
                ASSERT(pHttpConn);
                break;
            }
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
            if (NULL == pHttpFile)
            {
                ASSERT(pHttpFile);
                break;
            }
            pHttpFile->AddRequestHeaders("Cache-Control: no-cache");
            pHttpFile->AddRequestHeaders("Content-Type: application/x-www-form-urlencoded");
            pHttpFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)strFormData, strFormData.GetLength());

            DWORD dwRet;
            pHttpFile->QueryInfoStatusCode(dwRet);
            if (HTTP_STATUS_OK != dwRet)
            {
                break;
            }

            strOut.Empty();
            char szBuf[1024];
            DWORD dwRead = 0;
            while (dwRead = pHttpFile->Read(szBuf, sizeof(szBuf)))
            {
                strOut += CString(szBuf, dwRead);
            }

            if (m_funHttpFinished)
            {
                m_funHttpFinished(true, strOut);
            }
            HTTP_RELEASE;
            return true;
        } while (0);
    }
    catch (CException*)
    {
        if (m_funHttpFinished)
        {
            m_funHttpFinished(false, "");
        }
        HTTP_RELEASE;
        return false;
    }

    if (m_funHttpFinished)
    {
        m_funHttpFinished(false, "");
    }
    HTTP_RELEASE;
    return false;
}

int CHttpClient::HexToI(const char *s)
{
    int ch, value;

    ch = s[0];
    if (isupper(ch)) {
        ch = tolower(ch);
    }
    value = ((ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10)) * 16;

    ch = s[1];
    if (isupper(ch)) {
        ch = tolower(ch);
    }
    value += (ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10);

    return value;
}

CString CHttpClient::UrlEncode(LPCTSTR szInStr)
{
    int in_str_len = (int)strlen(szInStr);
    CString out_str;
    register unsigned char c;
    unsigned char *to, *start;
    unsigned char const *from, *end;
    unsigned char hexchars[] = "0123456789ABCDEF";

    from = (unsigned char *)szInStr;
    end = (unsigned char *)szInStr + in_str_len;
    start = to = (unsigned char *) malloc(3*in_str_len+1);

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
        } else if ((c < '0' && c != '-' && c != '.') ||
            (c < 'A' && c > '9') ||
            (c > 'Z' && c < 'a' && c != '_') ||
            (c > 'z')) {
                to[0] = '%';
                to[1] = hexchars[c >> 4];
                to[2] = hexchars[c & 15];
                to += 3;
        } else {
            *to++ = c;
        }
    }
    *to = 0;

    out_str = (char *) start;
    free(start);
    return out_str;
}

CString CHttpClient::UrlDecode(LPCTSTR szInStr)
{
    int in_str_len = (int)strlen(szInStr);
    CString out_str;
    char *str;

    str = _strdup(szInStr);
    char *dest = str;
    char *data = str;

    while (in_str_len--) {
        if (*data == '+') {
            *dest = ' ';
        } else if (*data == '%' && in_str_len >= 2 && isxdigit((int) *(data + 1))
            && isxdigit((int) *(data + 2))) {
                *dest = (char) HexToI(data + 1);
                data += 2;
                in_str_len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }
    *dest = '\0';
    out_str = str;
    free(str);
    return out_str;
}

bool CHttpClient::Utf8ToMb(CString& mb, const CString& utf8)
{
    std::vector<char> vmb;

    // convert an UTF8 string to widechar 
    int nLen = MultiByteToWideChar(CP_UTF8, 0, utf8, utf8.GetLength(), NULL, 0);
    WCHAR* lpszW = new WCHAR[nLen];
    int nRtn = MultiByteToWideChar(CP_UTF8, 0, utf8, utf8.GetLength(), lpszW, nLen);
    if(nRtn != nLen)
    {
        delete[] lpszW;
        return false;
    }

    // convert an widechar string to Multibyte 
    int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);
    if (MBLen <=0)
    {
        return false;
    }
    vmb.resize(MBLen);
    nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, &vmb[0], MBLen, NULL, NULL);
    delete[] lpszW;

    if(nRtn != MBLen)
    {
        vmb.clear();
        return false;
    }
    mb = CString(&vmb[0], (int)vmb.size());
    return true;
}

bool CHttpClient::MbToUtf8(CString& utf8, const CString& mb)
{
	
	CStringW unicode(mb);

	int nLen = WideCharToMultiByte(CP_UTF8, 0, unicode.GetString(), unicode.GetLength(), NULL, 0, NULL, NULL);
	if (nLen <= 0)
	{
		return false;
	}

    char* pchar = new char[nLen+1];
	memset(pchar, 0, nLen+1);
	WideCharToMultiByte(CP_UTF8, 0, unicode.GetString(), unicode.GetLength(), pchar, nLen, NULL, NULL);
	utf8 = pchar;
	delete[] pchar;
	return true;
	
	return true;
}

CString CHttpClient::UrlEncodeJoinFormData(const FORMDATAMAP& data)
{
    if (data.empty())
    {
        return "";
    }

    CString strFormData;

    //format:a=1&b=2 -> strUrl编码
    for (FORMDATAMAP::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        strFormData += UrlEncode(it->first) + "=" + UrlEncode(it->second) + "&";
    }
    if (!strFormData.IsEmpty())
    {
        int nLen = strFormData.GetLength();
        if ('&' == strFormData[nLen-1])
        {
            strFormData.Delete(nLen-1);
        }
    }
    return strFormData;
}

bool CHttpClient::UploadFileEx(CHttpFile* pHttpFile, const CString& strFilePath, CString& strOut)
{
    if (NULL == pHttpFile)
    {
        return false;
    }

    //parse file name
    CString strFileName = strFilePath.Mid(strFilePath.ReverseFind('\\') + 1);
    if (strFileName.IsEmpty())
    {
        return false;
    }

    //add http header
    CString strTmp;
    CString strBoundary = "------mx_jzyx_com_mx_jzyx_com";
    strTmp.Format("Content-Type:multipart/form-data;boundary=%s", strBoundary);
    pHttpFile->AddRequestHeaders(strTmp);

    //curstom http header:不通用,要根据目标网址的提交协议进行修改,要修改 @guozs
    CString strPut1, strPut2, strPut3;
    strPut1.Format("--%s\r\nContent-Disposition: form-data; name=\"MAX_FILE_SIZE\"\r\n\r\n2000000\r\n", strBoundary);
    strPut2.Format("--%s\r\nContent-Disposition: form-data; name=\"upfile\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n", strBoundary, strFileName);
    strPut3.Format("\r\n--%s--\r\n", strBoundary);

    CFile file;
    if (FALSE == file.Open(strFilePath, CFile::shareDenyNone | CFile::modeRead))
    {
        return false;
    }
    static const DWORD Max_File_Size = (DWORD)(1.5 * 1024 * 1024); //只允许上传1.5MB
    if (file.GetLength() >= Max_File_Size)
    {
        return false;
    }

    //send request
    DWORD nTotalLen = file.GetLength() + strPut1.GetLength() + strPut2.GetLength() + strPut3.GetLength();
    if (FALSE == pHttpFile->SendRequestEx(nTotalLen)) // => EndRequest
    {
        return false;
    }

    //write file to http
    file.SeekToBegin();
    char szBuf[1024];
    DWORD nRead = 0;
    DWORD nCur = 0;

    pHttpFile->Write(strPut1, strPut1.GetLength());
    pHttpFile->Write(strPut2, strPut2.GetLength());
    nCur += strPut1.GetLength() + strPut2.GetLength();
    if (m_funHttpProgress) m_funHttpProgress(nCur, nTotalLen);

    while (nRead = file.Read(szBuf, sizeof(szBuf)))
    {
        pHttpFile->Write(szBuf, nRead);

        nCur += nRead;
        if (m_funHttpProgress) m_funHttpProgress(nCur, nTotalLen);
    }
    pHttpFile->Write(strPut3, strPut3.GetLength());
    nCur += strPut3.GetLength();
    if (m_funHttpProgress) m_funHttpProgress(nCur, nTotalLen);

    pHttpFile->EndRequest();
    file.Close();

    //get responce text
    DWORD dwRet = 0;
    pHttpFile->QueryInfoStatusCode(dwRet);
    if (HTTP_STATUS_OK == dwRet)
    {
        strOut.Empty();
        while (nRead = pHttpFile->Read(szBuf, sizeof(szBuf)))
        {
            strOut += CString(szBuf, nRead);
        }
    }

    return true;
}

//http post协议,目前平台只支持post,图片文件大小会受限制
bool CHttpClient::UploadFile(const CString& strUrl, const CString& strFilePath, const CString& strOut)
{
    DWORD dwServiceType;
    CString strServer;
    CString strObject;
    INTERNET_PORT nPort;
    CHttpConnection* pHttpConn = NULL;
    CHttpFile* pHttpFile = NULL;

    try
    {
        do 
        {
            if (FALSE == AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
            {
                ASSERT(0);
                break;
            }
            pHttpConn = m_internetSession.GetHttpConnection(strServer, nPort);
            if (NULL == pHttpConn)
            {
                ASSERT(pHttpConn);
                break;
            }
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
            if (NULL == pHttpFile)
            {
                ASSERT(pHttpFile);
                break;
            }

            CString strOut;
            bool nRet = UploadFileEx(pHttpFile, strFilePath, strOut);

            if (m_funHttpFinished)
            {
                m_funHttpFinished(nRet, strOut);
            }
            HTTP_RELEASE;
            return nRet;
        } while (0);
    }
    catch (CException*)
    {
        if (m_funHttpFinished)
        {
            m_funHttpFinished(false, "");
        }
        HTTP_RELEASE;
        return false;
    }

    if (m_funHttpFinished)
    {
        m_funHttpFinished(false, "");
    }
    HTTP_RELEASE;
    return false;
}

//strName:空   表示使用网址的文件名
//strname:非空 表示使用strName作为文件名
bool CHttpClient::DownloadFile(const CString& strUrl, const CString& strPath, const CString& strName)
{
    DWORD dwServiceType;
    CString strServer;
    CString strObject;
    INTERNET_PORT nPort;
    CHttpConnection* pHttpConn = NULL;
    CHttpFile* pHttpFile = NULL;
    CString strFileName;
    CString strFilePath;

    try
    {
        do 
        {
            if (FALSE == AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
            {
                ASSERT(0);
                break;
            }

            strFileName = strObject.Mid(strObject.ReverseFind('/') + 1);
            if (strFileName.IsEmpty())
            {
                ASSERT(strFileName);
                break;
            }
            if (strName.IsEmpty())
            {
                strFilePath = strPath + "\\" + strFileName;
            }
            else
            {
                strFilePath = strPath + "\\" + strName;
            }

            pHttpConn = m_internetSession.GetHttpConnection(strServer, nPort);
            if (NULL == pHttpConn)
            {
                ASSERT(pHttpConn);
                break;
            }
            pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
            if (NULL == pHttpFile)
            {
                ASSERT(pHttpFile);
                break;
            }
            if (FALSE == pHttpFile->SendRequest())
            {
                break;
            }

            DWORD dwRet = 0;
            pHttpFile->QueryInfoStatusCode(dwRet);
            if (HTTP_STATUS_OK != dwRet)
            {
                break;
            }

            DWORD dwTotalLen = 0;
            if (FALSE == pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, dwTotalLen))
            {
                break;
            }

            CFile file;
            DWORD dwRead = 0;
            DWORD dwCur = 0;
            char szBuf[1024];

            if (FALSE == file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite))
            {
                break;
            }

            while (dwRead = pHttpFile->Read(szBuf, sizeof(szBuf)))
            {
                file.Write(szBuf, dwRead);

                dwCur += dwRead;
                if (m_funHttpProgress) m_funHttpProgress(dwCur, dwTotalLen);
            }
            file.Close();

            if (m_funHttpFinished)
            {
                m_funHttpFinished(true, "");
            }
            HTTP_RELEASE;
            return true;
        } while (0);
    }
    catch (CException*)
    {
        if (m_funHttpFinished)
        {
            m_funHttpFinished(false, "");
        }
        HTTP_RELEASE;
        return false;
    }

    if (m_funHttpFinished)
    {
        m_funHttpFinished(false, "");
    }
    HTTP_RELEASE;
    return false;
}


/**************CAsyncHttpClient****************/

void CAsyncHttpClient::Get(const CString& strUrl, CbHttpProgress funProgress/* = NULL*/, CbHttpFinished funFinished/* = NULL*/)
{
    Reset();
    m_strUrl = strUrl;
    m_funHttpFinished = funFinished;
    m_funHttpProgress = funProgress;

    HANDLE hThread = CreateThread(NULL, 0, CbGet, (LPVOID)this, 0, NULL);
    if (hThread)
    {
        CloseHandle(hThread);
    }
}

void CAsyncHttpClient::Post(const CString& strUrl, const FORMDATAMAP& formDataMap, CbHttpProgress funProgress/* = NULL*/, CbHttpFinished funFinished/* = NULL*/)
{
    Post(strUrl, CHttpClient::UrlEncodeJoinFormData(formDataMap), funProgress, funFinished);
}

void CAsyncHttpClient::Post(const CString& strUrl, const CString& strFormData, CbHttpProgress funProgress/* = NULL*/, CbHttpFinished funFinished/* = NULL*/)
{
    Reset();
    m_strUrl = strUrl;
    m_strFormData = strFormData;
    m_funHttpFinished = funFinished;
    m_funHttpProgress = funProgress;

    HANDLE hThread = CreateThread(NULL, 0, CbPost, (LPVOID)this, 0, NULL);
    if (hThread)
    {
        CloseHandle(hThread);
    }
}

void CAsyncHttpClient::UploadFile(const CString& strUrl, const CString& strFilePath, CbHttpProgress funProgress/* = NULL*/, CbHttpFinished funFinished/* = NULL*/)
{
    Reset();
    m_strUrl = strUrl;
    m_strFilePath = strFilePath;
    m_funHttpFinished = funFinished;
    m_funHttpProgress = funProgress;

    HANDLE hThread = CreateThread(NULL, 0, CbUploadFile, (LPVOID)this, 0, NULL);
    if (hThread)
    {
        CloseHandle(hThread);
    }
}

void CAsyncHttpClient::DownloadFile(const CString& strUrl, const CString& strPath, const CString& strName/*=""*/, CbHttpProgress funProgress/* = NULL*/, CbHttpFinished funFinished/* = NULL*/)
{
    Reset();
    m_strUrl = strUrl;
    m_strFilePath = strPath;
    m_strFileName = strName;
    m_funHttpFinished = funFinished;
    m_funHttpProgress = funProgress;

    HANDLE hThread = CreateThread(NULL, 0, CbDownloadFile, (LPVOID)this, 0, NULL);
    if (hThread)
    {
        CloseHandle(hThread);
    }
}

DWORD WINAPI CAsyncHttpClient::CbGet(LPVOID lpParameter)
{
    CAsyncHttpClient* that = (CAsyncHttpClient*)lpParameter;
    if (NULL == that)
    {
        return FALSE;
    }

    CHttpClient httpClient;
    httpClient.RegisterCallBack(that->m_funHttpProgress, that->m_funHttpFinished);
    CString strOut;
    httpClient.HttpGet(that->m_strUrl, strOut);
    return TRUE;
}

DWORD WINAPI CAsyncHttpClient::CbPost(LPVOID lpParameter)
{
    CAsyncHttpClient* that = (CAsyncHttpClient*)lpParameter;
    if (NULL == that)
    {
        return FALSE;
    }

    CHttpClient httpClient;
    httpClient.RegisterCallBack(that->m_funHttpProgress, that->m_funHttpFinished);
    CString strOut;
    httpClient.HttpPost(that->m_strUrl, that->m_strFormData, strOut);
    return TRUE;
}

DWORD WINAPI CAsyncHttpClient::CbUploadFile(LPVOID lpParameter)
{
    CAsyncHttpClient* that = (CAsyncHttpClient*)lpParameter;
    if (NULL == that)
    {
        return FALSE;
    }

    CHttpClient httpClient;
    httpClient.RegisterCallBack(that->m_funHttpProgress, that->m_funHttpFinished);
    CString strOut;
    httpClient.UploadFile(that->m_strUrl, that->m_strFilePath, strOut);
    return TRUE;
}

DWORD WINAPI CAsyncHttpClient::CbDownloadFile(LPVOID lpParameter)
{
    CAsyncHttpClient* that = (CAsyncHttpClient*)lpParameter;
    if (NULL == that)
    {
        return FALSE;
    }

    CHttpClient httpClient;
    httpClient.RegisterCallBack(that->m_funHttpProgress, that->m_funHttpFinished);
    CString strOut;
    httpClient.DownloadFile(that->m_strUrl, that->m_strFilePath, that->m_strFileName);
    return TRUE;
}

void CAsyncHttpClient::Reset()
{
    m_strUrl.Empty();
    m_strFormData.Empty();
    m_strFilePath.Empty();
    m_strFileName.Empty();
    m_funHttpFinished = NULL;
    m_funHttpProgress = NULL;
}
