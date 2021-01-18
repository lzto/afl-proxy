AFL Proxy
----------------------
This is AFL proxy(AP). This repo contains a lighweight proxy program(**ap**) launched by AFL.
A library **aplib** used by client to communicate with AP.

<pre>
|--------\    spawn AP process                   IPC with client using
|  AFL    | <------------------> [./ap fuzz.data]<------------------>   QEMU[KVM]
``````^```    create fuzz data         |          /dev/shm/afl-proxy       |
       \                               |                                   |
        \                              |                                   |
         \   code coverage bitmap      | enable perf-pt(PIDX) <------ap_attach_pt(PIDX)
          `--------------------------> |                                vmx_on()
</pre>

At a high level, AP creates a shared memory(**/dev/shm/afl-proxy**) and accepts requests from the real client, QEMU e.g.
Client get fuzz data location through SHM.
The code coverage information is provided by client by explicitly calling **ap_log_pc(uint64_t)** at each basicblock or it could be provided by PT. If using PT, client need to call **ap_attach_pt(PIDX)** to enable PT-monitor in ap.
Also, if PT is used, clients don't have to call ** ap_log_pc** at each basic block thus saving time.
The client program should be linked with aplib.

AP supports two different modes to collect code coverage. One is through explicit call to **ap_log_pc(uint64_t pc)**, another is through PT.

AP relies on Linux **perf** subsystem to collect PT trace.


Installation
---------------

* download AFL from https://github.com/google/AFL.git and compile
* compile afl-proxy and aplib

```
$(afl-proxy): make
```

* download QEMU from https://github.com/lzto/qemu and compile sfpv2-fuzz branch
* prepare rootfs image **TODO**
* prepare target kernel **TODO**

Run AFL proxy
-------------

First, run AP using the following command

```
cd run
./run.sh
```

After AP is launched, start QEMU in another terminal


Coding Style
-------------

use LLVM coding style and run **make indent**

Limitations
------------

- only single core/thread is supported when using PT
