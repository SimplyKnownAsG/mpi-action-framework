
import subprocess


CXXFLAGS = subprocess.check_output('mpic++ --showme', shell=True).strip().split()
CXXFLAGS += ['-I.']

FLAGS = {
        'flags': CXXFLAGS
        }

def FlagsForFile(filename, **kwargs):
    return FLAGS

