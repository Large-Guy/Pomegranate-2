#ifndef POMEGRANATE_ENGINE_CORE_RESOURCE_H
#define POMEGRANATE_ENGINE_CORE_RESOURCE_H
#include<string>
#include<core/serializable.h>

class Resource : public Serializable {
private:
    std::string _name = "";
    std::string _path = "";
    uint _id = 0;
    bool _runtime = false;
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


#endif //POMEGRANATE_ENGINE_RESOURCE_H
