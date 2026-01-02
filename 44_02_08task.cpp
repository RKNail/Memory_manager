#include "os_mem.h"
#include <stdlib.h>
#include <iostream>

#define MAX_NAME 32

struct node {
    char is_root;
    int to;
    int from;
    char* name;
    node* next;
    node* prev;
};

struct link_list {
    char* from;
    char* to;
    link_list* next;
    link_list* prev;
};

int tot_size = 0;
int nodes = 0;
char created = 0;
node* lst = NULL;
link_list* links = NULL;

int create(int size, int num_pages) {
    if (!created) {
        ++created;
        tot_size = size;
        return 1;
    }
    return 0;
}
// Создаём объект, если его имя не длиннее 32 символов
// Если такого объекта нет
// Если не достигнуто ограничение на количество объектов
int create_object(const char* name) {
    if (!created or nodes == tot_size or not name or strlen(name) > MAX_NAME) return 0;
    node* it = lst, *prev = NULL;
    char* obj_name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    if (!obj_name) return 0;
    strcpy(obj_name, name);
    while (it) {
        if (strcmp(it->name, name) > 0) {
            break;
        } else if (strcmp(it->name, name) == 0) {
            free(obj_name);
            return 0;
        }
        prev = it;
        it = it->next;
    }
    node* obj = (node*)malloc(sizeof(node));
    if (!obj) return 0;
    obj->name = obj_name;
    obj->is_root = 0;
    obj->to = 0;
    obj->from = 0;
    obj->next = NULL;
    obj->prev = NULL;
    ++nodes;
    if (lst) {
        if (it) {
            if (prev) {
                obj->prev = prev;
                obj->next = it;
                it->prev = obj;
                prev->next = obj;
            } else {
                it->prev = obj;
                obj->next = it;
                lst = obj;
            }
        } else {
            if (prev) prev->next = obj;
            obj->prev = prev;
        }
    } else {
        lst = obj;
    }
    return 1;
}

// Ищем среди объектов существует ли объект с именем name
node* find(const char* name) {
    node* it = lst;
    while (it) {
        if (strcmp(it->name, name) == 0) {
            return it;
        }
    }
    return NULL;
}

int destroy_object(const char* name) {
    if (not name) return 0;
    node* obj = find(name);
    if (!obj) return 0;
    if (obj->prev) obj->prev->next = obj->next;
    if (obj->next) obj->next->prev = obj->prev;
    free(obj);
    obj = NULL;
    link_list* it = links, *begin = NULL,  *end = NULL;
    while (it) {
        int compare = strcmp(it->from, name);
        if (compare == 0) {
            if (!begin) begin = it;
            node* tmp = find(it->to);
            if (tmp) {
                --tmp->to;
            }
            end = it;
        } else if (compare > 0) {
            if (!begin) return 1;
            break;
        }
        it = it->next;
    }
    if (!end) {
        begin->prev->next = NULL;
        while (begin) {
            free(begin->from);
            free(begin->to);
            end = begin;
            begin = begin->next;
            free(end);
        }
    } else {
        begin->prev->next = end->next;
        end->next->prev = begin->prev;

    }
    return 1;
}

// Создаём ссылку из объекта object1_name в объект object2_name
// Храним в списке, где элементы упорядочены по первому значению
// (по второму было лень делать)
// Если не нашли первый или второй объект, то возвращаем 0
// Не выделили память - 0
// Достигнуто ограничение на количество ссылок в первом объекте(уже есть 16 ссылок)
// Всё остальное 1
int link(const char* object1_name, const char* object2_name) {
    if (not object1_name or not object2_name) return 0;
    char exist_1 = 0, exist_2 = 0;
    node* it = lst, *to_ptr, *from_ptr;
    while (it) {
        if (strcmp(it->name, object1_name) == 0) {
            exist_1 = 1;
            from_ptr = it;
        } else if (strcmp(it->name, object2_name) == 0) {
            exist_2 = 1;
            to_ptr = it;
        }
        it = it->next;
    }
    if (not exist_1 or not exist_2 or from_ptr->from == 16) return 0;
    link_list* link = (link_list*)malloc(sizeof(link_list));
    if (!link) return 0;
    char* obj1_name = (char*)malloc(sizeof(char) * (strlen(object1_name) + 1));
    if (!obj1_name) {
        free(link);
        return 0;
    }
    char* obj2_name = (char*)malloc(sizeof(char) * (strlen(object2_name) + 1));
    if (!obj2_name) {
        free(obj1_name);
        free(link);
        return 0;
    }
    strcpy(obj1_name, object1_name);
    strcpy(obj2_name, object2_name);
    link->from = obj1_name;
    link->to = obj2_name;
    link->next = NULL;
    link->prev = NULL;
    ++to_ptr->to;
    ++from_ptr->from;
    if (links) {
        link_list* link_it = links;
        while (link_it) {
            if (strcmp(link->from, link_it->from) >= 0) {
                link->prev = link_it->prev;
                link->next = link_it;
                link_it->prev = link;
                if (link->prev) link->prev->next = link;
                else links = link;
                break;
            }
            link_it = link_it->next;
        }
    } else {
        links = link;
    }
    return 1;
}

void print_link_counts() {
    node* it = lst;
    while (it) {
        std::cout << it->name << " " << it->to << std::endl;
    }
}

int destroy() {
    if (!created) return 0;
    created = 0;
    node* node_it = lst;
    while (node_it) {
        free(node_it->name);
        free(node_it);
    }

}


void setup_memory_manager(memory_manager_t* mm) {
    mm->create = create;
    mm->destroy = destroy;
    mm->create_object = create_object;
    mm->link = link;
    mm->destroy_object = destroy_object;
    mm->print_link_counts = print_link_counts;
}