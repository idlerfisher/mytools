/**************************************************************************
 * @file QRCode.h
 * @brief ��ά��������
 * @author guozs@woobest.com
 * @date 2016-03-31 15:09:36
 * @copyright (c) 2011-2016 woobest.com Inc. All rights reserved. 
 *************************************************************************/
#pragma once

#include "QRCode/qrencode.h"

class CQRCode
{
public:
    CQRCode(void);
    CQRCode(const std::string& outputFile, const std::string& inputText, int pixel = QRCODE_PIXEL, COLORREF color = 0);
    ~CQRCode(void);
    void SetOutputFile(const std::string& outputFile);
    void SetInputText(const std::string& inputText);
    void SetPixel(int pixel);
    void SetColor(COLORREF color);
    bool General();

private:
    std::string m_strOutputFile; //����ļ���
    std::string m_strInputText; //��������
    int m_nPixel; //���ش�С[1-?]
    COLORREF m_color; //��ά����ɫ
    QRcode* m_qrcode; //��ά��

    static const int QRCODE_PIXEL = 5;
};
