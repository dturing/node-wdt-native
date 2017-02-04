//
// Created by anton on 21.03.15.
//

#include "cvMotion.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> cvMotion::constructor;

float weight, scale;
double shift;
int mode;

cv::Mat accImage, tmpImage;
cv::Size tmpSize;

void
cvMotion::Init(Local<Object> target) {
    Nan::HandleScope scope;

    //Class
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(cvMotion::New);
    constructor.Reset( ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("cvMotion").ToLocalChecked());

    // Prototype
    Nan::SetPrototypeMethod(ctor, "process", Process);

    target->Set(Nan::New("cvMotion").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(cvMotion::New) {
        Nan::HandleScope scope;

        tmpSize.width = tmpSize.height = 0;

        info.GetReturnValue().Set(info.Holder());
}


cvMotion::cvMotion(): Nan::ObjectWrap() {

}

NAN_METHOD(cvMotion::Reset) {
        Nan::HandleScope scope;

        if (!accImage.empty()) {
            accImage.release();
        }
        tmpSize.width = tmpSize.height = 0;

        info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(cvMotion::Process) {
        Nan::HandleScope scope;

        Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
        cv::Mat srcMat = cv::Mat::zeros(src->mat.size(), CV_8UC3);
        src->mat.convertTo(srcMat, CV_8UC3);

        Local<Object> outMatrixWrap = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *outMatrix = Nan::ObjectWrap::Unwrap<Matrix>(outMatrixWrap);

        weight = (float)info[1]->NumberValue();
        scale = (float)info[2]->NumberValue();
        shift = (double)info[3]->NumberValue();
        mode = info[4]->IntegerValue();

        if (srcMat.size() != accImage.size()) {
            accImage = cv::Mat::zeros(srcMat.size(), CV_32FC3);
            srcMat.convertTo(accImage, CV_32FC3);
            tmpImage = cv::Mat::zeros(srcMat.size(), CV_8UC3 );
        }

        cv::accumulateWeighted( srcMat, accImage, weight );
        accImage.convertTo(tmpImage, CV_8UC3);


        switch ( mode ) {
            case 0:
                cv::absdiff( srcMat, tmpImage, outMatrix->mat );
            break;
            case 1:
                cv::subtract( srcMat, tmpImage, outMatrix->mat );
            break;
            case 2:
                cv::subtract( tmpImage, srcMat, outMatrix->mat );
            break;
        }


        if ( scale != 1 || shift != 0 ) {
            outMatrix->mat.convertTo(src->mat, CV_8UC3, scale, shift);
        } else {
            src->mat = outMatrix->mat;
        }

        info.GetReturnValue().Set(Nan::Null());

        //info.GetReturnValue().Set(outMatrixWrap);
}
