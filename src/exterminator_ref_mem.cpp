#include <iostream>
#include <fstream>
#include <string>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

//for name value pairs when doing XML archiving
#include <boost/serialization/nvp.hpp>

class MyClassWithNoDefaultConstructor;
std::ostream& operator<<(std::ostream& os, const MyClassWithNoDefaultConstructor& object);

class MyClassWithNoDefaultConstructor
{
	public:
		MyClassWithNoDefaultConstructor(int i, char& c_)
			: intMember(i), c(c_)
		{}
		const int& getConstIntMember() const
		{
			return intMember;
		}
		const char& getConstCharRefMember() const
		{
			return c;
		}
	private:
		const int intMember;
		/*const*/ char& c;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			//ar & BOOST_SERIALIZATION_NVP(c);
		}

	friend class boost::serialization::access;
	friend std::ostream& operator<<(std::ostream& os, const MyClassWithNoDefaultConstructor& object);
};

std::ostream& operator<<(std::ostream& os, const MyClassWithNoDefaultConstructor& object)
{
	os << "\nMyClassWithNoDefaultConstructor contents:\n";
	os << "intMember - " << object.intMember << "\n";
	os << "charmember - " << object.c << "\n";
	return os;
}

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const MyClassWithNoDefaultConstructor* t, const unsigned int file_version)
		{
			ar << boost::serialization::make_nvp("intMember", t->getConstIntMember());
			ar << boost::serialization::make_nvp("charrefMember", t->getConstCharRefMember());
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, MyClassWithNoDefaultConstructor* t, const unsigned int file_version)
		{
			int m;
			char *c = new char();
			ar >> boost::serialization::make_nvp("intMember",m);
			ar >> boost::serialization::make_nvp("charrefMember",*c);
			::new(t)MyClassWithNoDefaultConstructor(m,*c);
		}
	}
}

void SerializeMyClassWithNoDefaultConstructor(const std::string& filename)
{
	char c = 'X';
    MyClassWithNoDefaultConstructor object(111, c);
    std::ofstream ofs(filename.c_str());
    assert(ofs.good());
    boost::archive::xml_oarchive xml_oa(ofs);
    MyClassWithNoDefaultConstructor * object_ptr = & object;
    xml_oa << BOOST_SERIALIZATION_NVP(object_ptr);
}

void DeserializeMyClassWithNoDefaultConstructor(const std::string& filename)
{
	MyClassWithNoDefaultConstructor * object_ptr;
	std::ifstream ifs(filename.c_str());
	assert(ifs.good());
	boost::archive::xml_iarchive xml_ia(ifs);
	xml_ia >> BOOST_SERIALIZATION_NVP(object_ptr);
	std::cout << *object_ptr;
	delete &(object_ptr->getConstCharRefMember());   //delete the char created on free store inside load_construct_data...
	delete object_ptr;
}

int main()
{
	const std::string filenameMyClassWithNoDefaultConstructor="/tmp/testfileMyClassWithNoDefaultConstructor.xml";
	try
	{
		SerializeMyClassWithNoDefaultConstructor(filenameMyClassWithNoDefaultConstructor);
		DeserializeMyClassWithNoDefaultConstructor(filenameMyClassWithNoDefaultConstructor);
	}
	catch(const boost::archive::archive_exception& ex)
	{
		std::cout << ex.what() << "\n";
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << "\n";
	}
	return 0;
}
