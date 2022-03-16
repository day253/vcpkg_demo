#include <gflags/gflags.h>

#include "prediction_types.h"

#include <butil/logging.h>
#include <butil/time.h>
#include <brpc/channel.h>
#include <brpc/thrift_message.h>
#include <bvar/bvar.h>

bvar::LatencyRecorder g_latency_recorder("client");

DEFINE_string(server, "0.0.0.0:8019", "IP Address of server");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    brpc::Channel channel;

    brpc::ChannelOptions options;
    options.protocol = brpc::PROTOCOL_THRIFT;
    options.timeout_ms = FLAGS_timeout_ms /*milliseconds*/;
    options.max_retry = FLAGS_max_retry;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0)
    {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }

    brpc::ThriftStub stub(&channel);

    while (!brpc::IsAskedToQuit())
    {
        brpc::Controller cntl;
        com::shumei::service::PredictRequest req;
        com::shumei::service::PredictResult res;

        req.__set_data("hello");

        stub.CallMethod("predict", &cntl, &req, &res, NULL);

        if (cntl.Failed())
        {
            LOG(ERROR) << "Fail to send thrift request, " << cntl.ErrorText();
            sleep(1); // Remove this sleep in production code.
        }
        else
        {
            g_latency_recorder << cntl.latency_us();
            LOG(INFO) << "Thrift Response: " << res;
        }

        LOG_EVERY_SECOND(INFO)
            << "Sending thrift requests at qps=" << g_latency_recorder.qps(1)
            << " latency=" << g_latency_recorder.latency(1);

        sleep(1);
    }

    LOG(INFO) << "EchoClient is going to quit";
    return 0;
}
