#ifndef ACCESSOR_H
#define ACCESSOR_H

#define GETTER(name, methodName, Type)  Type methodName() const;
#define SETTER(name, methodName, Type)  void methodName(const Type &name);

#define GETTER_IMPL(Class, name, methodName, Type) Type Class::methodName() const { return this->name; }
#define SETTER_IMPL(Class, name, methodName, Type) void Class::methodName(const Type &name) { this->name = name; }

#endif // ACCESSOR_H
