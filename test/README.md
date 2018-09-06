The states of Dot are very similar to that of Android

Dot
```cpp
void onConnect(std::string, int port); //connection is established
void onResume(); //connection is resumed
void onPause(); //connection gets disconnected
void onDisconnect(); //connection destroyed
```

```cpp
void startDot(DotFactory.get("MQTT"));
```

Looper executes all the operations linearly based on priority in the network thread(netThread)
Handler is used to send tasks to the looper

```Dot``` is extended and the above functions are overrided to 

MessageMetdata mm;