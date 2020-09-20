#pragma once

/**
 * Мета-данные SharedPtr с счётчиком
 */
struct SharedMeta {
    /**
     * Счётчик ссылок
     */
    unsigned refCounter;

    SharedMeta() : refCounter(1) {};

    ~SharedMeta() {};

    void acquire() {
        refCounter++;
    }

    void release() {
        refCounter--;
    }
};

/**
 * Умный указатель Shared_ptr
 *
 * Разделённое владение ресурсом
 * Можно копировать
 * Нельзя переместить
 *
 * Огромная пятёрка:
 * * Конструктор +
 * * Деструктор +
 * * Конструктор копирования +
 * * Оператор присваивания копированием +
 * * Конструктор перемещения +
 * * Оператор присваивания перемешением +
 */
template<typename T>
class SharedPtr {
public:
    /**
     * Пустой конструктор
     * Инициализируем ресурс через nullptr
     */
    SharedPtr() : pointer(nullptr), meta(new SharedMeta) {
    };

    /**
     * Обычный конструктор
     * @param pointer
     */
    explicit SharedPtr(T *pointer)
            : pointer(pointer), meta(new SharedMeta) {
    };

    /**
     * Конструктор копирования
     * @param other
     */
    SharedPtr(const SharedPtr<T> &other) {
        this->pointer = other.pointer;
        this->meta = other.meta;
        meta->acquire();
    };

    /**
     * Оператор присваивания через копирование
     * @param other
     * @return
     */
    SharedPtr<T> &operator=(const SharedPtr<T> &other) {
        if (this == &other) return *this;
        destroy();
        this->pointer = other.pointer;
        this->meta = other.meta;
        meta->acquire();
    };

    /**
     * Конструктор перемещения
     * @param other
     */
    SharedPtr(SharedPtr<T> &&other) = delete;

    /**
     * Оператор присваивания перемещением
     * @param other
     * @return
     */
    SharedPtr &operator=(SharedPtr &&other) = delete;

    /**
     * Приведение к bool
     * @return
     */
    explicit operator bool() const {
        return pointer != nullptr;
    }

    /**
     * Разыменование
     * @return
     */
    T &operator*() const {
        assert(pointer != nullptr);
        return *pointer;
    }

    /**
     * Оператор стрелочки
     * @return
     */
    T *operator->() const {
        assert(pointer != nullptr);
        return pointer;
    }

    /**
     * Вернуть указатель на ресурс
     * @return
     */
    T *get() const {
        return pointer;
    }

    /**
     * Сбросить умный указатель
     */
    void reset() {
        meta->release();
        if (meta->refCounter == 0) {
            delete meta;
            delete pointer;
        }
        meta = new SharedMeta;
        pointer = nullptr;
    }

    /**
     * Сбросить умный указатель и перезаписать новым ресурсом
     * @param _pointer
     */
    void reset(T *_pointer) {
        assert(_pointer == nullptr || _pointer != pointer);
        meta->release();
        if (meta->refCounter == 0) {
            delete meta;
            delete pointer;
        }
        meta = new SharedMeta;
        pointer = _pointer;
    }

    /**
     * Отпустить ресурс, не удаляя его
     */
    void release() {
        meta->release();
        if (meta->refCounter == 0) {
            delete meta;
        }
        pointer = nullptr;
        meta = new SharedMeta;
    }

    /**
     * Деструктор
     */
    ~SharedPtr() {
        destroy();
    }

private:
    /**
     * Указатель на ресурс
     */
    T *pointer = nullptr;

    /**
     * Мета-информация и счётчики
     */
    SharedMeta *meta;

    /**
     * Уничтожение ресурса
     */
    void destroy() {
        meta->release();
        if (meta->refCounter == 0) {
            delete pointer;
            delete meta;
        }
        pointer = nullptr;
    }
};