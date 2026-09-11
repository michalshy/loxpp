#include "memory.h"

void free_objects(Object* head) {
    while (head) {
        Object* to_delete = head;
        head = head->get_next();
        delete to_delete;
    }
}
