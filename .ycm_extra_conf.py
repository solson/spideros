import os

def FlagsForFile(filename, **kwargs):
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    flags = os.popen('scons -Q ycm=1').read().split()

    # Force YCM to recognize all files as C++, including header files
    flags.extend(['-x', 'c++'])

    return {
        'flags': flags,
        'do_cache': True
    }
