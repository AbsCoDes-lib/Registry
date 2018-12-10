#include "stdafx.h"
#include "CppUnitTest.h"

#include <Registry\Registry.h>
#include <Registry\RegistryKey.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace abscodes::registry;

namespace Registry
{		
	TEST_CLASS(Registry_Tests)
	{
	public:

		TEST_METHOD(GetKey_Test)
		{
			auto software = GetKey(RegistryHive::CurrentUser, "Software");
			auto microsotf = GetKey(software, "Microsoft");

			try
			{
				auto test = GetKey(RegistryHive::CurrentUser, "Test");
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(GetSet_Test)
		{
			// Create a subkey named TestRegistryKey under HKEY_CURRENT_USER.
			auto TestRegistryKey = CurrentUser().CreateSubKey("TestRegistryKey");

			// Values
			RegistryValue dw1(RegistryValueType::DWord); dw1.DWord() = 1;
			RegistryValue qw1(RegistryValueType::QWord); qw1.QWord() = 2147483648;
	
			// Test SetValue and GetValue
			abscodes::registry::SetValue(TestRegistryKey, "DValue", dw1);
			Assert::IsTrue(abscodes::registry::GetValue(TestRegistryKey, "DValue").DWord() == dw1.DWord());
			abscodes::registry::SetValue(TestRegistryKey, "QValue", qw1);
			Assert::IsTrue(abscodes::registry::GetValue(TestRegistryKey, "QValue").QWord() == qw1.QWord());
			
			// Test SetInt and GetInt
			abscodes::registry::SetInt(TestRegistryKey, "iValue", -10);
			Assert::IsTrue(abscodes::registry::GetInt(TestRegistryKey, "iValue", 0) == -10);
			Assert::IsTrue(abscodes::registry::GetString(TestRegistryKey, "iValue", "0") == "-10");
		}
  };
}
