"""
device simulation - ksz884x
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

pyaplib = ctypes.cdll.LoadLibrary('../pylib/build/pyaplib.so')
pyaplib.get_devmem_size.restype = ctypes.c_int
pyaplib.get_devmem.restype = ctypes.c_char_p
pyaplib.get_msg_type.restype = ctypes.c_int
pyaplib.get_req_addr.restype = ctypes.c_int
pyaplib.get_req_size.results = ctypes.c_int

device_clock_sec = 1

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
    pyaplib.init(genome_id)
    # launch a new qemu
    print("Device model service is started, you can launch qemu now");
    cnt = 0
    stime = time.time()
    last_clock_tick = time.time()
    begin_to_send_irq = 0
    while True :
        ''' check request from QEMU '''
        if (pyaplib.get_msg_type(genome_id) == 3):
            begin_to_send_irq = 1
            pyaplib.check_new_request(genome_id)
            req_type = pyaplib.get_req_type(genome_id)
            if (req_type == 0):
                # read addr and size from shm
                addr = int(pyaplib.get_req_addr(genome_id))
                size = int(pyaplib.get_req_size(genome_id))
                clk = 0xffffffff
                network_input = (addr, size, cnt, clk)
                '''network_input = network_input + get_input_selected(genome_id);'''
                network_input = network_input + get_input(genome_id);
                #print(network_input)
                output = net.activate(network_input)
                #print(output)
                dev_data = int(output[0])
                #print("dev_data:{}".format(dev_data))
                pyaplib.set_data(genome_id, dev_data)
            cnt = cnt+1
            pyaplib.do_respond(genome_id)
        ''' check IRQ '''
        if (begin_to_send_irq) and (time.time() - last_clock_tick>device_clock_sec):
            clk = 1
            network_input = (-1, -1, cnt, clk)
            network_input = network_input + get_input(genome_id);
            output = net.activate(network_input)
            assert_irq = int(output[1])
            if (assert_irq!=0):
                pyaplib.assert_irq(genome_id)
            else:
                pyaplib.deassert_irq(genome_id)
            last_clock_tick = time.time()
        ''' timeout? '''
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
