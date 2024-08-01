#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

// Check the config.json, as I remember if there aren't any info about version program need to give out mistake

int main() {
    ConverterJSON searchMachine;

    InvertedIndex structure;
    structure.UpdateDocumentBase(searchMachine.GetTextDocuments());

    SearchServer searchServer(structure);
    searchServer.search(searchMachine.GetRequests());
}
