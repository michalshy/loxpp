#ifndef object_h
#define object_h

#include <string>
#include <string_view>
#include <utility>

class Object {
  public:
    virtual ~Object() = default;
    virtual void print() = 0;
};

class StringObject : public Object {
    std::string str;

  public:
    explicit StringObject(std::string_view str) : str(str) {}
    void print() override;
    [[nodiscard]] std::string_view value() { return str; }
};

template <typename T, typename... Args>
T* allocate_object(Args&&... args) {
    return new T(std::forward<Args>(args)...);
}

#endif
