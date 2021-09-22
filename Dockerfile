FROM vladimir1/grpc_cxx:1.3.0-ubuntu-dynamic as base

COPY ./app .

CMD ["/app"]