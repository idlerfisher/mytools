#include <afx.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include "CSpreadSheet.h"

using namespace std;

//�洢������
std::vector<CStringArray*> g_allRows;

//�洢���в�������ĸ���
std::vector<CString> g_allRandomCells;

//�洢����
CStringArray g_title;

//�ͷ���������
void FreeRow(std::vector<CStringArray*>& vecRow)
{
    for (std::vector<CStringArray*>::iterator it = vecRow.begin(); it != vecRow.end(); ++it)
    {
        if (*it)
        {
            (*it)->RemoveAll();
            delete *it;
            *it = NULL;
        }
    }
    vecRow.clear();
}

//��ȡ����������
void ReadAllRows(CSpreadSheet& excel, std::vector<CStringArray*>& vecRow, CStringArray& arrTitle)
{
    FreeRow(vecRow);

    //��ȡ����
    arrTitle.RemoveAll();
    excel.ReadRow(arrTitle, 1);

    //��2�п�ʼ��������
    int nRow = excel.GetTotalRows();
    for (int i = 2; i < nRow; ++i)
    {
        CStringArray* pArr = new CStringArray;
        excel.ReadRow(*pArr, i);
        vecRow.push_back(pArr);
    }
}

//��ȡ��Ҫ�����������
void ReadRandomColumns(CSpreadSheet& excel, const std::vector<CString>& vecTitle, std::vector<CString>& vecCell)
{
    if (vecTitle.empty())
    {
        cout << "��������������Ҫ����������б��⡣" << endl;
        return;
    }

    vecCell.clear();
    CStringArray arr;
    for (int i = 0; i < vecTitle.size(); ++i)
    {
        arr.RemoveAll();
        excel.ReadColumn(arr, vecTitle[i]);
        //��һ���Ǳ���
        for (int j = 1; j < arr.GetSize(); ++j)
        {
            vecCell.push_back(arr[j]);
        }
    }

    //���
    if (!vecCell.empty())
    {
        std::random_shuffle(vecCell.begin(), vecCell.end());
    }
}

//����ת����������
std::vector<int> Title2Column(CStringArray& arrAllTitle, const std::vector<CString>& vecTitle)
{
    std::vector<int> vec;
    for (int i = 0; i < vecTitle.size(); ++i)
    {
        for (int j = 0; j < arrAllTitle.GetSize(); ++j)
        {
            if (arrAllTitle[j] == vecTitle[i])
            {
                vec.push_back(j);
            }
        }
    }
    return vec;
}

//�ϲ�����
void MergeExcelData(std::vector<CStringArray*>& vecRow, const std::vector<int>& vecCol, std::vector<CString>& vecCell)
{
    DWORD nColNum = vecCol.size();
    DWORD nRow = vecRow.size();
    DWORD nCellRow = vecCell.size() / nColNum;
    if (nCellRow != nCellRow)
    {
        printf("�����󡿺ϲ�����ʧ��,������ƥ��:������=%u �����������=%u\n", nRow, nCellRow);
        return;
    }

    //���зŵ������������
    int* aCol = new int[nColNum];
    if (NULL == aCol)
    {
        return;
    }
    for (int i = 0; i < nColNum; ++i)
    {
        aCol[i] = vecCol[i];
    }

    //�ϲ�����
    DWORD nCount = 0;
    DWORD nCol = 0;
    for (std::vector<CStringArray*>::iterator it = vecRow.begin(); it != vecRow.end(); ++it)
    {
        if (NULL == *it)
        {
            printf("�����쳣\n");
            delete[] aCol;
            return;
        }
        CStringArray& rRow = *(*it);
        for (DWORD i = 0; i < nColNum; ++i)
        {
            nCol = aCol[i];
            if (vecCell.empty())
            {
                printf("���������ݲ�ƥ��,��%u��ʧ��\n", nCount);
                delete[] aCol;
                return;
            }
            else
            {
                rRow.SetAt(nCol, vecCell.back());
                vecCell.pop_back();
            }
        }
        ++nCount;
    }

    delete[] aCol;
}

//̽��һ��δʹ�õ��ļ���
CString GetNotExistsFileName(const CString& strLeftName, const CString& strRightName)
{
    CString strFileName = strLeftName + "_bak." + strRightName;
    int nCount = 0;
    while (nCount < 1000)
    {
        strFileName.Format("%s_%04d.%s", strLeftName.GetString(), ++nCount, strRightName.GetString());
        if (GetFileAttributes(strFileName) == INVALID_FILE_ATTRIBUTES)
        {
            break;
        }
    }
    return strFileName;
}

//д����excel
bool SaveNewExcel(const CString& strFileName, const CString& strSheet, CStringArray& arrAllTitle, std::vector<CStringArray*>& vecRow, CString& strDst)
{
    int nIndex = strFileName.ReverseFind('.');
    if (-1 == nIndex)
    {
        printf("����������ʧ��,�ļ������Ϸ�\n");
        return false;
    }
    CString strLeftName = strFileName.Left(nIndex);
    CString strRightName = strFileName.Right(strFileName.GetLength() - nIndex - 1);
    CString strDstName = GetNotExistsFileName(strLeftName, strRightName);

    CString strSheetName = "Sheet1"; //Ĭ��
    if (!strSheet.IsEmpty())
    {
        strSheetName = strSheet;
    }
    CSpreadSheet excel(strDstName, strSheetName, "false");
    excel.BeginTransaction();
    excel.AddHeaders(arrAllTitle);
    for (std::vector<CStringArray*>::iterator it = vecRow.begin(); it != vecRow.end(); ++it)
    {
        if (*it)
        {
            excel.AddRow(*(*it));
        }
        else
        {
            printf("������д�����,������Ϊ��\n");
            break;
        }
    }
    strDst = strDstName;
    return excel.Commit();
}

#define INI_FILE "excel.ini"

int main()
{
    srand(time(NULL)); //������,����ÿ��������һ��

    cout << "ʹ�÷���:\n"
        " 1.�״����л����һ�������ļ�excel.ini\n"
        " 2.��excel.ini������filename�ļ�����\n"
        " 3.��excel.ini������columns������б���,��#�ŷָ�\n"
        " 4.��excel.ini������sheet,Ĭ��Ϊ�ղ�Ҫ����,���Ƕ�ȡ����е�ĳ���ӱ�\n"
        " 5.�б��ⲻ������ڴ�����\n"
        " 6.exe��xls�����ͬһĿ¼��˫������exe,����һ�����ļ�\n"
        << endl;

    char szPath[256] = {0};
    GetCurrentDirectory(sizeof(szPath), szPath);

    CString strIni;
    strIni.Format("%s\\%s", szPath, INI_FILE);

    if (GetFileAttributes(INI_FILE) == INVALID_FILE_ATTRIBUTES)
    {
        CFile file;
        if (!file.Open(INI_FILE, CFile::modeCreate | CFile::modeWrite))
        {
            printf("�����󡿴��������ļ�ʧ��\n");
            system("pause");
            return 0;
        }
        file.Close();
        WritePrivateProfileString("excel", "filename", "test.xls", strIni);
        WritePrivateProfileString("excel", "columns", "title1#title2#title3", strIni);
        WritePrivateProfileString("excel", "sheet", "", strIni);
        printf("����%s�ļ���������Ϣ\n", INI_FILE);
        system("pause");
        return 0;
    }

    char szFileName[256] = {0};
    char szTitle[1024] = {0};
    char szSheet[256] = {0};
    GetPrivateProfileString("excel", "filename", "", szFileName, sizeof(szFileName), strIni);
    GetPrivateProfileString("excel", "columns", "", szTitle, sizeof(szTitle), strIni);
    GetPrivateProfileString("excel", "sheet", "", szSheet, sizeof(szSheet), strIni);
    if (strlen(szFileName) == 0 || strlen(szTitle) == 0)
    {
        printf("����������%s�ļ�������ȷ����Ϣ\n", INI_FILE);
        system("pause");
        return 0;
    }

    //��������
    std::vector<CString> vecTitle;
    CString strTitles(szTitle);
    CString strFileName(szFileName);
    CString strSheet(szSheet);
    int nStart = 0;
    CString strSub = strTitles.Tokenize("#", nStart);
    while (nStart != -1)
    {
        if (!strSub.IsEmpty())
        {
            vecTitle.push_back(strSub);
            //cout << strSub << endl;
        }
        strSub = strTitles.Tokenize("#", nStart);
    }
    if (vecTitle.empty())
    {
        cout << "����������������������" << endl;
        system("pause");
        return 0;
    }

    CSpreadSheet excel(strFileName, strSheet, false);
    cout << "1.��������..." << endl;
    ReadAllRows(excel, g_allRows, g_title); //��������
    
//     for (int i = 0; i < g_title.GetSize(); ++i)
//     {
//         cout << i << " -> " << g_title[i] << endl;
//     }
    if (g_title.IsEmpty())
    {
        cout << "�����󡿱��û������" << endl;
        system("pause");
        return 0;
    }

    cout << "2.�������..." << endl;
    ReadRandomColumns(excel, vecTitle, g_allRandomCells); //�����������
    std::vector<int> vecCol = Title2Column(g_title, vecTitle);
    if (vecCol.empty() || vecCol.size() != vecTitle.size())
    {
        cout << "�����������ļ��ı����������ݲ���Ӧ" << endl;
        system("pause");
        return 0;
    }
    cout << "3.�ϲ�����..." << endl;
    MergeExcelData(g_allRows, vecCol, g_allRandomCells); //�ϲ�����
    cout << "4.д������...";
    CString strDstName;
    bool bSave = SaveNewExcel(strFileName, strSheet, g_title, g_allRows, strDstName);
    if (bSave)
    {
        cout << "�ɹ�д���ļ� " << strDstName << endl;
    }
    else
    {
        cout << "д��ʧ��" << endl;
    }
    cout << "5.�ͷ��ڴ�..." << endl;
    FreeRow(g_allRows);
    cout << "�������" << endl;
    system("pause");

    return 0;
}