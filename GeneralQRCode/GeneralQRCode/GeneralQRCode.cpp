// GeneralQRCode.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "QRCode.h"

int _tmain(int argc, _TCHAR* argv[])
{
    CQRCode code("qrcode.bmp", "Generate QRCode by guozs");
    code.General();

	return 0;
}

