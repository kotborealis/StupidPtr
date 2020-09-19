#pragma once

/**
 * Умный указатель Unique_ptr
 *
 * Держит владение ресурсом
 * Не копируется
 * Можно переместить
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
class UniquePtr {
public:
    /**
     * Пустой конструктор
     * Инициализируем ресурс через nullptr
     */
    UniquePtr() : pointer(nullptr) {};

    /**
     * Обычный конструктор
     * @param pointer
     */
    explicit UniquePtr(T *pointer)
            : pointer(pointer) {};

    /**
     * Конструктор копирования
     *
     * Удалён, нельзя копировтаь
     * @param other
     */
    UniquePtr(const UniquePtr<T> &other) = delete;

    /**
     * Оператор присваивания через копирование
     *
     * Удалён, нельзя копировтаь
     * @param other
     * @return
     */
    UniquePtr<T> &operator=(const UniquePtr<T> &other) = delete;

    /**
     * Конструктор перемещения
     *
     * Забирает ресурс у other
     * @param other
     */
    UniquePtr(UniquePtr<T> &&other) {
        this->destroy();
        this->pointer = other.pointer;
        other.pointer = nullptr;
    }

    /**
     * Оператор присваивания перемещением
     *
     * Забирает ресурс у other
     * @param other
     * @return
     */
    UniquePtr &operator=(UniquePtr &&other) {
        if(this == &other) return *this;

        this->destroy();
        this->pointer = other.pointer;
        other.pointer = nullptr;

        return *this;
    }

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
        destroy();
    }

    /**
     * Сбросить умный указатель и перезаписать новым ресурсом
     * @param _pointer
     */
    void reset(T *_pointer) {
        assert(_pointer == nullptr || _pointer != this->pointer);
        destroy();
        this->pointer = _pointer;
    }

    /**
     * Отпустить ресурс, не удаляя его
     */
    void release() {
        pointer = nullptr;
    }

    /**
     * Деструктор
     */
    ~UniquePtr() {
        destroy();
    }

private:
    /**
     * Указатель на ресурс
     */
    T *pointer = nullptr;

    /**
     * Уничтожение ресурса
     */
    void destroy() {
        delete pointer;
        pointer = nullptr;
    }
};