#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "imgui/imgui.h"

namespace test {
	class Test
	{
	private:

	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnImGUI() {};
	};

	class TestMenu : public Test {
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnImGUI() override;

		template<typename T>
		void RegisterTest(const std::string& name) {
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	};
}

