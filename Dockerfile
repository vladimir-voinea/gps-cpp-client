ARG BASE_IMAGE_VERSION
FROM vladimir1/grpc_cxx:$BASE_IMAGE_VERSION as base

RUN apt-get update
RUN apt-get install -q -yy libgps-dev

COPY ./app .

CMD ["/app"]