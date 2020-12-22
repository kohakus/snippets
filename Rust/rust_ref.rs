// Reference Rules for a variable
// a). At any given time, you can have either (but not both of) one mutable reference or any number of immutable references.
// b). References must always be valid.

// First we define a simple struct
struct Foo<T> {
    data: T,
}

impl<T> Foo<T> {
    fn new(data: T) -> Self {
        Foo {
            data,
        }
    }
}


// [Test1]
// A variable cannot have its mutable reference and immutable reference at the same time.
fn test1() {
    let mut foo = Foo::new("hello");

    let _x = &mut foo;
    let _y = &foo;

    println!("{}", _x.data);
    println!("{}", _y.data);
}
// Test1 tells:
// error[E0502]: cannot borrow `foo` as immutable because it is also borrowed as mutable.


// [Test2]
// We can use mutable and immutable references at the same time as long as they don't point to a same variable.
fn test2() {
    let mut foo = Foo::new("hello");
    let mut bar = Foo::new("world");

    let _x = &mut foo;
    let _y = &bar;

    println!("{}", _x.data);
    println!("{}", _y.data);
}
// Test2 tells:
// compiled successfully without error.


// [Test3]
// A variable can have its multiple immutable references at the same time.
fn test3() {
    let mut foo = Foo::new("hello");

    let _x = &foo;
    let _y = &foo;

    println!("{}", _x.data);
    println!("{}", _y.data);
}
// Test3 tells:
// compiled successfully without error.


// [Test4]
// To prevent data conflict, a variable cannot have its multiple mutable references at the same time.
fn test4() {
    let mut foo = Foo::new("hello");

    let _x = &mut foo;
    let _y = &mut foo;

    println!("{}", _x.data);
    println!("{}", _y.data);
}
// Test4 tells:
// error[E0499]: cannot borrow `foo` as mutable more than once at a time.


// [Test5]
// We can use multiple mutable references at the same time as long as they don't point to a same variable.
fn test5() {
    let mut foo = Foo::new("hello");
    let mut bar = Foo::new("world");

    let _x = &mut foo;
    let _y = &mut bar;

    println!("{}", _x.data);
    println!("{}", _y.data);
}
// Test5 tells:
// compiled successfully without error.


// [Test6] (additional test)
// We can just define unsafe references and get a passed compile, if we don't use them at all.
// This may probably benefiting from non-lexical lifetimes.
fn test6() {
    let mut foo = Foo::new("hello");

    let _x = &mut foo;
    let _y = &mut foo;
    let _z = &foo;

    println!("{}", &foo.data);
}
// Test6 tells:
// compiled successfully without error.