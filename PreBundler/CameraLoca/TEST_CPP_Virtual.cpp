#include <iostream>

using namespace std;

class Animal
{
public:
	void print_animal_kind()
	{
		cout <<"animal, print_animal_kind:" <<animal_kind_private << endl;
	}
	virtual void eat() { std::cout << "I'm eating generic food.\n"; }
	int animal_kind_pubilc;
private:
	int animal_kind_private;
protected:
	int animal_kind_protect;
};

class Cat : public Animal
{
public:
	void print_cat_kind()
	{	
		cout << "cat, print_cat_kind:" << cat_kind_public << endl;
	}
	virtual void eat() { std::cout << "I'm eating a rat.\n"; }
	int cat_kind_public;
private:
	int cat_kind_private;
protected:
	int cat_kind_protected;
};

class SmallCat:public Cat
{
public:
	virtual void eat()
	{ 
		small_cat_kind = 0;
		std::cout << "I am eat a small rat.\n";
	}
private:
	int small_cat_kind;
};

void func(Animal *xyz) { xyz->eat(); }

int main(){

	Animal *animal = new Animal;
	Cat *cat = new Cat;
	SmallCat* scat = new SmallCat;

	func(animal); // outputs: "I'm eating generic food."
	func(cat);
	func(scat);

	animal->eat();
	cat->eat();
	scat->eat();
	
	cat->print_animal_kind();
	cat->print_cat_kind();
	
	return 0;
}