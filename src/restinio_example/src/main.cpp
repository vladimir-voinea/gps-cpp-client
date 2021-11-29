#include <restinio/all.hpp>

#include <spdlog/spdlog.h>

int main()
{
    spdlog::info("Before");

    restinio::run(
        restinio::on_this_thread()
        .port(8080)
        .address("localhost")
        .request_handler([](auto req) {
            return req->create_response(restinio::status_ok()).set_body("Hello, World!").done();
        }));

    spdlog::info("After");

    return 0;
}