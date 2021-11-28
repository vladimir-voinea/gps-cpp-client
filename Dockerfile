ARG BASE_IMAGE_VERSION
FROM vladimir1/cxx_base:$BASE_IMAGE_VERSION as base

COPY ./app .

CMD ["/app"]