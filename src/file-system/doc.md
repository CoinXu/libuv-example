# FileSystem

文件读取和写入涉及到几个最基本的接口：
```c
//
// 函数原型定义如下
//
int uv_fs_open(
  uv_loop_t* loop,
  uv_fs_t* req,
  const char* path,
  int flags,
  int mode, uv_fs_cb cb
);
int uv_fs_read(
  uv_loop_t* loop,
  uv_fs_t* req,
  uv_file file,
  const uv_buf_t bufs[],
  unsigned int nbufs,
  int64_t offset,
  uv_fs_cb cb
);
int uv_fs_write(
  uv_loop_t* loop,
  uv_fs_t* req,
  uv_file file,
  const uv_buf_t bufs[],
  unsigned int nbufs,
  int64_t offset,
  uv_fs_cb cb
);
int uv_fs_close(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb);

//
// 数据结构定义如下
//
struct uv_fs_s {
  UV_REQ_FIELDS
  uv_fs_type fs_type;
  uv_loop_t* loop;
  uv_fs_cb cb;
  ssize_t result;
  void* ptr;
  const char* path;
  uv_stat_t statbuf;  /* Stores the result of uv_fs_stat() and uv_fs_fstat(). */
  UV_FS_PRIVATE_FIELDS
};
```
还有诸多文件相关的接口，与linux系统函数一一对应。[详见](http://docs.libuv.org/en/v1.x/fs.html#api)

以上接口看名字就知道其功能，只是参数有些费解，在本篇文章中分别说明。[完整代码](./main.c)

## int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode, uv_fs_cb cb)
[int uv_fs_open](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_open)

该API与linux系统函数`open(2)` [https://linux.die.net/man/2/open](https://linux.die.net/man/2/open) 近似，
只是添加了loop与callback支持。

### 返回值
+ 执行正常时：`int` 文件描述 (__file descriptor__)
+ 执行错误时：`int -1`

### 参数说明
|    Type     | Name   | Describe |
| ----------  | ------ | -------- |
| uv_loop_t*  | loop   | 该函数挂载的轮询线程，一般为默认线程 `uv_default_loop()` |
| uv_fs_t*    | req    | 一个用于保存执行结果的结构，详见 [uv_fs_t](http://docs.libuv.org/en/v1.x/loop.html#c.uv_fs_t) |
| const char* | path   | 文件目录，如: `char* path = "/opt/app/libuv/doc.md"` |
| int         | flags  | 访问标识，可能的值为 `O_RDONLY`, `O_WRONLY` 和 `O_RDWR`，分别对应 `read-only`, `write-only` 和 `read/write`。此外还有诸多文件创建标识，比如`O_APPEND`, `O_ASYNC`, `O_CREAT`等，此处不一一列出。[详见](https://linux.die.net/man/2/open) |
| int         | mode   | 指定新创建的文件权限，只对`flags`为`O_CREAT`有效，如果`flags`为非`O_CREAT`，该参数将会被忽略。|
| uv_fs_cb    | cb     | 执行成功后的回调，执行该回调时，会传入`req`参数。`uv_fs_cb`原型定义为`void (*uv_fs_cb)(uv_fs_t* req)` |

__注__ : Window中使用`CreateFileW`，始终使用`binary mode`打开文件，所以flags参数不支持`O_BINARY`和`O_TEXT`。

### 使用示例
```c
void on_open(uv_fs_t* req);
uv_fs_t oepn_req;
const char* path = "/opt/app/libuv/doc.md";

uv_fs_open(uv_default_loop(), &open_req, path, O_RDONLY, 0, on_open);
```

## int uv_fs_read(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb)
[int uv_fs_read](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_read)

该API与linux系统函数`preadv(2)` [https://linux.die.net/man/2/preadv](https://linux.die.net/man/2/preadv) 近似，
只是添加了loop与callback支持。

### 返回值：
+ 执行正常时：`int` 返回读取内容的数量，单位为字节(bytes)
+ 执行错误时：`int -1`

### 参数说明
|     Type       | Name   | Describe |
| -------------- | ------ | -------- |
| uv_loop_t*     | loop   | 该函数挂载的轮询线程，一般为默认线程 `uv_default_loop()` |
| uv_fs_t*       | req    | 与uv_fs_open一样，只是`req.result`的结果保存内容为读取的内容的长度 |
| uv_file        | file   | 文件描述符，比如`int 10`，libuv使用自定的类型`uv_file`是为了跨平台 |
| const uv_buf_t | bufs   | 读取文件缓存的buffer，类型为`uv_buf_t []`，[uv_buf_t](http://docs.libuv.org/en/v1.x/misc.html#c.uv_buf_t) |
| unsigned int   | nbufs  | bufs的`length` |
| int64_t        | offset | 读取的偏移位置 |
| uv_fs_cb       | cb     | `void (*uv_fs_cb)(uv_fs_t* req)` |

### 使用示例
```c
void on_read(uv_fs_t* req);
uv_fs_t read_req;
unsigned int len = 1000 * sizeof(char);
char *base = (char *) malloc(len);
uv_buf_t iov = uv_buf_init(base, len);

uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
```

## int uv_fs_write(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb);
[int uv_fs_write](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_write)

该API与linux系统函数`pwritev(2)` [https://linux.die.net/man/2/preadv](https://linux.die.net/man/2/pwritev) 近似，
只是添加了loop与callback支持。

### 返回值：
+ 执行正常时: `int` 为已写入的字节数
+ 执行失败时：`int -1`

### 参数说明：
|     Type       | Name   | Describe |
| -------------- | ------ | -------- |
| uv_loop_t*     | loop   | 该函数挂载的轮询线程，一般为默认线程 `uv_default_loop()` |
| uv_fs_t*       | req    | 一个用于保存执行结果的结构，详见 [uv_fs_t](http://docs.libuv.org/en/v1.x/loop.html#c.uv_fs_t) |
| uv_file        | file   | 文件描述符，比如`int 10`，libuv使用自定的类型`uv_file`是为了跨平台 |
| const uv_buf_t | bufs   | 读取文件缓存的buffer，类型为`uv_buf_t []`，[uv_buf_t](http://docs.libuv.org/en/v1.x/misc.html#c.uv_buf_t) |
| unsigned int   | nbufs  | bufs的`length` |
| int64_t        | offset | 读取的偏移位置 |
| uv_fs_cb       | cb     | `void (*uv_fs_cb)(uv_fs_t* req)` |

### 使用示例
```c
uv_fs_t write_req;
unsigned int len = 1000 * sizeof(char);
char *base = (char *) malloc(len);
uv_buf_t iov = uv_buf_init(base, len);

uv_fs_write(uv_default_loop(), &write_req, STDOUT_FILENO, &iov, 1, -1, on_write);
```

## int uv_fs_close(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb);
[int uv_fs_close](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_close)

该API与linux系统函数`close(2)` [http://linux.die.net/man/2/close](http://linux.die.net/man/2/close) 近似，
只是添加了loop与callback支持。

### 返回值：
+ 执行正常时: `int 0`
+ 执行失败时：`int -1`

### 参数说明：
|     Type       | Name   | Describe |
| -------------- | ------ | -------- |
| uv_loop_t*     | loop   | 该函数挂载的轮询线程，一般为默认线程 `uv_default_loop()` |
| uv_fs_t*       | req    | 一个用于保存执行结果的结构，详见 [uv_fs_t](http://docs.libuv.org/en/v1.x/loop.html#c.uv_fs_t) |
| uv_file        | file   | 文件描述符，比如`int 10`，libuv使用自定的类型`uv_file`是为了跨平台 |
| uv_fs_cb       | cb     | `void (*uv_fs_cb)(uv_fs_t* req)` |

#### 使用示例
```c
uv_fs_t close_req;
uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
```

__注__:
1. 所有的函数都可以不传`uv_fs_cb`，此时为同步调用
2. 不同的函数传入的`uv_fs_t* req`在回调函数中其`req->result`的代表的值不同(虽然它们都是int类型)。
   `uv_fs_open`中的`req->result`保存的是打开文件的描述符，
   `uv_fs_read`中的`req->result`保存的是已读取内容的数量，
   `uv_fs_write`中的`req->result`保存的是已写入内容的数量。
3. 以上所有函数只有同步调用时(`uv_fs_cb`传入`NULL`)才有返回值