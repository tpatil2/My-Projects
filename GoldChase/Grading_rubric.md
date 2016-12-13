
Summery: There are two different .cpp files for client daemon and server daemon.
  1.client_daemon.cpp
  2.server_daemon.cpp


* Compiling — Program compiles without error --------------------------YES (15%)

* Running —------------------------------------------------------------YES (20%)
    - Program runs according to requirement


* daemonID init --------------------------------------------------------YES (5%)
    - Client & Server processes initialize daemonID field of gameBoard -
    - gamebord.h (LINE NO : 9 )
    - client_daemon.cpp (LINE NO: 289  )
    - server_daemon.cpp  (LINE NO: 206  )

* send SIGHUP- YES(5%)
    - test_prog.cpp (LINE NO: 366, 550, 647, 867)

* Socket init --------------------------------------------------------- YES(10%)
    (map successfully sent to client)
    - server_daemon.cpp (LINE NO: 313)

* Trap SIGHUP -------------------------------------------------------- YES (10%)
    (SIGHUP handler)
    - client_daemon.cpp (LINE NO: 112 )
    - server_daemon.cpp  (LINE NO: 112 )

* Process "Socket Player”----------------------------------------------YES (10%)
      (update message queues and PID when player joins or leaves)
    - client_daemon.cpp (LINE NO: 317 to 363  )
    - server_daemon.cpp  (LINE NO: 330 to 380  )


* Trap SIGUSR2--------------------------------------------------------- NO-( 5%)  
* Process "Socket Message”--------------------------------------------- NO-( 5%)


* Trap SIGUSR1----------------------------------------------------------YES( 5%)
  -client_daemon.cpp (LINE NO: 63 to 106  )
  -server_daemon.cpp  (LINE NO: 59 to 102  )

* Process "Socket Map" ------------------------------------------------ YES( 5%)
  - pdates the shared memory and sends a SIGUSR1 signal to each
    game process running on the server
  - client_daemon.cpp (LINE NO: 382 to 407 )
  - server_daemon.cpp  (LINE NO: 367 to 392 )

* Daemon integration -------------------------------------------------- YES (5%)
  -The client and server are integrated into the goldchase
    executable to start as daemons.
  - test_prog.cpp( LINE NO: 186 and 355)
