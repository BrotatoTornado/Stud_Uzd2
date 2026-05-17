/**
 * @file vector_tests.cpp
 * @brief Google Test testai Vector<T> klasei.
 *
 * Paleidimas:
 *   cmake -B build -DBUILD_TESTS=ON
 *   cmake --build build
 *   cd build && ctest --output-on-failure
 *
 * Arba tiesiogiai:
 *   ./vector_tests
 */

#include "Vector.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

//Patikrina, kad Vector ir std::vector turi tuos pačius elementus
#define EXPECT_VEC_EQ(myVec, stdVec)                                  \
    do {                                                               \
        ASSERT_EQ((myVec).size(), (stdVec).size());                    \
        for (std::size_t _i = 0; _i < (myVec).size(); ++_i)          \
            EXPECT_EQ((myVec)[_i], (stdVec)[_i])                      \
                << "  index=" << _i;                                   \
    } while (false)

//1. Konstruktoriai

TEST(VectorConstructors, Default)
{
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorConstructors, CountValue)
{
    Vector<int> v(5, 42);
    std::vector<int> ref(5, 42);
    EXPECT_VEC_EQ(v, ref);
    EXPECT_EQ(v.size(), 5u);
}

TEST(VectorConstructors, CountDefault)
{
    Vector<int> v(4);
    EXPECT_EQ(v.size(), 4u);
    for (std::size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], 0);
}

TEST(VectorConstructors, RangeIterator)
{
    std::vector<int> src = {1, 2, 3, 4, 5};
    Vector<int> v(src.begin(), src.end());
    EXPECT_VEC_EQ(v, src);
}

TEST(VectorConstructors, InitializerList)
{
    Vector<int> v = {10, 20, 30, 40};
    std::vector<int> ref = {10, 20, 30, 40};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorConstructors, CopyConstructor)
{
    Vector<int> orig = {1, 2, 3};
    Vector<int> copy(orig);
    EXPECT_EQ(copy.size(), orig.size());
    for (std::size_t i = 0; i < orig.size(); ++i)
        EXPECT_EQ(copy[i], orig[i]);
    // Nepriklausomos kopijos
    copy.push_back(99);
    EXPECT_EQ(orig.size(), 3u);
}

TEST(VectorConstructors, MoveConstructor)
{
    Vector<int> orig = {7, 8, 9};
    const auto origSize = orig.size();
    Vector<int> moved(std::move(orig));
    EXPECT_EQ(moved.size(), origSize);
    EXPECT_EQ(moved[0], 7);
    EXPECT_TRUE(orig.empty());
    EXPECT_EQ(orig.data(), nullptr);
}

//2. Priskyrimo operatoriai

TEST(VectorAssignment, CopyAssignment)
{
    Vector<int> a = {1, 2, 3};
    Vector<int> b;
    b = a;
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[1], 2);
    // Nepriklausomos
    a.push_back(4);
    EXPECT_EQ(b.size(), 3u);
}

TEST(VectorAssignment, MoveAssignment)
{
    Vector<int> a = {5, 6, 7};
    Vector<int> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 5);
    EXPECT_TRUE(a.empty());
}

TEST(VectorAssignment, InitializerListAssignment)
{
    Vector<int> v = {1, 2, 3};
    v = {100, 200};
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[1], 200);
}

TEST(VectorAssignment, SelfAssignment)
{
    Vector<int> v = {1, 2, 3};
    v = v;
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
}

//3. assign()

TEST(VectorAssign, AssignCountValue)
{
    Vector<int> v = {9, 9, 9, 9};
    v.assign(3, 7);
    std::vector<int> ref(3, 7);
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorAssign, AssignRange)
{
    std::vector<int> src = {10, 20, 30};
    Vector<int> v = {1, 2};
    v.assign(src.begin(), src.end());
    EXPECT_VEC_EQ(v, src);
}

TEST(VectorAssign, AssignInitList)
{
    Vector<int> v;
    v.assign({5, 6, 7, 8});
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[3], 8);
}

//4. Elementų prieiga

TEST(VectorAccess, OperatorBracket)
{
    Vector<int> v = {10, 20, 30};
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[2], 30);
    v[1] = 99;
    EXPECT_EQ(v[1], 99);
}

TEST(VectorAccess, At_Valid)
{
    Vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(2), 3);
}

TEST(VectorAccess, At_OutOfRange)
{
    Vector<int> v = {1, 2, 3};
    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorAccess, FrontBack)
{
    Vector<int> v = {10, 20, 30};
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
    v.front() = 1;
    v.back()  = 99;
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 99);
}

TEST(VectorAccess, Data)
{
    Vector<int> v = {3, 1, 4};
    int* p = v.data();
    EXPECT_EQ(p[0], 3);
    p[1] = 99;
    EXPECT_EQ(v[1], 99);
}

//5. Iteratoriai

TEST(VectorIterators, BeginEnd)
{
    Vector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;
    EXPECT_EQ(sum, 15);
}

TEST(VectorIterators, ConstBeginEnd)
{
    const Vector<int> v = {2, 4, 6};
    int sum = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it)
        sum += *it;
    EXPECT_EQ(sum, 12);
}

TEST(VectorIterators, ReverseIterator)
{
    Vector<int> v = {1, 2, 3};
    std::vector<int> rev(v.rbegin(), v.rend());
    std::vector<int> expected = {3, 2, 1};
    EXPECT_EQ(rev, expected);
}

TEST(VectorIterators, RangeFor)
{
    Vector<int> v = {10, 20, 30};
    int sum = 0;
    for (const auto& x : v)
        sum += x;
    EXPECT_EQ(sum, 60);
}

TEST(VectorIterators, StdAlgorithms)
{
    Vector<int> v = {5, 3, 1, 4, 2};
    std::sort(v.begin(), v.end());
    for (std::size_t i = 0; i + 1 < v.size(); ++i)
        EXPECT_LE(v[i], v[i + 1]);
    EXPECT_EQ(*std::min_element(v.begin(), v.end()), 1);
    EXPECT_EQ(*std::max_element(v.begin(), v.end()), 5);
}

//6. Talpa

TEST(VectorCapacity, EmptySizeCapacity)
{
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
    v.push_back(1);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v.size(), 1u);
}

TEST(VectorCapacity, Reserve)
{
    Vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100u);
    EXPECT_EQ(v.size(), 0u);
    // Mažesnė rezervacija nieko nedaro
    v.reserve(10);
    EXPECT_GE(v.capacity(), 100u);
}

TEST(VectorCapacity, ShrinkToFit)
{
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 5; ++i)
        v.push_back(i);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
    EXPECT_EQ(v.size(), 5u);
}

TEST(VectorCapacity, GrowthOnPushBack)
{
    Vector<int> v;
    std::size_t prevCap = v.capacity();
    int reallocations = 0;
    for (int i = 0; i < 1000; ++i)
    {
        v.push_back(i);
        if (v.capacity() != prevCap)
        {
            prevCap = v.capacity();
            ++reallocations;
        }
    }
    EXPECT_LE(reallocations, 20);
    EXPECT_EQ(v.size(), 1000u);
}

//7. Modifikatoriai – push_back / pop_back / clear

TEST(VectorModifiers, PushBack_Copy)
{
    Vector<std::string> v;
    std::string s = "hello";
    v.push_back(s);
    EXPECT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(s, "hello");  // s nepakeistas
}

TEST(VectorModifiers, PushBack_Move)
{
    Vector<std::string> v;
    std::string s = "world";
    v.push_back(std::move(s));
    EXPECT_EQ(v[0], "world");
    EXPECT_TRUE(s.empty());  // perkeltas
}

TEST(VectorModifiers, EmplaceBack)
{
    Vector<std::pair<int,int>> v;
    v.emplace_back(3, 4);
    EXPECT_EQ(v[0].first,  3);
    EXPECT_EQ(v[0].second, 4);
}

TEST(VectorModifiers, PopBack)
{
    Vector<int> v = {1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v.back(), 2);
}

TEST(VectorModifiers, Clear)
{
    Vector<int> v = {1, 2, 3};
    const auto cap = v.capacity();
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), cap);  // talpa išlieka
}

//8. insert()

TEST(VectorInsert, InsertAtBegin)
{
    Vector<int> v = {2, 3, 4};
    v.insert(v.begin(), 1);
    std::vector<int> ref = {1, 2, 3, 4};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorInsert, InsertAtEnd)
{
    Vector<int> v = {1, 2, 3};
    v.insert(v.end(), 4);
    EXPECT_EQ(v.back(), 4);
    EXPECT_EQ(v.size(), 4u);
}

TEST(VectorInsert, InsertAtMiddle)
{
    Vector<int> v = {1, 3, 4};
    v.insert(v.begin() + 1, 2);
    std::vector<int> ref = {1, 2, 3, 4};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorInsert, InsertCountValue)
{
    Vector<int> v = {1, 5};
    v.insert(v.begin() + 1, 3u, 0);
    std::vector<int> ref = {1, 0, 0, 0, 5};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorInsert, InsertRange)
{
    Vector<int> v = {1, 5};
    std::vector<int> extra = {2, 3, 4};
    v.insert(v.begin() + 1, extra.begin(), extra.end());
    std::vector<int> ref = {1, 2, 3, 4, 5};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorInsert, InsertInitializerList)
{
    Vector<int> v = {1, 5};
    v.insert(v.begin() + 1, {2, 3, 4});
    std::vector<int> ref = {1, 2, 3, 4, 5};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorInsert, InsertReturnIterator)
{
    Vector<int> v = {1, 2, 4};
    auto it = v.insert(v.begin() + 2, 3);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(v[2], 3);
}

//9. emplace()=

TEST(VectorEmplace, EmplaceMiddle)
{
    Vector<std::pair<int,int>> v = {{1,1}, {3,3}};
    auto it = v.emplace(v.begin() + 1, 2, 2);
    EXPECT_EQ(it->first,  2);
    EXPECT_EQ(it->second, 2);
    EXPECT_EQ(v.size(), 3u);
}

//10. erase()

TEST(VectorErase, EraseSingle)
{
    Vector<int> v = {1, 2, 3, 4};
    auto it = v.erase(v.begin() + 1);
    std::vector<int> ref = {1, 3, 4};
    EXPECT_VEC_EQ(v, ref);
    EXPECT_EQ(*it, 3);
}

TEST(VectorErase, EraseFirst)
{
    Vector<int> v = {10, 20, 30};
    v.erase(v.begin());
    EXPECT_EQ(v.front(), 20);
    EXPECT_EQ(v.size(), 2u);
}

TEST(VectorErase, EraseLast)
{
    Vector<int> v = {10, 20, 30};
    v.erase(v.end() - 1);
    EXPECT_EQ(v.back(), 20);
    EXPECT_EQ(v.size(), 2u);
}

TEST(VectorErase, EraseRange)
{
    Vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 1, v.begin() + 4);
    std::vector<int> ref = {1, 5};
    EXPECT_VEC_EQ(v, ref);
    EXPECT_EQ(*it, 5);
}

TEST(VectorErase, EraseAll)
{
    Vector<int> v = {1, 2, 3};
    v.erase(v.begin(), v.end());
    EXPECT_TRUE(v.empty());
}

TEST(VectorErase, EraseEmpty)
{
    Vector<int> v = {1, 2, 3};
    auto it = v.erase(v.begin() + 1, v.begin() + 1);  // tuščias diapazonas
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(v.size(), 3u);
}

//11. resize()

TEST(VectorResize, ResizeShrink)
{
    Vector<int> v = {1, 2, 3, 4, 5};
    v.resize(3);
    std::vector<int> ref = {1, 2, 3};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorResize, ResizeGrowDefault)
{
    Vector<int> v = {1, 2};
    v.resize(5);
    EXPECT_EQ(v.size(), 5u);
    for (std::size_t i = 2; i < v.size(); ++i)
        EXPECT_EQ(v[i], 0);
}

TEST(VectorResize, ResizeGrowValue)
{
    Vector<int> v = {1, 2};
    v.resize(5, 99);
    EXPECT_EQ(v.size(), 5u);
    for (std::size_t i = 2; i < v.size(); ++i)
        EXPECT_EQ(v[i], 99);
}

TEST(VectorResize, ResizeSame)
{
    Vector<int> v = {1, 2, 3};
    v.resize(3);
    EXPECT_EQ(v.size(), 3u);
}

//12. swap()

TEST(VectorSwap, MemberSwap)
{
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {10, 20};
    a.swap(b);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(b[0], 1);
}

TEST(VectorSwap, NonMemberSwap)
{
    Vector<int> a = {1, 2};
    Vector<int> b = {9, 8, 7};
    swap(a, b);
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(b.size(), 2u);
}

//13. Palyginimo operatoriai

TEST(VectorComparison, EqualityOperator)
{
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {1, 2, 3};
    Vector<int> c = {1, 2, 4};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(VectorComparison, LexicographicOrder)
{
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {1, 2, 4};
    Vector<int> c = {1, 2};
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(c < a);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(b >= a);
}

//14. Non-member erase / erase_if

TEST(VectorNonMember, Erase)
{
    Vector<int> v = {1, 2, 3, 2, 4, 2};
    const auto n = erase(v, 2);
    EXPECT_EQ(n, 3u);
    std::vector<int> ref = {1, 3, 4};
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorNonMember, EraseIf)
{
    Vector<int> v = {1, 2, 3, 4, 5, 6};
    const auto n = erase_if(v, [](int x){ return x % 2 == 0; });
    EXPECT_EQ(n, 3u);
    std::vector<int> ref = {1, 3, 5};
    EXPECT_VEC_EQ(v, ref);
}

//15. Didelio kiekio push_back

TEST(VectorStress, ManyPushBack)
{
    Vector<int> v;
    std::vector<int> ref;
    for (int i = 0; i < 100000; ++i)
    {
        v.push_back(i);
        ref.push_back(i);
    }
    EXPECT_VEC_EQ(v, ref);
}

TEST(VectorStress, ReserveAndFill)
{
    Vector<int> v;
    v.reserve(50000);
    for (int i = 0; i < 50000; ++i)
        v.push_back(i);
    EXPECT_EQ(v.size(), 50000u);
    EXPECT_GE(v.capacity(), 50000u);
}

//16. String elementai

TEST(VectorString, PushAndAccess)
{
    Vector<std::string> v;
    v.push_back("pirmas");
    v.push_back("antras");
    v.emplace_back("trecias");
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], "antras");
}

TEST(VectorString, CopyAndModify)
{
    Vector<std::string> a = {"x", "y", "z"};
    Vector<std::string> b = a;
    b[0] = "CHANGED";
    EXPECT_EQ(a[0], "x");  // a nepakeistas
    EXPECT_EQ(b[0], "CHANGED");
}

//17. Perskirstymų skaičius (atitikimas std::vector elgesiui)

TEST(VectorReallocations, CountVsStdVector)
{
    const int N = 100000;

    // std::vector perskirstymai
    {
        std::vector<int> sv;
        int stdReallocations = 0;
        std::size_t prevCap = sv.capacity();
        for (int i = 1; i <= N; ++i)
        {
            sv.push_back(i);
            if (sv.capacity() != prevCap)
            {
                prevCap = sv.capacity();
                ++stdReallocations;
            }
        }

        //Mano Vector perskirstymai
        Vector<int> mv;
        int myReallocations = 0;
        std::size_t prevCapMy = mv.capacity();
        for (int i = 1; i <= N; ++i)
        {
            mv.push_back(i);
            if (mv.capacity() != prevCapMy)
            {
                prevCapMy = mv.capacity();
                ++myReallocations;
            }
        }

        //Abiejų perskirstymų skaičiai turi būti panašūs
        EXPECT_NEAR(myReallocations, stdReallocations, 5) << "std::vector: " << stdReallocations << "  Vector: " << myReallocations;
    }
}

//18. Vector su nestandartiniu tipu (move-only)

TEST(VectorMoveOnly, UniquePtr)
{
    Vector<std::unique_ptr<int>> v;
    v.push_back(std::make_unique<int>(1));
    v.push_back(std::make_unique<int>(2));
    v.emplace_back(std::make_unique<int>(3));
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(*v[0], 1);
    EXPECT_EQ(*v[2], 3);
}

//19. std::accumulate su Vector

TEST(VectorAlgorithm, Accumulate)
{
    Vector<int> v = {1, 2, 3, 4, 5};
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    EXPECT_EQ(sum, 15);
}

TEST(VectorAlgorithm, Find)
{
    Vector<int> v = {10, 20, 30, 40};
    auto it = std::find(v.begin(), v.end(), 30);
    ASSERT_NE(it, v.end());
    EXPECT_EQ(*it, 30);
}

TEST(VectorAlgorithm, Transform)
{
    Vector<int> v = {1, 2, 3, 4};
    Vector<int> result(v.size());
    std::transform(v.begin(), v.end(), result.begin(),
                   [](int x){ return x * x; });
    std::vector<int> expected = {1, 4, 9, 16};
    EXPECT_VEC_EQ(result, expected);
}

//20. get_allocator

TEST(VectorAllocator, GetAllocator)
{
    Vector<int> v;
    auto alloc = v.get_allocator();
    // Tiesiog patikriname, kad grąžina teisingą tipą
    static_assert(
        std::is_same_v<decltype(alloc), std::allocator<int>>,
        "get_allocator turi grazinti std::allocator<int>");
    (void)alloc;
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
