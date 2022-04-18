"""
device simulation - sundance
"""

from __future__ import print_function
import os
import neat
import time
import ctypes
import multiprocessing as mp
import pickle
import re
import time, threading
import subprocess
import os

pyaplib = ctypes.cdll.LoadLibrary('../pylib/build/pyaplib.so')
pyaplib.get_devmem_size.restype = ctypes.c_int
pyaplib.get_devmem.restype = ctypes.c_char_p
pyaplib.get_msg_type.restype = ctypes.c_int
pyaplib.get_req_addr.restype = ctypes.c_int
pyaplib.get_req_size.results = ctypes.c_int

parallel_size = 20
#import visualize
device_clock_sec = 0.1
qemutimeout = 120

'''
should look at coverage and count number of errors and path explored
'''
def get_fitness(shmid):
    '''fname = "/home/tong/qemu-afl-image/vm-testing-"+str(shmid)+".log"'''
    fname = "/home/tong/qemu-afl-image/9p-"+str(shmid)
    cmd = "/home/tong/afl-proxy/ml/calculate_fitness.sh " + fname
    ret = subprocess.getoutput(cmd)
    return float(ret)

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

def get_input_selected(genome_id):
    network_input = ()
    devmcnt = pyaplib.get_devmem_cnt(genome_id)
    c8_devm = ctypes.cast(pyaplib.get_devmem(genome_id, 0), ctypes.POINTER(ctypes.c_char))
    network_input = network_input + tuple(c8_devm[32]);
    network_input = network_input + tuple(c8_devm[1061]);
    network_input = network_input + tuple(c8_devm[1062]);
    network_input = network_input + tuple(c8_devm[289]);
    network_input = network_input + tuple(c8_devm[290]);
    network_input = network_input + tuple(c8_devm[300]);
    network_input = network_input + tuple(c8_devm[319]);
    return network_input

# 3 input: addr, size, total count
# 65 output: 64bit data + interrupt trigger
# fitness is total count

def eval_single_genome(genome_id, genome, config, out):
    #print("id:{}".format(genome_id))
    pyaplib.init(genome_id)
    donefile="/home/tong/qemu-afl-image/9p-"+str(genome_id)+"/done"
    if os.path.exists(donefile):
        os.remove(donefile)
    pyaplib.launch_qemu(genome_id)
    cnt = 0
    stime = time.time()
    last_clock_tick = time.time()
    net = neat.nn.FeedForwardNetwork.create(genome, config)
    begin_to_send_irq = 0
    need_restart = 0
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
        etime = time.time()
        if (int(etime - stime)>qemutimeout):
            need_restart = 1
            break
        if (os.path.exists(donefile)):
            break
    '''fitness = get_fitness(genome_id) + cnt / 100.0'''
    '''fitness = get_fitness(genome_id) / cnt * 1000'''
    if (need_restart==0):
        fitness = get_fitness(genome_id)
        out.put(fitness)
    #genome.fitness = fitness
    print('Fitness gen {0}={1} RCNT:{2}'.format(genome_id, fitness, cnt))
    # kill qemu
    pyaplib.kill_qemu(genome_id)
    # tear down shm
    pyaplib.uninit(genome_id)
    return need_restart
    


def eval_genomes_parallel(genomes, config):
    for i in range(0,len(genomes), parallel_size):
        output = mp.Queue()
        
        processes = [mp.Process(target=eval_single_genome, args=(genome_id, genome, config, output)) for genome_id, genome in genomes [i:i+parallel_size]]
        [p.start() for p in processes]
        [p.join() for p in processes]
        results = [output.get() for p in processes]
        for n, r in enumerate(results):
            genomes[i+n][1].fitness = r

'''evaluate genome in single thread'''
def eval_genomes(genomes, config):
    for genome_id, genome in genomes:
        output = mp.Queue()
        need_restart = 1
        while(need_restart):
            need_restart = eval_single_genome(genome_id, genome, config, output);
            if (not need_restart):
                genome.fitness = output.get()


def run(config_file):
    # Load configuration.
    config = neat.Config(neat.DefaultGenome, neat.DefaultReproduction,
                         neat.DefaultSpeciesSet, neat.DefaultStagnation,
                         config_file)

    # Create the population, which is the top-level object for a NEAT run.
    p = neat.Population(config)

    # load checkpoint?
    p = neat.Checkpointer.restore_checkpoint("neat-checkpoint-8")

    # Add a stdout reporter to show progress in the terminal.
    p.add_reporter(neat.StdOutReporter(True))
    stats = neat.StatisticsReporter()
    p.add_reporter(stats)
    p.add_reporter(neat.Checkpointer(1))

    # Run for up to 300 generations.
    #winner = p.run(eval_genomes, 1)
    winner = p.run(eval_genomes_parallel, 100)
    win = p.best_genome
    pickle.dump(winner, open('winner.pkl', 'wb'))
    pickle.dump(win, open('real_winner.pkl', 'wb'))

    # Display the winning genome.
    print('\nBest genome:\n{!s}'.format(winner))

if __name__ == '__main__':
    # Determine path to configuration file. This path manipulation is
    # here so that the script will run successfully regardless of the
    # current working directory.
    local_dir = os.path.dirname(__file__)
    config_path = os.path.join(local_dir, 'config')
    run(config_path)
