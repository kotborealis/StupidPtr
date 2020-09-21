# StupidPtr
Stupid&amp;broken smart pointers in C++

## Заготовочка для WeakPtr

```c++
struct ControlBlock {
    ControlBlock() : refCount(1), weakRefCount(0) {};

    void acquire() {
        refCount++;
    }

    void release() {
        refCount--;
    }

    void acquireWeak() {
        refCount++;
    }

    void releaseWeak() {
        refCount--;
    }

    unsigned refCount;
    unsigned weakRefCount;
};

template<typename T>
class WeakPtr {
    WeakPtr(SharedPtr<T> ptr) {
        this->controlBlock = ptr.controlBlock;
        this->controlBlock->acquireWeak();
        this->pointer = ptr.pointer;
    }

    lock() {
        if(refCount > 0)
            return SharedPtr(pointer);
    }

    ~WeakPtr() {
        controlBlock->releaseWeak();
        if(controlBlock->refCount == 0 && controlBlock->weakRefCount == 0){
            delete controlBlock;
        }
    }
};
```
