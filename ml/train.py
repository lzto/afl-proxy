"""
device simulation - nozomi
"""

from __future__ import print_function
import os
import neat
import time
import ctypes
import multiprocessing as mp
import pickle

pyaplib = ctypes.cdll.LoadLibrary('pylib/build/pyaplib.so')
pyaplib.get_devmem_size.restype = ctypes.c_int
pyaplib.get_devmem.restype = ctypes.c_char_p

#import visualize

qemutimeout = 120

# 3 input: addr, size, total count
# 65 output: 64bit data + interrupt trigger
# fitness is total count

def eval_single_genome(genome_id, genome, config, out):
    #print("id:{}".format(genome_id))
    pyaplib.init(genome_id)
    pyaplib.launch_qemu(genome_id)
    cnt = 0
    stime = time.time()
    net = neat.nn.FeedForwardNetwork.create(genome, config)
    while True :
        if (pyaplib.get_msg_type(genome_id) == 3):
            pyaplib.check_new_request(genome_id)
            req_type = pyaplib.get_req_type(genome_id)
            if (req_type == 0):
                # read addr and size from shm
                addr = pyaplib.get_req_addr(genome_id)
                size = pyaplib.get_req_size(genome_id)
                network_input = (addr, size, cnt)
                output = net.activate(network_input)
                #print(output)
                dev_data = 0
                for x in range(64):
                    dev_data = dev_data<<1
                    if (output[x]>0.5):
                        dev_data = dev_data + 1;
                #print("dev_data:{}".format(dev_data))
                pyaplib.set_data(genome_id, dev_data)
            cnt = cnt+1
            pyaplib.do_respond(genome_id)
        etime = time.time()
        if (int(etime - stime)>qemutimeout):
            break
    fitness = cnt/100
    out.put(fitness)
    #genome.fitness = fitness
    print("Fitness gen {}={}".format(genome_id, fitness));
    # kill qemu
    pyaplib.kill_qemu(genome_id)
    # tear down shm
    pyaplib.uninit(genome_id)
    


def eval_genomes_parallel(genomes, config):
    parallel_size = 16
    for i in range(0,len(genomes), parallel_size):
        output = mp.Queue()
        
        processes = [mp.Process(target=eval_single_genome, args=(genome_id, genome, config, output)) for genome_id, genome in genomes [i:i+parallel_size]]
        [p.start() for p in processes]
        [p.join() for p in processes]
        results = [output.get() for p in processes]
        for n, r in enumerate(results):
            genomes[i+n][1].fitness = r
    
def eval_genomes(genomes, config):
    for genome_id, genome in genomes:
        genome.fitness = 4.0
        net = neat.nn.FeedForwardNetwork.create(genome, config)
        # create afl shm
        pyaplib.init(0)
        # launch a new qemu
        pyaplib.launch_qemu(0)
        cnt = 0
        stime = time.time()
        while True :
            if (pyaplib.get_msg_type(0) == 3):
                pyaplib.check_new_request(0)
                req_type = pyaplib.get_req_type(0)
                if (req_type == 0):
                    # read addr and size from shm
                    addr = pyaplib.get_req_addr(0)
                    size = pyaplib.get_req_size(0)
                    network_input = (addr, size)
                    devmcnt = pyaplib.get_devmem_cnt(0)
                    for dmi in range(devmcnt):
                        devmsize = pyaplib.get_devmem_size(0, dmi)
                        c8_devm = ctypes.cast(pyaplib.get_devmem(0, dmi), ctypes.POINTER(ctypes.c_char))
                        devm = [ c8_devm[i] for i in range(devmsize) ]
                        print(devm)
                        # now append to the network_input
                        network_input = network_input + tuple(devm)
                    print(network_input)
                    output = net.activate(network_input)
                    #print(output)
                    dev_data = 0
                    for x in range(64):
                        dev_data = dev_data<<1
                        if (output[x]>0.5):
                            dev_data = dev_data + 1;
                    #print("dev_data:{}".format(dev_data))
                    pyaplib.set_data(0,dev_data)
                cnt = cnt+1
                pyaplib.do_respond(0)
            # do a 10 sec test
            etime = time.time()
            if (int(etime - stime)>30):
                break
        genome.fitness = cnt/100
        print('Fitness:{}'.format(genome.fitness))
        # kill qemu
        pyaplib.kill_qemu(0)
        # tear down shm
        pyaplib.uninit(0)


def run(config_file):
    # Load configuration.
    config = neat.Config(neat.DefaultGenome, neat.DefaultReproduction,
                         neat.DefaultSpeciesSet, neat.DefaultStagnation,
                         config_file)

    # Create the population, which is the top-level object for a NEAT run.
    p = neat.Population(config)

    #p = neat.Checkpointer.restore_checkpoint("neat-checkpoint-1")

    # Add a stdout reporter to show progress in the terminal.
    p.add_reporter(neat.StdOutReporter(True))
    stats = neat.StatisticsReporter()
    p.add_reporter(stats)
    p.add_reporter(neat.Checkpointer(5))

    # Run for up to 300 generations.
    winner = p.run(eval_genomes, 300)
    #winner = p.run(eval_genomes_parallel, 300)
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
