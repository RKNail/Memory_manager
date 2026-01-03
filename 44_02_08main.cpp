#include <iostream>
#include "os_mem.h"

using namespace std;

int main() {
    memory_manager_t mm;
    setup_memory_manager(&mm);
    printf("%d created mm, expected 1\n", mm.create(3, 0));
    printf("%d tried to create new mm, expected 0\n", mm.create(2, 0));
    printf("%d created object O1, expected 1\n", mm.create_object("O1"));
    printf("%d created object O2, expected 1\n", mm.create_object("O2"));
    printf("%d created object O3, expected 1\n", mm.create_object("O3"));
    printf("%d tried to create again O1, expected 0\n", mm.create_object("O1"));
    printf("%d tried to create again O2, expected 0\n", mm.create_object("O2"));
    printf("%d tried to create again O3, expected 0\n", mm.create_object("O3"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 1\n", mm.link("O1","O2"));
    printf("%d created O1->O2, expected 0 due to limit=16\n", mm.link("O1","O2"));
    printf("%d created O1->O3, expected 0 due to limit=16\n", mm.link("O1", "O3"));
    printf("%d created O2->O3, expected 1\n", mm.link("O2", "O3"));
    mm.print_link_counts();
    printf("%d destroy O2, expected 1\n", mm.destroy_object("O2"));
    mm.print_link_counts();
    printf("%d tried to create O1->O3, expected 0 due to old links didn't delete\n", mm.link("O1","O3"));
    printf("%d destroy mm, expected 1\n", mm.destroy());
    mm.print_link_counts();
    return 0;
}
