#include "DynamoAPI.h"

int main() {
    //TableCreate("Test");
    InsertItem("Test", "HelloW");
    GetItem("Test", "HelloW");
    return 0;
}