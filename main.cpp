#include "ConverterJSON.h"
#include "gtest/gtest.h"

TEST(sample_test_case, sample_test){
    ConverterJSON searchMachine;
    searchMachine.GetTextDocuments();
    EXPECT_EQ(1, 1);
}

/*
int main() {
    ConverterJSON searchMachine;
    searchMachine.GetTextDocuments();
}*/
