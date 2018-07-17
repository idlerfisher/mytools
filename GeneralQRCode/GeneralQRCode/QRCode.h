/**************************************************************************
 * @file QRCode.h
 * @brief 二维码生成类
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
    std::string m_strOutputFile; //输出文件名
    std::string m_strInputText; //输入内容
    int m_nPixel; //像素大小[1-?]
    COLORREF m_color; //二维码颜色
    QRcode* m_qrcode; //二维码

    static const int QRCODE_PIXEL = 5;
};
