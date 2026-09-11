#ifndef object_h
#define object_h

#include <string>
#include <utility>

enum class ObjectType { STRING };

class Object {
    ObjectType type;

  public:
    virtual ~Object() = default;
    virtual void print() = 0;

    bool IsType(ObjectType checked) { return type == checked; }

  protected:
    explicit Object(ObjectType type) : type(type) {}
};

class StringObject : public Object {
    std::string str;

  public:
    explicit StringObject(std::string _str)
        : Object(ObjectType::STRING), str(std::move(_str)) {}

    void print() override;
    [[nodiscard]] const std::string& value() const { return str; }
};

template <typename T, typename... Args>
T* allocate_object(Args&&... args) {
    return new T(std::forward<Args>(args)...);
}

#endif
