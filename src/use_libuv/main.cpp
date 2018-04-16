#include <iostream>

#include <libuv/uv.h>

const int DEFAULT_PORT = 8080;
const int DEFAULT_BACKLOG = 128;

struct sockaddr_in addr;

void on_new_connection(uv_stream_t* server, int status) {
    std::cout << "on_new_connection" << std::endl;
}

void timer_cb(uv_timer_t* handle) {
    std::cout << "timer_cb" << std::endl;
}

void idle_cb(uv_idle_t* handle) {
    // std::cout << "idle_cb" << std::endl;
}

int main(int argc, char* argv[]) {
    uv_loop_t* loop = uv_default_loop();
    uv_loop_init(loop);

    // tcp 连接
    uv_tcp_t server;
    uv_tcp_init(loop, &server);
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }

    // 定时器
    uv_timer_t time_req;
    uv_timer_init(loop, &time_req);
    uv_timer_start(&time_req, timer_cb, 1000, 1000);

    // 闲置
    uv_idle_t idler;
    uv_idle_init(loop, &idler);
    uv_idle_start(&idler, idle_cb);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    return 0;
}



//#include <libuv/uv.h>
//
//int main(int argc, char* argv[]) {
//	uv_loop_t *loop = uv_default_loop();
//	uv_loop_init(loop);
//	
//	uv_run(loop, UV_RUN_DEFAULT);
//
//	uv_loop_close(loop);
//
//	return 0;
//}
