//
// Created by sandy on 31/08/18.
//

#include "../src/private/Scripting/FunctionTables.h"

using namespace sce::scripting::private_;

void TestFunction00() {}
int TestFunction01() {
  return 1;
}
void TestFunction02(int) {}
int TestFunction03(int i) {
  return i;
}
void TestFunction04(const char *) {}
const char * TestFunction05() {
  return "Test";
}
void TestFunction06(const std::string &) {}
const std::string TestFunction07() {
  return "Test";
}
void TestFunction08(const std::pair<int, int> &) {}  // TODO implement and test
const std::pair<int, int> TestFunction09() {
  return std::make_pair<int, int>(0, 1);
}
void TestFunction10(const std::vector<int> &) {}  // TODO implement and test
const std::vector<int> TestFunction11() {
  return {
      0, 1, 2, 3, 4, 5, 6,
  };
}
void TestFunction12(const std::array<int, 7> &) {}  // TODO implement and test
const std::array<int, 7> TestFunction13() {
  return {
      0, 1, 2, 3, 4, 5, 6,
  };
}
void TestFunction14(const std::unordered_map<std::string, int> &) {
}  // TODO implement and test
const std::unordered_map<std::string, int> TestFunction15() {
  return {
      {"0", 1},
      {"2", 3},
      {"4", 5},
  };
}
const void TestFunction16(int, std::vector<int>, int) {}

auto r00 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction00);
auto r01 = AUTO_BIND_C_FUNCTION_TO_LUA(int, TestFunction01);
auto ra00 = AUTO_BIND_C_FUNCTION_TO_LUA_ALIASED("T0", void, TestFunction00);
auto ra01 = AUTO_BIND_C_FUNCTION_TO_LUA_ALIASED("T1", int, TestFunction01);
auto r02 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction02, int);
auto r03 = AUTO_BIND_C_FUNCTION_TO_LUA(int, TestFunction03, int);
auto r04 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction04, const char *);
auto r05 = AUTO_BIND_C_FUNCTION_TO_LUA(const char *, TestFunction05);
auto r06 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction06, const std::string);
auto r07 = AUTO_BIND_C_FUNCTION_TO_LUA(const std::string, TestFunction07);
typedef std::pair<int, int> int_pair_t;
// auto r08 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction08, const
// int_pair_t&);
auto r09 = AUTO_BIND_C_FUNCTION_TO_LUA(int_pair_t, TestFunction09);
typedef std::vector<int> int_vector_t;
// auto r10 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction10, const
// int_vector_t&);
auto r11 = AUTO_BIND_C_FUNCTION_TO_LUA(int_vector_t, TestFunction11);
typedef std::array<int, 7> int_array_t;
// auto r12 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction12, const
// int_array_t&);
auto r13 = AUTO_BIND_C_FUNCTION_TO_LUA(int_array_t, TestFunction13);
typedef std::unordered_map<std::string, int> int_unordered_map_t;
// auto r14 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction14, const
// int_unordered_map_t&);
auto r15 = AUTO_BIND_C_FUNCTION_TO_LUA(int_unordered_map_t, TestFunction15);
auto r16 = AUTO_BIND_C_FUNCTION_TO_LUA(void, TestFunction16, int,
                                       std::vector<int>, int);

class Test;
static std::shared_ptr<Test> CreateTest(std::string) {
  return std::make_shared<Test>();
}
typedef std::unordered_map<std::string, int> unordered_map_si;
class Test {
 public:
  void FooVoid() {}
  int FooIntVoid() {
    return 1;
  }
  uint8_t FooInt8Void() {
    return 1;
  }
  uint16_t FooInt16Void() {
    return 1;
  }
  uint32_t FooInt32Void() {
    return 1;
  }
  uint64_t FooInt64Void() {
    return 1;
  }
  uint8_t FooUint8Void() {
    return 1;
  }
  uint16_t FooUint16Void() {
    return 1;
  }
  uint32_t FooUint32Void() {
    return 1;
  }
  uint64_t FooUint64Void() {
    return 1;
  }
  void FooVoidInt(int) {}
  void FooVoidIntInt(int, int) {}
  void FooVoidIntIntInt(int, int, int) {}
  void FooVoidInt8(int8_t) {}
  void FooVoidInt16(int16_t) {}
  void FooVoidInt32(int32_t) {}
  void FooVoidInt64(int64_t) {}
  void FooVoidUint8(uint8_t) {}
  void FooVoidUint16(uint16_t) {}
  void FooVoidUint32(uint32_t) {}
  void FooVoidUint64(uint64_t) {}
  void FooStr(std::string) {}
  int FooIntStr(std::string) {
    return 1;
  }
  void FooVectorInt(std::vector<int>) {}
  std::vector<int> FooVectorIntVoid() {
    return {
        1, 2, 3, 4, 5,
    };
  }
  void FooUnorderedMapInt(unordered_map_si) {}
  unordered_map_si FooUnorderedMapIntVoid() {
    return {
        {"1", 2},
        {"3", 4},
    };
  }
  void FooSharedPtr(std::shared_ptr<Test>) {}
  std::shared_ptr<Test> FooFactorySharedPtr() {
    return CreateTest("");
  }
  std::shared_ptr<Test> FooFactoryMixed(std::vector<int>, int) {
    return CreateTest("");
  }
};
static void UseTest(std::shared_ptr<Test>) {}
DEFINE_LUA_USERDATA_TYPE_PUSH(Test)
DEFINE_LUA_USERDATA_TYPE_POP(Test)

auto rc00 = LUA_USERDATA_CAST_PTR<Test>(nullptr, 0, "Test");
auto rc01 = LUA_USERDATA_CAST(nullptr, 0, Test);
auto rc02 =
    AUTO_BIND_C_FUNCTION_TO_LUA(std::shared_ptr<Test>, CreateTest, std::string);
auto rc03 = AUTO_BIND_C_FUNCTION_TO_LUA(void, UseTest, std::shared_ptr<Test>);
auto rc04 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, void, FooVoid);
auto rc05 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, int, FooIntVoid);
auto rc06 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, int8_t, FooInt8Void);
auto rc07 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, int16_t, FooInt16Void);
auto rc08 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, int32_t, FooInt32Void);
auto rc09 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, int64_t, FooInt32Void);
auto rc10 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, uint8_t, FooUint8Void);
auto rc11 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, uint16_t, FooUint16Void);
auto rc12 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, uint32_t, FooUint32Void);
auto rc13 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, uint64_t, FooUint32Void);
auto rc14 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidInt, int);
auto rc15 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidInt8, int8_t);
auto rc16 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidInt16, int16_t);
auto rc17 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidInt32, int32_t);
auto rc18 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidInt64, int64_t);
auto rc19 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidUint8, uint8_t);
auto rc20 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidUint16, uint16_t);
auto rc21 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidUint32, uint32_t);
auto rc22 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVoidUint64, uint64_t);
auto rc23 =
    AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA2(Test, void, FooVoidIntInt, int, int);
auto rc24 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA3(Test, void, FooVoidIntIntInt,
                                                  int, int, int);
auto rc25 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(
    Test, void, FooUnorderedMapInt, unordered_map_si);
auto rc26 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, unordered_map_si,
                                                  FooUnorderedMapIntVoid);
auto rc27 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooVectorInt,
                                                  std::vector<int>);
auto rc28 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, std::vector<int>,
                                                  FooVectorIntVoid);
auto rc29 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA1(Test, void, FooSharedPtr,
                                                  std::shared_ptr<Test>);
auto rc30 = AUTO_BIND_C_USERDATA_FUNCTION_TO_LUA0(Test, std::shared_ptr<Test>,
                                                  FooFactorySharedPtr);
