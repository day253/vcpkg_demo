#include "predict_handler.h"

void PredictServiceImpl::ProcessThriftFramedRequest(
    brpc::Controller *cntl,
    brpc::ThriftFramedMessage *req,
    brpc::ThriftFramedMessage *res,
    google::protobuf::Closure *done)
{
    if (cntl->thrift_method_name() == "predict")
    {
        return predict(cntl, req->Cast<com::shumei::service::PredictRequest>(),
                       res->Cast<com::shumei::service::PredictResult>(), done);
    }
    else
    {
        cntl->SetFailed(brpc::ENOMETHOD, "Fail to find method=%s",
                        cntl->thrift_method_name().c_str());
        done->Run();
    }
}

void PredictServiceImpl::predict(
    brpc::Controller *cntl,
    const com::shumei::service::PredictRequest *req,
    com::shumei::service::PredictResult *res,
    google::protobuf::Closure *done)
{
    brpc::ClosureGuard done_guard(done);
    res->detail = req->data;
}