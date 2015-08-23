/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef TESTO_HPP
#define TESTO_HPP



class Kadombase
{
public:
  virtual void callso()
  {
	  kke::Logger::Log("base callso()");
  }	
  void caller()
  {
	  kke::Logger::Log("base caller()");
	  callso();
  }
  virtual ~Kadombase()
  {
	  kke::Logger::Log("base ~Kadombase()");
  }
};

class Kadomderv : public Kadombase
{
public:
	void callso()
	{
		kke::Logger::Log("derv callso()");
	}
	
    ~Kadomderv()
	{
		kke::Logger::Log("derv ~Kadomderv()");
	}
};

#endif