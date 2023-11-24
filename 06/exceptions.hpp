#include <exception>
#include <string>

class BraceException : public std::exception {
    std::string _msg;

   public:
    BraceException(const std::string &msg) : _msg(msg){};
    const char *what() const noexcept override { return _msg.data(); }
};

class ArgException : public std::exception {
    std::string _msg;

   public:
    ArgException(const std::string &err) : _msg(err){};
    const char *what() const noexcept override { return _msg.data(); }
};