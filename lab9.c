#include <stdio.h>
#include <stdlib.h> // Include this for malloc and exit

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Define a structure for each node in the chain
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Define the HashType structure with an array of pointers to Nodes
struct HashType
{
    int size;
    struct Node** array;
};

// Compute the hash function to get the index for a given id
int hash(int id, int size)
{
    return id % size;
}

// Add a record to the hash table
void addToHash(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id, hashTable->size);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = record;
    newNode->next = hashTable->array[index];
    hashTable->array[index] = newNode;
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* hashTable)
{
    int i;
    for (i = 0; i < hashTable->size; ++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = hashTable->array[i];
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, id, order;
    char name;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d %c %d\n", &id, &name, &order);
            pRecord->id = id;
            pRecord->name = name;
            pRecord->order = order;
        }

        fclose(inFile);
    }

    return dataSz;
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);

    // Create a hash table with a size equal to or larger than recordSz
    struct HashType hashTable;
    hashTable.size = recordSz * 2; // Example size calculation
    hashTable.array = (struct Node**)malloc(sizeof(struct Node*) * hashTable.size);
    if (hashTable.array == NULL)
    {
        printf("Cannot allocate memory for hash table\n");
        exit(-1);
    }

    // Initialize hash table array to NULL
    for (int i = 0; i < hashTable.size; ++i)
    {
        hashTable.array[i] = NULL;
    }

    // Add records to the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        addToHash(&hashTable, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(&hashTable);

    // Free memory for hash table array and records
    for (int i = 0; i < hashTable.size; ++i)
    {
        struct Node* current = hashTable.array[i];
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable.array);
    free(pRecords);

    return 0;
}
