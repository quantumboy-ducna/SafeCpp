## API Reference

### Introduction

`Safe` provides a runtime library that tracks every heap-allocated instance of types that inherit `Safe::SafeContextBase` and offers recycling/repurpose mechanisms. It provides a mechanism that is similar to managed contexts in programming languages like C#, that is to handle memory management automatically. However, standard C++ doesn't have neither garbage collectors (for high performance) and reference counts for pointers and references (for preventing the CPU from crawling). The idea is to make sure any instance of any type that inherits from `Safe::SafeContextBase`, once allocated on memory heap, must be managed and all pointers to them are tracked. In this safe context, the default allocation is on heap-based memory, and memory allocation on stack is just an extension that bypasses the managed runtime for higher performance and flexibility. Without allowing stack-based allocation, a variable of a safe type can't be a member of another class or structure without defining them as a pointer type and reference type. C++ also doesn't overriding the default behaviors of raw pointers and references. A reference, once bound to a variable, can't be assigned to any other variable. Thus, stack-based allocation under safe context can lead to dangling pointers or dangling references.

The term "managed types" in this library can be understood as types that inherit from `Safe::SafeContextBase` and don't need to manage memory manually. Unlike other programming languages like C# have automatic garbage collection for reference types (the C# classes) which rely on reference counting, C++ doesn't have any language feature that allow reference counting for pointer(s) and reference(s). Smart pointer types implemented in C++ standard library may manage memory automatically. But if the smart pointers themselves are allocated on memory heap, they must be deallocated manually and thus potentially leak memory. So, polymorphic instance(s) of `Safe::SafeContextBase` allocated on memory heap will have neither dangling pointer nor dangling reference nor memory leak. Furthermore, the lifetime(s) of the instance(s) will be as long as possible to make sure the pointer(s) and the reference(s) are always valid in a program. This design will hold a lot of pressure on the memory if not used carefully.

You may ask if there is any way to handle the case where the memory usages of programs that use `Safe::SafeContextBase` are heavy? The only way to reclaim memory in this situation is to recycle instances that are no longer intented to be used. However, that will be confusing as you are holding a reference to an instance that has been recycled and after being repurposed, the underlying data of that instance changed dramatically. This is a trade-off for being safe, since unlike programming languages such as Rust, data in C++ is mutable by default.

`Safe` also has a feature that allows bulk allocation (memory arena) that quickly allocates multiple instances of the same type derived from `Safe::SafeContextBase`, for high performance purposes. It is the class template `Safe::SafeContextBase::SafeMemoryChunk` that is designed to inherit `Safe::SafeContextBase` and be capable of creating and managing memory chunk(s). The namespace also has several utility types that provide useful features under the safe context, like `SafeFunction` and `SafeEvent`.


### Definitions

- `CPP_SAFE_LIBRARY_TRADE` — an annotation macro that is used to export/import public symbols in dynamic linkages on Windows; expand the macro by defining one of the build flags:
	- `CPP_SAFE_DYNAMICALLY_LINKED_LIBRARY_EXPORT` (defined when building the dynamically linked library on Windows, expands to `__declspec(dllexport)`).
	- `CPP_SAFE_DYNAMICALLY_LINKED_LIBRARY_IMPORT` (defined when linking to the dynamically linked library on Windows, expands to `__declspec(dllimport)`).
	- `CPP_SAFE_STATICALLY_LINKED_LIBRARY_EXPORT` (defined when building the statically linked library on Windows, expands to nothing).
	- `CPP_SAFE_STATICALLY_LINKED_LIBRARY_IMPORT` (defined when linking to the statically linked library on Windows, expands to nothing).

- `safe` — a convenience macro defined in `SafeTrade.h` that expands to `: public Safe::SafeContextBase` to declare a derived type quickly; use with care as macros are not scoped and can affect readability.

- `WINDOWS`, `UNIX` — OS detection macros defined in `SafeNamespace.h` to simplify platform-specific code in the project.

- When building and consuming the dynamic libraries on Windows, ensure to use the same CRT runtime (`/MD` for Release mode or `/MDd` for Debug mode) as of the libraries and define the correct export/import macro.

> [!NOTE]
On Windows, in order to avoid the confusion between the `.lib` files that go with `.dll` in dynamic libraries, `Safe` will use `.sll` as the file extension for static libraries. Linking to the static libraries with `.sll` extension is fine like linking to `.lib` files in other projects.

### Features

The library consists of a namespace called `Safe`. Namespace `Safe` offers several classes for custom managed types and bulk allocation with memory chunks.

- [SafeContextBase](#safecontextbase) — the base type for managed instances; any type that inherits this class will be managed by `Safe` runtime library and doesn't manage memory manually.
- [SafeContextBase::SafeMemoryManager](#safecontextbasesafememorymanager) — internal type; a class that manages memory for `Safe` runtime library and is only provided with a forward declaration.
- [SafeContextBase::SafeMemoryChunk&lt;GenericTypeOfSafeContextDerivative&gt;](#safecontextbasesafememorychunkgenerictypeofsafecontextderivative) — a fixed-size class template that performs bulk arena allocations to gain higher performance.
- [SafeFunction](#safefunction) - a functional class that is managed and essentially wraps `std::function`.
- [SafeEvent](#safeevent) - an event class that provides event-driven mechanisms under the safe context.
- [SafeEventHandler&lt;GenericTypeOfSafeEvent&gt;](#safeeventhandlergenerictypeofsafeevent) - an event class that provides event-driven mechanisms under the safe context.
- [SafeString&lt;GenericTypeOfCharacter&gt;](#safestringgenerictypeofcharacter) - an immutable string class under the safe context.
- [SafeStringBuilder](#safestringbuilder) - an immutable string builder class under the safe context which is responsible for creating instance(s) of `SafeString`.
- [SafeContextException](#safecontextexception) - an exception class that is used for exception handling inside namespace `Safe`.

#### <a name="safecontextbase"></a> SafeContextBase

##### Declaration

```c++
class SafeContextBase;
```


##### Description

Declared and defined in `SafeContextBase.h`. The common base class type for any instance managed by the `Safe` runtime library. Inherit from this type to opt into tracking, recycling and chunk features.


##### Member Types

- `DefaultConstructionInvoker`
```c++
public:
	typedef void (*DefaultConstructionInvoker)(SafeContextBase* const instancePointer);
```

This functional pointer type acts as the function prototype for invoking the default constructor of any derivative of `SafeContextBase`.

- `CopyConstructionInvoker`
```c++
public:
	typedef void (*CopyConstructionInvoker)(SafeContextBase* const instancePointer,const SafeContextBase* const otherInstancePointer);
```

This functional pointer type acts as the function prototype for invoking the copy constructor of any derivative of `SafeContextBase`.


##### Methods

- `SafeContextBase()`
```c++
protected: 
	explicit SafeContextBase();
```

Default constructor of type `SafeContextBase`. It constructs a polymorphic instance of type `SafeContextBase` that are managed by `Safe` runtime library.

- `SafeContextBase(const bool& classification)`
```c++
private:
	explicit SafeContextBase(const bool& classification);
```

A specialized constructor of type `SafeContextBase`. It constructs a polymorphic instance of type `SafeContextBase` while bypassing the tracking classification.

- `SafeContextBase(const SafeContextBase& other)`
```c++
protected:
	explicit SafeContextBase(const SafeContextBase& other) noexcept(false);
```

Copy constructor of type `SafeContextBase`. It constructs a polymorphic instance of type `SafeContextBase` from another one.

- `SafeContextBase(SafeContextBase&& other)`
```c++
protected:
	explicit SafeContextBase(SafeContextBase&& other) noexcept(false);
```

Move constructor of type `SafeContextBase`. It constructs a polymorphic instance of type `SafeContextBase` from another one.

- `~SafeContextBase()`
```c++
protected:
	virtual ~SafeContextBase() noexcept(false);
```

Destructor of type `SafeContextBase`. It destructs a polymorphic instance of type `SafeContextBase`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually. It results in a defined behavior and enforces heap-only allocations for derived types.

- `operator new(std::size_t memorySize)`
```c++
protected:
	static void* operator new(std::size_t memorySize);
```

Overload of `operator new()`. It allocates a block of memory whose size provided by argument `memorySize`.

- `operator delete(void* const memoryPointer)`
```c++
protected:
	static void operator delete(void* const memoryPointer) noexcept;
```

Overload of `operator delete()`. It deallocates a block of memory whose pointer provided by argument `memoryPointer`.

- `operator new[](std::size_t memoryArraySize)`
```c++
private:
	static void* operator new[](std::size_t memoryArraySize);
```

Overload of `operator new[]()`. It allocates a memory array whose size provided by argument `memoryArraySize`.

- `operator delete[](void* const memoryArrayPointer)`
```c++
private:
	static void operator delete[](void* const memoryArrayPointer) noexcept;
```

Overload of `operator delete[]()`. It deallocates a memory array whose pointer provided by argument `memoryArrayPointer`.

- `operator=(const SafeContextBase& other)`
```c++
protected:
	SafeContextBase& operator=(const SafeContextBase& other) noexcept;
```

Overload of `operator=`, the copy assignment operator. It performs copy assignment from an instance of type `SafeContextBase` to another one.
	
- `operator=(SafeContextBase&& other)`
```c++
protected:
	SafeContextBase& operator=(SafeContextBase&& other) noexcept;
```

Overload of `operator=`, the move assignment operator. It performs move assignment from an instance of type `SafeContextBase` to another one.

- `operator==(const SafeContextBase& other)`
```c++
public:
	bool operator==(const SafeContextBase& other) const noexcept;
```

Overload of `operator==`, the equality comparison operator. It compares the current instance of type `SafeContextBase` to another one by reference equality.
	
- `constructInstanceOnMemoryHeap()`
```c++
public:
	static SafeContextBase& constructInstanceOnMemoryHeap();
```
	
This helper method allocates and constructs an instance of type `SafeContextBase` on memory heap.

- `destructInstanceOnMemoryHeap(SafeContextBase* const instancePointer)`
```c++
private:
	static void destructInstanceOnMemoryHeap(SafeContextBase* const instancePointer) noexcept;
```

This helper method destructs and deallocates an instance of type `SafeContextBase` on memory heap.

- `deallocateArrayOnMemoryHeap(SafeContextBase* const arrayPointer)`
```c++
private:
	static void deallocateArrayOnMemoryHeap(SafeContextBase* const arrayPointer) noexcept;
```

This helper method destructs and deallocates an array of instance(s) of type `SafeContextBase` on memory heap.

- `deallocateChunkOnMemoryHeap(SafeContextBase* const chunkPointer)`
```c++
private:
	static void deallocateChunkOnMemoryHeap(SafeContextBase* const chunkPointer) noexcept;
```

This helper method destructs and deallocates a memory chunk of instance(s) of type `SafeContextBase` on memory heap.

- `reconstructSafely(SafeContextBase* const instancePointer,const DefaultConstructionInvoker& constructionInvoker)`
```c++
private:
	static void reconstructSafely(SafeContextBase* const instancePointer,const DefaultConstructionInvoker& constructionInvoker);
````

This helper method reconstructs a polymorphic instance of type `SafeContextBase` on a pre-allocated memory address whose pointer is provided by argument `instancePointer`, using a default constructor provided by argument `constructionInvoker`.

- `helpInitializeChunk(std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality,const DefaultConstructionInvoker& constructionInvoker)`
```c++
private:
	static void helpInitializeChunk(std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality,const DefaultConstructionInvoker& constructionInvoker);
```

This helper method helps initialize a memory chunk of polymorphic instance(s).

- `helpDuplicateChunk(std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::vector<SafeContextBase*>& sourceElementPointers,const std::size_t& cardinality,const CopyConstructionInvoker& constructionInvoker)`
```c++
private:
	static void helpDuplicateChunk(std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::vector<SafeContextBase*>& sourceElementPointers,const std::size_t& cardinality,const CopyConstructionInvoker& constructionInvoker);
```

This helper method helps duplicate a memory chunk of polymorphic instance(s).

- `helpDisposeChunk(const std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality)`
```c++
private:
	static void helpDisposeChunk(const std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality);
```

This helper method helps dispose a memory chunk of polymorphic instance(s).

- `destroyDerivedChunkOnMemoryHeap(const std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality)`
```c++
private:
	static void destroyDerivedChunkOnMemoryHeap(const std::vector<SafeContextBase*>& chunkBufferElementPointers,const std::size_t& cardinality);
```

This helper method destroys a memory chunk of polymorphic instance(s).

- `upcycle(const std::type_info& typeInformation)`
```c++
private:
	static SafeContextBase* upcycle(const std::type_info& typeInformation);
```
	
This helper method upcycles a polymorphic instance of type `SafeContextBase`, deduced from polymorphic type information provided by argument `typeInformation`.

- `finalize(SafeContextBase*& pointer)`
```c++
private:
	static void finalize(SafeContextBase*& pointer) noexcept;
```

This helper method finalizes a pointer to an instance of type `SafeContextBase` by setting it to `nullptr`.

- `enableMultithreadedMode()`
```c++
public:
	static void enableMultithreadedMode() noexcept;
```

This method enables multi-threaded mode for the safe context. Once enabled, it can't be reverted to single-threaded mode. `SafeMemoryManager` will then be guarded by synchronization primitives to ensure thread-safety.

- `recycle(SafeContextBase& instanceReference)`
```c++
public:
	static void recycle(SafeContextBase& instanceReference);
```

This method recycles a polymorphic instance of type `SafeContextBase` provided by argument `instanceReference`. After calling this method, any access to the instance is not meaningful. The reference(s) to the recycled instance(s) will always be valid but can refer to some instance(s) that had been repurposed.

- `recycle(SafeContextBase* const instancePointer)`
```c++
public:
	static void recycle(SafeContextBase* const instancePointer);
```

This method recycles a polymorphic instance of type `SafeContextBase` provided by argument `instancePointer`. After calling this method, any access to the instance is not meaningful. The pointer(s) to the recycled instance(s) will always be valid but can refer to some instance(s) that had been repurposed.

- `repurpose()`
```c++
public:
	template<typename GenericTypeOfSafeContextDerivative> static inline GenericTypeOfSafeContextDerivative& repurpose();
```

This method template repurposes an instance of type `GenericTypeOfSafeContextDerivative` that has been recycled. `GenericTypeOfSafeContextDerivative` must be a type that inherits `SafeContextBase`. If no instance of such type has been recycled, it will construct an instance of `GenericTypeOfSafeContextDerivative` by the default constructor instead.

- `createDerivedChunkOnMemoryHeap(const std::size_t& chunkCardinality)`
```c++
public:
	template<typename GenericTypeOfSafeContextDerivative> static inline SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& createDerivedChunkOnMemoryHeap(const std::size_t& chunkCardinality);
```

This method template acts as a helper to create a memory chunk of instances of `GenericTypeOfSafeContextDerivative` whose cardinality is provided by argument `chunkCardinality`. `GenericTypeOfSafeContextDerivative` must be a type that inherits `SafeContextBase`. 

- `getMemorySize()`
```c++
public:
	virtual std::size_t getMemorySize() const noexcept final;
```

This method returns the actual memory size of the current polymorphic instance of type `SafeContextBase`.

- `getTypeInfo()`
```c++
public:
	const std::type_info& getTypeInfo() const noexcept;
```

This method returns the polymorphic type information of the current polymorphic instance of type `SafeContextBase`. The type information is defined by C++ standard library's `std::type_info`.

- `getHashCode()`
```c++
public:
	std::size_t getHashCode() const noexcept;
```
	
This method returns the hash code of the current polymorphic instance's type.
	

#### <a name="safecontextbasesafememorymanager"></a> SafeContextBase::SafeMemoryManager

##### Declaration

```c++
class SafeMemoryManager;
```


##### Description

An internal class that provides the core functionalities for `Safe` runtime library.


#### <a name="safecontextbasesafememorychunkgenerictypeofsafecontextderivative"></a> SafeContextBase::SafeMemoryChunk&lt;GenericTypeOfSafeContextDerivative&gt;

##### Declaration

```c++
template<typename GenericTypeOfSafeContextDerivative> class SafeContextBase::SafeMemoryChunk;
```


##### Description

Declared and defined in `SafeMemoryChunk.h`. A specialized class template for bulk allocation. This class template inherits `SafeContextBase` and manages multiple instance(s) of type `GenericTypeOfSafeContextDerivative` on a continguous block of memory. `GenericTypeOfSafeContextDerivative` must be a type that inherits `SafeContextBase`.



##### Member Types

- `SafeMemoryChunkElementMutator`
```c++
public:
	typedef void (*SafeMemoryChunkElementMutator)(GenericTypeOfSafeContextDerivative* const instancePointer);
```

This functional pointer type acts as the function prototype for invoking the default constructor of any derivative of `SafeContextBase`.


##### Type aliases

- `SafeVector`
```c++
	template<typename GenericTypeOfSafeContextDerivative> using SafeVector = SafeContextBase::SafeMemoryChunk<GenericTypeOfSafeContextDerivative>;
```

An alias that serves as a safe version of `std::vector`.


##### Methods

- `SafeMemoryChunk()`
```c++
public:
	inline constexpr explicit SafeMemoryChunk();
```

Default constructor of type `SafeMemoryChunk`. It constructs a generic instance of type `SafeMemoryChunk`. Each instance of `SafeMemoryChunk` is a memory chunk that has 10 elements.

- `SafeMemoryChunk(const std::size_t& cardinality)`
```c++
public:
	inline explicit SafeMemoryChunk(const std::size_t& cardinality)
```

Constructor of `SafeMemoryChunk`. It constructs a generic instance of type `SafeMemoryChunk`. Each instance of `SafeMemoryChunk` is a memory chunk that has a fixed cardinality provided by argument `cardinality`.

- `SafeMemoryChunk(const SafeMemoryChunk& other)`
```c++
public:
	inline SafeMemoryChunk(const SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& other);
```

Copy constructor of `SafeMemoryChunk`. It constructs a generic instance of type `SafeMemoryChunk` from another one with the same generic type argument.

- `SafeMemoryChunk(SafeMemoryChunk&&)`
```c++
private:
	inline SafeMemoryChunk(SafeMemoryChunk<GenericTypeOfSafeContextDerivative>&& other) noexcept(false);
```

Move constructor of `SafeMemoryChunk`. It constructs a generic instance of type `SafeMemoryChunk` from another one with the same generic type argument.

- `~SafeMemoryChunk()`
```c++
public:
	inline ~SafeMemoryChunk() noexcept(true) override;
```
	
Destructor of `SafeMemoryChunk`. It destructs a generic instance of type `SafeMemoryChunk`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually.

- `operator=(const SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& other)`
```c++
private:
	inline SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& operator=(const SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& other) = delete;
```

Overload of `operator=`, the copy assignment operator. The operator is disabled for `SafeMemoryChunk`.
	
- `operator=(SafeMemoryChunk<GenericTypeOfSafeContextDerivative>&& other)`
```c++
private:
	inline SafeMemoryChunk<GenericTypeOfSafeContextDerivative>& operator=(SafeMemoryChunk<GenericTypeOfSafeContextDerivative>&& other) = delete;
```

Overload of `operator=`, the move assignment operator. The operator is disabled for `SafeMemoryChunk`.

- `operator[](const std::size_t& index)`
```c++
public:
	inline GenericTypeOfSafeContextDerivative operator[](const std::size_t& index);
```

Overloads of `operator[]`, the indexer operators. They refer to the element instance(s) of type `GenericTypeOfSafeContextDerivative` in the current chunk by index provided by arguments `index`.

- `throwOutOfBoundException(const std::size_t& index,const std::size_t& cardinality) noexcept(false)`
```c++
public:
	[[noreturn]] static inline void throwOutOfBoundException(const std::size_t& index,const std::size_t& cardinality) noexcept(false);
```

This method throws an exception related to out-of-bound errors and doesn't return.

- `throwAccessViolationException(const std::size_t& index) noexcept(false)`
```c++
public:
	[[noreturn]] static inline void throwAccessViolationException(const std::size_t& index) noexcept(false)
```

This method throws an exception related to access violation errors and doesn't return.

- `getCardinality()`
```c++
public:
	inline std::size_t getCardinality() const noexcept
```

This method returns the cardinality of the current chunk instance.

- `contains(const GenericTypeOfSafeContextDerivative& element)`
```c++
public:
	inline bool contains(const GenericTypeOfSafeContextDerivative& element) const;
```

This method checks if the current instance of `SafeMemoryChunk` contains an element of type `GenericTypeOfSafeContextDerivative` specified by argument `element`.

- `mutate(const std::size_t& index,const SafeMemoryChunkElementMutator& mutator)`
```c++
public:
	inline void mutate(const std::size_t& index,const SafeMemoryChunkElementMutator& mutator);
```

This method acts as an alias for `mutateElementByIndex` that is capable of mutating chunk element(s) by index.

- `mutateElementByIndex(const std::size_t& index,const SafeMemoryChunkElementMutator& mutator)`
```c++
public:
	inline void mutateElementByIndex(const std::size_t& index,const SafeMemoryChunkElementMutator& mutator);
```

This method is capable of mutating chunk element(s) by index.

- `dispose()`
```c++
public:
	inline void dispose();
```

This method disposes the current chunk instance, makes all elements in the current chunk instance refer to a defaulted instance. After calling this method, any access to the elements of the current chunk instance is not meaningful. Every element on that memory chunk will refer to two defaulted instances instead, one is immutable and one is mutable.


#### <a name="safefunction"></a> SafeFunction

##### Declaration

```c++
template<typename GenericType> class SafeFunction;
template<typename GenericTypeOfReturn,typename ...GenericTypesOfArguments> class SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>;
```


##### Description

Declared and defined in `SafeFunction.h`. A functional class type that inherits `SafeContextBase` and wraps `std::function` under the safe context.


##### Methods

- `SafeFunction()`
```c++
public:
	inline explicit SafeFunction();
```

Default constructor of type `SafeFunction`. It constructs a generic instance of `SafeFunction`.

- `SafeFunction(const GenericTypeOfFunction& functionallyInvocable)`
```c++
public:
	template<class GenericTypeOfFunction> inline SafeFunction(const GenericTypeOfFunction& functionallyInvocable);
```

A specialized constructor of `SafeFunction`. It constructs a generic instance of `SafeFunction` from a functional argument `functionallyInvocable`.

- `SafeFunction(const std::function<GenericTypeOfReturn(GenericTypesOfArguments...)>& function)`
```c++
public:
	inline SafeFunction(const std::function<GenericTypeOfReturn(GenericTypesOfArguments...)>& function);
```

A specialized constructor of `SafeFunction`. It constructs a generic instance of `SafeFunction` from a functional argument `function`.

- `SafeFunction(const SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>& other)`
```c++
public:
	inline SafeFunction(const SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>& other);
```
	
Copy constructor of type `SafeFunction`. It constructs a generic instance of type `SafeFunction` from another one with the same generic type argument.

- `SafeFunction(GenericTypeOfFunction&& functionallyInvocable)`
```c++
public:
	template<class GenericTypeOfFunction> inline SafeFunction(GenericTypeOfFunction&& functionallyInvocable);
```

A specialized move constructor of `SafeFunction`. It constructs a generic instance of `SafeFunction` from a functional argument `functionallyInvocable`.

- `SafeFunction(std::function<GenericTypeOfReturn(GenericTypesOfArguments...)>&& function)`
```c++
public:
	inline SafeFunction(std::function<GenericTypeOfReturn(GenericTypesOfArguments...)>&& function);
```

A specialized move constructor of `SafeFunction`. It constructs a generic instance of `SafeFunction` from a functional argument `function`.

- `SafeFunction(SafeFunction&& other)`
```c++
public:
	inline SafeFunction(SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>&& other) noexcept(false);
```

Move constructor of type `SafeFunction`. It constructs a generic instance of type `SafeFunction` from another one with the same generic type argument.

- `~SafeFunction()`
```c++
protected:
	inline virtual ~SafeFunction() noexcept(false) override
```

Destructor of type `SafeFunction`. It destructs a generic instance of type `SafeFunction`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually.

- `operator=(const SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>& other)`
```c++
public:
	inline SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>& operator=(const SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>& other);
```
	
Overload of `operator=`, the copy assignment operator. It performs copy assignment from a generic instance of type `SafeFunction` to another one.
	
- `operator=(SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>&& other)`
```c++
public:
	inline SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>& operator=(SafeFunction<GenericTypeOfReturn(GenericTypesOfArguments...)>&& other) noexcept(false);
```
	
Overload of `operator=`, the move assignment operator. It performs move assignment from a generic instance of type `SafeFunction` to another one.

- `operator()(GenericTypesOfArguments... arguments)`
```c++
public:
	inline GenericTypeOfReturn operator()(GenericTypesOfArguments... arguments) const;
```

Overload of `operator()`, the functional invocation operator. It invokes the current instance of `SafeFunction` as a functor.

- `checkInvocability()`
```c++
private:
	bool checkInvocability() const noexcept;
```

This method checks if the current instance of `SafeFunction` is invocable.

- `isInvocable()`
```c++
public:
	bool isInvocable() const noexcept;
```

This method shows if the current instance of `SafeFunction` is invocable.

#### <a name="safeevent"></a> SafeEvent

##### Declaration

```c++
class SafeEvent;
```


##### Description

Declared and defined in `SafeEvent.h`. An event class that inherits `SafeContextBase` and provides base event-driven mechanisms under the safe context.


##### Member Types

- `SafeEventOccurrence`

	An alias for `SafeFunction<void()>`. It acts like an event-driven functional type.


##### Methods

- `SafeEvent()`
```c++
public:
	explicit SafeEvent();
```

Default constructor of type `SafeEvent`. It constructs an instance of `SafeEvent`.

- `SafeEvent(const SafeEventOccurrence& occurrence)`
```c++
public:
	explicit SafeEvent(const SafeEventOccurrence& occurrence);
```

A specialized constructor of `SafeEvent`. It constructs an instance of `SafeEvent` and specifies how the event occurs by argument `occurrence`.

- `SafeEvent(const SafeEvent& other)`
```c++
public:
	SafeEvent(const SafeEvent& other);
```
	
Copy constructor of type `SafeEvent`. It constructs an instance of type `SafeEvent` from another one.

- `SafeEvent(SafeEventOccurrence&& occurrence)`
```c++
public:
	explicit SafeEvent(SafeEventOccurrence&& occurrence) noexcept(false);
```

A specialized move constructor of `SafeEvent`. It constructs an instance of `SafeEvent` and specifies how the event occurs by argument `occurrence`.

- `SafeEvent(SafeEvent&& other)`
```c++
public:
	SafeEvent(SafeEvent&& other) noexcept(false);
```

Move constructor of type `SafeEvent`. It constructs an instance of type `SafeEvent` from another one.

- `~SafeEvent()`
```c++
protected:
	virtual ~SafeEvent() noexcept(false) override;
```

Destructor of type `SafeEvent`. It destructs an instance of type `SafeEvent`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually.

- `operator=(const SafeEvent& other)`
```c++
public:
	SafeEvent& operator=(const SafeEvent& other);
```
	
Overload of `operator=`, the copy assignment operator. It performs copy assignment from an instance of type `SafeEvent` to another one.
	
- `operator=(SafeEvent&& other)`
```c++
public:
	SafeEvent& operator=(SafeEvent&& other) noexcept(false);
```
	
Overload of `operator=`, the move assignment operator. It performs move assignment from an instance of type `SafeEvent` to another one.

- `getCancellation()`
```c++
public:
	bool getCancellation() const noexcept;
```

This method returns the cancellation state of the current instance of `SafeEvent`.

- `setCancellation(const bool& cancellation)`
```c++
public:
	void setCancellation(const bool& cancellation) noexcept;
```

This method assigns the cancellation state of the current instance of `SafeEvent`.

- `setCancellation(bool&& cancellation)`
```c++
public:
	void setCancellation(bool&& cancellation) noexcept;
```

This method assigns the cancellation state of the current instance of `SafeEvent`.

- `getOccurrence()`
```c++
public:
	SafeEventOccurrence getOccurrence() const;
```

This method returns the event-driven function of the current instance of `SafeEvent`.

- `setCancellation(const bool& cancellation)`
```c++
public:
	void setOccurrence(const SafeEventOccurrence& occurrence);
```

This method assigns the event-driven function of the current instance of `SafeEvent`.

- `setCancellation(bool&& cancellation)`
```c++
public:
	void setOccurrence(SafeEventOccurrence&& occurrence) noexcept;
```

This method assigns the event-driven function of the current instance of `SafeEvent`.

- `broadcast()`
```c++
public:
	void broadcast();
```

This method broadcasts the current event and triggers the underlying event-driven function.

- `cancel()`
```c++
public:
	void cancel() noexcept;
```

This method cancels the current event and halts the underlying event-driven function.

- `raise()`
```c++
public:
	void raise() noexcept;
```

This method raises the current event if the event has been cancelled.

- `abrogate()`
```c++
public:
	void abrogate() noexcept;
```

This method abrogates the current event if the event is broadcasting.


#### <a name="safeeventhandlergenerictypeofsafeevent"></a> SafeEventHandler&lt;GenericTypeOfSafeEvent&gt;

##### Declaration

```c++
template<typename GenericTypeOfSafeEvent> class SafeEventHandler;
```


##### Description

Declared and defined in `SafeEvent.h`. An event handler class that inherits `SafeContextBase`, wraps `SafeFunction<void(const GenericTypeOfSafeEvent&)>` and provides base event-driven handling mechanisms under the safe context.


##### Member Types

- `SafeEventHandle`
```c++
public:
	typedef SafeFunction<void()> SafeEventOccurrence;
```

This functional type acts like an event-driven handler.


##### Methods

- `SafeEventHandler()`
```c++
public:
	inline explicit SafeEventHandler();
```

Default constructor of type `SafeEventHandler`. It constructs a generic instance of `SafeEventHandler`.

- `SafeEventHandler(const std::function<void(const GenericTypeOfSafeEvent&)>& eventHandle)`
```c++
public:
	inline explicit SafeEventHandler(const std::function<void(const GenericTypeOfSafeEvent&)>& eventHandle);
```

A specialized constructor of `SafeEventHandler`. It constructs a generic instance of `SafeEventHandler` from argument `eventHandle`.

- `SafeEventHandler(const SafeEventHandler<GenericTypeOfSafeEvent>& other)`
```c++
public:
	inline SafeEventHandler(const SafeEventHandler<GenericTypeOfSafeEvent>& other) noexcept(false);
```
	
Copy constructor of type `SafeEventHandler`. It constructs a generic instance of type `SafeEventHandler` from another one.

- `SafeEventHandler(std::function<void(const GenericTypeOfSafeEvent&)>&& eventHandle)`
```c++
public:
	inline explicit SafeEventHandler(std::function<void(const GenericTypeOfSafeEvent&)>&& eventHandle);
```

A specialized move constructor of `SafeEventHandler`. It constructs a generic instance of `SafeEventHandler` from argument `eventHandle`.

- `SafeEventHandler(SafeEventHandler<GenericTypeOfSafeEvent>&& other)`
```c++
public:
	inline SafeEventHandler(SafeEventHandler<GenericTypeOfSafeEvent>&& other) noexcept(false);
```

Move constructor of type `SafeEventHandler`. It constructs a generic instance of type `SafeEventHandler` from another one.

- `~SafeEventHandler()`
```c++
protected:
	inline virtual ~SafeEventHandler() noexcept(false) override;
```

Destructor of type `SafeEventHandler`. It destructs a generic instance of type `SafeEventHandler`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually.

- `operator=(const SafeEventHandler& other)`
```c++
public:
	inline SafeEventHandler<GenericTypeOfSafeEvent>& operator=(const SafeEventHandler<GenericTypeOfSafeEvent>& other);
```
	
Overload of `operator=`, the copy assignment operator. It performs copy assignment from a generic instance of type `SafeEventHandler` to another one.
	
- `operator=(SafeEventHandler&& other)`
```c++
public:
	inline SafeEventHandler<GenericTypeOfSafeEvent>& operator=(SafeEventHandler<GenericTypeOfSafeEvent>&& other) noexcept(false);
```
	
Overload of `operator=`, the move assignment operator. It performs move assignment from a generic instance of type `SafeEventHandler` to another one.

- `handle(const GenericTypeOfSafeEvent& event)`
```c++
public:
	inline void handle(const GenericTypeOfSafeEvent& event);
```

This method handles an event provided by argument `event`.

- `handle(const GenericTypeOfSafeEvent* const eventPointer)`
```c++
public:
	inline void handle(const GenericTypeOfSafeEvent* const eventPointer);
```

This method handles an event provided by argument `eventPointer`.


#### <a name="safestringgenerictypeofcharacter"></a> SafeString&lt;GenericTypeOfCharacter&gt;

##### Declaration

```c++
template<typename GenericTypeOfCharacter> class SafeString;
```


##### Description

Declared and defined in `SafeString.h`. An immutable string class template that inherits `SafeContextBase`.


##### Member Types

- `SafeStringBuilder` - a safe class that is used for producing instance(s) of `SafeString`.

	
##### Methods

- `SafeString()`
```c++
public:
	explicit SafeString();
```

Default constructor of `SafeString`. It constructs a generic instance of type `SafeString`.


- `SafeString(const GenericTypeOfCharacter* const characterPointer)`
```c++
public:
	SafeString(const GenericTypeOfCharacter* const characterPointer);
```

Constructor of `SafeString`. It constructs a generic instance of type `SafeString` from a character pointer of type `const GenericTypeOfCharacter* const`.

- `SafeString(const GenericTypeOfCharacter* const characterPointer,const std::size_t& length)`
```c++
public:
	SafeString(const GenericTypeOfCharacter* const characterPointer,const std::size_t& length);
```

Constructor of `SafeString`. It constructs a generic instance of type `SafeString` from a character pointer of type `const GenericTypeOfCharacter* const` and a specified length that is the length of the underlying character sequence.

- `SafeString(const std::basic_string<GenericTypeOfCharacter>& primitiveString)`
```c++
public:
	SafeString(const std::basic_string<GenericTypeOfCharacter>& primitiveString);
```

Constructor of `SafeString`. It constructs a generic instance of type `SafeString` from a primitive string instance of type `std::basic_string`.

- `SafeString(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	SafeString(const SafeString<GenericTypeOfCharacter>& other);
```

Copy constructor of type `SafeString`. It constructs a generic instance of type `SafeString` from another one.

- `SafeString(std::basic_string<GenericTypeOfCharacter>&& primitiveString)`
```c++
public:
	SafeString(std::basic_string<GenericTypeOfCharacter>&& primitiveString);
```

Move constructor of `SafeString`. It constructs a generic instance of type `SafeString` from a primitive string instance of type `std::basic_string`.

- `SafeString(SafeString<GenericTypeOfCharacter>&& other)`
```c++
public:
	SafeString(SafeString<GenericTypeOfCharacter>&& other) noexcept(false);
```

Move constructor of type `SafeString`. It constructs a generic instance of type `SafeString` from another one.

- `~SafeString()`
```c++
public:
	virtual ~SafeString() noexcept(false);
```

Destructor of type `SafeString`. It destructs a generic instance of type `SafeString`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually.

- `operator=(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	SafeString<GenericTypeOfCharacter>& operator=(const SafeString<GenericTypeOfCharacter>& other);
```

Overload of `operator=`, the copy assignment operator. It performs copy assignment from a generic instance of type `SafeString` to another one.

- `operator=(SafeString<GenericTypeOfCharacter>&& other) noexcept(false)`
```c++
public:
	SafeString<GenericTypeOfCharacter>& operator=(SafeString<GenericTypeOfCharacter>&& other) noexcept(false);
```

Overload of `operator=`, the move assignment operator. It performs move assignment from a generic instance of type `SafeString` to another one.

- `operator==(const std::basic_string<GenericTypeOfCharacter>& data)`
```c++
public:
	bool operator==(const std::basic_string<GenericTypeOfCharacter>& data) const noexcept;
```

Overload of `operator==`, the equality comparison operator. It compares the current instance of type `SafeString` to an instance of `std::basic_string<GenericTypeOfCharacter>` provided by argument `data`.

- `operator==(const std::basic_string_view<GenericTypeOfCharacter>& dataView)`
```c++
public:
	bool operator==(const std::basic_string_view<GenericTypeOfCharacter>& dataView) const noexcept;
```

Overload of `operator==`, the equality comparison operator. It compares the current instance of type `SafeString` to an instance of `std::basic_string_view<GenericTypeOfCharacter>` provided by argument `dataView`.

- `operator==(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	bool operator==(const SafeString<GenericTypeOfCharacter>& other) const noexcept;
```

Overload of `operator==`, the equality comparison operator. It compares the current instance of type `SafeString` to another one provided by argument `other`.

- `operator+(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	SafeString<GenericTypeOfCharacter>& operator+(const SafeString<GenericTypeOfCharacter>& other);
```

Overload of `operator+`, the addition operator. It produces a generic instance of `SafeString` from the addition operation by concatenating another instance of `SafeString` to the current instance.

- `operator-(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	SafeString<GenericTypeOfCharacter>& operator-(const SafeString<GenericTypeOfCharacter>& other);
```

Overload of `operator-`, the subtraction operator. It produces a generic instance of `SafeString` from the addition operation by removing another instance of `SafeString` from the current instance.

- `operator[](const std::size_t& index)`
```c++
public:
	const GenericTypeOfCharacter operator[](const std::size_t& index) const;
```

Overload of `operator[]`, the indexer operator. It returns the elemental character in the current instance of `SafeString` based on an index provided by argument `index`.

- `getLength()`
```c++
public:
	std::size_t getLength() const noexcept;
```

This method returns the string length of the current instance of `SafeString`.

- `at(const std::size_t& index)`
```c++
public:
	GenericTypeOfCharacter at(const std::size_t& index) const;
```

This method returns the character value by index in the current instance of `SafeString`.

- `contains(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	bool contains(const SafeString<GenericTypeOfCharacter>& other) const;
```

This method checks if the current instance containing another instance of `SafeString` provided by argument `other`.

- `compareSubstring(const size_t& startingIndex,const size_t& length,const std::basic_string<GenericTypeOfCharacter>& data)`
```c++
public:
	bool compareSubstring(const size_t& startingIndex,const size_t& length,const std::basic_string<GenericTypeOfCharacter>& data) const;
```

This method compares a sub-string of the current instance of `SafeString` defined by arguments `startingIndex` and `length`, to a generic instance of `std::basic_string` specified by argument `data`.

- `compareSubstring(const std::size_t& startingIndex,const std::size_t& length,const std::basic_string_view<GenericTypeOfCharacter>& dataView)`
```c++
public:
	bool compareSubstring(const std::size_t& startingIndex,const std::size_t& length,const std::basic_string_view<GenericTypeOfCharacter>& dataView) const;
```

This method compares a sub-string of the current instance of `SafeString` defined by arguments `startingIndex` and `length`, to a generic instance of `std::basic_string_view` specified by argument `dataView`.

- `compareSubstring(const std::size_t& startingIndex,const std::size_t& length,const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	bool compareSubstring(const std::size_t& startingIndex,const std::size_t& length,const SafeString<GenericTypeOfCharacter>& other) const;
```

This method compares a sub-string of the current instance of `SafeString` defined by arguments `startingIndex` and `length`, to another instance of `SafeString` specified by argument `other`.

- `substr(const std::size_t& startingIndex,const std::size_t& count)`
```c++
public:
	SafeString<GenericTypeOfCharacter> substr(const std::size_t& startingIndex,const std::size_t& count) const;
```

This method returns a sub-string of the current instance of `SafeString` defined by arguments `startingIndex` and `length`.

- `getFirstIndexOfCharacter(const GenericTypeOfCharacter character)`
```c++
public:
	std::size_t getFirstIndexOfCharacter(const GenericTypeOfCharacter character) const noexcept;
```

This method returns the first index encountered of the character value provided by argument `character` in the current instance of `SafeString`.

- `getLastIndexOfCharacter(const GenericTypeOfCharacter character)`
```c++
public:
	std::size_t getLastIndexOfCharacter(const GenericTypeOfCharacter character) const noexcept;
```

This method returns the last index encountered of the character value provided by argument `character` in the current instance of `SafeString`.

- `findFirst(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	std::size_t findFirst(const SafeString<GenericTypeOfCharacter>& other) const noexcept;
```

This method returns the first index encountered of the instance of `SafeString` provided by argument `other` in the current instance of `SafeString`.

- `findLast(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	std::size_t findLast(const SafeString<GenericTypeOfCharacter>& other) const noexcept;
```

This method returns the last index encountered of the instance of `SafeString` provided by argument `other` in the current instance of `SafeString`.

- `obtainPrimitiveString()`
```c++
public:
	std::basic_string<GenericTypeOfCharacter> obtainPrimitiveString() const;
```

This method converts the current instance of `SafeString` to a generic instance of primitive string type `std::basic_string`.

- `obtainPrimitiveStringView()`
```c++
public:
	std::basic_string_view<GenericTypeOfCharacter> obtainPrimitiveStringView() const noexcept;
```

This method returns a view to the current instance of `SafeString` by a generic instance of primitive string view type `std::basic_string_view`.


#### <a name="safestringbuilder"></a> SafeStringBuilder

##### Declaration

```c++
template<typename GenericTypeOfCharacter> class SafeString<GenericTypeOfCharacter>::SafeStringBuilder;
```


##### Description

Declared and defined in `SafeString.h`. A string builder class that inherits `SafeContextBase` and is used for creating instance(s) of `SafeString`.


	
##### Methods

- `SafeStringBuilder()`
```c++
public:
	explicit SafeStringBuilder();
```

Default constructor of `SafeStringBuilder`. It constructs an instance of type `SafeStringBuilder`.


- `SafeStringBuilder(const std::basic_string<GenericTypeOfCharacter>& primitiveString)`
```c++
public:
	explicit SafeStringBuilder(const std::basic_string<GenericTypeOfCharacter>& primitiveString);
```

Constructor of `SafeStringBuilder`. It constructs an instance of type `SafeStringBuilder` from a primitive string instance of type `std::basic_string`.

- `SafeStringBuilder(const SafeStringBuilder<GenericTypeOfCharacter>& other)`
```c++
public:
	SafeStringBuilder(const SafeStringBuilder<GenericTypeOfCharacter>& other);
```

Copy constructor of type `SafeStringBuilder`. It constructs an instance of type `SafeStringBuilder` from another one.

- `SafeStringBuilder(std::basic_string<GenericTypeOfCharacter>&& primitiveString)`
```c++
public:
	explicit SafeStringBuilder(std::basic_string<GenericTypeOfCharacter>&& primitiveString);
```

Move constructor of `SafeStringBuilder`. It constructs an instance of type `SafeStringBuilder` from a primitive string instance of type `std::basic_string`.

- `SafeStringBuilder(SafeStringBuilder&& other)`
```c++
public:
	SafeStringBuilder(SafeStringBuilder&& other) noexcept(false);
```

Move constructor of type `SafeStringBuilder`. It constructs an instance of type `SafeStringBuilder` from another one.

- `~SafeStringBuilder()`
```c++
public:
	virtual ~SafeStringBuilder() noexcept(false);
```

Destructor of type `SafeStringBuilder`. It destructs an instance of type `SafeStringBuilder`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually.

- `operator=(const SafeStringBuilder& other)`
```c++
public:
	SafeStringBuilder& operator=(const SafeStringBuilder& other);
```

Overload of `operator=`, the copy assignment operator. It performs copy assignment from an instance of type `SafeStringBuilder` to another one.

- `operator=(SafeStringBuilder&& other)`
```c++
public:
	SafeStringBuilder& operator=(SafeStringBuilder&& other) noexcept(false);
```

Overload of `operator=`, the move assignment operator. It performs move assignment from an instance of type `SafeStringBuilder` to another one.

- `operator==(const SafeString<GenericTypeOfCharacter>& other)`
```c++
public:
	bool operator==(const SafeStringBuilder& other) const noexcept;
```

Overload of `operator==`, the equality comparison operator. It compares the current instance of type `SafeStringBuilder` to another one provided by argument `other`.

- `operator[](const std::size_t& index)`
```c++
public:
	GenericTypeOfCharacter& operator[](const std::size_t& index);
```

Overload of `operator[]`, the indexer operator. It returns the elemental character in the current instance of `SafeStringBuilder` based on an index provided by argument `index`.

- `getLength()`
```c++
public:
	std::size_t getLength() const noexcept;
```

This method returns the current length of the current instance of `SafeStringBuilder`.

- `append(const GenericTypeOfCharacter* const primitiveDataPointer)`
```c++
public:
	void append(const GenericTypeOfCharacter* const primitiveDataPointer);
```

This method appends a character sequence specified by argument `primitiveDataPointer` to the current instance of `SafeStringBuilder`.

- `append(const std::basic_string<GenericTypeOfCharacter>& primitiveData)`
```c++
public:
	void append(const std::basic_string<GenericTypeOfCharacter>& primitiveData);
```

This method appends a primitive string specified by argument `primitiveData` to the current instance of `SafeStringBuilder`.

- `append(const GenericTypeOfCharacter character)`
```c++
public:
	void append(const GenericTypeOfCharacter character);
```

This method appends a character specified by argument `character` to the current instance of `SafeStringBuilder`.

- `insert(const std::size_t& index,const GenericTypeOfCharacter character)`
```c++
public:
	void insert(const std::size_t& index,const GenericTypeOfCharacter character);
```

This method inserts a character specified by argument `character` to the current instance of `SafeStringBuilder` at a specified position provided by argument `index`.

- `insert(const std::size_t& index,const GenericTypeOfCharacter* const dataPointer)`
```c++
public:
	void insert(const std::size_t& index,const GenericTypeOfCharacter* const dataPointer);
```

This method inserts a character sequence specified by argument `dataPointer` to the current instance of `SafeStringBuilder` at a specified position provided by argument `index`.

- `insert(const std::size_t& index,const std::basic_string<GenericTypeOfCharacter>& data)`
```c++
public:
	void insert(const std::size_t& index,const std::basic_string<GenericTypeOfCharacter>& data);
```

This method inserts a primitive string specified by argument `data` to the current instance of `SafeStringBuilder` at a specified position provided by argument `index`.

- `remove(const std::size_t& index,const std::size_t& count)`
```c++
public:
	void remove(const std::size_t& index,const std::size_t& count);
```

This method removes a number of character(s) specified by argument `count` in the current instance of `SafeStringBuilder`, starting from a position provided by argument `index`.

- `remove(const GenericTypeOfCharacter* const dataPointer)`
```c++
public:
	void remove(const GenericTypeOfCharacter* const dataPointer);
```

This method removes a part of data whose value is equal to the argument `dataPointer` in the current instance of `SafeStringBuilder`.

- `remove(const std::basic_string<GenericTypeOfCharacter>& data)`
```c++
public:
	void remove(const std::basic_string<GenericTypeOfCharacter>& data);
```

This method removes a part of data whose value is equal to the argument `data` in the current instance of `SafeStringBuilder`.

- `remove(const SafeString<GenericTypeOfCharacter>& data)`
```c++
public:
	void remove(const SafeString<GenericTypeOfCharacter>& data);
```

This method removes a part of data whose value is equal to the argument `data` in the current instance of `SafeStringBuilder`.

- `replace(const std::size_t& position,const std::size_t& count,const std::basic_string<GenericTypeOfCharacter>& data)`
```c++
public:
	void replace(const std::size_t& position,const std::size_t& count,const std::basic_string<GenericTypeOfCharacter>& data);
```

This method replaces a part of data whose value is equal to the argument `data` in the current instance of `SafeStringBuilder`, starting from position specified by argument `position` and having the length provided by argument `count`.

- `clear()`
```c++
public:
	void clear() noexcept;
```

This method clears the underlying data buffer of the current instance of `SafeStringBuilder`, making it an empty string buffer.

- `reserve(const std::size_t& cardinality)`
```c++
public:
	void reserve(const std::size_t& cardinality);
```

This method reserves a size of memory for the current instance of `SafeStringBuilder`, specified by argument `cardinality`.

- `toString()`
```c++
public:
	SafeString<GenericTypeOfCharacter> toString() noexcept;
```

This method converts the current instance of `SafeStringBuilder` to an instance of `SafeString` by performing a move semantics from the underlying buffer to the target.


#### <a name="safecontextexception"></a> SafeContextException

##### Declaration

```c++
class SafeContextException;
```


##### Description

Declared and defined in `SafeContextException.h`. An exception class that provides basic exception handling mechanisms merely for `Safe` runtime library. This class inherits `SafeContextBase` and `std::exception`.


##### Methods

- `SafeContextException()`
```c++
public:
	explicit SafeContextException();
```

Default constructor of type `SafeContextException`. It constructs an instance of `SafeContextException`.

- `SafeContextException(const std::string& message)`
```c++
public:
	explicit SafeContextException(const std::string& message);
```

A specialized constructor of `SafeContextException`. It constructs an instance of `SafeContextException` with a custom message provided by argument `message`.

- `SafeContextException(const SafeContextException& other)`
```c++
public:
	SafeContextException(const SafeContextException& other);
```
	
Copy constructor of type `SafeContextException`. It constructs an instance of type `SafeContextException` from another one.

- `SafeContextException(std::string&& message)`
```c++
public:
	explicit SafeContextException(std::string&& message);
```

A specialized move constructor of `SafeContextException`. It constructs an instance of `SafeContextException` with a custom message provided by argument `message`.

- `SafeContextException(SafeContextException&& other)`
```c++
public:
	SafeContextException(SafeContextException&& other) noexcept(false);
```

Move constructor of type `SafeContextException`. It constructs an instance of type `SafeContextException` from another one.

- `~SafeContextException()`
```c++
protected:
	virtual ~SafeContextException() noexcept override;
```

Destructor of type `SafeContextException`. It destructs an instance of type `SafeContextException`. This destructor will always throw an exception and eventually terminate the program if trying to invoke manually.

- `operator=(const SafeContextException& other)`
```c++
public:
	SafeContextException& operator=(const SafeContextException& other);
```
	
Overload of `operator=`, the copy assignment operator. It performs copy assignment from an instance of type `SafeContextException` to another one.
	
- `operator=(SafeContextException&& other)`
```c++
public:
	SafeContextException& operator=(SafeContextException&& other) noexcept;
```
	
Overload of `operator=`, the move assignment operator. It performs move assignment from an instance of type `SafeContextException` to another one.

- `operator==(const SafeContextException& other)`
```c++
public:
	bool operator==(const SafeContextException& other) const noexcept;
```

Overload of `operator==`, the equality comparison operator. It compares the current instance of type `SafeContextException` to another one in terms of their exception messages.

- `what()`
```c++
public:
	const char* what() const noexcept override;
```

Overridden from `std::exception`. This method returns the raw exception message of the current instance of type `SafeContextException`.

- `getMessage()`
```c++
public:
	std::string getMessage() const;
```

This method returns the exception message of the current instance of type `SafeContextException`.

- `setMessage(const std::string& message)`
```c++
public:
	void setMessage(const std::string& message);
```

This method assigns the exception message provided by argument `message` to the current instance of type `SafeContextException`.
	
- `setMessage(std::string&& message)`
```c++
public:
	void setMessage(std::string&& message) noexcept;
```

This method assigns the exception message provided by argument `message` to the current instance of type `SafeContextException`.

For detailed semantics, examples and usage patterns see [Guide](Guide.md).


### Legal & Licensing Information

Required Notice: Copyright©2026, Nguyễn Anh Đức (workofduc@gmail.com)

`SafeCpp` is dual-licensed to accommodate both open-source development and proprietary commercial use:

* **Open-Source Track (GPLv3):** This project is licensed under the **GNU General Public License v3**. It is 100% free and open for personal study, academic research, and open-source projects. In accordance with the GPLv3, any software that integrates this library under this track must also have its full source code made public.
* **Commercial Track:** For commercial entities wishing to embed `SafeCpp` into proprietary, closed-source software, a separate commercial license is required. This grants you the legal right to use the library without being bound by the GPLv3 copyleft requirements.

For commercial licensing inquiries, pricing, or to obtain a proprietary license agreement, please contact: workofduc@gmail.com.