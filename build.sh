arch=$(uname -m)

IMAGE_NAME="gps_service"

APP1_NAME="gps_service_app"
APP1_PATH_IN_BUILD_IMAGE="/source/build/src/gps_service_app/gps_service_app"

APP2_NAME="gps_service_client_app"
APP2_PATH_IN_BUILD_IMAGE="/source/build/src/gps_service_client_app/gps_service_client_app"

APP3_NAME="gstreamer_app"
APP3_PATH_IN_BUILD_IMAGE="/source/build/src/gstreamer_app/gstreamer_app"

#################
echo Building $IMAGE_NAME:build
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $IMAGE_NAME:build . -f Dockerfile.build --progress plain

#################
docker container create --name extract $IMAGE_NAME:build  

#################
docker container cp extract:$APP1_PATH_IN_BUILD_IMAGE ./app

echo Building $APP1_NAME:latest
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $APP1_NAME:latest . --progress plain
rm ./app

#################
docker container cp extract:$APP2_PATH_IN_BUILD_IMAGE ./app

echo Building $APP2_NAME:latest
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $APP2_NAME:latest . --progress plain
rm ./app

#################
docker container cp extract:$APP3_PATH_IN_BUILD_IMAGE ./app

echo Building $APP3_NAME:latest
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $APP3_NAME:latest . --progress plain
rm ./app

#################
docker container rm -f extract
