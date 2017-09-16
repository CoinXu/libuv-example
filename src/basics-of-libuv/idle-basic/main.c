#include <stdio.h>
#include <uv.h>

int64_t counter = 0;

void wait_for_a_while(uv_idle_t *handle) {
    counter++;
    if (counter >= 10e6) {
        // 当执行规定次数后停止 handle
        printf("wait_for_a_while was called %lli times", counter);
        uv_idle_stop(handle);
    }
}

//
// http://docs.libuv.org/en/v1.x/idle.html
// Idle handles will run the given callback once per loop iteration,
// right before the uv_prepare_t handles.
//
// Note: The notable difference with prepare handles is that when there are active idle handles,
// the loop will perform a zero timeout poll instead of blocking for i/o.
//
// Warning: Despite the name, idle handles will get their callbacks called on every loop iteration,
// not when the loop is actually “idle”.
//
//
//
// Idle handles 将会在每个循环迭代中执行一次给定的callback，执行时间在 `uv_prepare_t` handles 处理之前
//
// 注：值得注意的是，与预处理(prepare) handles不同在于：当有空闲的handle时，循环将执行零超时轮询（译注：是指加入异步队列么？）来替代阻塞I/O
//
// 警告：请忽略idle handle的名字，idle handles 将在每个循环中获取并执行其回调，而不是当轮询真正"空闲"时。
//

int main() {
    uv_idle_t idler;
    uv_loop_t *loop = uv_default_loop();

    // 初始化 idle handle
    uv_idle_init(loop, &idler);
    // 启用 idle handle 并传入回调
    uv_idle_start(&idler, wait_for_a_while);

    printf("Idling... \n");

    // ==========================================
    // http://docs.libuv.org/en/v1.x/loop.html
    // ==========================================

    // int uv_run(uv_loop_t* loop, uv_run_mode mode)
    // This function runs the event loop. It will act differently depending on the specified mode:

    // UV_RUN_DEFAULT: Runs the event loop until there are no more active and referenced handles or requests.
    // Returns non-zero if uv_stop() was called and there are still active handles or requests. Returns zero in all other cases.
    //
    // UV_RUN_ONCE: Poll for i/o once. Note that this function blocks if there are no pending callbacks.
    // Returns zero when done (no active handles or requests left), or non-zero if more callbacks
    // are expected (meaning you should run the event loop again sometime in the future).
    //
    // UV_RUN_NOWAIT: Poll for i/o once but don’t block if there are no pending callbacks.
    // Returns zero if done (no active handles or requests left), or non-zero if more callbacks
    // are expected (meaning you should run the event loop again sometime in the future).

    // 开始轮询，直到该loop中没有活动的引用
    // 在此例中，当uv_idle_stop(handle)调用后，所有的活动执行完成
    // 此时将会停止loop
    uv_run(loop, UV_RUN_DEFAULT);

    // 关闭轮询
    // Releases all internal loop resources.
    uv_loop_close(loop);

    return 0;
}