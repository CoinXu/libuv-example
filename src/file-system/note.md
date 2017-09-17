# FileSystem

文件读取和写入涉及到几个最基本的API。
```c
// 打开文件
int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode, uv_fs_cb cb)
// 读取文件
int uv_fs_read(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb)
// 写入文件
int uv_fs_write(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb)
// 关闭文件
int uv_fs_close(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb)
```

## int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode, uv_fs_cb cb)
[uv_fs_open](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_open)

该API与linux系统函数`open(2)` [https://linux.die.net/man/2/open](https://linux.die.net/man/2/open) 近似，
只是添加了loop与callback支持。

返回值：
+ 执行正常时：`int` 文件描述 (__file descriptor__)
+ 执行错误时：`int -1`

参数说明：

|    Type     | Name   | Describe |
| ----------  | ------ | -------- |
| uv_loop_t*  | loop   | 该函数挂载的轮询线程，一般为默认线程 `uv_default_loop()` |
| uv_fs_t*    | req    | 一个用于保存读取结果的结构，详见 [uv_fs_t](http://docs.libuv.org/en/v1.x/loop.html#c.uv_fs_t) |
| const char* | path   | 文件目录，如: `char* path = "/opt/app/libuv/doc.md"` |
| int         | flags  | 访问标识，可能的值为 `O_RDONLY`, `O_WRONLY` 和 `O_RDWR`，分别对应 `read-only`, `write-only` 和 `read/write`。此外还有诸多文件创建标识，比如`O_APPEND`, `O_ASYNC`, `O_CREAT`等，此处不一一列出。[详见](https://linux.die.net/man/2/open) |
| int         | mode   | 指定新创建的文件权限，只对`flags`为`O_CREAT`有效，如果`flags`为非`O_CREAT`，该参数将会被忽略。|
| uv_fs_cb    | cb     | 执行成功后的回调，执行该回调时，会传入`req`参数。`uv_fs_cb`原型定义为`void (*uv_fs_cb)(uv_fs_t* req)` |

使用示例
```c
const char* path = "/opt/app/libuv/doc.md"
uv_fs_open(uv_default_loop(), &open_req, path, O_RDONLY, 0, on_open);
```

## int uv_fs_read(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb)
[uv_fs_read](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_read)

该API与linux系统函数`preadv(2)` [https://linux.die.net/man/2/preadv](https://linux.die.net/man/2/preadv) 近似，
只是添加了loop与callback支持。

返回值：
+ 执行正常时：`int` 返回读取内容的数量，单位为字节(bytes)
+ 执行错误时：`int -1`

参数说明：

|     Type       | Name   | Describe |
| -------------- | ------ | -------- |
| uv_loop_t*     | loop   | 该函数挂载的轮询线程，一般为默认线程 `uv_default_loop()` |
| uv_fs_t*       | req    | 一个用于保存读取结果的结构，详见 [uv_fs_t](http://docs.libuv.org/en/v1.x/loop.html#c.uv_fs_t) |
| uv_file        | file   | 文件描述符，比如`int 10`，libuv使用自定的类型`uv_file`是为了跨平台 |
| const uv_buf_t | bufs   | 读取文件缓存的buffer，类型为`uv_buf_t []`，[uv_buf_t](http://docs.libuv.org/en/v1.x/misc.html#c.uv_buf_t) |
| unsigned int   | nbufs  | bufs的`length` |
| int64_t        | offset | 读取的领衔位置 |
| uv_fs_cb       | cb     | 执行成功后的回调，执行该回调时，会传入`req`参数。`uv_fs_cb`原型定义为`void (*uv_fs_cb)(uv_fs_t* req)` |

