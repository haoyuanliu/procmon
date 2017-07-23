#include <map>
#include <stdarg.h>
#include <stdio.h>

#include "StatData.h"

class httpResponse {
    public:
        enum httpStatus {
            unknown,
            k200Ok = 200,
            k404NotFound = 404,
        };
        void setStatus(httpStatus status) {
            status_ = status;
        }
        void setStatusStr(const std::string & str) {
            statusStr_ = str;
        }
        void setCloseConnection(bool on) {
            closeConnection_ = on;
        }
        bool getCloseConnection() {
            return closeConnection_;
        }
        void setContentType(const std::string &type) {
            addHeader("Content-Type", type);
        }
        void addHeader(const std::string &key, const std::string &value) {
            headers_[key] = value;
        }
        void setBody(const std::string &body) {
            body_ = body;
        }
        void appendBody(const std::string &str);
        void appendBody(const char * fmt, ...) __attribute__ ((format (scanf, 2, 3)));
        void appendTableRow(const char *name, int value);
        void appendTableRow(const char *name, long value);
        void appendTableRow(const char *name, double value);
        void appendTableRow(const char *name, std::string value);

        std::string toString();

    private:
        std::map<std::string, std::string> headers_;
        httpStatus status_;
        std::string statusStr_;
        bool closeConnection_;
        std::string body_;
};
