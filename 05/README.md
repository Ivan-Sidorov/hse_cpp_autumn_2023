# Домашнее задание к лекции №6

## Классы сериализатор и десериализатор

Класс сериализатор содержит следующие методы:
```c++
class Serializer {
   public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }
};
```

Класс десериализатор содержит следующие методы:
```c++
class Deserializer {
   public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }
};
```

`Makefile` содержит цель-тест:
```bash
make
make test
make clean
```