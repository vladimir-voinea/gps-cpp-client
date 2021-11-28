PWD=`pwd`

IMAGE_NAME="gps_service"
BASE_IMAGE_VERSION="latest"

APP1_NAME="gps_service_app"
APP1_PATH_IN_BUILD_IMAGE="./build/src/gps_service_app/gps_service_app"

APP2_NAME="gps_service_client_app"
APP2_PATH_IN_BUILD_IMAGE="./build/src/gps_service_client_app/gps_service_client_app"

APP3_NAME="gstreamer_app"
APP3_PATH_IN_BUILD_IMAGE="./build/src/gstreamer_app/gstreamer_app"

#################
echo Building $IMAGE_NAME:build
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $IMAGE_NAME:build . -f Dockerfile.build --progress plain
docker run -v $PWD:/source -it $IMAGE_NAME:build

#################
cp $APP1_PATH_IN_BUILD_IMAGE ./app

echo Building $APP1_NAME:latest
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $APP1_NAME:latest .
rm ./app

#################
cp $APP2_PATH_IN_BUILD_IMAGE ./app

echo Building $APP2_NAME:latest
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $APP2_NAME:latest .
rm ./app

#################
cp $APP3_PATH_IN_BUILD_IMAGE ./app

echo Building $APP3_NAME:latest
docker build --build-arg BASE_IMAGE_VERSION=$BASE_IMAGE_VERSION -t $APP3_NAME:latest .
rm ./app
