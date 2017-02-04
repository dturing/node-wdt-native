//
// Created by Anton on 28/04/15.
//

#include "cvDisplayFormat.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> cvDisplayFormat::constructor;

void
cvDisplayFormat::Init(Local<Object> target) {
    Nan::HandleScope scope;

    //Class
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(cvDisplayFormat::New);
    constructor.Reset( ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("cvDisplayFormat").ToLocalChecked());

    // Prototype
    Nan::SetPrototypeMethod(ctor, "process", Process);

    target->Set(Nan::New("cvDisplayFormat").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(cvDisplayFormat::New) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(info.Holder());
}


cvDisplayFormat::cvDisplayFormat(): Nan::ObjectWrap() {

}

NAN_METHOD(cvDisplayFormat::Reset) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(cvDisplayFormat::Process) {
        Nan::HandleScope scope;

        Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

        if (src->mat.channels() != 4) {
            cv::Mat srcMat = cv::Mat::zeros(src->mat.size(), CV_8UC3);
            src->mat.convertTo(src->mat, CV_8U);
            if (src->mat.channels() < 3) {
                cv::cvtColor(src->mat, srcMat, CV_GRAY2RGB, 3);
            } else if (src->mat.channels() == 3) {
                srcMat = src->mat;
            }
            cv::Mat alphaMat = cv::Mat(srcMat.size(), CV_8UC1, cv::Scalar(255));
            std::vector<cv::Mat> sourceMats;
            sourceMats.push_back(srcMat);
            sourceMats.push_back(alphaMat);
            cv::Mat alphaImg = cv::Mat(srcMat.size(), CV_8UC4);
            std::vector<cv::Mat> dstMats;
            dstMats.push_back(alphaImg);

            int from_to[] = { 0,0, 1,1, 2,2, 3,3 };

            cv::mixChannels(sourceMats, dstMats, from_to, 4);

            src->mat = dstMats[0];
        }

        info.GetReturnValue().Set(Nan::Null());
}
