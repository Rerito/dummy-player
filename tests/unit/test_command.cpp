
#include "gtest/gtest.h"
#include "commands/command.hpp"
#include <vector>

int sum(int a, int b) {
    return a + b;
}


struct CommandTest : testing::Test {
};

TEST_F(CommandTest, RunCommand) {
    using namespace dp;
    auto s = command<decltype(sum)>{&sum}({"12", "25"});
    ASSERT_EQ(s, "37") << "expected sum(12,25) to be 37 but was " << s << " instead.";
}
