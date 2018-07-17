#include "stdafx.h"
#include "QRCode.h"

#define FREEQRCODE(a) do { if(a) { QRcode_free(a); a=NULL; } } while(0)

CQRCode::CQRCode(void)
{
    m_qrcode = NULL;
    m_nPixel = QRCODE_PIXEL;
    m_color = 0;//black
}

CQRCode::CQRCode( const std::string& outputFile, const std::string& inputText, int pixel/* = QRCODE_PIXEL*/, COLORREF color/* = 0*/ )
{
    m_qrcode = NULL;
    m_strOutputFile = outputFile;
    m_strInputText = inputText;
    m_nPixel = pixel;
    m_color = color;
}

CQRCode::~CQRCode(void)
{
    FREEQRCODE(m_qrcode);
}

void CQRCode::SetOutputFile( const std::string& outputFile )
{
    m_strOutputFile = outputFile;
}

void CQRCode::SetInputText( const std::string& inputText )
{
    m_strInputText = inputText;
}

void CQRCode::SetPixel( int pixel )
{
    m_nPixel = pixel;
}

void CQRCode::SetColor( COLORREF color )
{
    m_color = color;
}

bool CQRCode::General()
{
    if (m_strOutputFile.empty() || m_strInputText.empty())
    {
        return false;
    }

    FREEQRCODE(m_qrcode);

    unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
    unsigned char*	pRGBData, *pSourceData, *pDestData;

    m_qrcode = QRcode_encodeString(m_strInputText.c_str(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);
    if (!m_qrcode)
    {
        return false;
    }

    unWidth = m_qrcode->width;
    unWidthAdjusted = unWidth * m_nPixel * 3;
    if (unWidthAdjusted % 4)
    {
        unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
    }
    unDataBytes = unWidthAdjusted * unWidth * m_nPixel;

    // Allocate pixels buffer
    if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
    {
        return false;
    }

    // Preset to white
    memset(pRGBData, 0xff, unDataBytes);


    // Prepare bmp headers
    BITMAPFILEHEADER kFileHeader;
    kFileHeader.bfType = 0x4d42;  // "BM"
    kFileHeader.bfSize =	sizeof(BITMAPFILEHEADER) +
	    sizeof(BITMAPINFOHEADER) +
	    unDataBytes;
    kFileHeader.bfReserved1 = 0;
    kFileHeader.bfReserved2 = 0;
    kFileHeader.bfOffBits =	sizeof(BITMAPFILEHEADER) +
	    sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER kInfoHeader;
    kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    kInfoHeader.biWidth = unWidth * m_nPixel;
    kInfoHeader.biHeight = -((int)unWidth * m_nPixel);
    kInfoHeader.biPlanes = 1;
    kInfoHeader.biBitCount = 24;
    kInfoHeader.biCompression = BI_RGB;
    kInfoHeader.biSizeImage = 0;
    kInfoHeader.biXPelsPerMeter = 0;
    kInfoHeader.biYPelsPerMeter = 0;
    kInfoHeader.biClrUsed = 0;
    kInfoHeader.biClrImportant = 0;


    // Convert QrCode bits to bmp pixels
    pSourceData = m_qrcode->data;
    for(y = 0; y < unWidth; y++)
    {
        pDestData = pRGBData + unWidthAdjusted * y * m_nPixel;
        for(x = 0; x < unWidth; x++)
        {
            if (*pSourceData & 1)
            {
                for(l = 0; l < m_nPixel; l++)
                {
                    for(n = 0; n < m_nPixel; n++)
                    {
                        *(pDestData +		n * 3 + unWidthAdjusted * l) =	GetBValue(m_color);
                        *(pDestData + 1 +	n * 3 + unWidthAdjusted * l) =	GetGValue(m_color);
                        *(pDestData + 2 +	n * 3 + unWidthAdjusted * l) =	GetRValue(m_color);
                    }
                }
            }
            pDestData += 3 * m_nPixel;
            pSourceData++;
        }
    }

    // Output the bmp file
    FILE* file = fopen(m_strOutputFile.c_str(), "wb+");
    if (!file)
    {
        return false;
    }
    fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    fwrite(pRGBData, sizeof(unsigned char), unDataBytes, file);
    fclose(file);

    // Free data
    free(pRGBData);
    return true;
}
