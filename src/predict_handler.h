
#include <brpc/server.h>
#include <brpc/thrift_service.h>
#include "prediction_types.h"

class PredictServiceImpl : public brpc::ThriftService
{
public:
    void ProcessThriftFramedRequest(brpc::Controller *cntl,
                                    brpc::ThriftFramedMessage *req,
                                    brpc::ThriftFramedMessage *res,
                                    google::protobuf::Closure *done) override;

    void predict(brpc::Controller *cntl,
                 const com::shumei::service::PredictRequest *req,
                 com::shumei::service::PredictResult *res,
                 google::protobuf::Closure *done);
};