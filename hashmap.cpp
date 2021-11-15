#include "HashMap.hpp"
#include <string>
#include <iostream>

//Initializing Buckets first wit nullptr
void HashMap::initializeBucket(Node ** bucket, unsigned int bucketSize)
{
    for(unsigned int i = 0; i < bucketSize; ++i)
    {
        bucket[i] = nullptr;
    }
}

//Constructor
HashMap::HashMap()
    :hashFunction{[=] (std::string const & key) {return (key.length()*1099511628211) % bucketSize;}}, bucket{new Node* [bucketSize]}
{
    initializeBucket(bucket, bucketSize);
}

//Paramterized Constructor
HashMap::HashMap(HashFunction hashFunction)
    :hashFunction {hashFunction}, bucket{new Node* [bucketSize]}
{
    initializeBucket(bucket, bucketSize);
}

//Copy Constructor
HashMap::HashMap(const HashMap& hm)
    :hashFunction{[=] (std::string const & key) {return (key.length()*1099511628211) % bucketSize;}}, bucket{new Node* [bucketSize]}
{
    initializeBucket(bucket, bucketSize);

    for(unsigned int i= 0; i < bucketSize; ++i)
    {
        bucket[i] = hm.bucket[i];
    }
}
//Destructor
HashMap::~HashMap()
{
    delete[] bucket;
}

//Copy Assignment Constructor
HashMap& HashMap::operator=(const HashMap& hm)
{
    if(this != &hm)
    {
        Node** newBucket = new Node* [bucketSize];

        initializeBucket(newBucket, bucketSize);

        for(unsigned int i = 0; i < bucketSize; ++i )
        {
            newBucket[i] = hm.bucket[i];
            
            
            //Traversal node
            Node* curr = bucket[i];

            //Assigning Values
            while(curr != nullptr)
            {  
                newBucket[i] ->key = bucket[i]->key;
                newBucket[i] ->value = bucket[i]->value;
                curr=curr->next;
            }
        }
        hashFunction=hm.hashFunction;
        numOfKeys = hm.numOfKeys;
        bucketSize=hm.bucketSize;
        calcLoadFactor=hm.calcLoadFactor;
        delete[] newBucket;
    }

    return *this;
}

//Add Function
void HashMap::add(const std::string& key, const std::string& value)
{
    //Checking if strings  are empty
    if (key.empty() || value.empty())
    {
        return;
    }


    if(contains(key) == false)
    {
        //Calculating Load Factor
        double calcLoadFactor = (numOfKeys+1) / bucketSize;

        //Will Rehash If load factor is above 0.8
        if(calcLoadFactor > 0.8)
        {
            //Resiszing bucketSize 
            unsigned int newBucketSize = bucketSize * 2 + 1;
            unsigned int newBucketIndex = 0;

            //Getting Index calling hashfunction
            newBucketIndex = hashFunction(key);

            //Dynamically Allocating new array of pointers
            Node ** newBucket = new Node *[newBucketSize];
            
            //Filling Array
            initializeBucket(newBucket, newBucketSize);
            
            //Traversal node
            Node* curr = bucket[newBucketIndex];

            //Assigning Values
            while(curr != nullptr)
            {  
                newBucket[newBucketIndex] ->key = curr->key;
                newBucket[newBucketIndex] ->value = curr->value;
                curr=curr->next;
            }
            //Deleting
            delete[] bucket;

            //Assign rehash to the new array
            bucket = newBucket;
            bucketSize = newBucketSize;

            //For Size() function
            numOfKeys++;
            calcLoadFactor = numOfKeys / bucketSize;
            delete[] newBucket;
        }
        else
        {
            //Traversal Node
            Node* curr = bucket[getIndex(key)];

            //Assigning Values
            while(curr != nullptr)
            {
                bucket[getIndex(key)]->key = key;
                bucket[getIndex(key)]->value = value;
                
            }
            //For Size() Function
            numOfKeys++;
         
            // std::cout << bucket[getIndex(key)]->key  <<std::endl;
            //     std::cout <<bucket[getIndex(key)]->value <<std::endl;
        }
    }
    else
    {
        return;
    }
}

bool HashMap::remove(const std::string& key)
{
    //Traversal Node
    Node* curr = bucket[getIndex(key)];
    //Node pointer to previous node
    Node* prev;

    //Counter to see if it is the first linked list
    unsigned int count = 0;

    while(curr != nullptr)
    {
        if(curr->key == key)
        {
            if(count == 0)
            {
                //Reassining the links
                bucket[getIndex(key)]->next = curr->next;
            }
            else
            {
                //Reassgining links
                prev->next = curr->next;
            }
            
            //deleteing node
            delete curr;
            return true;
        }
        //Traversing to node we want to delete
        count++;

        //Reassignment
        prev = curr;
        curr=curr->next;

        //Decreasing Size()
        numOfKeys--;
        
    }
    return false;
}
bool HashMap::contains(const std::string& key) const
{

    //looping through each bucket index to check if we have the  key
    for(unsigned int i= 0; i < bucketSize; ++i)
    {
        //Traversal Node
        Node* curr = bucket[i];
        while(curr != nullptr )
        {
            if(curr->key == key) // if we have the value
            {
                std::cout << curr->key;
                return true;
            }
            curr = curr->next;
        }
    }
    
    return false;
}

std::string HashMap:: value(const std::string& key) const
{
    //if the string is empty just return empty string
    if(key.empty())
    {
        return "";
    }

    //hash key
    unsigned int index = hashFunction(key);
    
    //if we have the key return the value
    if(contains(key)==true)
    {
        return bucket[index]->value;
    }

    //Return empty string
    return "";
}

//Size function
unsigned int HashMap::size() const
{
    return numOfKeys;
}

//BucketCount function
unsigned int HashMap:: bucketCount() const
{
    return bucketSize;
}

//Load Factor function
double HashMap::loadFactor() const
{
    return calcLoadFactor;
}

//Finding the max bucket size
unsigned int HashMap::maxBucketSize() const
{
    unsigned int count = 0;
    unsigned int maxSize = 0;

    for(unsigned int i=0; i < bucketSize; ++i)
    {
        //Traversal
        Node* curr = bucket[i];
        while(curr != nullptr)
        {
            curr = curr->next; 
            count++;//To see how many linked lists there are
        }

        //Checking if which one is the max
        if(count > maxSize)
        {
            maxSize = count;
        }
    }

    return maxSize;
}

unsigned int HashMap::getIndex( const std::string& key)
{
    //hashing
    unsigned int bucketIndex = hashFunction(key);
    //there is scenario where the default hash function returns something that is larger the the number of buckets
    if(bucketIndex > bucketSize)
    {
        return  bucketIndex%bucketSize;
    }
    else
        return bucketIndex;

}
