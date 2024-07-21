#ifndef POMEGRANATEENGINE_RESOURCE_H
#define POMEGRANATEENGINE_RESOURCE_H
#include<string>
#include<serializable.h>

class Resource : public Serializable {
private:
    std::string _name;
    std::string _path;
    uint _id;
    bool _runtime;
    static uint resourceCount;
public:
    explicit Resource(std::string path = "", std::string name = "");
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getPath() const;
    [[nodiscard]] uint getId() const;

    virtual void reload();

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_RESOURCE_H
