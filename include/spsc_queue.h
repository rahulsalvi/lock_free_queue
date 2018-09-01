#ifndef SPSC_QUEUE_H
#define SPSC_QUEUE_H

#include <atomic>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

using std::size_t;
using std::atomic;
using std::is_trivially_copyable;
using std::memcpy;

/**
 * @brief Rahul's Utilities
 */
namespace rutils {
    /**
     * @brief Single Producer, Single Consumer Queue
     *
     * @tparam T the type of object the queue holds. must be trivially copyable
     * @tparam N the max number of items of type T that the queue can hold
     */
    template <class T, size_t N>
    class spsc_queue {
        static_assert(is_trivially_copyable<T>::value, "spsc_queue: T must be trivially copyable");
        public:
            /**
             * @brief default constructor
             *
             * @param data a preallocated buffer that the queue will use to hold objects. must
             *             be large enough to hold N items of type T
             */
            spsc_queue(T* const data);
            /**
             * @brief default destructor
             */
            ~spsc_queue();
            /**
             * @brief blocking enqueue
             *
             * @param obj a constant reference to the object to be copied to the queue
             *
             * @return 0 on success, 1 on failure
             */
            int enqueue(const T &obj);
            /**
             * @brief blocking dequeue
             *
             * @param obj a reference to an object that the dequeued object will overwrite
             *
             * @return 0 on success, 1 on failure
             */
            int dequeue(T &obj);
            /**
             * @brief nonblocking enqueue, returns immediately if the queue is full
             *
             * @param obj a constant reference to the object to be copied to the queue
             *
             * @return 0 if the item was successfully enqueued, 1 if the item was not enqueued
             */
            int try_enqueue(const T &obj);
            /**
             * @brief nonblocking dequeue, returns immediately if the queue is empty
             *
             * @param obj a reference to an object that the dequeued object will overwrite
             *
             * @return 0 if an item was successfully dequeued, 1 if no item was dequeued
             */
            int try_dequeue(T &obj);
            /**
             * @brief returns the size of the queue in bytes
             *
             * @return the size of the queue in bytes
             */
            size_t size() const;
            /**
             * @brief returns the max number of items of type T the queue can hold
             *
             * @return the max number of items of type T the queue can hold
             */
            size_t capacity() const;
            /**
             * @brief returns the current number of items in the queue
             *
             * @return the current number of items in the queue
             */
            size_t item_count() const;
            /**
             * @brief returns whether the queue is full
             *
             * @return true if the queue is full, false otherwise
             */
            bool full() const;
            /**
             * @brief returns whether the queue is empty
             *
             * @return true if the queue is empty, false otherwise
             */
            bool empty() const;
        private:
            /**
             * @brief performs an enqueue
             *
             * @param obj the object to be enqueued
             */
            void do_enqueue(const T &obj);
            /**
             * @brief performs a dequeue
             *
             * @param obj the location to store the dequeued object
             */
            void do_dequeue(T &obj);
            T* const _data;
            T* _write;
            T* _read;
            atomic<size_t> _item_count;
    };

    template <class T, size_t N>
    spsc_queue<T, N>::spsc_queue(T* const data) : _data(data), _write(data), _read(data) {
        _item_count.store(0);
    }

    template <class T, size_t N>
    spsc_queue<T, N>::~spsc_queue() {}

    template <class T, size_t N>
    int spsc_queue<T, N>::enqueue(const T& obj) {
        while (full()) {}
        do_enqueue(obj);
        return 0;
    }

    template <class T, size_t N>
    int spsc_queue<T, N>::dequeue(T& obj) {
        while (empty()) {}
        do_dequeue(obj);
        return 0;
    }

    template <class T, size_t N>
    int spsc_queue<T, N>::try_enqueue(const T& obj) {
        if (full()) {return 1;}
        do_enqueue(obj);
        return 0;
    }

    template <class T, size_t N>
    int spsc_queue<T, N>::try_dequeue(T& obj) {
        if (empty()) {return 1;}
        do_dequeue(obj);
        return 0;
    }

    template <class T, size_t N>
    size_t spsc_queue<T, N>::size() const {
        return (sizeof(T) * N);
    }

    template <class T, size_t N>
    size_t spsc_queue<T, N>::capacity() const {
        return N;
    }

    template <class T, size_t N>
    size_t spsc_queue<T, N>::item_count() const {
        return _item_count.load();
    }

    template <class T, size_t N>
    bool spsc_queue<T, N>::full() const {
        return (_item_count.load() == N);
    }

    template <class T, size_t N>
    bool spsc_queue<T, N>::empty() const {
        return (_item_count.load() == 0);
    }

    template <class T, size_t N>
    void spsc_queue<T, N>::do_enqueue(const T& obj) {
        memcpy(_write, &obj, sizeof(T));
        _write = (_write + 1 == (_data+N)) ? _data : _write + 1;
        _item_count++;
    }

    template <class T, size_t N>
    void spsc_queue<T, N>::do_dequeue(T& obj) {
        memcpy(&obj, _read, sizeof(T));
        _read = (_read + 1 == (_data+N)) ? _data : _read + 1;
        _item_count--;
    }
}

#endif // SPSC_QUEUE_H
