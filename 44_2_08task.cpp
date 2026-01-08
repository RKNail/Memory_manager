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
    while (it) {
        if (strcmp(it->name, name) > 0) {
            break;
        } else if (strcmp(it->name, name) == 0) {
            return 0;
        }
        prev = it;
        it = it->next;
    }
    char* obj_name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    if (!obj_name) return 0;
    strcpy(obj_name, name);
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
        it = it->next;
    }
    return NULL;
}

int destroy_object(const char* name) {
    if (not name) return 0;
    node* tmp = find(name);
    if (not tmp) return 0;
    if (tmp->prev) tmp->prev->next = tmp->next;
    else lst = tmp->next;
    if (tmp->next) tmp->next->prev = tmp->prev;
    free(tmp->name);
    free(tmp);
    --nodes;
    link_list* link_it = links, *temp = NULL;
    while (link_it) {
        int cmp = strcmp(link_it->from, name);
        if (cmp == 0) {
            tmp = find(link_it->to);
            if (tmp) --tmp->to;
            if (link_it->prev) link_it->prev->next = link_it->next;
            else links = link_it->next;
            if (link_it->next) link_it->next->prev = link_it->prev;
            temp = link_it;
            link_it = link_it->next;
            free(temp->from);
            free(temp->to);
            free(temp);
            continue;
        } else if (cmp > 0) {
            break;
        }
        link_it = link_it->next;
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
    link_list* prev = NULL;
    if (links) {
        link_list* link_it = links;
        while (link_it) {
            if (strcmp(link_it->from, link->from) >= 0) {
                break;
            }
            prev = link_it;
            link_it = link_it->next;
        }
        link->next = link_it;
        link->prev = prev;
        if (link_it) link_it->prev = link;
        if (prev) prev->next = link;
        else links = link;
    } else {
        links = link;
    }
    return 1;
}

void print_link_counts() {
    node* it = lst;
    while (it) {
        std::cout << it->name << " " << it->to << std::endl;
        it = it->next;
    }
}

void print_objects() {
    node* it = lst;
    while (it) {
        std::cout << it->name << std::endl;
        it = it->next;
    }
}

int destroy() {
    if (!created) return 0;
    --created;
    tot_size = 0;
    nodes = 0;
    node* node_it = lst, *node_it1 = lst;
    while (node_it) {
        free(node_it->name);
        node_it1 = node_it;
        node_it = node_it->next;
        free(node_it1);
    }
    link_list* link_it = links, *link_it1 = links;
    while (link_it) {
        free(link_it->from);
        free(link_it->to);
        link_it1 = link_it;
        link_it = link_it->next;
        free(link_it1);
    }
    lst = NULL;
    links = NULL;
    return 1;
}


void setup_memory_manager(memory_manager_t* mm) {
    mm->create = create;
    mm->destroy = destroy;
    mm->create_object = create_object;
    mm->link = link;
    mm->destroy_object = destroy_object;
    mm->print_link_counts = print_link_counts;
    mm->print_objects = print_objects;
}