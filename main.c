#include <stdlib.h>
#include <stdio.h>

typedef struct node_t {
    int key;
    int value;
    struct node_t* next;
} node_t;

unsigned int hash(unsigned int key, int numBuckets)
{
    return key % numBuckets;
}

node_t* getValueIfExists(int key, node_t** buckets, int numBuckets)
{
    node_t* bucket = buckets[hash(key, numBuckets)];
    
    while (bucket != NULL)
    {
        if (bucket->key == key)
        {            
            return bucket;
        }
        bucket = bucket->next;
    }
    
    return NULL;
}

void insertNewValue(int key, int value, node_t** buckets, int numBuckets)
{
    unsigned int index = hash(key, numBuckets);
    node_t* bucket = buckets[index];
    
    if (bucket == NULL)
    {
        // If there is not a collision, create a new key value pair and set its values.
        buckets[index] = (node_t*)calloc(1, sizeof(node_t*));
        buckets[index]->key = key;
        buckets[index]->value = value;
    }
    else
    {
        // If there was a collision:
        // Iterate until we have found the last key value pair.
        while(bucket->next != NULL) bucket = bucket->next;
        
        // Create a new key value pair and set its values.
        node_t* newKeyValuePair = (node_t*)calloc(1, sizeof(node_t*));
        newKeyValuePair->key = key;
        newKeyValuePair->value = value;
        
        // Set the last key value pair to point to this new key value pair.
        bucket->next = newKeyValuePair;
    }
}

void freeAllBuckets(node_t** buckets, int numBuckets)
{
    for (int i = 0; i < numBuckets; ++i)
    {
        node_t* bucket = buckets[i];
        
        while(bucket != NULL)
        {
            node_t* bucketToFree = bucket;
            bucket = bucketToFree->next;
            free(bucketToFree);
        }
    }
}

int majorityElement(int* nums, int numsSize)
{
    node_t** buckets = (node_t**)calloc(numsSize, sizeof(node_t*));
    
    int majorityElement = nums[0];
    int majorityElementOccurences = 1;
    insertNewValue(majorityElement, majorityElementOccurences, buckets, numsSize);
    
    for (int i = 1; i < numsSize; ++i)
    {
        node_t* keyValuePair = getValueIfExists(nums[i], buckets, numsSize);

        if (keyValuePair != NULL)
        {
            keyValuePair->value++;
            
            if (keyValuePair->value > majorityElementOccurences)
            {
                majorityElementOccurences = keyValuePair->value;
                majorityElement = keyValuePair->key;
            }
        }
        else
        {
            insertNewValue(nums[i], 1, buckets, numsSize);
        }
    }
    
    freeAllBuckets(buckets, numsSize);
    free(buckets);
    
    return majorityElement;
}

int main()
{
    int numsSize = 7;
    int* nums = (int*)malloc(numsSize * sizeof(int));
    nums[0] = 1;
    nums[1] = 1;
    nums[2] = 1;
    nums[3] = 1;
    nums[4] = 1;
    nums[5] = 1;
    nums[5] = 8;

    printf("%d", majorityElement(nums, numsSize));

    free(nums);
}