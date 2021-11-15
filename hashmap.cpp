#include "HashMap.hpp"
#include <string>

void HashMap::initializeBucket(Node ** bucket, unsigned int bucketSize)
{
    for(unsigned int i = 0; i < bucketSize; ++i)
    {
        bucket[i] = nullptr;
    }
}
//Constructor
HashMap::HashMap()
    :hashFunction{[=] (std::string const & key) {return (key.length()*1099511628211) % bucketSize;}}, bucket{new Node* [INITIAL_BUCKET_COUNT]}
{
    initializeBucket(bucket, bucketSize);
}

//Paramterized Constructor
HashMap::HashMap(HashFunction hashFunction)
    :hashFunction {hashFunction}, bucket{new Node* [INITIAL_BUCKET_COUNT]}
{
    initializeBucket(bucket, bucketSize);
}

//Copy Constructor

//Destructor
HashMap::~HashMap()
{
    delete[] bucket;
}

//Copy Assignment Constructor


//Add Function
void HashMap::add(const std::string& key, const std::string& value)
{
    if (key.empty() || value.empty())
    {
        return;
    }

    if(contains(key) == false)
    {
        double calcLoadFactor = (numOfKeys+1) / bucketSize;

        if(calcLoadFactor > 0.8)
        {
            unsigned int newBucketSize = bucketSize * 2 + 1;
            unsigned int newBucketIndex = 0;
            //Getting Index
            newBucketIndex = hashFunction(key);
            Node ** newBucket = new Node *[newBucketSize];
            
            for(unsigned int i = 0 ; i < newBucketSize; ++i)
            {
                newBucket[i] = nullptr;
            }
            
            Node* curr = bucket[newBucketIndex];
            while(curr != nullptr)
            {  
                
                newBucket[newBucketIndex] ->key = curr->key;
                newBucket[newBucketIndex] ->value = curr->value;
                curr=curr->next;
            }
            
            //Asssigning New value
            newBucket[newBucketIndex] ->key = key;
            newBucket[newBucketIndex] ->value = value;
            //Deleting
            delete[] bucket;

            //Assign rehash to the new array
            bucket = newBucket;
            bucketSize = newBucketSize;
            numOfKeys++;
            calcLoadFactor = numOfKeys / bucketSize;
            //delete[] newBucket;
        }
        else
        {

            bucket[getIndex(key)]->key = key;
            bucket[getIndex(key)]->value = value;
            numOfKeys++;
        }
    }
    else
    {
        return;
    }
}

bool HashMap::remove(const std::string& key)
{
    Node* curr = bucket[getIndex(key)];
    Node* prev;
    unsigned int count = 0;

    while(curr != nullptr)
    {
        if(curr->key == key)
        {
            if(count == 0)
            {
                bucket[getIndex(key)]->next = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            
            delete curr;
            return true;
        }
        count++;
        prev = curr;
        curr=curr->next;
        numOfKeys--;
        
    }
    return false;
}
bool HashMap::contains(const std::string& key) const
{
    bool result = false;

    for(unsigned int i = 0; i < bucketSize; ++i)
    {
        Node* curr = bucket[i];
        while(curr != nullptr )
        {
            if(curr->key == key )
            {
                result = true;
            }
            curr = curr->next;
        }
    }
    return result;
}

std::string HashMap:: value(const std::string& key) const
{
    if(key.empty())
    {
        return "";
    }

    unsigned int index = hashFunction(key);
    
    if(contains(key)==true)
    {
        return bucket[index]->value;
    }

    return "";
}

unsigned int HashMap::size() const
{
    return numOfKeys;
}

unsigned int HashMap:: bucketCount() const
{
    return bucketSize;
}

double HashMap::loadFactor() const
{
    return calcLoadFactor;
}

unsigned int HashMap::maxBucketSize() const
{
    unsigned int count = 0;
    unsigned int maxSize = 0;

    for(unsigned int i=0; i < bucketSize; ++i)
    {
        Node* curr = bucket[i];
        while(curr != nullptr)
        {
            curr = curr->next; 
            count++;
        }
        if(count > maxSize)
        {
            maxSize = count;
        }
    }

    return maxSize;
}

unsigned int HashMap::getIndex(const std::string& key)
{
    unsigned int bucketIndex = hashFunction(key);
    if(bucketIndex > bucketSize)
    {
        return  bucketIndex%bucketSize;
    }
    else
        return bucketIndex;

}