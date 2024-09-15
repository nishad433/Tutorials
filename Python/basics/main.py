#!/usr/bin/python3

class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def __str__(self):
        return f"{self.name}({self.age})"

    def myfunc(self):
        print("Hello my name is %s" %(self.name))

def func1(s,no):
    print("string=%s no=%d" %(s,no))


x_str=str(5)    #x_str will be '5'
x_int=int(5)    #x_str will be 5
x_float=float(5)    #x_str will be 5.0
print("x=",x_int)
print("x=%d" %(x_int))
print("x=%f" %(x_float))
print("%r %r %r" %(type(x_int), type(x_str), type(x_float)))


a="Nishad"
# is same as
b='Nishad'
print("a=%s b=%r" %(a,b))

# Many Values to Multiple Variables
x,y,z = "Orange", "Banana", "Cherry"
print(x)
print(y)
print(z)

# One Value to Multiple Variables
x = y = z = "Orange"
print(x)
print(y)
print(z)


# Unpack a Collection
fruits = ["apple", "banana", "cherry"]
x, y, z = fruits
print(x)
print(y)
print(z)


x = "Python"
y = "is"
z = "awesome"
print(x, y, z)

x = "Python "
y = "is "
z = "awesome"
print(x + y + z)

x = 5
y = "John"
print(x, y)

p = Person("John", 36)
# f-string
print(f"x={x} a={a} p={p}")
func1("Nishad",45)
p.myfunc()

