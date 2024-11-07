# handle
```cpp
class network::handle
```
A class that serves as a wrapper around the humble Unix file descriptor, mainly managing its lifetime.

## Associated types
- `Sharedhandle`
A reference to a handle of which the ownership is shared amongst several scopes. This type is mainly used in the context of the `poller`.

## Member types
- `exception`	exception that can be thrown when a lower-level operation on the file descriptor fails (e.g. where C-style `open()` would return -1 on failure, the handle will throw a `handle::exception`).
- `raw_type`	The type of the underlying file descriptor: equates `int`.

## Member constants
- `static constexpr raw_type	_invalid_handle = -1;`

## Methods
### Constructors
```cpp
handle();                                  // 1
virtual ~handle();                         // 2
handle(raw_type fd);                            // 3
handle(handle const& that);                // 4
handle(handle&& that) noexcept;            // 5
handle&	operator=(handle const& that);	   // 6
handle&	operator=(handle&& that) noexcept; // 7
```
1. Default constructor: initialize the underlying file descriptor to `_invalid_handle`.
2. Destructor: `close()` the underlying file descriptor.
3. Initialize the underlying file descriptor to `fd`.
4. Copy constructor: initialize the underlying file descriptor to a `dup()`licate of `that`.
5. Move constructor: initialize the underlying file descriptor to `that`; its file descriptor is set to `_invalid_handle`.
6. Copy assigner: set the underlying file descriptor to a `dup()`licate of `that`.
7. Move assigner: set  the underlying file descriptor to `that`; its file descriptor is set to `_invalid_handle`.

### Other methods
All other methods are `protected`.
#### raw
```cpp
handle::raw_type	handle::raw() const noexcept;
```
Return the raw file descriptor.
#### close
```cpp
void	handle::close();
```
Release the underlying file descriptor and close it.
##### exceptions
- `handle::exception` on failure of `close()`.

#### release
```cpp
handle::raw_type	handle::release() noexcept;
```
Release the underlying file descriptor and return it to the caller.

