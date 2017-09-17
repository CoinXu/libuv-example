//
// Created by coinxu on 17/09/2017.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <unistd.h>

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;
uv_buf_t iov;

void on_write(uv_fs_t *req);
void on_read(uv_fs_t *req);
void on_open(uv_fs_t *req);


void on_write(uv_fs_t *req) {
    assert(req == &write_req);
    if (req->result < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror((int) req->result));
    } else {
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
    }
}

void on_read(uv_fs_t *req) {
    assert(req == &read_req);

    if (req->result < 0) {
        fprintf(stderr, "Read error: %s\n", uv_strerror((int) req->result));
    } else if (req->result == 0) {
        uv_fs_t close_req;
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    } else if (req->result > 0) {
        iov.len = req->result;
        // 输出到标准输出 STDOUT_FILENO
        // https://linux.die.net/man/2/pwritev
        uv_fs_write(uv_default_loop(), &write_req, STDOUT_FILENO, &iov, 1, -1, on_write);
    }
}

// typedef void (*uv_fs_cb)(uv_fs_t* req);
void on_open(uv_fs_t *req) {
    assert(req == &open_req);
    if (req->result >= 0) {
        fprintf(stdout, "iov.length %zu", iov.len);
        //        UV_EXTERN int uv_fs_read(uv_loop_t* loop,
        //                                 uv_fs_t* req,
        //                                 uv_file file,
        //                                 const uv_buf_t bufs[],
        //                                 unsigned int nbufs,    // 缓冲区的个数
        //                                 int64_t offset,
        //                                 uv_fs_cb cb);
        //
        // uv_buf_t 定义的是一个具有 {
        //   char* base;
        //   size_t len;
        // }
        // 的指针
        // 所以此函数的 bufs 为 uv_buf_t[] 类型，意味着调用的时候可以传入多个 uv_buf_t。
        // 就像Linux原有的readv一样
        //
        // Reference
        // http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_read
        // https://linux.die.net/man/2/preadv
        // http://blog.csdn.net/ieearth/article/details/46730669
        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
    } else {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int) req->result));
    }
}


int main(int argc, char **argv) {

    unsigned int len = 1000 * sizeof(char);
    char *base = (char *) malloc(len);
    iov = uv_buf_init(base, len);

    fprintf(stdout, "Open path is: %s\n", argv[1]);
    uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&read_req);
    uv_fs_req_cleanup(&write_req);

    return 0;
}
