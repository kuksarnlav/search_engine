#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    ConverterJSON searchMachine;

    InvertedIndex structure;
    structure.UpdateDocumentBase(searchMachine.GetTextDocuments());

    SearchServer searchServer(structure);
    searchServer.search(searchMachine.GetRequests());
}
