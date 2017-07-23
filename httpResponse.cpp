#include "httpResponse.h"

std::string httpResponse::toString() {
    char buf[32];
    std::string res;
    snprintf(buf, sizeof buf, "HTTP/1.1 %d ", status_);
    res.append(buf);
    res.append(statusStr_);
    res.append("\r\n");

    if (closeConnection_) {
        res.append("Connection: close\r\n");
    } else {
        snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
        res.append(buf);
        res.append("Connection: Keep-Alive\r\n");
    }
    std::map<std::string, std::string>::iterator it;
    for (it = headers_.begin(); it != headers_.end(); ++it) {
        res.append(it->first);
        res.append(":");
        res.append(it->second);
        res.append("\r\n");
    }

    res.append("\r\n");
    res.append(body_);
    return res;
}

void httpResponse::appendBody(const std::string &str) {
    body_.append(str);
}

void httpResponse::appendBody(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(buf, sizeof buf, fmt, args);
    va_end(args);
    body_.append(buf);
}

void httpResponse::appendTableRow(const char * name, int value) {
    appendBody("<tr><td>%s</td><td>%d</td></tr>\n", name, value);
}

void httpResponse::appendTableRow(const char * name, long value) {
    appendBody("<tr><td>%s</td><td>%ld</td></tr>\n", name, value);
}
void httpResponse::appendTableRow(const char * name, double value) {
    appendBody("<tr><td>%s</td><td>%.2f</td></tr>\n", name, value);
}
void httpResponse::appendTableRow(const char * name, std::string value) {
    appendBody("<tr><td>%s</td><td>%s</td></tr>\n", name, value.c_str());
}

