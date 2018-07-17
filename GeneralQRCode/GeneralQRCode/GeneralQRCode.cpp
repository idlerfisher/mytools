// GeneralQRCode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "QRCode.h"

int _tmain(int argc, _TCHAR* argv[])
{
    CQRCode code("qrcode.bmp", "Generate QRCode by guozs");
    code.General();

	return 0;
}

