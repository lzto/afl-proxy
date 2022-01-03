"""
device simulation - mxser
"""

from __future__ import print_function
import os
import neat
import neat.genome
import gzip
import time
import ctypes
import multiprocessing as mp
import pickle
import re

pyaplib = ctypes.cdll.LoadLibrary('pylib/build/pyaplib.so')
pyaplib.get_devmem_size.restype = ctypes.c_int
pyaplib.get_devmem.restype = ctypes.c_char_p
pyaplib.get_msg_type.restype = ctypes.c_int
pyaplib.get_req_addr.restype = ctypes.c_int
pyaplib.get_req_size.results = ctypes.c_int

def get_input(genome_id):
    network_input = ()
    devmcnt = pyaplib.get_devmem_cnt(genome_id)
    for dmi in range(devmcnt):
        devmsize = int( pyaplib.get_devmem_size(genome_id, dmi) / 8)
        c8_devm = ctypes.cast(pyaplib.get_devmem(genome_id, dmi), ctypes.POINTER(ctypes.c_char))
        devm=()
        for i in range(devmsize):
            devm = devm + tuple(c8_devm[i])
        #devm = [ c8_devm[i] for i in range(devmsize) ]
        #print(devm)
        # now append to the network_input
        network_input = network_input + devm
    return network_input

def run(config_file, result_file):
    # use id 0
    genome_id = 0
    # Load configuration.
    config = neat.Config(neat.DefaultGenome, neat.DefaultReproduction,
                         neat.DefaultSpeciesSet, neat.DefaultStagnation,
                         config_file)
    genome = pickle.load(open(result_file, 'rb'))
    net = neat.nn.FeedForwardNetwork.create(genome, config)
    # create afl shm
    pyaplib.init(0)
    # launch a new qemu
    print("Device model service is started, you can launch qemu now");
    cnt = 0
    stime = time.time()
    while True :
        if (pyaplib.get_msg_type(0) == 3):
            pyaplib.check_new_request(0)
            req_type = pyaplib.get_req_type(0)
            if (req_type == 0):
                # read addr and size from shm
                addr = int(pyaplib.get_req_addr(genome_id))
                size = int(pyaplib.get_req_size(genome_id))
                network_input = (addr, size, cnt)
                network_input = network_input + get_input(genome_id);
                #print(network_input)
                output = net.activate(network_input)
                #print(output)
                dev_data = int(output[0])
                #print("dev_data:{}".format(dev_data))
                pyaplib.set_data(0,dev_data)
                print('Read {0} Byte @ {1} = {2}'.format(hex(addr), size, hex(dev_data)))
            cnt = cnt+1
            pyaplib.do_respond(0)
        # do a 10 sec test
        #etime = time.time()
        # if (int(etime - stime)>30):
        #    break
    # kill qemu
    #pyaplib.kill_qemu(0)
    # tear down shm
    pyaplib.uninit(0)


if __name__ == '__main__':
    # Determine path to configuration file. This path manipulation is
    # here so that the script will run successfully regardless of the
    # current working directory.
    local_dir = os.path.dirname(__file__)
    config_path = os.path.join(local_dir, 'config')
    run(config_path, "real_winner.pkl")
