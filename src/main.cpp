#include<iostream>
#include <vector2.h>

//Create a nested serializable struct

struct test : public Serializable {
    Vector2 v;
    void serialize(Archive& a) const override {
        a << v;
    }

    void deserialize(Archive& a) const override {
        a >> (Serializable*)&v;
    }
};

int main() {
    //Create a test object
    test t;
    t.v = Vector2(1, 2);
    //Print the object
    std::cout << "Before serialization: " << t.v.x << ", " << t.v.y << std::endl;
    //Serialize the object
    Archive a;
    t.serialize(a);
    //Write the archive to a file
    a.write_to_file("test.bin");
    //Read the archive from a file
    Archive b;
    b.read_from_file("test.bin");
    //Deserialize the object
    test t2;
    t2.deserialize(b);
    //Print the object
    std::cout << "After serialization: " << t2.v.x << ", " << t2.v.y << std::endl;
    return 0;
}