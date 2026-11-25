#include "test.hpp"

void CheckSize(const Stack* stack, size_t expected_size) {
  ASSERT_EQ(expected_size, Size(stack));  // <-- also got CE here? heheh
}

void CheckTop(const Stack* stack, const char* expected_top) {
  ASSERT_STREQ(expected_top, Top(stack));
}

TEST(Constness, Test) {
  Stack stack;
  const char* string = "abc";

  Push(&stack, string);
  CheckSize(&stack, 1);
  CheckTop(&stack, string);

  ASSERT_STREQ(string, Top(&stack));
  Pop(&stack);

  CheckSize(&stack, 0);
  ASSERT_EQ(nullptr, Top(&stack));
}

TEST_F(StackTest, SimpleTop) {
  const char* string = "abc";

  Push(string);
  ASSERT_STREQ(string, Top());
  ASSERT_STREQ(string, Top());
  Pop();
}

TEST_F(StackTest, DoublePush) {
  const char* string_1 = "a";
  const char* string_2 = "b";

  Push(string_1);
  Push(string_2);
  ASSERT_EQ(2, Size());
  ASSERT_STREQ(string_2, Top());
  Pop();
  ASSERT_STREQ(string_1, Top());
  Pop();
}

TEST_F(StackTest, EmptyPop) {
  ASSERT_EQ(nullptr, Top());
  Pop();
}

TEST_F(StackTest, PeekAfterSingleInsert) {
  const char* number_1 = "1";

  Push(number_1);
  ASSERT_STREQ(number_1, Top());
}

TEST_F(StackTest, SizeTracksGrowth) {
  const char* number_1 = "1";
  const char* number_2 = "2";
  const char* number_3 = "3";

  ASSERT_EQ(0, Size());
  Push(number_1);
  ASSERT_EQ(1, Size());
  Push(number_2);
  ASSERT_EQ(2, Size());
  Push(number_3);
  ASSERT_EQ(3, Size());
}

TEST_F(StackTest, InterleavedOpsWithClear) {
  const char* number_3 = "3";
  const char* number_14 = "14";
  const char* number_1 = "1";
  const char* number_2 = "2";

  Push(number_3);
  Push(number_14);
  ASSERT_EQ(2, Size());
  Clear();
  Push(number_1);
  ASSERT_STREQ(number_1, Top());
  Push(number_2);
  ASSERT_STREQ(number_2, Top());
  ASSERT_STREQ(number_2, Top());
  Pop();
  ASSERT_EQ(1, Size());
  ASSERT_STREQ(number_1, Top());
  Pop();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, PopLIFO) {
  const char* number_10 = "10";
  const char* number_20 = "20";

  Push(number_10);
  Push(number_20);
  ASSERT_STREQ(number_20, Top());
  Pop();
  ASSERT_STREQ(number_10, Top());
  Pop();
}

TEST_F(StackTest, AllOperationsOneObject) {
  const char* number_12345 = "12345";

  Push(number_12345);
  ASSERT_STREQ(number_12345, Top());
  ASSERT_STREQ(number_12345, Top());
  Pop();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, PeekShiftsAfterRemoval) {
  const char* letter_a = "a";
  const char* letter_b = "b";
  const char* letter_c = "c";

  Push(letter_a);
  Push(letter_b);
  Push(letter_c);
  ASSERT_STREQ(letter_c, Top());
  Pop();
  ASSERT_STREQ(letter_b, Top());
}

TEST_F(StackTest, ClearResetsCount) {
  const char* letter_x = "x";
  const char* letter_y = "y";

  Push(letter_x);
  Push(letter_y);
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, RemovesInReverse) {
  const char* alpha = "alpha";
  const char* beta = "beta";
  const char* gamma = "gamma";

  Push(alpha);
  Push(beta);
  Push(gamma);
  ASSERT_STREQ(gamma, Top());
  Pop();
  ASSERT_STREQ(beta, Top());
  Pop();
  ASSERT_STREQ(alpha, Top());
  Pop();
}

TEST_F(StackTest, SizeStaysZeroOnEmpty) {
  ASSERT_EQ(0, Size());
  ASSERT_EQ(0, Size());
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, PeekThenClear) {
  const char* first = "first";
  const char* second = "second";

  Push(first);
  Push(second);
  ASSERT_STREQ(second, Top());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, ExtraPop) {
  const char* number_100 = "100";

  Push(number_100);
  ASSERT_STREQ(number_100, Top());
  Pop();
  ASSERT_EQ(nullptr, Top());
  Pop();
}

TEST_F(StackTest, Duplicats) {
  const char* test = "test";

  Push(test);
  Push(test);
  ASSERT_STREQ(test, Top());
  Pop();
  ASSERT_STREQ(test, Top());
  Pop();
  ASSERT_EQ(nullptr, Top());
  Pop();
}

TEST_F(StackTest, CountOneTwoMany) {
  const char* letter_a = "a";
  const char* letter_b = "b";
  const char* letter_c = "c";
  const char* letter_d = "d";
  const char* letter_e = "e";
  const char* letter_f = "f";

  Push(letter_a);
  ASSERT_EQ(1, Size());
  Push(letter_b);
  ASSERT_EQ(2, Size());
  Push(letter_c);
  Push(letter_d);
  Push(letter_e);
  Push(letter_f);
  ASSERT_EQ(6, Size());
}

TEST_F(StackTest, TopTopTop) {
  const char* one = "one";
  const char* two_upper = "TWO";
  const char* three = "three";

  Push(one);
  ASSERT_STREQ(one, Top());
  Push(two_upper);
  ASSERT_STREQ(two_upper, Top());
  Push(three);
  ASSERT_STREQ(three, Top());
}

TEST_F(StackTest, ClearOnEmptyThenUse) {
  const char* number_1 = "1";

  Clear();
  Push(number_1);
  ASSERT_STREQ(number_1, Top());
  Pop();
}

TEST_F(StackTest, NotReallyLongTokens) {
  const char* longstring = "longstring";
  const char* anotherlongstring = "anotherlongstring";

  Push(longstring);
  Push(anotherlongstring);
  ASSERT_EQ(2, Size());
  ASSERT_STREQ(anotherlongstring, Top());
  Pop();
  ASSERT_STREQ(longstring, Top());
}

TEST_F(StackTest, RemnantCountAfterPops) {
  const char* number_42 = "42";
  const char* number_43 = "43";
  const char* number_44 = "44";

  Push(number_42);
  Push(number_43);
  Push(number_44);
  ASSERT_STREQ(number_44, Top());
  Pop();
  ASSERT_STREQ(number_43, Top());
  Pop();
  ASSERT_EQ(1, Size());
}

TEST_F(StackTest, UpDownSizes) {
  ASSERT_EQ(0, Size());
  const char* number_999 = "999";

  Push(number_999);
  ASSERT_EQ(1, Size());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, PopOrTop_thatisthequestion) {
  const char* apple = "apple";
  const char* banana = "banana";
  const char* cherry = "cherry";

  Push(apple);
  Push(banana);
  Push(cherry);
  ASSERT_STREQ(cherry, Top());
  Pop();
  ASSERT_STREQ(banana, Top());
}

TEST_F(StackTest, ClearPush) {
  const char* number_1 = "1";
  const char* number_2 = "2";
  const char* number_3 = "3";

  Push(number_1);
  Push(number_2);
  Clear();
  Push(number_3);
  ASSERT_STREQ(number_3, Top());
}

TEST_F(StackTest, Exhaust) {
  const char* abc = "abc";
  const char* def = "def";
  const char* ghi = "ghi";

  Push(abc);
  Push(def);
  Push(ghi);
  ASSERT_STREQ(ghi, Top());
  Pop();
  ASSERT_STREQ(def, Top());
  Pop();
  ASSERT_STREQ(abc, Top());
  Pop();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, PeekClear) {
  const char* top = "top";
  const char* new_value = "new";

  Push(top);
  ASSERT_STREQ(top, Top());
  Clear();
  Push(new_value);
  ASSERT_STREQ(new_value, Top());
}

TEST_F(StackTest, SizeAroundRemoval) {
  const char* stack_value = "stack";

  Push(stack_value);
  ASSERT_EQ(1, Size());
  ASSERT_STREQ(stack_value, Top());
  Pop();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, SizePop) {
  const char* hello = "hello";
  const char* world = "world";
  const char* abcd_upper = "ABCD";

  Push(hello);
  Push(world);
  Push(abcd_upper);
  ASSERT_EQ(3, Size());
  ASSERT_STREQ(abcd_upper, Top());
  Pop();
  ASSERT_STREQ(world, Top());
  Pop();
  ASSERT_STREQ(hello, Top());
  Pop();
}

TEST_F(StackTest, PeekClear_2) {
  const char* number_123 = "123";
  const char* number_456 = "456";

  Push(number_123);
  Push(number_456);
  ASSERT_STREQ(number_456, Top());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, ClearInMid) {
  const char* element = "element";
  const char* another = "another";
  const char* final = "final";

  Push(element);
  Push(another);
  Clear();
  ASSERT_EQ(0, Size());
  Push(final);
  ASSERT_STREQ(final, Top());
}

TEST_F(StackTest, PopOrTop_thatisthequestion_2) {
  const char* number_1 = "1";
  const char* number_2 = "2";
  const char* number_3 = "3";
  const char* number_4 = "4";

  Push(number_1);
  Push(number_2);
  Push(number_3);
  Push(number_4);
  ASSERT_STREQ(number_4, Top());
  Pop();
  ASSERT_STREQ(number_3, Top());
  Pop();
  ASSERT_STREQ(number_2, Top());
}

TEST_F(StackTest, MidSizedToken_Put) {
  const char* letter = "letter";
  std::string mid(20000, 'p');
  const char* another = "another";

  Push(letter);
  Push(mid.c_str());
  Push(another);
  ASSERT_EQ(3, Size());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, HugeToken_Put) {
  const char* initial = "initial";
  std::string huge(1500000, 'p');
  const char* final = "final";

  Push(initial);
  Push(huge.c_str());
  Push(final);
  ASSERT_STREQ(final, Top());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, MidSizedToken_Peek) {
  const char* letter = "letter";
  std::string mid(2000, 'p');

  Push(letter);
  Push(mid.c_str());
  ASSERT_STREQ(mid.c_str(), Top());
  ASSERT_EQ(2, Size());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, HugeToken_Peek) {
  const char* initial = "initial";
  std::string huge(1500000, 'p');
  const char* final = "final";

  Push(initial);
  Push(huge.c_str());
  Push(final);
  ASSERT_STREQ(final, Top());
  Pop();
  ASSERT_STREQ(huge.c_str(), Top());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, Hang) {
  const char* initial = "initial";

  Push(initial);
  {
    std::string hanging = "abc";
    Push(hanging.c_str());
  }
  const char* expected = "abc";

  ASSERT_STREQ(expected, Top());
  Clear();
  ASSERT_EQ(0, Size());
}

TEST_F(StackTest, Changed) {
  const char* initial = "initial";

  Push(initial);
  std::string to_be_changed = "huhuhuhhuuuuhuhhuhuhuhuu";
  std::string old_copy = to_be_changed;
  Push(to_be_changed.c_str());
  to_be_changed[10] = 'i';

  ASSERT_STREQ(old_copy.c_str(), Top());
  Clear();
  ASSERT_EQ(0, Size());
}
