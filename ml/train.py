"""
device simulation - sundance
"""

from __future__ import print_function
import os
import neat
import time
import ctypes
pyaplib = ctypes.cdll.LoadLibrary('pylib/build/pyaplib.so')

#import visualize

# 3 input: addr, size, total count
# 65 output: 64bit data + interrupt trigger
# fitness is total count

def eval_genomes(genomes, config):
    for genome_id, genome in genomes:
        genome.fitness = 4.0
        net = neat.nn.FeedForwardNetwork.create(genome, config)
        # create afl shm
        pyaplib.init()
        # launch a new qemu
        pyaplib.launch_qemu()
        cnt = 0
        stime = time.time()
        while True :
            if (pyaplib.get_msg_type() == 3):
                pyaplib.check_new_request()
                req_type = pyaplib.get_req_type()
                if (req_type == 0):
                    # read addr and size from shm
                    addr = pyaplib.get_req_addr()
                    size = pyaplib.get_req_size()
                    i = (addr, size, cnt)
                    output = net.activate(i)
                    #print(output)
                    dev_data = 0
                    for x in range(64):
                        dev_data = dev_data<<1
                        if (output[x]>0.5):
                            dev_data = dev_data + 1;
                    #print("dev_data:{}".format(dev_data))
                    pyaplib.set_data(dev_data)
                cnt = cnt+1
                pyaplib.do_respond()
            etime = time.time()
            if (int(etime - stime)>10):
                break
        genome.fitness = cnt/100
        print('Fitness:{}'.format(genome.fitness))
        # kill qemu
        pyaplib.kill_qemu()
        # tear down shm
        pyaplib.uninit()


def run(config_file):
    # Load configuration.
    config = neat.Config(neat.DefaultGenome, neat.DefaultReproduction,
                         neat.DefaultSpeciesSet, neat.DefaultStagnation,
                         config_file)

    # Create the population, which is the top-level object for a NEAT run.
    p = neat.Population(config)

    # Add a stdout reporter to show progress in the terminal.
    p.add_reporter(neat.StdOutReporter(True))
    stats = neat.StatisticsReporter()
    p.add_reporter(stats)
    p.add_reporter(neat.Checkpointer(5))

    # Run for up to 300 generations.
    winner = p.run(eval_genomes, 300)

    # Display the winning genome.
    print('\nBest genome:\n{!s}'.format(winner))

if __name__ == '__main__':
    # Determine path to configuration file. This path manipulation is
    # here so that the script will run successfully regardless of the
    # current working directory.
    local_dir = os.path.dirname(__file__)
    config_path = os.path.join(local_dir, 'config')
    run(config_path)
