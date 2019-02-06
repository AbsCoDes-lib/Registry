#include "stdafx.h"
#include "CppUnitTest.h"

#include <Registry\Registry.h>
#include <Registry\RegistryKey.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace abscodes::registry;

namespace RegistryTests
{		
	TEST_CLASS(RegistryKey_Tests)
	{
	public:

		TEST_METHOD(Default_Keys_from_hives)
		{
			try
			{
				Assert::IsTrue(ClassesRoot().Get() == RegistryKey(RegistryHive::ClassesRoot).Get());
				Assert::IsTrue(CurrentUser().Get() == RegistryKey(RegistryHive::CurrentUser).Get());
				Assert::IsTrue(LocalMachine().Get() == RegistryKey(RegistryHive::LocalMachine).Get());
				Assert::IsTrue(Users().Get() == RegistryKey(RegistryHive::Users).Get());
				Assert::IsTrue(PerformanceData().Get() == RegistryKey(RegistryHive::PerformanceData).Get());
				Assert::IsTrue(CurrentConfig().Get() == RegistryKey(RegistryHive::CurrentConfig).Get());
				Assert::IsTrue(DynData().Get() == RegistryKey(RegistryHive::DynData).Get());
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(CreateSubKey)
		{
			// Create a subkey named TestRegistryKey under HKEY_CURRENT_USER.
			auto TestRegistryKey = CurrentUser().CreateSubKey("TestRegistryKey");
			Assert::IsTrue(TestRegistryKey.GetName() == "TestRegistryKey");

			// Create two subkeys under HKEY_CURRENT_USER\TestRegistryKey. The
			// keys are disposed when execution exits the using statement.
			auto testName = TestRegistryKey.CreateSubKey("TestName");
			auto testSettings = TestRegistryKey.CreateSubKey("TestSettings");
			Assert::IsTrue(testName.GetName() == "TestRegistryKey\\TestName");
			Assert::IsTrue(testSettings.GetName() == "TestRegistryKey\\TestSettings");
		}

		TEST_METHOD(CreateSubKeyCascasde)
		{
			// Create a subkey named TestRegistryKey\TestName under HKEY_CURRENT_USER.
			auto testName = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestName");
			Assert::IsTrue(testName.GetName() == "TestRegistryKey\\TestName");

			// Create a subkey named TestRegistryKey\TestSettings under HKEY_CURRENT_USER.
			auto testSettings = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestSettings");
			Assert::IsTrue(testSettings.GetName() == "TestRegistryKey\\TestSettings");
		}

		TEST_METHOD(CreateDeleteValue)
		{
			auto testSettings = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestSettings");

			// Create data for the TestSettings subkey.
			testSettings.SetStringValue("Language", "French");
			testSettings.SetStringValue("Level", "Intermediate");
			testSettings.SetDwordValue("ID", 123);
			
			Assert::IsTrue(testSettings.GetStringValue("Language") == "French");
			Assert::IsTrue(testSettings.GetStringValue("Level") == "Intermediate");
			Assert::IsTrue(testSettings.GetDwordValue("ID") == 123);
		}

		TEST_METHOD(EnumValue)
		{
			auto testSettings = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestSettings");

			// Create data for the TestSettings subkey.
			testSettings.SetStringValue("Language", "French");
			testSettings.SetStringValue("Level", "Intermediate");
			testSettings.SetDwordValue("ID", 123);

			// Enum values
			auto values = testSettings.EnumValues();
			Assert::IsTrue(values.size() == 3);
			Assert::IsTrue(values[0].first == "Language");
			Assert::IsTrue(values[0].second == RegistryValueType::String);
			Assert::IsTrue(values[1].first == "Level");
			Assert::IsTrue(values[1].second == RegistryValueType::String);
			Assert::IsTrue(values[2].first == "ID");
			Assert::IsTrue(values[2].second == RegistryValueType::DWord);
		}

		TEST_METHOD(OpenSubKey)
		{
			// Open the software subkey under HKEY_CURRENT_USER.
			auto software = CurrentUser().OpenSubKey("software", RegistryAccessRights::Read);
			Assert::IsTrue(software.GetName() == "software");

			// Open the System subkey under HKEY_CURRENT_USER.
			auto system = CurrentUser().OpenSubKey("System", RegistryAccessRights::AllAccess);
			Assert::IsTrue(system.GetName() == "System");

			// Create and open subkey
			auto testNameCreate = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestName");
			auto testNameOpen = CurrentUser().OpenSubKey("TestRegistryKey\\TestName");
			Assert::IsTrue(testNameCreate.GetName() == testNameOpen.GetName());
		}

		TEST_METHOD(DeleteSubKey)
		{
			auto testName = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestName");
			auto testSettings = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestSettings");

			// delete all keys one by one.
			CurrentUser().DeleteSubKey("TestRegistryKey\\TestName");
			CurrentUser().DeleteSubKey("TestRegistryKey\\TestSettings");
			CurrentUser().DeleteSubKey("TestRegistryKey");
		}

		TEST_METHOD(DeleteSubKeyExecption)
		{
			auto testName = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestName");
			auto testSettings = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestSettings");

			// Cannot delete key with subkeys
			try
			{
				CurrentUser().DeleteSubKey("TestRegistryKey");
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(DeleteSubKeyTree)
		{
			auto testName = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestName");
			auto testSettings = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestSettings");

			// Create data for the TestSettings subkey.
			testSettings.SetStringValue("Language", "French");
			testSettings.SetStringValue("Level", "Intermediate");
			testSettings.SetDwordValue("ID", 123);

			// delete all keys one by one.
			CurrentUser().DeleteSubKeyTree("TestRegistryKey");
		}
		
		TEST_METHOD(EnumSubKey)
		{
			auto testName = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestName");
			auto testSettings = CurrentUser().CreateSubKey("TestRegistryKey").CreateSubKey("TestSettings");
			
			std::vector<std::string> keys = CurrentUser().OpenSubKey("TestRegistryKey").EnumSubKeys();
			Assert::IsTrue(keys.size() == 2);
			Assert::IsTrue(keys[0] == "TestName");
			Assert::IsTrue(keys[1] == "TestSettings");
		}
		
		TEST_METHOD(FullTest)
		{
			// Create a subkey named TestRegistryKey under HKEY_CURRENT_USER.
			auto TestRegistryKey = CurrentUser().CreateSubKey("TestRegistryKey");

			// Create two subkeys under HKEY_CURRENT_USER\TestRegistryKey. The
			// keys are disposed when execution exits the using statement.
			auto testName = TestRegistryKey.CreateSubKey("TestName");
			auto testSettings = TestRegistryKey.CreateSubKey("TestSettings");

			// Create data for the TestSettings subkey.
			testSettings.SetStringValue("Language", "French");
			testSettings.SetStringValue("Level", "Intermediate");
			testSettings.SetDwordValue("ID", 123);

			// Check the information from the TestRegistryKey subkey.
			auto keys = TestRegistryKey.EnumSubKeys();
			Assert::IsTrue(keys.size() == 2);
			Assert::IsTrue(keys[0] == "TestName");
			Assert::IsTrue(keys[1] == "TestSettings");

			// Enum values
			auto values = testSettings.EnumValues();
			Assert::IsTrue(values.size() == 3);
			Assert::IsTrue(values[0].first == "Language");
			Assert::IsTrue(values[0].second == RegistryValueType::String);
			Assert::IsTrue(values[1].first == "Level");
			Assert::IsTrue(values[1].second == RegistryValueType::String);
			Assert::IsTrue(values[2].first == "ID");
			Assert::IsTrue(values[2].second == RegistryValueType::DWord);

            // Delete the ID value.
            testSettings.DeleteValue("ID");

			// Enum values
			values = testSettings.EnumValues();
			Assert::IsTrue(values.size() == 2);
			Assert::IsTrue(values[0].first == "Language");
			Assert::IsTrue(values[0].second == RegistryValueType::String);
			Assert::IsTrue(values[1].first == "Level");
			Assert::IsTrue(values[1].second == RegistryValueType::String);

			//
			TestRegistryKey.DeleteSubKey("TestName");

			// Check the information from the TestRegistryKey subkey.
			keys = TestRegistryKey.EnumSubKeys();
			Assert::IsTrue(keys.size() == 1);
			Assert::IsTrue(keys[0] == "TestSettings");

			// delete all keys one by one.
			CurrentUser().DeleteSubKeyTree("TestRegistryKey");
		}
  };
}
