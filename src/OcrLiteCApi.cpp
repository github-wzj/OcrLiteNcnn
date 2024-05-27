#ifdef __CLIB__

#include "OcrLiteCApi.h"
#include "OcrLite.h"

#include "windows.h"

extern "C"
{
typedef struct {
    OcrLite OcrObj;
    std::string strRes;
} OCR_OBJ;

bool hBitMapToMat(HBITMAP& _hBmp,Mat& _mat)
{
    BITMAP bmp;    
    GetObject( _hBmp, sizeof(BITMAP), &bmp );    
    int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;   
    int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;    
    Mat v_mat;
    v_mat.create( cvSize( bmp.bmWidth,bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels) );
    GetBitmapBits( _hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, v_mat.data );  
    _mat = v_mat;
    return TRUE;   
}

_QM_OCR_API OCR_HANDLE
OcrInit(const char *szDetModel, const char *szClsModel, const char *szRecModel, const char *szKeyPath, int nThreads,
        int gpuIndex) {

    OCR_OBJ *pOcrObj = new OCR_OBJ;
    if (pOcrObj) {
        pOcrObj->OcrObj.setNumThread(nThreads);
        pOcrObj->OcrObj.setGpuIndex(gpuIndex);
        pOcrObj->OcrObj.initModels(szDetModel, szClsModel, szRecModel, szKeyPath);

        return pOcrObj;
    } else {
        return nullptr;
    }

}
//bitmap参数
_QM_OCR_API OCR_BOOL
OcrDetect(OCR_HANDLE handle, HBITMAP& _hBmp, OCR_PARAM *pParam) {

    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (!pOcrObj)
        return FALSE;

    OCR_PARAM Param = *pParam;
    if (Param.padding == 0)
        Param.padding = 50;

    if (Param.maxSideLen == 0)
        Param.maxSideLen = 1024;

    if (Param.boxScoreThresh == 0)
        Param.boxScoreThresh = 0.6;

    if (Param.boxThresh == 0)
        Param.boxThresh = 0.3f;

    if (Param.unClipRatio == 0)
        Param.unClipRatio = 2.0;

    if (Param.doAngle == 0)
        Param.doAngle = 1;

    if (Param.mostAngle == 0)
        Param.mostAngle = 1;
   Mat my_mat;
   hBitMapToMat(_hBmp, my_mat) 
    
    OcrResult result = pOcrObj->OcrObjdetect(my_mat, Param.padding, Param.maxSideLen,
                                              Param.boxScoreThresh, Param.boxThresh, Param.unClipRatio,
                                              Param.doAngle != 0, Param.mostAngle != 0);
   
    if (result.strRes.length() > 0) {
        pOcrObj->strRes = result.strRes;
        return TRUE;
    } else
        return FALSE;
}

_QM_OCR_API OCR_BOOL
OcrDetect(OCR_HANDLE handle, const char *imgPath, const char *imgName, OCR_PARAM *pParam) {

    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (!pOcrObj)
        return FALSE;

    OCR_PARAM Param = *pParam;
    if (Param.padding == 0)
        Param.padding = 50;

    if (Param.maxSideLen == 0)
        Param.maxSideLen = 1024;

    if (Param.boxScoreThresh == 0)
        Param.boxScoreThresh = 0.6;

    if (Param.boxThresh == 0)
        Param.boxThresh = 0.3f;

    if (Param.unClipRatio == 0)
        Param.unClipRatio = 2.0;

    if (Param.doAngle == 0)
        Param.doAngle = 1;

    if (Param.mostAngle == 0)
        Param.mostAngle = 1;

    OcrResult result = pOcrObj->OcrObj.detect(imgPath, imgName, Param.padding, Param.maxSideLen,
                                              Param.boxScoreThresh, Param.boxThresh, Param.unClipRatio,
                                              Param.doAngle != 0, Param.mostAngle != 0);
    if (result.strRes.length() > 0) {
        pOcrObj->strRes = result.strRes;
        return TRUE;
    } else
        return FALSE;
}


_QM_OCR_API int OcrGetLen(OCR_HANDLE handle) {
    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (!pOcrObj)
        return 0;
    return pOcrObj->strRes.size() + 1;
}

_QM_OCR_API OCR_BOOL OcrGetResult(OCR_HANDLE handle, char *szBuf, int nLen) {
    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (!pOcrObj)
        return FALSE;

    if (nLen > pOcrObj->strRes.size()) {
        strncpy(szBuf, pOcrObj->strRes.c_str(), pOcrObj->strRes.size());
        szBuf[pOcrObj->strRes.size() - 1] = 0;
    }

    return pOcrObj->strRes.size();
}

_QM_OCR_API void OcrDestroy(OCR_HANDLE handle) {
    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (pOcrObj)
        delete pOcrObj;
}

};
#endif

