#include <cstdlib>
#include <iostream>
#include <string>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h> 
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/ScalarAttributeType.h>
//Getitem
#include <aws/dynamodb/model/GetItemRequest.h>
//PutItem
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>

#include "DynamoAPI.h"

#ifdef __cplusplus

extern "C" {
#endif

    // Inside this "extern C" block, I can define C functions that are able to call C++ code

    void TableCreate(const char *name) {    
        Aws::SDKOptions options;
        std::cout<< name;
        
        Aws::InitAPI(options);
        {
            Aws::Client::ClientConfiguration clientConfig;
            Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);
            Aws::DynamoDB::Model::CreateTableRequest req;

            Aws::DynamoDB::Model::AttributeDefinition haskKey;
            haskKey.SetAttributeName("Name");
            haskKey.SetAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);
            req.AddAttributeDefinitions(haskKey);

            Aws::DynamoDB::Model::KeySchemaElement keyscelt;
            keyscelt.WithAttributeName("Name").WithKeyType(Aws::DynamoDB::Model::KeyType::HASH);
            req.AddKeySchema(keyscelt);

            Aws::DynamoDB::Model::ProvisionedThroughput thruput;
            thruput.WithReadCapacityUnits(5).WithWriteCapacityUnits(5);
            req.SetProvisionedThroughput(thruput);

            req.SetTableName(name);

            const Aws::DynamoDB::Model::CreateTableOutcome& result = dynamoClient.CreateTable(req);
            if (result.IsSuccess())
            {
                std::cout << "Table \"" << result.GetResult().GetTableDescription().GetTableName() <<
                    " was created!" << std::endl;
            }
            else
            {
                std::cout << "Failed to create table: " << result.GetError().GetMessage();
            }
        }
        Aws::ShutdownAPI(options);
        
    }

    void GetItem(const char *table, const char *name) {    

        Aws::SDKOptions options;
        std::cout<< name;
        
        Aws::InitAPI(options);
        {
            Aws::Client::ClientConfiguration clientConfig;
            Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);
            Aws::DynamoDB::Model::GetItemRequest req;
            /*
            if (!projection.empty())
                req.SetProjectionExpression(projection);
            */
            Aws::DynamoDB::Model::AttributeValue haskKey;
            haskKey.SetS(name);
            req.AddKey("Name", haskKey);
            req.SetTableName(table);

            const Aws::DynamoDB::Model::GetItemOutcome& result = dynamoClient.GetItem(req);
            if (result.IsSuccess())
            {
                const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& item = result.GetResult().GetItem();
                if (item.size() > 0)
                {
                    for (const auto& i : item)
                        std::cout << i.first << ": " << i.second.GetS() << std::endl;
                }
                else
                {
                    std::cout << "No item found with the key " << name << std::endl;
                }

            }
            else
            {
                std::cout << "Failed to get item: " << result.GetError().GetMessage();
            }
        }
        Aws::ShutdownAPI(options);
    }


    void InsertItem(const char *table, const char *name) {    

        Aws::SDKOptions options;
        std::cout<< name;
        const char* attributes[] = {"name:Aniket", "age:34", "dob:010101"};
        int station = 888;

        Aws::InitAPI(options);
        {
            Aws::Client::ClientConfiguration clientConfig;
            Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);

            Aws::DynamoDB::Model::PutItemRequest pir;
            pir.SetTableName(table);

            Aws::DynamoDB::Model::AttributeValue av;
            av.SetS(name);
            pir.AddItem("Name", av);
            std::cout << "Done!" << attributes[0];
            
            for (int x = 0; x < 3; x++)
            {
                const Aws::String arg(attributes[x]);

                const Aws::Vector<Aws::String>& flds = Aws::Utils::StringUtils::Split(arg, ':');
                if (flds.size() == 2)
                {
                    Aws::DynamoDB::Model::AttributeValue val;
                    val.SetS(flds[1]);
                    pir.AddItem(flds[0], val);
                }
                else
                {
                    std::cout << "Invalid argument: " << arg << std::endl ;
                }
            }

            const Aws::DynamoDB::Model::PutItemOutcome result = dynamoClient.PutItem(pir);
            if (!result.IsSuccess())
            {
                std::cout << result.GetError().GetMessage() << std::endl;
                
            }
                std::cout << "Done!" << std::endl;
              
        }
                
        Aws::ShutdownAPI(options);
    }

#ifdef __cplusplus
}


#endif