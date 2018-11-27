#include "stdafx.h"
#include "CppUnitTest.h"

#include <functional>

#include "Registry\Registry.h"
#include "Registry\RegistryException.h"
#include "Registry\RegistryValue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AbsCoDes::Registry;
using namespace AbsCoDes::Registry::Exceptions;

namespace RegistryTests
{		
	TEST_CLASS(RegistryValue_Tests)
	{
	public:

		TEST_METHOD(RegistryValue_Constructors)
		{
			RegistryValue v1;
			Assert::IsTrue(v1.GetType() == RegistryValueType::None);

			RegistryValue v2(RegistryValueType::DWord);
			Assert::IsTrue(v2.GetType() == RegistryValueType::DWord);

			RegistryValue v3(RegistryValueType::QWord);
			Assert::IsTrue(v3.GetType() == RegistryValueType::QWord);

			RegistryValue v4(RegistryValueType::String);
			Assert::IsTrue(v4.GetType() == RegistryValueType::String);

			RegistryValue v5(RegistryValueType::ExpandString);
			Assert::IsTrue(v5.GetType() == RegistryValueType::ExpandString);

			RegistryValue v6(RegistryValueType::MultiString);
			Assert::IsTrue(v6.GetType() == RegistryValueType::MultiString);

			RegistryValue v7(RegistryValueType::Binary);
			Assert::IsTrue(v7.GetType() == RegistryValueType::Binary);
		}

		TEST_METHOD(RegistryValue_None)
		{
			RegistryValue rv(RegistryValueType::DWord);
			rv.Reset();
			Assert::IsTrue(rv.IsEmpty());
		}

		TEST_METHOD(RegistryValue_Getter)
		{
			RegistryValue v2(RegistryValueType::DWord);
			Assert::IsTrue(v2.DWord() == 0);
			
			std::function<DWORD (void)> f2 = [v2] { return v2.DWord(); };
			Assert::IsTrue(f2() == 0);

			RegistryValue v3(RegistryValueType::QWord);
			Assert::IsTrue(v3.QWord() == 0);
			
			std::function<ULONGLONG (void)> f3 = [v3] { return v3.QWord(); };
			Assert::IsTrue(f3() == 0);

			RegistryValue v4(RegistryValueType::String);
			Assert::IsTrue(v4.String() == "");
			
			std::function<std::string (void)> f4 = [v4] { return v4.String(); };
			Assert::IsTrue(f4() == "");

			RegistryValue v5(RegistryValueType::ExpandString);
			Assert::IsTrue(v5.ExpandString() == "");
			
			std::function<std::string (void)> f5 = [v5] { return v5.ExpandString(); };
			Assert::IsTrue(f5() == "");

			RegistryValue v6(RegistryValueType::MultiString);
			Assert::IsTrue(v6.MultiString().size() == 0);
			
			std::function<std::vector<std::string> (void)> f6 = [v6] { return v6.MultiString(); };
			Assert::IsTrue(f6().size() == 0);

			RegistryValue v7(RegistryValueType::Binary);
			Assert::IsTrue(v7.Binary().size() == 0);
			
			std::function<std::vector<BYTE> (void)> f7 = [v7] { return v7.Binary(); };
			Assert::IsTrue(f7().size() == 0);
		}

		TEST_METHOD(RegistryValue_Setter)
		{
			RegistryValue v2(RegistryValueType::DWord);
			v2.DWord() = 1;
			Assert::IsTrue(v2.DWord() == 1);

			RegistryValue v3(RegistryValueType::QWord);
			v3.QWord() = 1;
			Assert::IsTrue(v3.QWord() == 1);

			RegistryValue v4(RegistryValueType::String);
			v4.String() = "String";
			Assert::IsTrue(v4.String() == "String");

			RegistryValue v5(RegistryValueType::ExpandString);
			v5.ExpandString() = "ExpandString";
			Assert::IsTrue(v5.ExpandString() == "ExpandString");

			RegistryValue v6(RegistryValueType::MultiString);
			std::vector<std::string> multiString;
			multiString.push_back("MultiString 1");
			multiString.push_back("MultiString 2");
			v6.MultiString() = multiString;
			Assert::IsTrue(v6.MultiString().size() == 2);

			RegistryValue v7(RegistryValueType::Binary);
			std::vector<BYTE> binary;
			binary.push_back(1);
			binary.push_back(2);
			v7.Binary() = binary;
			Assert::IsTrue(v7.Binary().size() == 2);
		}

		TEST_METHOD(RegistryValue_Execptions)
		{
			RegistryValue v2(RegistryValueType::None);
			std::function<DWORD (void)> f2 = [v2] { return v2.DWord(); };
			Assert::ExpectException<Exceptions::RegistryException>(f2);

			RegistryValue v3(RegistryValueType::None);
			std::function<ULONGLONG (void)> f3 = [v3] { return v3.QWord(); };
			Assert::ExpectException<Exceptions::RegistryException>(f3);

			RegistryValue v4(RegistryValueType::None);
			std::function<std::string (void)> f4 = [v4] { return v4.String(); };
			Assert::ExpectException<Exceptions::RegistryException>(f4);

			RegistryValue v5(RegistryValueType::None);
			std::function<std::string (void)> f5 = [v5] { return v5.ExpandString(); };
			Assert::ExpectException<Exceptions::RegistryException>(f5);

			RegistryValue v6(RegistryValueType::None);
			std::function<std::vector<std::string> (void)> f6 = [v6] { return v6.MultiString(); };
			Assert::ExpectException<Exceptions::RegistryException>(f6);

			RegistryValue v7(RegistryValueType::None);
			std::function<std::vector<BYTE> (void)> f7 = [v7] { return v7.Binary(); };
			Assert::ExpectException<Exceptions::RegistryException>(f7);
		}

	};
}
