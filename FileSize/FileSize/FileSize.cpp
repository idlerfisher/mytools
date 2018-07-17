// FileSize.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <afxwin.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

//路径 时间戳
typedef std::map<CString, DWORD> MapFileInfo;

//程序名称
CString g_AppName;

#define OUT_FILE "project.manifest"

typedef std::pair<CString, DWORD> FILEPAIR;

int cmp(const FILEPAIR& a, const FILEPAIR& b)
{
    return a.second > b.second;
}

void SearchFile(const CString& strBasePath, const CString& strPath, MapFileInfo& mapInfo)
{
    CFileFind ff;
    BOOL bRet = ff.FindFile(strPath + "\\*.*");
    if (FALSE == bRet)
    {
        return;
    }

    CTime tmFile;
    CString strPathTmp, strFileName;
    while (bRet)
    {
        bRet = ff.FindNextFile();

        if (ff.IsDots())
        {
            continue;
        }
        else if (ff.IsDirectory())
        {
            SearchFile(strBasePath, ff.GetFilePath(), mapInfo);
        }
        else //文件
        {
            strFileName = ff.GetFileName();
            // 			if (strFileName == g_AppName)
            // 			{
            // 				continue;
            // 			}

            ff.GetLastWriteTime(tmFile);
            CString strTmp = ff.GetFilePath();
            strTmp.Replace(strBasePath, "");
            strTmp.Replace("\\", "/");
            mapInfo[strTmp] = (DWORD)tmFile.GetTime();
        }
    }
}

void SearchFileSize(const CString& strBasePath, const CString& strPath, MapFileInfo& mapInfo)
{
    cout << "scan " << strPath << endl;

    CFileFind ff;
    BOOL bRet = ff.FindFile(strPath + "\\*.*");
    if (FALSE == bRet)
    {
        return;
    }

    DWORD size = 0;
    CString strPathTmp;
    while (bRet)
    {
        bRet = ff.FindNextFile();

        if (ff.IsDots())
        {
            continue;
        }
        else if (ff.IsDirectory())
        {
            SearchFileSize(strBasePath, ff.GetFilePath(), mapInfo);
        }
        else //文件
        {
            CString strTmp = ff.GetFilePath();
            // 			strTmp.Replace(strBasePath, "");
            // 			strTmp.Replace("\\", "/");
            size = ff.GetLength();
            if (size >= 1048576)
            {
                mapInfo[strTmp] = size;
            }
        }
    }
}

void Print(const MapFileInfo& mapInfo)
{
    //test
    //     for (MapFileInfo::iterator it = mapInfo.begin(); it != mapInfo.end(); ++it)
    //     {
    //         cout << it->first << " " << it->second << endl;
    //     }
    CFile file;
    if (!file.Open(OUT_FILE, CFile::modeWrite | CFile::modeCreate))
    {
        return;
    }

    CString strLine;
    const int nSize = mapInfo.size();
    int nCount = 0;

    strLine = "{\n"
        "\t\"packageUrl\": \"http://192.168.3.49:3003/update\",\n"
        "\t\"remoteManifestUrl\": \"http://192.168.3.49:3003/update/project.manifest\",\n"
        "\t\"remoteVersionUrl\": \"http://192.168.3.49:3003/update/version.manifest\",\n"
        "\t\"version\": \"1.0.11273\",\n"
        "\t\"engineVersion\": \"3.2\",\n";
    file.Write(strLine, strLine.GetLength());

    strLine = "\t\"assets\" : {\n";
    file.Write(strLine, strLine.GetLength());
    for (MapFileInfo::const_iterator it = mapInfo.begin(); it != mapInfo.end(); ++it)
    {
        ++nCount;
        if (nCount == nSize)
        {
            strLine.Format("\t\t\"%s\" : {\n"
                "\t\t\t\"md5\" : \"%d\"\n"
                "\t\t}\n",
                it->first, it->second);
            //strLine.Format("\"%s\" : { \"md5\" : \"%d\" }\n", it->first, it->second);
        }
        else
        {
            strLine.Format("\t\t\"%s\" : {\n"
                "\t\t\t\"md5\" : \"%d\"\n"
                "\t\t},\n",
                it->first, it->second);
            //strLine.Format("\"%s\" : { \"md5\" : \"%d\" },\n", it->first, it->second);
        }
        file.Write(strLine, strLine.GetLength());
    }

    strLine = "\t},\n"
        "\t\"searchPaths\": [\n"
        "\t\t\"script\",\n"
        "\t\t\"src\",\n"
        "\t\t\"res\",\n"
        "\t\t\"res/map\",\n"
        "\t\t\"res/scene\",\n"
        "\t\t\"res/scene/gameMain\",\n"
        "\t\t\"res/scene/battleResult\",\n"
        "\t\t\"res/scene/battleBg/train\",\n"
        "\t\t\"res/scene/gameStart\",\n"
        "\t\t\"res/scene/publish\"\n"
        "\t]\n";
    file.Write(strLine, strLine.GetLength());

    strLine = "}\n";
    file.Write(strLine, strLine.GetLength());

    file.Close();
}

CString size2str(long long  dwSize)
{
    static const long long   c_arr[4] =
    {
        1024,
        1024 * 1024,
        1024 * 1024 * 1024,
        1024 * 1024 * 1024 * 1024LL
    };

    CString str;
    if (dwSize < c_arr[0])
    {
        str.Format("%d B", dwSize);
    }
    else if (dwSize < c_arr[1])
    {
        str.Format("%.2f KB", 1.0 * dwSize / 1024);
    }
    else if (dwSize < c_arr[2])
    {
        str.Format("%.2f MB", 1.0 * dwSize / 1024 / 1024);
    }
    else if (dwSize < c_arr[3])
    {
        str.Format("%.2f GB", 1.0 * dwSize / 1024 / 1024 / 1024);
    }
    return str;
}

void PrintFileSize(const MapFileInfo& mapInfo)
{
    std::vector<FILEPAIR> vecPair;
    for (MapFileInfo::const_iterator it = mapInfo.begin(); it != mapInfo.end(); ++it)
    {
        vecPair.push_back(make_pair(it->first, it->second));
    }
    std::sort(vecPair.begin(), vecPair.end(), cmp);

    CFile file;
    file.Open("filesize.txt", CFile::modeWrite | CFile::modeCreate);
    CString strText;
    long long nSize = 0;
    for (std::vector<FILEPAIR>::iterator it = vecPair.begin(); it != vecPair.end(); ++it)
    {
        nSize += it->second;
        strText.Format("%s\t=> %s\r\n", size2str(it->second), it->first);
        file.Write(strText, strText.GetLength());
        //cout << size2str(it->second) << " => " << it->first << endl;
    }
    strText.Format("占用空间:%s\r\n", size2str(nSize));
    file.Write(strText, strText.GetLength());
    file.Close();
    //test
    //     for (MapFileInfo::iterator it = mapInfo.begin(); it != mapInfo.end(); ++it)
    //     {
    //         cout << it->first << " " << it->second << endl;
    //     }
}

int _tmain(int argc, _TCHAR* argv[])
{
    char szAppName[MAX_PATH];
    GetModuleFileName(NULL, szAppName, sizeof(szAppName));

    CString strPath(szAppName);
    int nPos = strPath.ReverseFind('\\');
    if (-1 == nPos)
    {
        return 0;
    }
    g_AppName = strPath.Right(strPath.GetLength() - nPos - 1);
    //cout << g_AppName << endl;

    CString strBasePath = strPath.Left(nPos + 1);
    //cout << strBasePath << endl;

    DeleteFile(strBasePath + "\\" + CString(OUT_FILE));

    //file time
    //	MapFileInfo mapInfo;
    // 	SearchFile(strBasePath, strBasePath, mapInfo);
    // 	Print(mapInfo);

    //file size
    MapFileInfo mapInfo;
    SearchFileSize(strBasePath, strBasePath, mapInfo);
    PrintFileSize(mapInfo);
    system("pause");
    return 0;
}

