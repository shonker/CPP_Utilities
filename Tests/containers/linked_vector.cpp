#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <sstream> 
#include "../../include/utils/beta/containers/linked_vector/linked_vector.h"

#include "CppUnitTest.h"

struct disney
	{
	inline static size_t count{ 0 };
	int v;
	disney() {}
	disney(int v) : v{ v } { count++; }
	~disney() { count--; }

	static void reset() { count = 0; }
	};

namespace Microsoft
	{
	namespace VisualStudio
		{
		namespace CppUnitTestFramework
			{
			template<> static std::wstring ToString<utils::beta::containers::linked_vector<disney>::iterator> (const class utils::beta::containers::linked_vector<disney>::iterator& t)
				{
				std::wstringstream s;
				s << &(*t);
				return s.str();
				}
			template<> static std::wstring ToString<utils::beta::containers::linked_vector<disney, 2>::iterator>(const class utils::beta::containers::linked_vector<disney, 2>::iterator& t)
				{
				std::wstringstream s;
				s << &(*t);
				return s.str();
				}
			template<> static std::wstring ToString<utils::beta::containers::linked_vector<disney, 5>::iterator>(const class utils::beta::containers::linked_vector<disney, 5>::iterator& t)
				{
				std::wstringstream s;
				s << &(*t);
				return s.str();
				}
			}
		}
	}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Tests
	{
	TEST_CLASS(linked_vector)
		{
		public:

			TEST_METHOD(emplace_single_segment)
				{
				disney::reset();
				utils::beta::containers::linked_vector<disney> a;

				auto obj_0{ a.emplace(0) };
				Assert::AreEqual(size_t{ 1 }, disney::count);
				auto obj_1{ a.emplace(1) };
				Assert::AreEqual(size_t{ 2 }, disney::count);
				auto obj_2{ a.emplace(2) };
				Assert::AreEqual(size_t{ 3 }, disney::count);

				Assert::AreEqual(0, obj_0.v);
				Assert::AreEqual(1, obj_1.v);
				Assert::AreEqual(2, obj_2.v);
				}

			TEST_METHOD(emplace)
				{
				disney::reset();
				utils::beta::containers::linked_vector<disney, 2> a;

				auto obj_0{ a.emplace(0) };
				Assert::AreEqual(size_t{ 1 }, disney::count);
				auto obj_1{ a.emplace(1) };
				Assert::AreEqual(size_t{ 2 }, disney::count);
				auto obj_2{ a.emplace(2) };
				Assert::AreEqual(size_t{ 3 }, disney::count);

				Assert::AreEqual(0, obj_0.v);
				Assert::AreEqual(1, obj_1.v);
				Assert::AreEqual(2, obj_2.v);
				}
				
			TEST_METHOD(destructor_single_segment)
				{
				disney::reset();
				if (true)
					{
					utils::beta::containers::linked_vector<disney> a;
				
					auto& obj_0{ a.emplace(0) };
					auto& obj_1{ a.emplace(1) };
					auto& obj_2{ a.emplace(2) };
					Assert::AreEqual(size_t{ 3 }, disney::count);
					}
				Assert::AreEqual(size_t{ 0 }, disney::count);
				}

			TEST_METHOD(destructor)
				{
				disney::reset();
				if (true)
					{
					utils::beta::containers::linked_vector<disney, 2> a;

					auto& obj_0{ a.emplace(0) };
					auto& obj_1{ a.emplace(1) };
					auto& obj_2{ a.emplace(2) };
					Assert::AreEqual(size_t{ 3 }, disney::count);
					}
				Assert::AreEqual(size_t{ 0 }, disney::count);
				}

			TEST_METHOD(iterator_single_segment)
				{
				disney::reset();

				utils::beta::containers::linked_vector<disney> a;

				auto& obj_0{ a.emplace(0) };
				auto& obj_1{ a.emplace(1) };
				auto& obj_2{ a.emplace(2) };

				auto it{ a.begin() };
				Assert::AreEqual(0, it->v);
				it++;
				Assert::AreEqual(1, it->v);
				it++;
				Assert::AreEqual(2, it->v);
				it--;
				Assert::AreEqual(1, it->v);
				it--;
				Assert::AreEqual(0, it->v);

				it += 3;
				Assert::AreEqual(a.end(), it);

				it -= 3;
				Assert::AreEqual(a.begin(), it);
				}

			TEST_METHOD(iterator)
				{
				disney::reset();
				
				utils::beta::containers::linked_vector<disney, 2> a;
				
				auto& obj_0{ a.emplace(0) };
				auto& obj_1{ a.emplace(1) };
				auto& obj_2{ a.emplace(2) };
				
				auto it{ a.begin() };
				Assert::AreEqual(0, it->v);
				it++;
				Assert::AreEqual(1, it->v);
				it++;
				Assert::AreEqual(2, it->v);
				it--;
				Assert::AreEqual(1, it->v);
				it--;
				Assert::AreEqual(0, it->v);
				
				it += 3;
				Assert::AreEqual(a.end(), it);
				
				it -= 3;
				Assert::AreEqual(a.begin(), it);
				}

			TEST_METHOD(iterator_extra)
				{
				disney::reset();

				utils::beta::containers::linked_vector<disney, 5> a;

				auto& obj_0{ a.emplace(0) };
				auto& obj_1{ a.emplace(1) };
				auto& obj_2{ a.emplace(2) };
				auto& obj_3{ a.emplace(3) };
				auto& obj_4{ a.emplace(4) };
				auto& obj_5{ a.emplace(5) };
				auto& obj_6{ a.emplace(6) };
				auto& obj_7{ a.emplace(7) };

				auto it{ a.begin() };
				Assert::AreEqual(0, it->v);
				it++;
				Assert::AreEqual(1, it->v);
				it+=4;
				Assert::AreEqual(5, it->v);
				it--;
				Assert::AreEqual(4, it->v);
				it-=2;
				Assert::AreEqual(2, it->v);

				it += 6;
				Assert::AreEqual(a.end(), it);

				it -= 8;
				Assert::AreEqual(a.begin(), it);
				}
		};
	}