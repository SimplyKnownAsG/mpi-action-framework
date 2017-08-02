
import subprocess
import sys

if 'win' in sys.platform:
    CXXFLAGS = [r'-IC:/Program Files/Microsoft MPI/Inc']

else:
    CXXFLAGS = subprocess.check_output('mpic++ --showme', shell=True).strip().split()

CXXFLAGS += ['-I.']

FLAGS = {
        'flags': CXXFLAGS
        }

def FlagsForFile(filename, **kwargs):
    return FLAGS

