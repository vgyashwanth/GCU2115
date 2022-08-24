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
    CircularQueue(T *tArray, uint16_t u16MaxArraySize);

    bool EnQueue(T *pElement);

    bool DeQueue(T *pElement);

    uint16_t GetRemainingQSize();

    void PrintState();

private:
    T         *_pQArray;
    uint16_t  _u16MaxQSize;
    /* _u16Head == _u16Tail can be either interpreted as Q empty or Q full. This
       variable is used to distinguish Q full vs not full */ 
    bool      _bIsFull;
    uint16_t  _u16Head;
    uint16_t  _u16Tail;

};

template <class T>
CircularQueue<T>::CircularQueue(T *tArray, uint16_t u16MaxArraySize):
_u16MaxQSize(u16MaxArraySize),
_pQArray(tArray),
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
        return _u16Head-_u16Tail;
    }
    /* i.e _u16Head < _u16Tail */
    return _u16MaxQSize - _u16Tail + _u16Head;
}

template <class T>
void CircularQueue<T>::PrintState()
{
    printf("_u16Head:%d\n", _u16Head);
    printf("_u16Tail:%d\n", _u16Tail);
    printf("_bIsFull:%d\n", _bIsFull);
    printf("Size    :%d\n", GetRemainingQSize());
    printf("[");
    for(int i=0;i<_u16MaxQSize;i++)
    {
        printf("%d ", _pQArray[i]);
    }
    printf("]\n\n");
}

#endif
