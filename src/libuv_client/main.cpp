#include <iostream>
#include <thread>
#include <string>

#include <libuv/uv.h>
#include <glog/logging.h>

#include "pbrpc.pb.h"

const char* DEFAULT_IP = "0.0.0.0";
const int DEFAULT_PORT = 8080;

std::string text;
int result;

uv_loop_t* loop;
uv_tcp_t* client_socket;
uv_connect_t* client_connect;

uv_stream_t* server_socket;
uv_write_t write_req = {};

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void echo_read(uv_stream_t* server, ssize_t nread, const uv_buf_t* buf) {
    if ( nread == -1 ) {
        LOG(ERROR) << "error echo_read";
        return;
    }

    const std::string read_data(buf->base, nread);
    LOG(INFO) << "read: " << read_data;
}

void on_write_end(uv_write_t* req, int status) {
    if ( status == -1 ) {
        LOG(ERROR) << "error on_write_end";
        return;
    }

    // 回调先执行 alloc_buffer，再调用 echo_read
    uv_read_start(req->handle, alloc_buffer, echo_read);
}


void on_connect(uv_connect_t* req, int status) {
    if ( status == -1 ) {
        LOG(ERROR) << "error on_connect";
        return;
    }

    char* message = const_cast<char*>("hello from client");
    const int len = strlen(message);

    uv_buf_t buf = uv_buf_init(message, len);

    server_socket = req->handle;

    const int buf_count = 1;

    result = uv_write(&write_req, req->handle, &buf, buf_count, on_write_end);
    if ( result ) {
        LOG(ERROR) << uv_strerror(result);
    }

}

// 接收命令行输入
void GetInput() {
    while ( std::getline(std::cin, text) ) {
        auto msg = pbrpc::EchoRequest();
        msg.set_message("hello");
        if (!msg.SerializeToString(&text)) {
            LOG(INFO) << "error serialize to string";
        }
        const char* message = text.c_str();

        // const char* message = text.c_str();

        const int len = strlen(message);
        uv_buf_t buf = uv_buf_init(const_cast<char*>(message), len);
        const int buf_count = 1;
        result = uv_write(&write_req, server_socket, &buf, buf_count, on_write_end);
        if ( result ) {
            LOG(ERROR) << uv_strerror(result);
        }
    }
}

int main(int argc, char* argv[]) {

    //初始化 glog
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    // 启动一个线程，监听命令行输入
    std::thread thr(GetInput);

    // 初始化 libuv
    loop = uv_default_loop();

    // 新建 tcp 连接
    client_socket = new uv_tcp_t;
    uv_tcp_init(loop, client_socket);

    client_connect = new uv_connect_t;

    sockaddr_in dest = {};
    uv_ip4_addr(DEFAULT_IP, DEFAULT_PORT, &dest);

    uv_tcp_connect(client_connect,
                   client_socket,
                   reinterpret_cast<const struct sockaddr*>(&dest),
                   on_connect);

    uv_run(loop, UV_RUN_DEFAULT);

    // 等待监听命令行输入线程执行完成
    thr.join();

    uv_loop_close(loop);

    return 0;
}
