#ifndef object_h
#define object_h

#include <string>

enum class ObjectType { STRING };

class Object {
    ObjectType type;
    Object* next;

  public:
    virtual ~Object() = default;
    virtual void print() = 0;

    bool IsType(ObjectType checked) { return type == checked; }

    void set_next(Object* obj) { next = obj; }
    Object* get_next() { return next; }

  protected:
    explicit Object(ObjectType type) : type(type) {}
};

class StringObject : public Object {
    std::string str;

  public:
    explicit StringObject(std::string _str)
        : Object(ObjectType::STRING), str(std::move(_str)) {}

    StringObject(std::string a, std::string b)
        : Object(ObjectType::STRING), str(a + b) {}

    void print() override;
    [[nodiscard]] const std::string& value() const { return str; }
};

#endif
