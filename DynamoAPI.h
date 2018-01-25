#ifndef DYNAMOAPI_H 
#define DYNAMOAPI_H

#ifdef __cplusplus
extern "C" {
#endif

void TableCreate(const char *name);
void InsertItem(const char *table, const char *name);
void GetItem(const char *table, const char *name);

#ifdef __cplusplus
}
#endif


#endif