
#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/thrift_service.h>
#include "prediction_types.h"

DEFINE_int32(port, 8019, "TCP Port of this server");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
                                 "read/write operations during the last `idle_timeout_s'");
DEFINE_int32(max_concurrency, 0, "Limit of request processing in parallel");

class PredictServiceImpl : public brpc::ThriftService
{
public:
    void ProcessThriftFramedRequest(brpc::Controller *cntl,
                                    brpc::ThriftFramedMessage *req,
                                    brpc::ThriftFramedMessage *res,
                                    google::protobuf::Closure *done) override
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

    void predict(brpc::Controller *cntl,
                 const com::shumei::service::PredictRequest *req,
                 com::shumei::service::PredictResult *res,
                 google::protobuf::Closure *done)
    {
        brpc::ClosureGuard done_guard(done);
        res->detail = req->data;
    }
};

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    brpc::Server server;
    brpc::ServerOptions options;

    options.thrift_service = new PredictServiceImpl;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    options.max_concurrency = FLAGS_max_concurrency;

    if (server.Start(FLAGS_port, &options) != 0)
    {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }

    server.RunUntilAskedToQuit();
    return 0;
}
