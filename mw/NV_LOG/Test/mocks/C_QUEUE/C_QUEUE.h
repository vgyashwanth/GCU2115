/**
 * @file        [C_QUEUE]
 * @brief       C_QUEUE
 * @brief       This module implements a circular queue. The implementation is
 *              template based and hence data type agnostic.
 *              
 * @version     [Version number]
 * @author      Sudeep Chandrasekaran
 * @date        18th May 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _C_QUEUE_H_
#define _C_QUEUE_H_

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

template <class T>
class CircularQueue
{
public:
    /**
     * @Name   CircularQueue
     * @brief
     *  Constructs the circular queue.
     * @param  tArray           - Pointer to an array to store the elements
     * @param  u16MaxArraySize  - size of tArray
     * @return 
     * None
     */
    CircularQueue(T *tArray, uint16_t u16MaxArraySize);

    /**
     * @Name   EnQueue
     * @brief
     *  Puts an element onto the queue.
     * @param  pElement - Pointer to the element to be populated
     * @return 
     * true if the successfully populated.
     * false if the queue was full
     */
    bool EnQueue(T *pElement);

    /**
     * @Name   DeQueue
     * @brief
     *  Removes an element from the queue.
     * @param  pElement - Pointer to the container to copy the removed element.
     * @return 
     * true if the successfully removed.
     * false if the queue was empty
     */
    bool DeQueue(T *pElement);

    /**
     * @Name   GetRemainingQSize
     * @brief
     *  Returns the number of elements currently in the queue.
     * @param  - None
     * @return 
     * Number of elements in the queue.
     */
    uint16_t GetRemainingQSize();

    /**
     * @Name   Peek
     * @brief
     *  Fetches the oldest element in the queue.
     * @param  pElement - Pointer to the container to populate the element.
     * Note: This API does not remove the element from the queue.
     * @return 
     * true if the element was successfully fetched.
     */
    bool Peek(T *pElement);

private:
    T         *_pQArray;
    uint16_t  _u16MaxQSize;
    /* _u16Head == _u16Tail can be either interpreted as Q empty or Q full. This
       variable is used to distinguish Q full vs not full */
    bool      _bIsFull;
    /* Always points to the next free location */
    uint16_t  _u16Head;
    /* Always points to the oldest element in the queue */
    uint16_t  _u16Tail;

};

template <class T>
CircularQueue<T>::CircularQueue(T *tArray, uint16_t u16MaxArraySize):
_pQArray(tArray),
_u16MaxQSize(u16MaxArraySize),
_bIsFull(false),
_u16Head(0),
_u16Tail(0)
{

}

template <class T>
bool CircularQueue<T>::EnQueue(T *pElement)
{
    if(pElement == NULL)
    {
        return false;
    }
    if(_bIsFull)
    {
        return false;
    }
    _pQArray[_u16Head++] = *pElement;
    /* Wrap the head pointer */
    _u16Head %= _u16MaxQSize;
    /* Check weather the Q is full or not*/
    if(_u16Head == _u16Tail)
    {
        _bIsFull = true;
    }
    return true;
}

template <class T>
bool CircularQueue<T>::DeQueue(T *pElement)
{
    if(pElement == NULL)
    {
        return false;
    }
    /* Check if there is any element in the Q */
    if(!_bIsFull && (_u16Head == _u16Tail))
    {
        return false;
    }
    _bIsFull = false;
    *pElement = _pQArray[_u16Tail++];
    /* Wrap the tail pointer */
    _u16Tail %= _u16MaxQSize;
    return true;
}

/*Looks at the last element but does not deque it*/
template <class T>
bool CircularQueue<T>::Peek(T *pElement)
{
    if(pElement == NULL)
    {
        return false;
    }
    /* Check if there is any element in the Q */
    if(!_bIsFull && (_u16Head == _u16Tail))
    {
        return false;
    }
    *pElement = _pQArray[_u16Tail];
    return true;
}

template <class T>
uint16_t CircularQueue<T>::GetRemainingQSize()
{
    if(_u16Head == _u16Tail)
    {
        if(_bIsFull)
        {
            return _u16MaxQSize;
        }
        return 0;
    }
    if(_u16Head > _u16Tail)
    {
        return (uint16_t)(_u16Head-_u16Tail);
    }
    /* i.e _u16Head < _u16Tail */
    return (uint16_t)(_u16MaxQSize - _u16Tail + _u16Head);
}

#endif
