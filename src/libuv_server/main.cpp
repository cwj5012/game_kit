#include <iostream>

#include <libuv/uv.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "pbrpc.pb.h"


const int DEFAULT_PORT = 8080;
const int DEFAULT_BACKLOG = 128;

struct sockaddr_in addr;
uv_loop_t* loop = nullptr;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

// 写数据回调结束后，释放内存
void free_write_req(uv_write_t* req) {
    write_req_t* wr = (write_req_t*)req;
    free(wr->buf.base);
    free(wr);
}

// 读数据回调前，分配 uv_buf_t 内存
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

// 写数据回调
void echo_write(uv_write_t* req, int status) {
    if ( status ) {
        LOG(ERROR) << "Write error " << uv_strerror(status);
    }
    free_write_req(req);
}

// 读数据回调
void echo_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    // 打印本地和远程的 IP 和端口
    struct sockaddr_in sockname{};
    struct sockaddr_in peername{};

    int namelen = sizeof(sockname);
    const int str_buf_len = sizeof(sockname);
    char str_buf[str_buf_len];
    int r = 0;

    r = uv_tcp_getsockname(reinterpret_cast<const uv_tcp_t*>(client),
                           reinterpret_cast<sockaddr*>(&sockname),
                           &namelen);
    if ( r ) {
        LOG(ERROR) << "error getsockname";
    }
    LOG(INFO) << "local address: " << inet_ntop(sockname.sin_family, &sockname.sin_addr, str_buf, str_buf_len)
        << ":" << ntohs(sockname.sin_port);

    r = uv_tcp_getpeername(reinterpret_cast<const uv_tcp_t*>(client),
                           reinterpret_cast<sockaddr*>(&peername),
                           &namelen);
    if ( r ) {
        LOG(ERROR) << "error getpeername";
    }

    LOG(INFO) << "remote address: " << inet_ntop(peername.sin_family, &peername.sin_addr, str_buf, str_buf_len)
        << ":" << ntohs(peername.sin_port);


    if ( nread > 0 ) {
        const std::string read_data(buf->base, nread);
        auto msg = pbrpc::EchoRequest();
        msg.ParseFromString(read_data);

        LOG(INFO) << "read: " << msg.message();

        write_req_t* req = new write_req_t;
        req->buf = uv_buf_init(buf->base, nread);
        uv_write(reinterpret_cast<uv_write_t*>(req), client, &req->buf, 1, echo_write);

        LOG(INFO) << "write: " << msg.message();

        return;
    }

    if ( nread < 0 ) {
        if ( nread != UV_EOF ) {
            LOG(ERROR) << "Read error " << uv_err_name(nread);
        }
        uv_close(reinterpret_cast<uv_handle_t*>(client), nullptr);
    }

    free(buf->base);
}

// 新客户端连接回调
void on_new_connection(uv_stream_t* server, int status) {
    LOG(INFO) << "on_new_connection";

    if ( status < 0 ) {
        LOG(ERROR) << "New connection error " << uv_strerror(status);
        return;
    }

    uv_tcp_t* client = new uv_tcp_t;
    uv_tcp_init(loop, client);
    if ( uv_accept(server, reinterpret_cast<uv_stream_t*>(client)) == 0 ) {
        uv_read_start(reinterpret_cast<uv_stream_t*>(client), alloc_buffer, echo_read);
    } else {
        uv_close(reinterpret_cast<uv_handle_t*>(client), nullptr);
    }
}

int main(int argc, char* argv[]) {

    // 初始化 glog
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;


    // 初始化 libuv
    loop = uv_default_loop();

    // tcp 连接
    uv_tcp_t server;
    uv_tcp_init(loop, &server);
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
    uv_tcp_bind(&server, reinterpret_cast<const struct sockaddr*>(&addr), 0);

    int r = uv_listen(reinterpret_cast<uv_stream_t*>(&server), DEFAULT_BACKLOG, on_new_connection);
    if ( r ) {
        LOG(ERROR) << "Listen error " << uv_strerror(r);
        return 1;
    }

    // 初始化 gtest 框架
    ::testing::InitGoogleTest(&argc, argv);

    // 运行所有 TEST 和 TEST_F 测试
    RUN_ALL_TESTS();

//    uv_run(loop, UV_RUN_DEFAULT);
//    uv_loop_close(loop);

    return 0;
}
