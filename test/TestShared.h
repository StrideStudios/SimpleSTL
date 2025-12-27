#pragma once

struct Abstract {
    Abstract() = default;
    Abstract(const size_t id): id(id) {}
    virtual ~Abstract() = default;
    virtual void print() const = 0;
    virtual void init(std::string inName) = 0;

    friend bool operator<(const Abstract& fst, const Abstract& snd) {
        return fst.id < snd.id;
    }

    friend bool operator==(const Abstract& fst, const Abstract& snd) {
        return fst.id == snd.id;
    }

    friend size_t getHash(const Abstract& obj) {
        return obj.id;
    }

    int id = 0;
};

struct Parent : Abstract{
    Parent() = default;
    Parent(const size_t id): Abstract(id) {}
    virtual ~Parent() = default;
    virtual void print() const {
        std::cout << "ID: " << id << std::endl;
    }

    virtual void init(std::string inName) override {}

    friend bool operator<(const Parent& fst, const Parent& snd) {
        return fst.id < snd.id;
    }

    friend bool operator==(const Parent& fst, const Parent& snd) {
        return fst.id == snd.id;
    }

    friend size_t getHash(const Parent& obj) {
        return obj.id;
    }
};

struct SObject : Parent {

    SObject() = default;
    SObject(const size_t id): Parent(id) {
        std::cout << "SOBJECT MAKE" << std::endl;
    }

    virtual void init(const std::string inName) override {
        name = inName;
        std::cout << "SOBJECT INIT" << std::endl;
    }

    void init(const std::string inName, const size_t inId) {
        name = inName;
        id = inId;
        std::cout << "SOBJECT INIT 2" << std::endl;
    }

    void destroy() const {
        std::cout << "SOBJECT " << name << " DESTROY" << std::endl;
    }

    std::string name = "None";

    virtual void print() const override {
        std::cout << "ID: " << id << " Name: " << name << std::endl;
    }

    friend bool operator<(const SObject& fst, const SObject& snd) {
        return fst.id < snd.id;
    }

    friend bool operator==(const SObject& fst, const SObject& snd) {
        return fst.id == snd.id;
    }

    friend size_t getHash(const SObject& obj) {
        return obj.id;
    }
};