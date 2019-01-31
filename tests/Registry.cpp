#include "stdafx.h"
#include "CppUnitTest.h"

#include <Registry\Registry.h>
#include <Registry\RegistryKey.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace abscodes::registry;

namespace RegistryTests
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

		TEST_METHOD(Enum_Test)
		{
			// Enum all sub keys in HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall
			auto TestRegistryKey = LocalMachine().OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
			std::vector<std::string> keys = TestRegistryKey.EnumSubKeys();


			auto TestRegistryKey32 = LocalMachine().OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
				, RegistryView::Registry32
				, RegistryAccessRights::Read
				, RegistryOption::None);
			std::vector<std::string> keys32 = TestRegistryKey32.EnumSubKeys();


			auto TestRegistryKey64 = LocalMachine().OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
				, RegistryView::Registry64
				, RegistryAccessRights::Read
				, RegistryOption::None);
			std::vector<std::string> keys64 = TestRegistryKey64.EnumSubKeys();

		}
  };
}
