FROM vladimir1/grpc_cxx:1.3.0-ubuntu-dynamic as base

RUN apt-get update
RUN apt-get install -q -yy libgps-dev

COPY ./app .

CMD ["/app"]