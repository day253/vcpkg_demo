
#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/thrift_service.h>
#include "prediction_types.h"
#include "src/predict_handler.h"

DEFINE_int32(port, 8019, "TCP Port of this server");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
                                 "read/write operations during the last `idle_timeout_s'");
DEFINE_int32(max_concurrency, 0, "Limit of request processing in parallel");


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
