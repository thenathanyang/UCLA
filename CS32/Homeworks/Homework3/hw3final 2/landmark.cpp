// landmark.cpp

#include <iostream>
#include <string>
using namespace std;

//Your declarations and implementations would go here

class Landmark
{
  public:
    Landmark(string name);
    virtual string color() const;
    virtual string icon() const = 0;
    string name() const;
    virtual ~Landmark();
  private:
    string m_name;
};

class Hotel : public Landmark
{
  public:
    Hotel(string name);
    virtual string icon() const;
    ~Hotel();
};

class Restaurant : public Landmark
{
  public:
    Restaurant(string name, int cap);
    virtual string icon() const;
    ~Restaurant();
  private:
    int m_cap;
};

class Hospital : public Landmark
{ 
  public:
    Hospital(string name);
    virtual string color() const;
    virtual string icon() const;
    ~Hospital();
  private:
    string m_color;
};



Landmark::Landmark(string name)
  : m_name(name)
{}

string Landmark::name() const
{
    return m_name;
}

string Landmark::color() const
{
    return "yellow";
}

Landmark::~Landmark()
{}



Hotel::Hotel(string name)
  : Landmark(name) 
{}

string Hotel::icon() const
{
    return "bed";
}

Hotel::~Hotel()
{
    cout << "Destroying the hotel " << name() << "." << endl;
}



Restaurant::Restaurant(string name, int cap)
  : Landmark(name), m_cap(cap)
{}

string Restaurant::icon() const
{
    if (m_cap < 40)
        return "small knife/fork";
    else
        return "large knife/fork";
}

Restaurant::~Restaurant()
{
    cout << "Destroying the restaurant " << name() << "." << endl;
}



Hospital::Hospital(string name)
  : Landmark(name)
{}

string Hospital::color() const
{
    return "blue";
}

string Hospital::icon() const
{
    return "H";
}

Hospital::~Hospital()
{
    cout << "Destroying the hospital " << name() << "." << endl;
}
