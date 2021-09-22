IMAGE_NAME="cpptemplate"

APP1_NAME="cpptemplate_server"
APP1_PATH_IN_BUILD_IMAGE="/source/build/src/grpc_server_app/grpc_server_app"

APP2_NAME="cpptemplate_client"
APP2_PATH_IN_BUILD_IMAGE="/source/build/src/grpc_client_app/grpc_client_app"

#################
echo Building $IMAGE_NAME:build
docker build -t $IMAGE_NAME:build . -f Dockerfile.build

#################
docker container create --name extract $IMAGE_NAME:build  

#################
docker container cp extract:$APP1_PATH_IN_BUILD_IMAGE ./app

echo Building $APP1_NAME:latest
docker build --no-cache -t $APP1_NAME:latest .
rm ./app

#################
docker container cp extract:$APP2_PATH_IN_BUILD_IMAGE ./app

echo Building $APP2_NAME:latest
docker build --no-cache -t $APP2_NAME:latest .
rm ./app

#################
docker container rm -f extract

