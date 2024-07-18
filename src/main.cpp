#include<iostream>
#include <vector2.h>
#include <entity.h>

int main() {
    Entity e = Entity("Hello World!");
    std::cout << e.getName() << std::endl;
    std::cout << e.getId() << std::endl;

    //Serialization
    Archive a;
    e.serialize(a);
    //Write to file
    a.write_to_file("entity.bin");

    //Read from file
    Archive b;
    b.read_from_file("entity.bin");
    Entity e2;
    e2.deserialize(b);
    std::cout << e2.getName() << std::endl;
    std::cout << e2.getId() << std::endl;
    return 0;
}