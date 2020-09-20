#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "UniquePtr.h"
#include "SharedPtr.h"

/**
 * Структура для тестирования
 */
struct TestStruct {
    explicit TestStruct(int value) :
            value(value) {
        ++instances;
    }

    ~TestStruct() {
        --instances;
    }

    void incr() {
        ++value;
    }

    void decr() {
        --value;
    }

    int value;
    static int instances;
};

int TestStruct::instances = 0;

/**
 * Методы для тестирования
 */
void helperRef(UniquePtr<TestStruct> &x) {
    REQUIRE(x->value == 123);
}
void helperMove(UniquePtr<TestStruct> x) {
    REQUIRE(x->value == 123);
}
void helperRef(SharedPtr<TestStruct> &x) {
    REQUIRE(x->value == 123);
}
void helperMove(SharedPtr<TestStruct> x) {
    REQUIRE(x->value == 123);
}

TEST_CASE("UniquePtr") {

    SECTION("empty") {
        UniquePtr<TestStruct> x;

        REQUIRE(x.get() == nullptr);
        REQUIRE(!!x == false);

        x.reset();

        REQUIRE(x.get() == nullptr);
        REQUIRE(!!x == false);
    }


    SECTION("RAII") {
        REQUIRE(TestStruct::instances == 0);

        {
            UniquePtr<TestStruct> x(new TestStruct(123));
            REQUIRE(TestStruct::instances == 1);
        }

        REQUIRE(TestStruct::instances == 0);
    }


    SECTION("release") {
        REQUIRE(TestStruct::instances == 0);
        auto s = new TestStruct(123);
        REQUIRE(TestStruct::instances == 1);
        UniquePtr<TestStruct> x(s);
        REQUIRE(TestStruct::instances == 1);
        x.release();
        REQUIRE(TestStruct::instances == 1);
        delete s;
    }

    SECTION("basic") {
        UniquePtr<TestStruct> x(new TestStruct(123));

        REQUIRE(x.get() != nullptr);
        REQUIRE(!!x == true);

        REQUIRE(x->value == 123);
        REQUIRE(x->instances == 1);

        x.reset();

        REQUIRE(TestStruct::instances == 0);
    }

    SECTION("ref") {
        UniquePtr<TestStruct> x(new TestStruct(123));
        REQUIRE(TestStruct::instances == 1);
        helperRef(x);
        REQUIRE(!!x == true);
        REQUIRE(TestStruct::instances == 1);
    }

    SECTION("move") {
        UniquePtr<TestStruct> x(new TestStruct(123));
        REQUIRE(TestStruct::instances == 1);
        helperMove(std::move(x));
        REQUIRE(!!x == false);
        REQUIRE(TestStruct::instances == 0);
    }

    SECTION("self-move") {
        UniquePtr<TestStruct> x(new TestStruct(123));
        REQUIRE(TestStruct::instances == 1);
        x = std::move(x);
        REQUIRE(!!x == true);
        REQUIRE(TestStruct::instances == 1);
        REQUIRE(x->value == 123);
    }

}

TEST_CASE("SharedPtr") {

    SECTION("empty") {
        SharedPtr<TestStruct> x;

        REQUIRE(x.get() == nullptr);
        REQUIRE(!!x == false);

        x.reset();

        REQUIRE(x.get() == nullptr);
        REQUIRE(!!x == false);
    }


    SECTION("RAII") {
        REQUIRE(TestStruct::instances == 0);

        {
            SharedPtr<TestStruct> x(new TestStruct(123));
            REQUIRE(TestStruct::instances == 1);
        }

        REQUIRE(TestStruct::instances == 0);
    }


    SECTION("release") {
        REQUIRE(TestStruct::instances == 0);
        auto s = new TestStruct(123);
        REQUIRE(TestStruct::instances == 1);
        SharedPtr<TestStruct> x(s);
        REQUIRE(TestStruct::instances == 1);
        x.release();
        REQUIRE(TestStruct::instances == 1);
        delete s;
    }

    SECTION("basic") {
        SharedPtr<TestStruct> x(new TestStruct(123));

        REQUIRE(x.get() != nullptr);
        REQUIRE(!!x == true);

        REQUIRE(x->value == 123);
        REQUIRE(x->instances == 1);

        x.reset();

        REQUIRE(TestStruct::instances == 0);
    }

    SECTION("Copy") {
        REQUIRE(TestStruct::instances == 0);
        SharedPtr<TestStruct> x(new TestStruct(123));
        SharedPtr<TestStruct> y(x);
        REQUIRE(TestStruct::instances == 1);
        REQUIRE(x->value == y->value);
        x->incr();
        y->incr();
        REQUIRE(x->value == y->value);
        x.reset();
        y.reset();
        REQUIRE(TestStruct::instances == 0);
    }
}