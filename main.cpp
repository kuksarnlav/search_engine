#include "ConverterJSON.h"
#include "InvertedIndex.h"
/*#include "gtest/gtest.h"

TEST(sample_test_case, sample_test){
    ConverterJSON searchMachine;
    InvertedIndex structure;
    structure.UpdateDocumentBase(searchMachine.GetTextDocuments());
}*/

int main() {
    ConverterJSON searchMachine;
    InvertedIndex structure;
    structure.UpdateDocumentBase(searchMachine.GetTextDocuments());
}
