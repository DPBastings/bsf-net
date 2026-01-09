# POLLER
```cpp
class bsf::net::poller: public handle;
```
Class representing a collection of sockets that can be _polled_ for the availability of I/O operations.
## Member types
- `class event`: A C++ representation of an event that might occur on a particular file descriptor. See more at the [poller::event] documentation.
- `enum class event_type`: An enumeration representing the type of event to occur. Possible values:
	- `read`: Data can be read from this file.
	- `write`: Data can be written to this file.
	- `closed`: The file was closed unexpectedly.
	- `exception`: An exceptional condition has occurred with this file.
	- `error`: An error occurred with this file.
	- `hangup`: The peer connection has been closed.
- `enum class Mode`: An enumeration representing the poller modality for a particular file descriptor. Possible values:
	- `edge_triggered`
	- `one_shot`
	- `wake_up`
	- `exclusive`
- `event_types` and `Modes`: Aggregate types containing an arbitrary number of `event_type` or `Mode` values.
- `events`. An aggregate type containing a collection of events that may be detected after `wait()`ing for them.
## Methods
### Basic operations
```cpp
poller();                              // 1
~poller() = default;                   // 2
poller(poller const& that) = delete;   // 3
poller(poller&& that);                 // 4
poller& operator=(poller const& that); // 5
poller&	operator=(poller&& that);      // 6
```
1. Default constructor: creates a new poller instance (using the `epoll` API).
2. Destructor: default implementation.
3. Copy constructor: explicitly deleted.
4. Move constructor.
5. Copy assigner: explicitly deleted.
6. Move assigner.
### Accessors
#### size
```cpp
size_t	poller::size() const noexcept;
```
- Return the number of handles that are currently on this poller's interest list.
### Utility
#### add
```cpp
template<typename T>
Sharedhandle	poller::add(T&& handle, event_types events, Modes modes);
```
- `handle` should be an instance of class `bsf::net::handle` or of a derived class.
- Add `handle` to the interest list, checking for events of `events` type in `modes` modes. This involves a call to `epoll_ctl()` and also converts `handle` into a shared handle, which is from henceforth (also) owned by this poller.
#### wait
```cpp
template<size_t MAX_EVENTS>
poller::events	poller::wait(unsigned int timeout) const;
```
- Return the events detected on this poller's interest list within `timeout` milliseconds, or up to `MAX_EVENTS` events.
#### modify
```cpp
void	poller::modify(Sharedhandle const& handle, event_types events, Modes modes);
```
- Modify the poller settings for `handle` to those specified by `events` and `modes`.
#### remove
```cpp
void	poller::remove(Sharedhandle const& handle);
```
- Remove `handle` from the poller's interest list. After calling this function, this poller will no longer be a shared owner of `handle`.