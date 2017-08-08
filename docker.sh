name=libuv
WORKSPACE_DIRECTORY=~/github/libuv-example

docker run \
--name ${name} -it \
-v ${WORKSPACE_DIRECTORY}:/opt/libuv \
libuv:1.0 bash

echo docker stop ${name} && docker rm ${name}