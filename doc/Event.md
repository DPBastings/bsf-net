# event
```cpp
class poller::event
```
Class representing an I/O event that can occur on a handle and be detected by a poller.
## Methods
### Accessors
#### happened
```cpp
bool	event::happened(event_type event) const noexcept;
```
Return whether an event of type `event` has occurred.
#### handle
```cpp
Sharedhandle	event::handle() const noexcept;
```
Return the handle on which the event(s) occurred.
##### Notes
- This methods returns a shared handle, meaning that ownership will be shared with the calling scope. This allows, for example, the easy removal of this handle from the poller that generated this event in case a hangup event or a zero-read was detected.