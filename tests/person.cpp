#include "person.h"

Person::Person(QObject* parent)
  : QObject(parent)
{
}

Person::~Person()
{
}

QString Person::name() const
{
  return m_name;
}

void Person::setName(const QString& name)
{
  m_name = name;
}

int Person::phoneNumber() const
{
  return m_phoneNumber;
}

void Person::setPhoneNumber(const int phoneNumber)
{
  m_phoneNumber = phoneNumber;
}

void Person::setGender(Gender gender)
{
  m_gender = gender;
}

Person::Gender Person::gender() const
{
  return m_gender;
}

QDate Person::dob() const
{
  return m_dob;
}

void Person::setDob(const QDate& dob)
{
  m_dob = dob;
}
