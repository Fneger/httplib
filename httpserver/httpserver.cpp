#include <httplib.h>
#include <iostream>

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

using namespace httplib;

int main(void) {

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    SSLServer svr(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
    Server svr;
#endif
    auto postJson = [](const Request &req, Response &res, const ContentReader &content_reader) {
        res.status = 200;
        res.set_content("{\n    result: true\n}", "application/json");
        if (req.is_multipart_form_data()) {
              // NOTE: `content_reader` is blocking until every form data field is read
              MultipartFormDataItems files;
              content_reader(
                [&](const MultipartFormData &file) {
                  files.push_back(file);
                  return true;
                },
                [&](const char *data, size_t data_length) {
                  files.back().content.append(data, data_length);
                  return true;
                });
            } else {
              std::string body;
              content_reader([&](const char *data, size_t data_length) {
                body.append(data, data_length);
                std::cout << body << std::endl;
                return true;
              });
            }
    };
    svr.Post("/post", postJson);
    svr.listen("0.0.0.0", 8080);
    return 0;
}
